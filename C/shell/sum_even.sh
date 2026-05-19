#!/bin/bash

sum_even() {
    limit=$1
    sum=0
    num=2
    count=0
    while [ $count -lt $limit ]; do
        sum=$((sum + num))
        num=$((num + 2))
        count=$((count + 1))
    done
    echo $sum
}

if [ $# -ne 1 ]; then
    echo "Usage: $0 <n>"
    exit 1
fi

n=$1
result=$(sum_even $n)
echo "Sum of first $n even numbers: $result"
