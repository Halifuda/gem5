#!/bin/bash

EXE=("2mm" "bzip2" "mcf" "bfs" "hello")

if [ $# -eq 1 ]; then
	if [ $1 == "c" ]; then
	    for bench in ${EXE[@]}; do
			./testc.sh $bench
		done 
	else
		echo -e "Use \033[01;32m\"$0 c\"\033[0m to test project 6"
	fi
	exit 0
fi

if [ $# -eq 0 ]; then
	for bench in ${EXE[@]}; do
		./test.sh $bench
	done
	exit 0
fi

echo -e "Use \033[01;32m\"$0\"\033[0m to test project 5"
echo -e "Use \033[01;32m\"$0 c\"\033[0m to test project 6"
