#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: $0 <test exe>"
	exit 0
fi

echo "[COLLECT.SH] $0 $1!"

echo "" > $1.txt

function collect_bp_stats() {
	echo "[BP] $1" >> $2.txt
	cat $2/$1/stats.txt | grep "simTicks" >> $2.txt
	cat $2/$1/stats.txt | grep "simInsts" >> $2.txt
	cat $2/$1/stats.txt | grep "cpi" >> $2.txt
	cat $2/$1/stats.txt | grep "ipc" >> $2.txt
	cat $2/$1/stats.txt | grep "BTB" >> $2.txt
	cat $2/$1/stats.txt | grep "branch" >> $2.txt
	cat $2/$1/stats.txt | grep "Squashed" >> $2.txt	
}

ALLBP=("ban" "local" "p8k" "p64k" "tour" "bimode" "simple" "myp")
BP=("local" "myp" "p8k" "simple" "tour" "bimode")

for bp in ${BP[@]}; do
	collect_bp_stats $bp $1
done

