#!/bin/bash
if [ ! -f ./.xff ]; then
    exit 0
fi

sed -i '2d' .xff
sed -i '10d' .xff
sed -i '11d' .xff
sed -i '13d' .xff
sed -i '22d' .xff
sed -i '23d' .xff
