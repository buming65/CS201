#!/bin/bash
rm -rf 1.out 2.out
echo "--------BUILDING--------"
cd ../Pass/build/
cmake -DCMAKE_BUILD_TYPE=Release ../Transforms/ValueNumbering
make -j4
cd -
echo "--------START--------"
echo "--------CASE1--------"
opt 1.ll -mem2reg -S -o 1.bc
opt -load ../Pass/build/libLLVMValueNumberingPass.so  -ValueNumbering < 1.bc > /dev/null
echo "--------CASE2--------"
opt 2.ll -mem2reg -S -o 2.bc
opt -load ../Pass/build/libLLVMValueNumberingPass.so  -ValueNumbering < 2.bc > /dev/null
echo "--------END--------"