#!/bin/sh

set -e

if [ "$1" = "--inplace" ]; then
    FLAG="-i"
else
    FLAG="-n --Werror"
fi

for FILE in $(find src -name "*.cpp" -o -name "*.h" -o -name "*.hpp")
do
    clang-format $FLAG $FILE
done
