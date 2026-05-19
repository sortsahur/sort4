#!/bin/bash

dir_count=0
link_count=0
file_count=0
total_size=0

ls -l | tail -n +2 | while read line; do
    type=$(echo "$line" | cut -c1)
    name=$(echo "$line" | awk '{print $NF}')
    size=$(echo "$line" | awk '{print $5}')

    case $type in
        d)
            echo "DIR ${name}/"
            dir_count=$((dir_count + 1))
            ;;
        l)
            echo "LINK ${name}@"
            link_count=$((link_count + 1))
            ;;
        -)
            echo "     $name"
            file_count=$((file_count + 1))
            total_size=$((total_size + size))
            ;;
    esac
done

echo ""
echo "Summary:"

dir_count=0; link_count=0; file_count=0; total_size=0
ls -l | tail -n +2 | while read line; do
    type=$(echo "$line" | cut -c1)
    size=$(echo "$line" | awk '{print $5}')
    case $type in
        d) dir_count=$((dir_count + 1)) ;;
        l) link_count=$((link_count + 1)) ;;
        -) file_count=$((file_count + 1)); total_size=$((total_size + size)) ;;
    esac
    echo "Directories: $dir_count"
    echo "Symbolic Links: $link_count"
    echo "Regular Files: $file_count"
    echo "Total Size of Regular Files: $total_size bytes"
done | tail -4
