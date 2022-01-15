#!/bin/bash

for (( i = 0; i < 100; i++ )); do
	sort -R testdata/short-list | ./sort | sha1sum
done | sort -u
