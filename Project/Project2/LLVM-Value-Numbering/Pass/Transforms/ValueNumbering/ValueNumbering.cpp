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
#include <iostream>
#include <fstream>

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
  // int vn_expression = 1;

  int searchHash(Value* v, bool *found){
    auto search = hashmap.find(v);
    auto temp = vn;
    // errs() << v;
    if(search != hashmap.end()){
      temp = search->second;
      *found = true;
      // errs() << "true\t";
    }
    else{
      *found = false;
      // errs() << "false\t";
      hashmap.insert(make_pair(v, vn++));
    }
    return temp;
  }

  int searchExpression(string exp, bool *found){
    auto search = expressionmap.find(exp);
    auto temp = vn;
    // vn_expression = vn;
    // errs() << "\n" << exp << "\n";
    if(search != expressionmap.end()){
      temp = search->second;
      *found = true;
      // errs() << "true\t";
    }
    else{
      expressionmap.insert(make_pair(exp, vn++));
      *found = false;
      // errs() << "false\t";
    }
    return temp;
  }

  ofstream CreateOut(Function &F){
    std::string file_name = F.getParent()->getSourceFileName();
    file_name = file_name.substr(0, file_name.rfind("."));
    file_name.append(".out");
    ofstream outfile;
    outfile.open(file_name, ios::app|ios::out);
    return outfile;
  }

  bool runOnFunction(Function &F) override {
    ofstream outfile = CreateOut(F);

    errs() << "ValueNumbering: \n";
    // errs() << F.getName() << "\n";
    
    for (auto& basic_block : F)
    {
      std::string operation;
      llvm::Value* op1;
      llvm::Value* op2;
      llvm::Value* op3;
      int op_1;
      int op_2;
      int op_temp_1;
      int op_temp_2;
      int op_3;
      int op_expression;
      
      int counter = 1;
      for (auto& inst : basic_block)
      {
        // errs() << inst << "\n";
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

          bool *found = new bool(false);
          op_1 = searchHash(op1, found);
          op_2 = searchHash(op2, found);

          // errs() << to_string(op_1) << "\t" << to_string(op_2) << "\n";
          op_temp_1 = op_1;
          op_temp_2 = op_2;

          if(op_1 > op_2){
            swap(op_temp_1, op_temp_2);
          }

          string expression = to_string(op_temp_1) + operation + to_string(op_temp_2);
          op_expression = searchExpression(expression, found);

          op3 = dyn_cast<Value>(&inst);
          auto search_expression = hashmap.find(op3);
          if(search_expression != hashmap.end()){
            op_3 = search_expression->second;
          }
          else{
            hashmap.insert(make_pair(op3, op_expression));
            op_3 = op_expression;
          }

          if(*found){
            errs() << "This Expression is Redundancy:" << inst << "\n";
            // vn--;
          }
          // op3 = dyn_cast<Value>(&inst);
          // op_3 = searchHash(op3, found);
          errs() << op_3 << "=" << op_1 << operation << op_2 << "\n";
          outfile << op_3 << "=" << op_1 << operation << op_2 << "\n";
      }
    }
  }
    outfile.close();
    return false;
  }
}; // end of struct ValueNumbering
}  // end of anonymous namespace

char ValueNumbering::ID = 0;
static RegisterPass<ValueNumbering> X("ValueNumbering", "ValueNumbering Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);