#!/bin/bash

smallest=""
smallest_len=9999

for file in *; do
    len=${#file}
    if [ $len -lt $smallest_len ]; then
        smallest_len=$len
        smallest=$file
    fi
done

echo "Smallest filename (by string length): $smallest"
echo "Length: $smallest_len characters"
