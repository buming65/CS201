#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/BasicBlock.h"
#include <string>
#include <map>
#include <set>
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <vector>

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "Liveness"

using namespace llvm;

namespace {
struct Liveness : public FunctionPass {
  static char ID;
  Liveness() : FunctionPass(ID) {}

  void computeVarKill(vector<BasicBlock*>& worklist, vector<set<string>>& varkill, vector<set<string>>& uevar){
    for(int i = 0; i< worklist.size(); i++){
      // worklist[i] -> printAsOperand(errs(), false);
      // errs() << "\n";
      for(auto& inst : *worklist[i]){
        if(inst.getOpcode() == Instruction::Load){
          // errs() << "This is Load \n";
          // errs() << inst << "\n";
          Value* op1 = inst.getOperand(0);
          string temp = op1->getName();
          // errs() << temp << "\n";
          if(varkill[i].find(temp) == varkill[i].end()){
            uevar[i].insert(temp);
          }
        }
        if(inst.getOpcode() == Instruction::Store){
          // errs() << "This is Store\n";
          // errs() << inst << "\n";
          Value* op1 = inst.getOperand(1);
          string temp = op1->getName();
          // errs() << temp << "\n";
          varkill[i].insert(temp);
        }
      }
    }
  }


  void computeLiveOut(int id, map<BasicBlock*, int> BB, vector<BasicBlock*>& worklist, vector<set<string>>& varkill, vector<set<string>>& uevar, vector<set<string>>& liveout){
    BasicBlock* b = worklist[id];
    for(BasicBlock *succ : successors(b)){
      auto search = BB.find(succ);
      int i = search->second;

      set<string> dest, dest1;
      set_difference(liveout[i].begin(), liveout[i].end(), varkill[i].begin(), varkill[i].end(), inserter(dest, dest.begin()));
      set_union(dest.begin(), dest.end(), uevar[i].begin(), uevar[i].end(), inserter(dest1, dest1.begin()));
      set_union(dest1.begin(), dest1.end(), liveout[id].begin(), liveout[id].end(), inserter(liveout[id], liveout[id].begin()));
      dest.clear();
      dest1.clear();
    }
  }

  ofstream CreateOut(Function &F){
    std::string file_name = F.getParent()->getSourceFileName();
    file_name = file_name.substr(0, file_name.rfind("."));
    file_name.append(".out");
    ofstream outfile;
    outfile.open(file_name, ios::app|ios::out);
    return outfile;
  }


  bool runOnFunction(Function &F) override{
    errs() << F.getName() << "\n";
    if(F.getName() == "main"){
      return false;
    }
    map<BasicBlock*, int> BB;
    vector<BasicBlock*> worklist;
    int count = 0;

    for(auto& basic_block : F){
      worklist.push_back(&basic_block);
      BB.insert(make_pair(&basic_block, count));
      count++;
    }
    errs() << "worklist" << worklist.size() << "\n";

    vector<set<string>> uevar(count);
    vector<set<string>> varkill(count);
    vector<set<string>> liveout(count);

    computeVarKill(worklist, varkill, uevar);

    bool flag = false;
    while(!flag){
      flag = true;
      for(int i = 0; i < worklist.size(); i++){
        set<string> pre = liveout[i];

        computeLiveOut(i, BB, worklist, varkill, uevar, liveout);
        if(!std::equal(pre.begin(), pre.end(), liveout[i].begin(), liveout[i].end())){
          flag = false;
        }
      }
    }

    ofstream outfile = CreateOut(F);
    for(int i = 0; i <= worklist.size()-1; i++){
      errs() << "i" << i << "\n";
      // errs() << "BasicBlock";
      string temp = worklist[i]->getName();
      errs() << temp << "\n";
      outfile << "----- " << temp << " -----\n";
      // worklist[i] -> printAsOperand(errs(), false);
      errs() << "UEVAR" << "\n";
      outfile << "UEVAR: ";
      for(auto it = uevar[i].begin(); it != uevar[i].end(); it++){
        errs() << *it <<"\n";
        outfile << *it << " ";
      }
      errs() << "\n";
      errs() << "VARKILL" << "\n";
      outfile << "\nVARKILL: ";
      for(auto it = varkill[i].begin(); it != varkill[i].end(); it++){
        errs() << *it <<"\n";
        outfile << *it << " ";
      }
      errs() << "Liveout" << "\n";
      outfile << "\nLIVEOUT: ";
      for(auto it = liveout[i].begin(); it != liveout[i].end(); it++){
        errs() << *it <<"\n";
        outfile << *it << " ";
      }
      errs() << "\n";
      outfile << "\n";
    }

    outfile.close();
    return false;
  }
}; // end of struct ValueNumbering
}  // end of anonymous namespace

char Liveness::ID = 0;
static RegisterPass<Liveness> X("Liveness", "Liveness Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);