#!/bin/bash

echo -n "Enter a number: "
read num

is_prime=1
if [ $num -le 1 ]; then
    is_prime=0
fi

for (( i=2; i*i<=num; i++ )); do
    if [ $((num % i)) -eq 0 ]; then
        is_prime=0
        break
    fi
done

if [ $is_prime -eq 1 ]; then
    echo "$num is a Prime number"
else
    echo "$num is not a Prime number"
fi

rev=0
temp=$num
while [ $temp -gt 0 ]; do
    rem=$((temp % 10))
    rev=$((rev * 10 + rem))
    temp=$((temp / 10))
done

echo "Reverse of $num is $rev"
