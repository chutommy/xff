#!/bin/bash
if [ ! -f ./dummy/file0.txt ]; then
    exit 0
fi

rm dummy/file0.txt
sed -i '2d' dummy/file1.txt
touch dummy/file2.txt
rmdir dummy/file3.txt
touch dummy/file3.txt
