#!/bin/bash
rm -rf 1.out 2.out
echo "--------BUILDING--------"
cd ../Pass/build/
cmake -DCMAKE_BUILD_TYPE=Release ../Transforms/Liveness
make -j4
cd -
echo "--------START--------"
echo "--------CASE1--------"
opt -load ../Pass/build/libLLVMLivenessPass.so  -Liveness < 1.ll > /dev/null
echo "--------CASE2--------"
opt -load ../Pass/build/libLLVMLivenessPass.so  -Liveness < 2.ll > /dev/null
echo "--------END--------"