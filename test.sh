#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <test exe>"
    exit 0
fi

OUTPAR=m5out/bp
ALLBP=("ban" "local" "p8k" "p64k" "tour" "bimode" "simple" "myp")
BP=("local" "myp" "p8k" "simple" "tour" "bimode")

SIM="build/ARM/gem5.opt"
SCRIPT="configs/proj5/two-level.py"
COLLSH="collect.sh"

function single_test() {
    # $1: bp name, $2: test exe
    $SIM --outdir=$OUTPAR/$2/$1 $SCRIPT --bp=$1 --test_exe=$2
}

for bp in ${BP[@]}; do
{
    single_test $bp $1
} &
done
wait

cd $OUTPAR
./$COLLSH $1

