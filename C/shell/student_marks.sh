#!/bin/bash

echo -n "Enter student name: "
read name

echo -n "Enter marks of subject 1: "
read m1
echo -n "Enter marks of subject 2: "
read m2
echo -n "Enter marks of subject 3: "
read m3

total=$((m1 + m2 + m3))
percentage=$((total * 100 / 300))

echo ""
echo "Student: $name"
echo "Total: $total / 300"
echo "Percentage: $percentage%"

if [ $percentage -ge 70 ]; then
    echo "Result: First Class"
elif [ $percentage -ge 60 ]; then
    echo "Result: Second Class"
elif [ $percentage -ge 40 ]; then
    echo "Result: Pass Class"
else
    echo "Result: Fail"
fi
