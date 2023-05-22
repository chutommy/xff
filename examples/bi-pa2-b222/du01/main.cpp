#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
#endif /* __PROGTEST__ */

class InputBuffer {
private:
    istream &in;
    size_t size;
    vector<uint8_t> buffer;

    uint8_t wipByte;
    uint8_t wipByteIdx;

    bool loadBuffer() {
        if (!in.read((char *) buffer.data(), buffer.size()))
            if (!in.eof()) return false;

        buffer.resize(in.gcount());
        size = 0;
        return true;
    }

    bool readByte(uint8_t &byte) {
        if (size >= buffer.size())
            if (!loadBuffer()) return false;

        byte = buffer[size++];
        return true;
    }

    static bool subByteUTF8Clean(uint8_t &byte) {
        // (1)(0)xxxxxx
        if ((byte & (1 << 7)) == 0
            || (byte & (1 << 6)) != 0)
            return false;

        byte = byte & 0b111111;
        return true;
    }

    bool readBit(bool &b) {
        if (wipByteIdx >= 8) {
            if (!readByte(wipByte)) return false;
            wipByteIdx = 0;
        }

        b = wipByte & (1 << wipByteIdx);
        wipByteIdx++;
        return true;
    }

public:

    InputBuffer(istream &in, size_t capacity)
            : in(in), size(capacity), buffer(capacity), wipByte(0), wipByteIdx(8) {}

    bool readIntUTF8(uint32_t &value) {
        uint8_t byte1 = 0, byte2 = 0, byte3 = 0, byte4 = 0;
        if (!readByte(byte1)) return false;

        // (0)xxxxxxx
        if ((byte1 & (1 << 7)) == 0) {
            value = byte1;
            return true;
        }

        // invalid 1(1)0xxxxx 10xxxxxx
        if ((byte1 & (1 << 6)) == 0) return false;

        // 11(0)xxxxx 10xxxxxx
        if ((byte1 & (1 << 5)) == 0) {
            if (!readByte(byte2)) return false;
            if (!subByteUTF8Clean(byte2)) return false;
            value = byte1 & 0b11111;
            value = (value << 6) | byte2;
            return true;
        }

        // 111(0)xxxx 10xxxxxx 10xxxxxx
        if ((byte1 & (1 << 4)) == 0) {
            if (!readByte(byte2) || !readByte(byte3)) return false;
            if (!subByteUTF8Clean(byte2) || !subByteUTF8Clean(byte3)) return false;
            value = byte1 & 0b1111;
            value = (value << 6) | byte2;
            value = (value << 6) | byte3;
            return true;
        }

        // 1111(0)xxx 10xxxxxx 10xxxxxx 10xxxxxx
        if ((byte1 & (1 << 3)) == 0) {
            if (!readByte(byte2) || !readByte(byte3) || !readByte(byte4)) return false;
            if (!subByteUTF8Clean(byte2) || !subByteUTF8Clean(byte3) || !subByteUTF8Clean(byte4)) return false;
            value = byte1 & 0b111;
            value = (value << 6) | byte2;
            value = (value << 6) | byte3;
            value = (value << 6) | byte4;
            return true;
        }

        return false;
    }

    bool readFibSeq(vector<bool> &seq) {
        bool previous = false, b;
        while (true) {
            if (seq.size() == 32) return false;
            if (!readBit(b)) return false;
            if (b and previous) return true;

            previous = b;
            seq.push_back(b);

            if (empty()) break;
        }

        for (bool b: seq)
            if (b) return false;
        seq.clear();
        return true;
    }

    bool empty() { return in.eof() && size == buffer.size() && wipByteIdx == 8; }
};

class OutputBuffer {
private:
    ostream &out;
    size_t size;
    vector<uint8_t> buffer;

    uint8_t wipByte;
    uint8_t wipByteIdx;

    bool flushBuffer() {
        if (!out.write((char *) buffer.data(), size))
            return false;
        size = 0;
        return true;
    }

    bool writeByte(unsigned int byte) {
        if (size >= buffer.size())
            if (!flushBuffer()) return false;

        buffer[size++] = byte;
        byte = 0;
        wipByteIdx = 0;
        return true;
    }

    bool writeByteWIP() {
        if (size >= buffer.size())
            if (!flushBuffer()) return false;

        buffer[size++] = wipByte;
        wipByte = 0;
        wipByteIdx = 0;
        return true;
    }

public:

    OutputBuffer(ostream &out, size_t capacity)
            : out(out), size(0), buffer(capacity), wipByte(0), wipByteIdx(0) {}

    bool appendBit(bool b) {
        if (wipByteIdx >= 8)
            if (!writeByteWIP())
                return false;

        wipByte = wipByte | (b << wipByteIdx);
        wipByteIdx++;
        return true;
    }

    bool writeUTF8(uint32_t value) {
        if (value <= 0x7f) {
            // 0xxxxxxx
            if (!writeByte(reinterpret_cast<uint8_t &>(value)))
                return false;

        } else if (value <= 0x7ff) {
            // 110xxxxx 10xxxxxx
            if (!writeByte((0b11000000) | (0b11111 & (value >> 6)))
                || !writeByte((0b10000000) | (0b111111 & value)))
                return false;

        } else if (value <= 0xffff) {
            // 1110xxxx 10xxxxxx 10xxxxxx
            if (!writeByte((0b11100000) | (0b1111 & (value >> 12)))
                || !writeByte((0b10000000) | (0b111111 & (value >> 6)))
                || !writeByte((0b10000000) | (0b111111 & value)))
                return false;

        } else if (value <= 0x10ffff) {
            // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            if (!writeByte((0b11110000) | (0b111 & (value >> 18)))
                || !writeByte((0b10000000) | (0b111111 & (value >> 12)))
                || !writeByte((0b10000000) | (0b111111 & (value >> 6)))
                || !writeByte((0b10000000) | (0b111111 & value)))
                return false;

        } else {
            return false;
        }

        return true;
    }

    bool flush(bool wipBytes = true) {
        if (wipBytes)
            if (!writeByteWIP())
                return false;
        if (!flushBuffer()) return false;
        return true;
    }
};

bool maxFib(const uint32_t val, uint32_t &maxVal, size_t &idx) {
    if (val == 0) {
        return false;
    } else if (val == 1) {
        maxVal = 1;
        idx = 0;
        return true;
    }

    size_t f1, f2 = 2, f3 = 3;
    idx = 1;
    while (f3 <= val) {
        f1 = f2;
        f2 = f3;
        f3 = f1 + f2;
        idx++;
    }

    maxVal = f2;
    return true;
}

vector<bool> toFibonacci(uint32_t value) {
    value++;

    vector<size_t> fib;
    vector<bool> seq;

    uint32_t maxVal = 0;
    size_t idx = 0;
    while (maxFib(value, maxVal, idx)) {
        fib.push_back(idx);
        value -= maxVal;
    }

    idx = 0;
    size_t l = fib.size();
    for (size_t i = 0; i < l; i++) {
        while (idx != fib[l - i - 1]) {
            seq.push_back(false);
            idx++;
        }

        seq.push_back(true);
        idx++;
    }

    seq.push_back(true);
    return seq;
}

uint32_t fromFibonacci(vector<bool> &seq) {
    uint32_t sum = 0;
    size_t f1 = 1, f2 = 2, f3 = 3;
    for (bool b: seq) {
        if (b) sum += f1;
        f1 = f2;
        f2 = f3;
        f3 = f1 + f2;
    }

    return sum - 1;
}

bool utf8ToFibonacci(const char *inFile, const char *outFile) {
    ifstream fin(inFile, ios::binary);
    if (!fin.is_open()) return false;

    ofstream fout(outFile, ios::binary);
    if (!fout.is_open()) return false;

    InputBuffer inputBuffer(fin, 1024);
    OutputBuffer outputBuffer(fout, 1024);

    while (true) {
        uint32_t x;
        if (!inputBuffer.readIntUTF8(x)) return false;
        if (x > 0x10ffff) return false;

        vector<bool> fib = toFibonacci(x);
        for (bool b: fib) outputBuffer.appendBit(b);

        if (inputBuffer.empty()) break;
    }

    if (!outputBuffer.flush()) return false;
    return true;
}

bool fibonacciToUtf8(const char *inFile, const char *outFile) {
    ifstream fin(inFile, ios::binary);
    if (!fin.is_open()) return false;

    ofstream fout(outFile, ios::binary);
    if (!fout.is_open()) return false;

    InputBuffer inputBuffer(fin, 1024);
    OutputBuffer outputBuffer(fout, 1024);

    while (true) {
        vector<bool> seq;
        if (!inputBuffer.readFibSeq(seq)) return false;
        if (!seq.empty()) {
            uint32_t val = fromFibonacci(seq);
            if (!outputBuffer.writeUTF8(val))
                return false;
        }
        if (inputBuffer.empty()) break;
    }

    if (!outputBuffer.flush(false)) return false;
    return true;
}

#ifndef __PROGTEST__

bool identicalFiles(const char *file1, const char *file2) {
    ifstream fin1(file1, ios::binary),
            fin2(file2, ios::binary);
    if (!fin1.is_open() || !fin2.is_open()) return false;

    while (true) {
        uint8_t f1, f2;
        if (!fin1.read(reinterpret_cast<char *>( &f1 ), sizeof(f1))
            || !fin2.read(reinterpret_cast<char *>( &f2 ), sizeof(f2))) {
            if (fin1.eof()) return true;
            return false;
        }
        if (f1 != f2) return false;
        if (fin1.eof() != fin2.eof()) return false;
    }
}

int main() {
    assert(utf8ToFibonacci("example/src_0.utf8", "output.fib")
           && identicalFiles("output.fib", "example/dst_0.fib"));
    assert(utf8ToFibonacci("example/src_1.utf8", "output.fib")
           && identicalFiles("output.fib", "example/dst_1.fib"));
    assert(utf8ToFibonacci("example/src_2.utf8", "output.fib")
           && identicalFiles("output.fib", "example/dst_2.fib"));
    assert(utf8ToFibonacci("example/src_3.utf8", "output.fib")
           && identicalFiles("output.fib", "example/dst_3.fib"));
    assert(utf8ToFibonacci("example/src_4.utf8", "output.fib")
           && identicalFiles("output.fib", "example/dst_4.fib"));
    assert(!utf8ToFibonacci("example/src_5.utf8", "output.fib"));

    assert(fibonacciToUtf8("example/src_6.fib", "output.utf8")
           && identicalFiles("output.utf8", "example/dst_6.utf8"));
    assert(fibonacciToUtf8("example/src_7.fib", "output.utf8")
           && identicalFiles("output.utf8", "example/dst_7.utf8"));
    assert(fibonacciToUtf8("example/src_8.fib", "output.utf8")
           && identicalFiles("output.utf8", "example/dst_8.utf8"));
    assert(fibonacciToUtf8("example/src_9.fib", "output.utf8")
           && identicalFiles("output.utf8", "example/dst_9.utf8"));
    assert(fibonacciToUtf8("example/src_10.fib", "output.utf8")
           && identicalFiles("output.utf8", "example/dst_10.utf8"));
    assert(!fibonacciToUtf8("example/src_11.fib", "output.utf8"));

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
