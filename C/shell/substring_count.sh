#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <main_string> <substring>"
    exit 1
fi

main=$1
sub=$2
count=0
temp=$main

while true; do
    case "$temp" in
        *"$sub"*)
            count=$((count + 1))
            temp="${temp#*$sub}"
            ;;
        *)
            break
            ;;
    esac
done

echo "String: $main"
echo "Substring: $sub"
echo "Occurrences: $count"
