#!/bin/bash
if [ -f ./dummy/file0.txt ]; then
    exit 0
fi

# create new file
touch dummy/file0.txt

# update file
echo "new line" >> dummy/file1.txt

# remove file
rm dummy/file2.txt

# turn file into non-regular file
rm dummy/file3.txt
mkdir dummy/file3.txt
