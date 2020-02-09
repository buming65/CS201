# Pass-LLVM-Value-Numbering
Writing a pass to find all the LVN-identifiable redundancy. Project 2 from CS201 UCR.

## Setup(From HelloPass) 
1. Clone this repo, see the file details below:
- [Pass](https://github.com/ufarooq/HelloPass-LLVM/tree/master/Pass "Pass") - root directory for build and source code
   - [Transforms](https://github.com/ufarooq/HelloPass-LLVM/tree/master/Pass/Transforms "Transforms") is top level directory for project.
   - [CMakeLists.txt](https://github.com/ufarooq/HelloPass-LLVM/blob/master/Pass/Transforms/CMakeLists.txt "CMakeLists.txt")  file is CMakeLists for the Project.
     - [ValueNumbering](https://github.com/ufarooq/HelloPass-LLVM/tree/master/Pass/Transforms/ValueNumbering "ValueNumbering") is top level directory for the Pass
	     - [ValueNumbering.cpp](https://github.com/ufarooq/HelloPass-LLVM/blob/master/Pass/Transforms/ValueNumbering/ValueNumbering.cpp "ValueNumbering.cpp") contains code for the Pass
	     - [CMakeLists.txt](https://github.com/ufarooq/HelloPass-LLVM/blob/master/Pass/Transforms/ValueNumbering/CMakeLists.txt "CMakeLists.txt") is CMakeLists for this Pass
   - [build](https://github.com/ufarooq/HelloPass-LLVM/tree/master/Pass/build "build") will build Pass in this directory
- [test](https://github.com/ufarooq/HelloPass-LLVM/tree/master/test "test") contains Test code
2. For Mac OSX users, uncomment the following line in Pass/Transforms/ValueNumbering/CMakeLists.txt
```bash
SET(CMAKE_MODULE_LINKER_FLAGS "-undefined dynamic_lookup")
```
3. Move to [Pass](https://github.com/ufarooq/HelloPass-LLVM/tree/master/Pass)/**build**/ directory using cd command on your local system. Next, execute the following command. If it executes successfully, proceed to next step.
```bash
cmake -DCMAKE_BUILD_TYPE=Release ../Transforms/ValueNumbering
```
4. Next execute make and it will generate *.so files under build directory. 
```bash
make -j4
```
5. Move to **test**/ directory and generate ***Test.ll*** file for Test.c using following command. 
```c++
clang -Xclang -disable-O0-optnone Test.c -O0 -S -emit-llvm -o Test.ll
```
6. Next generate ***Test.bc*** file for Test.ll using following command. 
```c++
opt Test.ll -mem2reg -S -o Test.bc
```
7. After generating test.bc, execute following command it execute the LLVM Pass. 
```bash
opt -load ../Pass/build/libLLVMValueNumberingPass.so  -ValueNumbering < Test.bc > /dev/null
```
## Code Explanation 
1. The implemented Pass extends from ``FunctionPass`` class and overrides ``runOnFunction(Function &F)`` function. And it basically finds the redundancy expression in basic block.

2. In this implement, I used two hash_table, the first one is to store the operand of each expression, the second one is to store the expression in string format.

   ```c++
   std::map<llvm::Value*, int> hashmap;
   std::map<std::string, int> expressionmap;
   ```

3. For hashmap which stores the operands. There is a function to judge whether this operand in the hashmap or not. And for each operand, it'll have one unique value number.

   ```c++
     int searchHash(Value* v, bool found){
       auto search = hashmap.find(v);
       auto temp = vn;
       // errs() << v;
       if(search != hashmap.end()){
         temp = search->second;
         found = true;
         errs() << "true\t";
       }
       else{
         found = false;
         errs() << "false\t";
         hashmap.insert(make_pair(v, vn++));
       }
       return temp;
     }
   ```

4. Also, there would be the situation that the expression is same but the operands are in different places. So I swap these two value numbers. And use the value numbers and the operation symbol to represent the expression.

   ```C++
     op_1 = searchHash(op1, found);
     op_2 = searchHash(op2, found);
   
     // errs() << to_string(op_1) << "\t" << to_string(op_2) << "\n";
   
     if(op_1 > op_2){
       swap(op_1, op_2);
     }
   
     string expression = to_string(op_1) + operation + to_string(op_2);
   ```

5. If the expression is in the expression map, it will return true, means this expression is redundant.

   ```c++
     bool searchExpression(string exp){
       auto search = expressionmap.find(exp);
       auto temp = vn_expression;
       errs() << "\n" << exp << "\n";
       if(search != expressionmap.end()){
         return true;
       }
       else{
         expressionmap.insert(make_pair(exp, vn_expression++));
         return false;
       }
     }
   
   ```
## Tests

1. For the c program below:

   ```c++
   
   void test(int a, int b, int c, int d, int e, int f) {
   	c = a + b;
   	d = a * c;
   	e = a;
   	a = a + 1;
   	d = a + b;
   	f = e + b;
   	f = e * f;
   }
   
   int main() {
   	test(1,2,3,4,5,6);
   	return 0;
   }
   
   ```

   The result is :

   ![image-20200209033214106](/Users/buming/Library/Application Support/typora-user-images/image-20200209033214106.png)

2. For c program below:

   ```c++
   void test(int a, int b, int c, int d, int e, int f) {
   	a = 1;
   	b = 2;
   	c = a + b;
   	d = b + a;
   	e = a * b;
   	f = b * a;
   	c = c * c;
   	f = f * e;
   }
   
   int main() {
   	test(1,2,3,4,5,6);
   	return 0;
   }
   
   ```

   The result is:

   ![image-20200209033450720](/Users/buming/Library/Application Support/typora-user-images/image-20200209033450720.png)