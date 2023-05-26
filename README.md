# xff (Indexing Tool)

## Introduction
This project is an indexing and search tool for the content (files and directories) within a specified directory. It consists of two main parts: indexing and searching. The indexing part creates a well-represented index of the files, while the searching part allows efficient file retrieval based on the index.

## Features
The tool provides the following functionalities:

1. Automatic Indexing: If the index does not exist or if the files have been modified, the tool performs indexing at startup. During this phase, it traverses the files and indexes their information, creating auxiliary files for subsequent searching. Manual reindexing is also supported if a complete reindexation is needed.

2. Suitable File Format and Structure: The tool uses a suitable file format and structure for the index. It keeps track of the files that needed to be reindexed and stores at least the file name, size, and last modification time.

3. Special Attributes for Different File Types: The tool implements special indexing attributes for at least three different file types. For example, text files can include words and their frequencies, images can include image size and color histograms, and HTML documents can include a list of documents that reference them.

4. Advanced Searching: The tool allows searching based on the indexing attributes, both individually and in combination. It supports queries defined by indexing attributes, logical AND combinations of two queries, and logical OR combinations of two queries.

5. Efficient Searching: The searching process is more efficient than manual searching through all the files, thanks to the indexing phase. The indexing is only performed when necessary, ensuring efficient retrieval of files.

## Specifications

### Indexing
- The working directory (`$PWD`) is always indexed. The index file is stored in the indexed directory (`$PWD/.xff`).
- A complete reindexation is performed if the index does not exist, is corrupted, or if the user manually triggers it.
- If the index exists but is not up-to-date, a partial reindexation is performed, indexing new files, removing deleted files, and reindexing modified files.
- Files are indexed recursively, including nested directories. However, the index file is only accessible in the specific directory being indexed.
- Manual reindexation can be triggered by running the `xff reset` command. To update an existing index or create a new one if it doesn't exist, the tool can be executed without any arguments (`xff`). The `xxf help` command displays the help message.

The following information is stored in the index:
1. File name and address.
2. File size in bytes.
3. Last modification time.

These attributes are tracked for all files. Additionally, the following special attributes are indexed for specific file types:

- Text files:
    1. Number of words.
    2. Readability score (Flesch-Kincaid test).
    3. Top 5 most used words and their frequencies.

- CSV files:
    1. Number of records.

- C++ files:
    1. Number of C++ keywords used.
    2. List of included files.

Only "regular files" are indexed, while directories, symbolic links, and other Linux system files falling under the category of "special files" are ignored.

The index is stored as a human-readable file, with each item separated by a new line. An example fragment of the index file (`$XFF_DIR/.xff`) is shown below:

```
... the end of the previous log ...
"./hello.txt"                             // relative path to the file
143237                                    // size of the file in B(ytes)
2023-05-14 12:10:09                       // last write time
23237                                     // word count
31.8157                                   // Flesh-Kincaid score
a 6 the 6 she 6 sees 5 is 2               // the most frequent words
"./pa2/pa2-semestral/src/main.cpp"        // the next file
1273                                      // ...
2023-05-14 12:10:09
930
algorithm cassert cctype cmath cstdio
  cstdlib cstring functional iomanip
  iostream list map memory set sstream
  string type_traits typeinfo unistd.h
  unordered_map unordered_set vector
... the beginning of the next log ...
```

### Searching

#### Based on Common Attributes

- Files with a specific name: `xxf name file1.json`
- Files with a name matching a regular expression: `xxf like ".*myfile.*"`
- Files with size equal to/less than/greater than a specified size in bytes: `xxf size/size-/size+ 108`
- Files last modified on/before/after a specified date and time: `xxf time/time-/time+ "2022-11-09 13:53:03"`

Example query for all files in a given branch of the filesystem:

```
$ xxf like ".*"

=== Search results ===

1) "/home/chutommy/CLionProjects/pa2-semestral/playground/./mydata/data2.json"
        File: "data2.json"
        Type: JSON
        Size: 43
      Modify: 2023-05-14 12:10:09

2) "/home/chutommy/CLionProjects/pa2-semestral/playground/./hw04/main.cpp"
        File: "main.cpp"
        Type: CPP
        Size: 1507
      Modify: 2023-05-15 01:03:39
    Keywords: 10
    Includes: CPPFile.h CSVFile.h DoubleTerm.h File.h IntTerm.h 
              StringTerm.h TXTFile.h Timestamp.h TimestampTerm.h filesystem 
              iostream string vector 

3) "/home/chutommy/CLionProjects/pa2-semestral/playground/./acad-papers/quantum-research/paper.txt"
        File: "paper.txt"
        Type: TXT
        Size: 7024
      Modify: 2023-05-15 01:02:20
       Words: 1066
         (72) the
         (38) of
         (30) and
         (27) quantum
         (25) a
 Readability: 66.8658

4) "/home/chutommy/CLionProjects/pa2-semestral/playground/./heights.csv"
        File: "heights.csv"
        Type: CSV
        Size: 3734
      Modify: 2023-05-15 01:00:39
        Rows: 201


=== Search complete. Matched files: [4/4] ===
```

#### Based on Individual Attributes

- Text files with a specific word count/equal to/less than/greater than: `xxf words/words-/words+ 42`
- Text files with a specific word in the top 5 most frequent words: `xxf top hello`
- Text files with a specific readability score/equal to/less than/greater than: `xxf readability/readability-/readability+ 50.00`
- CSV files with a specific number of records/equal to/less than/greater than: `xxf rows/rows-/rows+ 42`
- C++ files with a specific number of keywords/equal to/less than/greater than: `xxf keyword/keyword-/keyword+ 42`
- C++ files with a specific included library: `xxf include vector`

#### Combining Queries

- Logical AND: `xxf name file.txt and size 30 and words- 6`
    - Files with the name `file.txt`, size of `30 B` or less, and less than `6 words`


- Logical OR: `xxf name file.txt or size 30 or words- 6`
    - Files with the name `file.txt`, size of `30
