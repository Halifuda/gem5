#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <test exe>"
    exit 0
fi

OUTPAR=m5out/c
# CC: Cache Config
ALLCC=("default" "dcp" "ampm" "spf" "mysamp")
CC=("mysamp")

SIM="build/ARM/gem5.opt"
SCRIPT="configs/proj6/3-level.py"
COLLSH="collect.sh"

function single_test() {
    # $1: cache config name, $2: test exe
    $SIM --outdir=$OUTPAR/$2/$1 $SCRIPT --cachec=$1 --test_exe=$2
}

for cc in ${CC[@]}; do
{
    single_test $cc $1
} &
done
wait

cd $OUTPAR
./$COLLSH $1
