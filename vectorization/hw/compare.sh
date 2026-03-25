#!/usr/bin/env bash

# Usage: ./compare.sh <M> <K> <N>
if [ $# -ne 3 ]; then
    echo "Usage: $0 <M> <K> <N>"
    exit 1
fi

M=$1
K=$2
N=$3

make clean
make

# 1) Generate input.bin and ans.bin
echo "Generating input and answer files..."
./gen $M $K $N

echo "--------------------"
# 2) Run matrix multiplication (main) -> c.bin
echo "Running matrix multiplication..."
./main $M $K $N

echo "--------------------"
# 3) Compare with ans.bin
./float-diff blocked.bin ans.bin
./float-diff avx_blocked.bin ans.bin
./float-diff opt.bin ans.bin

echo "--------------------"
# Done
echo "Comparison complete."