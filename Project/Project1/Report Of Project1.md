# Report Of Project1
### Source (.c) to Binary (executable) 
clang test.c -o test_c_binary -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/ 

The input is the test.c.

The output is the test_c_binary.

### Source(.c) to Objective(.o)
clang -c test.c -o test_c_o.o -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/ 

The input is the test.c.

The output is the test_c_o.o

### Source (.c) to Machine assembly (.s) 
clang -S test.c -o test_c_s.s -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/ 

The input is the test.c.

The output is the test_c_s.s

### Source (.c) to LLVM bitcode (.bc) 
clang -emit-llvm -c test.c -o test_c_bc.bc -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/ 

The input is the test.c.

The output is the test_c_bc.bc

### Source (.c) to LLVM IR (.ll) 
clang  -emit-llvm -S test.c -o test_c_ll.ll -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/

The input is the test.c.

The output is the test_c_ll.ll

### LLVM IR (.ll) to LLVM bitcode (.bc) 
llvm-as test_c_ll.ll -o test_ll_bc.bc

The input is the test_c_ll.ll

The output is the test_ll_bc.bc

### LLVM bitcode (.bc) to LLVM IR (.ll) 
llvm-dis test_c_bc.bc -o test_bc_ll.ll

The input is the test_c_bc.bc

The output is the test_bc_ll.ll

### LLVM IR (.ll) to machine assembly (.s) 
llc test_c_ll.ll -o test_ll_s.s

The input is the test_c_ll.ll

The output is the test_ll_s.s

### interpret the LLVM IR (which directly prints the output without compilation) 
lli test_ll_bc.bc

The input is the test_ll_bc.bc

The output prints the line: GCD of 98 and 56 is 14 %

### Generate the CFG(s) of test.c
opt -dot-cfg test_c_bc.bc

dot -Tpdf .gcd.dot -o test_dot.pdf 

So for this step, firstly, use opt command to generate the dot files. 

The input is the LLVM bitcode file named test_c_bc.bc. 

The output is two files named .main.dot and .gcd.dot.

Secondly, use dot -Tpdf command to generate the pdf of the CFGs. 

The input is the file named .gcd.dot. 

The output is the pdf file shows the CFGs. 

