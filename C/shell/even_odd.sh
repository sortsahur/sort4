#!/bin/bash

> even
> odd

echo -n "Enter the number of elements: "
read count

echo "Enter $count numbers (one per line):"
for (( i=1; i<=count; i++ )); do
    read n
    if [ $((n % 2)) -eq 0 ]; then
        echo "$n" >> even
    else
        echo "$n" >> odd
    fi
done

echo ""
echo "Even numbers (saved to 'even'):"
cat even
echo ""
echo "Odd numbers (saved to 'odd'):"
cat odd
