void test() { 

  int a, b, c, d, e, f; 

  e = b + c; 

  if (e > 0) {   

    e = a; 

  } else {

    a = b + c; 

  } 

  a = e + c;

}

----- entry -----

UEVAR: b c 

VARKILL: e 

LIVEOUT: a b c e 

----- if.then -----

UEVAR: a 

VARKILL: e 

LIVEOUT: c e 

----- if.else -----

UEVAR: b c 

VARKILL: a 

LIVEOUT: c e 

----- if.end -----

UEVAR: c e 

VARKILL: a 

LIVEOUT: 




clang -Xclang -disable-O0-optnone 1.c -O0 -S -emit-llvm -o 1.ll
opt 1.ll -mem2reg -S -o 1.bc
opt -load ../Pass/build/libLLVMLivenessPass.so -Liveness < 1.bc > /dev/null


clang -Xclang -disable-O0-optnone 2.c -O0 -S -emit-llvm -o 2.ll
opt 2.ll -mem2reg -S -o 2.bc
opt -load ../Pass/build/libLLVMLivenessPass.so -Liveness < 2.bc > /dev/null