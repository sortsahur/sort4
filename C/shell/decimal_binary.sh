#!/bin/bash

dec_to_bin() {
    n=$1
    bin=""
    if [ $n -eq 0 ]; then
        bin="0"
    fi
    while [ $n -gt 0 ]; do
        rem=$((n % 2))
        bin="$rem$bin"
        n=$((n / 2))
    done
    echo "$bin"
}

bin_to_dec() {
    b=$1
    dec=0
    len=${#b}
    for (( i=0; i<len; i++ )); do
        bit=${b:$i:1}
        dec=$((dec * 2 + bit))
    done
    echo "$dec"
}

echo "1. Decimal to Binary"
echo "2. Binary to Decimal"
echo -n "Enter choice: "
read ch

case $ch in
    1)
        echo -n "Enter decimal number: "
        read num
        result=$(dec_to_bin $num)
        echo "Binary: $result"
        ;;
    2)
        echo -n "Enter binary number: "
        read num
        result=$(bin_to_dec $num)
        echo "Decimal: $result"
        ;;
    *)
        echo "Invalid choice"
        ;;
esac
