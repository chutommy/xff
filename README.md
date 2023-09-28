# xff (Indexing Tool)

## Introduction
This project offers a high-performance file-system indexing.

## Features

- auto-indexing: xff maintains a file-based index for fast queries
- smart updates: the tool keeps track of the files that need to be reindexed and uses just-in-time indexation
- extra meta-data: special file attributes for text file, csv files and c/c++ files
- logic-based searching: the tool allows searching based either on the indexing attributes or their logical compounds

## Specifications

### Indexing policy
- xff stores indexes in the working directories (as `./.xff` by default)
- complete reindexation is performed if the index does not exist, is corrupted, or if the user manually triggers it
- files are indexed recursively, including nested directories
- only regular files are indexed: directories, symbolic links, and other special files are ignored


The following attributes are stored in the index file: file name, location, size in bytes and last modification time
Additionally, the following special attributes are indexed for the specific file types:
- text files: word count, readability score (Flesch-Kincaid test), most used words and their frequencies
- csv files: number of records
- c/C++ files: number of C++ keywords used, list of included files

The index is stored as a human-readable file, with each item separated by a new line. An example fragment of the index file (`$XFF_DIR/.xff`) is shown below:

```
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
```

### Searching policy

#### Based on Common Attributes

- files with a specific name: `xxf name file1.json`
- files with a name matching a regular expression: `xxf like ".*myfile.*"`
- files with size equal to/less than/greater than a specified size in bytes: `xxf size/size-/size+ 108`
- files last modified on/before/after a specified date and time: `xxf time/time-/time+ "2022-11-09 13:53:03"`

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

- text files with a specific word count/equal to/less than/greater than: `xxf words/words-/words+ 42`
- text files with a specific word in the top 5 most frequent words: `xxf top hello`
- text files with a specific readability score/equal to/less than/greater than: `xxf readability/readability-/readability+ 50.00`
- csv files with a specific number of records/equal to/less than/greater than: `xxf rows/rows-/rows+ 42`
- c/c++ files with a specific number of keywords/equal to/less than/greater than: `xxf keyword/keyword-/keyword+ 42`
- c/c++ files with a specific included library: `xxf include vector`

#### Combining Queries

- Logical AND: `xxf name file.txt and size 30 and words- 6`
    - Files with the name `file.txt`, size of `30 B` or less, and less than `6 words`


- Logical OR: `xxf name file.txt or size 30 or words- 6`
    - Files with the name `file.txt`, size of `30
