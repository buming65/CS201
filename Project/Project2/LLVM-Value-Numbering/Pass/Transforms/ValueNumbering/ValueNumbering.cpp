#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include <string>
#include <map>
#include <typeinfo>

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "ValueNumbering"

using namespace llvm;

namespace {
struct ValueNumbering : public FunctionPass {
  static char ID;
  ValueNumbering() : FunctionPass(ID) {}
  std::map<llvm::Value*, int> hashmap;
  std::map<std::string, int> expressionmap;
  int vn = 1;
  int vn_expression = 1;

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

  bool runOnFunction(Function &F) override {
    errs() << "ValueNumbering: \n";
    // errs() << F.getName() << "\n";
    
    for (auto& basic_block : F)
    {
      std::string operation;
      llvm::Value* op1;
      llvm::Value* op2;
      int op_1;
      int op_2;
      
      int counter = 1;
      for (auto& inst : basic_block)
      {
        errs() << inst << "\n";
        if (inst.isBinaryOp())
        {
          if(inst.getOpcode() == Instruction::Add){
            operation = "+";
          }
          if(inst.getOpcode() == Instruction::Sub){
            operation = "-";
          }
          if(inst.getOpcode() == Instruction::Mul){
            operation = "*";
          }
          if(inst.getOpcode() == Instruction::UDiv){
            operation = "/";
          }
          int operand_num = inst.getNumOperands();
          if(operand_num==1){
            op1 = inst.getOperand(0);
          }
          if(operand_num==2){
            op1 = inst.getOperand(0);
            op2 = inst.getOperand(1);
          }
          // errs() << op1 << "\t" << op2 << "\n";

          bool found = new bool(false);
          op_1 = searchHash(op1, found);
          op_2 = searchHash(op2, found);

          // errs() << to_string(op_1) << "\t" << to_string(op_2) << "\n";

          if(op_1 > op_2){
            swap(op_1, op_2);
          }

          string expression = to_string(op_1) + operation + to_string(op_2);
          if(searchExpression(expression)){
            errs() << "This Expression is Redundancy:" << inst << "\n";
          }
      }
    }
  }
    return false;
  }
}; // end of struct ValueNumbering
}  // end of anonymous namespace

char ValueNumbering::ID = 0;
static RegisterPass<ValueNumbering> X("ValueNumbering", "ValueNumbering Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);