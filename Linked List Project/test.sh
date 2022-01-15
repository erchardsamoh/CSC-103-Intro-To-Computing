#!/bin/bash

opts=( "" "-u" "-r" "-ru" )
count=0

# run through test files and apply all combos of options:
for f in testdata/* ; do
	for (( i = 0; i < ${#opts[@]}; i++ )); do
		diff -q <(./sort ${opts[$i]} <$f) <(sort ${opts[$i]} <$f) &> /dev/null
		if (( $? != 0 )); then
			echo -en "\033[1;31mXXX:\033[0m "
			echo "check file $f with options ${opts[$i]}"
			(( count++ ))
		fi
	done
done

if (( count == 0 )); then
	echo -e "\033[32mTest passed :D\033[0m"
	echo "Your program might not be broken!"
fi
