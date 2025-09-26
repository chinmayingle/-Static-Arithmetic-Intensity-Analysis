#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>
#include <sstream>

using namespace llvm;
using namespace std;

#define pfel(i) errs() << i << "\n"
#define pf(i) errs() << i
#define pfvar(i) errs() << "%" << i
#define getnamefo(Si) Si->getPointerOperand()->getName().str()
#define pfop(operand)                                                          \
  operand->print(llvm::outs());                                                \
  llvm::outs();
#define pfopel(operand)                                                        \
  operand->print(llvm::outs());                                                \
  llvm::outs() << "\n";
#define N 100

namespace {
void iterateME(Instruction *i) {
  unsigned numOperands = i->getNumOperands();

  for (unsigned ii = 0; ii < numOperands; ++ii) {
    Value *operand = i->getOperand(ii);
    // pfel(operand->print());
    pf("\nindex :");
    pfel(ii);
    pf("operand type:");
    pfopel(operand->getType());
    pf("Plain operand :");
    pfopel(operand);
    pfel("----------------------");
  }
}
bool isInteger(const std::string &str) {
  if (str.empty() ||
      ((!isdigit(str[0])) && (str[0] != '-') && (str[0] != '+'))) {
    return false;
  }

  char *p;
  strtol(str.c_str(), &p, 10);

  return (*p == 0);
}
std::string getresultOperand(Instruction *i) {
  std::string s;
  raw_string_ostream os(s);
  i->printAsOperand(os, false);
  os.flush();
  std::string instructionSubstr = os.str();
  return instructionSubstr;
}
int stringToInt(const std::string &str) {
  if (!isInteger(str)) {
    // Handle error or return a default value
    // std::cerr << "Invalid integer string: " << str << std::endl;
    return 0; // Default value
  }

  int intValue;
  std::istringstream iss(str);
  iss >> intValue;
  return intValue;
}
void visitor(Function &F) {
//   errs() << "Hello from: " << F.getName() << "\n";
//   errs() << "number of arguments: " << F.arg_size() << "\n";
  for (Function::iterator bb = F.begin(), e = F.end(); bb != e; bb++) {
    for (BasicBlock::iterator i = bb->begin(), i2 = bb->end(); i != i2; i++) {
      pfel(*i);
    }
  }
}
void iterateBasicBlocks2(vector<BasicBlock *> &BBs) {

  for (BasicBlock *BB : BBs) {
    // Do something with each BasicBlock pointer 'BB'
    // For example, you can print its name
    outs() << "BasicBlock: " << BB->getName();
  }
  pfel("\n");
}
std::string getVariableNamefromoperand(Value *i) {
  if (llvm::ConstantInt *constantint = llvm::dyn_cast<llvm::ConstantInt>(i)) {

    // pfel("value is constant");
    int value = constantint->getSExtValue();
    return std::to_string(value);
    ;
    pf(value);
    pf("\n");
  } else {
    // pfel("value is variable!!!");

    // Check if the stored value is an instruction
    if (Instruction *storedInstruction = dyn_cast<Instruction>(i)) {

      // Get the name of the variable from the instruction
      std::string variableName = storedInstruction->getName().str();

      // Print the name of the variable
      return getresultOperand(storedInstruction);
      pfel("HMMMMMMMM" << getresultOperand(storedInstruction));
      pfvar(variableName);
      pf("\n");
      return variableName;
      // outs() << "Variable name: " << variableName << "\n";
    } else {
      // The stored value is not an instruction
      outs() << "Stored value is not an instruction.\n";
    }
  }
}

void iterateBasicBlocksVector(const vector<BasicBlock *> &BBs) {
  for (size_t i = 0; i < BBs.size(); ++i) {
    BasicBlock *BB = BBs[i];
    // Do something with each BasicBlock pointer 'BB'
    // For example, you can print its name
    outs() << "\n\tBasicBlock " << i << ": " << BB->getName() << "\n";
    BB->print(llvm::outs());
  }
}
// int handleLoadStoreInstruction(Instruction *I) {
//     if (!I)
//         return INT_MIN;

//     pfel("INSIDE HANDLE LOAD STORE INSTRICTOPNM\t\t"<<*I);

//     if (LoadInst *LI = dyn_cast<LoadInst>(I)) {
//         Value *PointerOperand = LI->getPointerOperand();
//         errs() << "Load Instruction: Loading value from pointer: " << *PointerOperand << "\n";
//         if (IntegerType *IntType = dyn_cast<IntegerType>(PointerOperand->getType())) {
//             // If the loaded value is an integer, print the integer value
//             if (IntType->getBitWidth() == 32) {
//                 if (ConstantInt *CI = dyn_cast<ConstantInt>(PointerOperand)) {
//                     int value = CI->getSExtValue();
//                     errs() << "Loaded Integer Value: " << value << "\n";
//                     return value;
//                 }
//             }
//         } else {
//             // If the loaded value is not an integer, handle it as an instruction
//             return handleLoadStoreInstruction(dyn_cast<Instruction>(PointerOperand));
//         }
//     } else if (StoreInst *SI = dyn_cast<StoreInst>(I)) {
//         Value *StoredValue = SI->getValueOperand();
//         Value *PointerOperand = SI->getPointerOperand();
//         errs() << "Store Instruction: Storing value " << *StoredValue << " to pointer: " << *PointerOperand << "\n";
//         if (!isa<IntegerType>(StoredValue->getType())) {
//             // If the stored value is not an integer, handle it as an instruction
//             return handleLoadStoreInstruction(dyn_cast<Instruction>(StoredValue));
//         }
//     } else {

//         return INT_MIN;
//         // Handle other instruction types if necessary
//     }
// }
// store i32 5, ptr %4, align 4
//   %19 = load i32, ptr %4, align 4
//   %20 = icmp sge i32 %19, 0
// void processCompareInstructionold(Instruction *I) {
//     // Ensure the instruction is a compare instruction
//     if (CmpInst *CI = dyn_cast<CmpInst>(I)) {
//         iterateME(I);
//         // pfel("OP1:::"<<CI->getOperand(0)->getNameOrAsOperand());
//         // pfel("OP2:::"<<CI->getOperand(1)->getNameOrAsOperand());
//         // // Get the operands of the compare instruction
//         Value *Op1 = CI->getOperand(0);
//         Value *Op2 = CI->getOperand(1); 
//         //operands can be an int or an instruction
//         // check if instruction we have to go to definitions
//         pf("Operands are\n");
//         pfopel(Op1);
//         pfopel(Op2);
//         pf("opernads ends");
//         pfel(getVariableNamefromoperand(Op1));
//         pfel(getVariableNamefromoperand(Op2));
        
//         int iop1,iop2;
//         // iop1=handleLoadStoreInstruction(Op1Inst);;
//         // iop2=handleLoadStoreInstruction(Op2Inst);;

//         // Process the operands recursively if they are instructions
//         if (Instruction *Op1Inst = dyn_cast<Instruction>(Op1)) {
//             iop1=handleLoadStoreInstruction(Op1Inst);
//         }
//         if (Instruction *Op2Inst = dyn_cast<Instruction>(Op2)) {
//             iop2=handleLoadStoreInstruction(Op2Inst);
//         }
//         if(iop1==INT_MIN||iop2==INT_MIN)pf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");

//         // Print the values of the operands
//         errs() << "\nOperand 1: " << iop1 << "\n";
//         errs() << "\nOperand 2: " << iop2 << "\n";
//     }
// }




// void processInstruction(Value *V) {
//     if (Instruction *I = dyn_cast<Instruction>(V)) {
//         // Process the operands recursively if they are instructions
//         for (Use &U : I->operands()) {
//             processInstruction(U.get());
//         }

//         // Handle specific instructions
//         if (LoadInst *LI = dyn_cast<LoadInst>(I)) {
//             Value *PointerOperand = LI->getPointerOperand();
//             errs() << "Load Instruction: Loading value from pointer: " << *PointerOperand << "\n";
//         } else if (StoreInst *SI = dyn_cast<StoreInst>(I)) {
//             Value *PointerOperand = SI->getPointerOperand();
//             Value *StoredValue = SI->getValueOperand();
//             errs() << "Store Instruction: Storing value " << *StoredValue << " to pointer: " << *PointerOperand << "\n";
//         } else {
//             // Handle other instruction types
//             errs() << "Instruction: " << *I << "\n";
//         }
//     } else if (ConstantExpr *CE = dyn_cast<ConstantExpr>(V)) {
//         // If the value is a constant expression, process its operands
//         for (Use &U : CE->operands()) {
//             processInstruction(U.get());
//         }
//     } else if (ConstantPointerNull *CPN = dyn_cast<ConstantPointerNull>(V)) {
//         // Handle the case of a constant pointer null
//         errs() << "Operand: null pointer\n";
//     } else {
//         // Handle other cases, such as constants
//         errs() << "Operand: " << *V << "\n";
//     }
// }

void iterateVectorandSetloopBasicblocks(const vector<BasicBlock *> &BBs,
                                        map<BasicBlock *, bool> &inLoop,
                                        vector<int> &rep) {
  for (size_t i = 0; i < BBs.size(); ++i) {
    BasicBlock *BB = BBs[i];
    inLoop[BB] = true;

    for (Instruction &I : *BB) {
      if (I.getOpcode() == Instruction::ICmp) {
        // handleLoadStoreInstruction(&I);
        // processCompareInstructionold(&I);

        if (ICmpInst *icmpInst = dyn_cast<ICmpInst>(&I)) {
          string resultOperand, stringOp1, stringOp2;

          // resultOperand = (getresultOperand(dyn_cast<Instruction>(ist)));
          stringOp1 = (getVariableNamefromoperand(icmpInst->getOperand(0)));
          stringOp2 = (getVariableNamefromoperand(icmpInst->getOperand(1)));

        //   pf(stringOp1 << "???" << stringOp2);

          if (isInteger(stringOp2)) {   
            // int op1 = stringToInt(stringOp1);
            int op2 = stringToInt(stringOp2);
            rep.push_back(op2);
            // pfel("REPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP"
            //      "PP"
            //      "PPPPPPPPPPPPPPPPPPPPPPP"
            //      << i << "-" << rep[i]);
          }
          //icmp sge i32 %19, 0
          // either one operand is int  for increment
          // zero operand then need to take another operand for decrement when comparison is wrt to 0
          //i=0 condition and decrement should be considered 


          // Value *op1 = icmpInst->getOperand(0); // First operand
          // Value *op2 = icmpInst->getOperand(1); // Second operand

          // // Get the integer values of the operands if possible
          // ConstantInt *constOp1 = dyn_cast<ConstantInt>(op1);
          // ConstantInt *constOp2 = dyn_cast<ConstantInt>(op2);
        } else {
          rep.push_back(1);
        }
        // if (*constOp1 && *constOp2) {
        //   // Both operands are constants
        //   int op1Value = constOp1->getSExtValue();
        //   int op2Value = constOp2->getSExtValue();
        //   rep[i] = op2Value;
        //   pfel("REPEATRTIENGPIENGINERPGINERGE IS " << rep[i]);
        // }
      }
      // else if (instCounter.find(I.getOpcodeName()) == instCounter.end())
      //   instCounter[I.getOpcodeName()] = 1;
      // else
      //   instCounter[I.getOpcodeName()] += 1;
    }
    // pfel("output for rep");
    // for (int i = 0; i < rep.size(); i++)
    //   pf("-" << rep[i]);

    // Do something with each BasicBlock pointer 'BB'
    // For example, you can print its name
    // outs() << "\n\tBasicBlock " << i << ": " << BB->getName() << "\n";
    // BB->print(llvm::outs());
  }
}
void dfs_cycle(BasicBlock *u, BasicBlock *p, map<BasicBlock *, int> color,
               map<BasicBlock *, BasicBlock *> par, int &cyclenumber,
               vector<vector<BasicBlock *>> *cycles) {
//   pfel("\n--\n");
//   u->print(llvm::outs());

  // already (completely) visited vertex.
  if (color[u] == 2) {
    return;
  }

  // seen vertex, but was not completely visited -> cycle detected.
  // backtrack based on parents to find the complete cycle.
  if (color[u] == 1) {
    // pfel("LOOPP DETECTED ");
    vector<BasicBlock *> v;
    cyclenumber++;

    BasicBlock *cur = p;
    v.push_back(cur);

    // backtrack the vertex which are
    // in the current cycle thats found
    while (cur != u) {
      cur = par[cur];
      v.push_back(cur);
    }
    // pfel("\t\tadded Cycle start");
    // iterateBasicBlocksVector(v);

    // pfel("\t\t Cycle end");
    cycles->push_back(v);
    return;
  }
  par[u] = p;

  // partially visited.
  color[u] = 1;

  for (BasicBlock *Succ : successors(u)) {
    // if it has not been visited previously
    // if (Succ == par[u]) {
    //     pf("***visited already\n");
    //   continue;
    // }
    dfs_cycle(Succ, u, color, par, cyclenumber, cycles);
    // if (dfs(Succ)) return true; // Stop DFS traversal if cycle found
  }

  // simple dfs on graph
  // for (BasicBlock* v : graph[u]) {

  // }

  // completely visited.
  color[u] = 2;
}
void iterateCycles(const vector<vector<BasicBlock *>> cycles,
                   map<BasicBlock *, bool> &inLoop, vector<int> &rep) {
  // Iterate over each cycle
//   pfel("+++++++++++\t\tCYCLEEEEEEEEEEEEEE" << cycles.size() << "|");

  for (size_t i = 0; i < cycles.size(); ++i) {
    // pf("cycle " << i << "\n");
    // iterateBasicBlocksVector(cycles[i]);
    iterateVectorandSetloopBasicblocks(cycles[i], inLoop, rep);
    // pf("output for rep+");
    // for (int i = 0; i < rep.size(); i++)
    //   pf("-" << rep[i]);
    // Do something with each BasicBlock pointer 'BB'
    // For example, you can print its name
    // outs() << "BasicBlock " << i << ": " << BB->getName() << "\n";
    // BB->print(llvm::outs());
  }
//   pfel("+++++++++++\t\tEND" << cycles.size() << "|");
}

std::map<std::string, int>
iterateLOOPforInstruction(const vector<vector<BasicBlock *>> cycles,
                          vector<int> rep) {
  std::map<std::string, int> instCounter;
  for (size_t i = 0; i < cycles.size(); ++i) {
    // pf("cycle " << i << "\n");

    for (size_t j = 0; j < cycles[i].size(); ++j) {
      BasicBlock *BB = cycles[i][j];
      int itr = rep[i];
      while (itr--) {
        for (Instruction &I : *BB) {
            // pfel("$"<<I);
          if (instCounter.find(I.getOpcodeName()) == instCounter.end())
            instCounter[I.getOpcodeName()] = 1;
          else
            instCounter[I.getOpcodeName()] += 1;
            // pfel("increment is"<<instCounter[I.getOpcodeName()]);
        }
      }

      // Do something with each BasicBlock pointer 'BB'
      // For example, you can print its name
      // outs() << "\n\tBasicBlock " << i << ": " << BB->getName() << "\n";
      // BB->print(llvm::outs());
    }

    // iterateBasicBlocksVector(cycles[i]);
    // iterateVectorandSetloopBasicblocks(cycles[i], inLoop);
    // Do something with each BasicBlock pointer 'BB'
    // For example, you can print its name
    // outs() << "BasicBlock " << i << ": " << BB->getName() << "\n";
    // BB->print(llvm::outs());
    // pfel("loop instrunctions after cycle "<<i<<"\n{{{{{{{{{{{{{{{");
//   for (auto &i : instCounter) {
//     dbgs() << i.first << ": " << i.second << "\n";
//     // if (i.first == "add" || i.first == "sub" || i.first == "mul" ||
//     //     i.first == "div" || i.first == "rem")
//     //   arithmeticOperations += i.second;
//     // else if (i.first == "load" || i.first == "store")
//     //   memoryOperation += i.second;
//   }
//   pf("\n}}}}}}}}}}}}}}}}}}}}}}}}");
  }
  return instCounter;
}
void checkcycle(Function &F) {
  vector<vector<BasicBlock *>> cycles;
  // int color[N];
  map<BasicBlock *, int> color;
  // BasicBlock* par[N];
  map<BasicBlock *, BasicBlock *> par;
  map<BasicBlock *, bool> inLoop;
  vector<int> rep;

  // store the numbers of cycle
  int cyclenumber = 0;
  int edges = 6;

  // call DFS to mark the cycles
  BasicBlock &fi = *F.begin();
  dfs_cycle(&fi, NULL, color, par, cyclenumber, &cycles);

  iterateCycles(cycles, inLoop, rep);
//   pfel("main unctino");

//   for (int i = 0; i < rep.size(); i++)
//     pfel(rep[i]);
  std::map<std::string, int> instCounter, instCounter2;

  for (BasicBlock &BB : F) {
    if (inLoop.find(&BB) == inLoop.end()) {
      for (Instruction &I : BB) {
        if (instCounter.find(I.getOpcodeName()) == instCounter.end())
          instCounter[I.getOpcodeName()] = 1;
        else
          instCounter[I.getOpcodeName()] += 1;
      }
    }
  }
  // for (auto &i : instCounter) {
  //   dbgs() << i.first << ": " << i.second << "\n";
  // }
  // dbgs() << "\n";

  instCounter2 = iterateLOOPforInstruction(cycles, rep);

  int arithmeticOperations, memoryOperation;
  arithmeticOperations = memoryOperation = 0;
//   pfel("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");

  for (auto &i : instCounter) {
    // dbgs() << i.first << ": " << i.second << "\n";
    if (i.first == "add" || i.first == "sub" || i.first == "mul" ||
        i.first == "div" || i.first == "rem")
      arithmeticOperations += i.second;
    else if (i.first == "load" || i.first == "store")
      memoryOperation += i.second;
  }
//   pf("loop instrunctions");
  for (auto &i : instCounter2) {
    // dbgs() << i.first << ": " << i.second << "\n";
    if (i.first == "add" || i.first == "sub" || i.first == "mul" ||
        i.first == "div" || i.first == "rem")
      arithmeticOperations += i.second;
    else if (i.first == "load" || i.first == "store")
      memoryOperation += i.second;
  }

//   pfel("arith" << arithmeticOperations << "memory operation"
//                << memoryOperation);

  float ans = float(arithmeticOperations) / float(memoryOperation);
//   pf("ANSWER IS" << ans);
  pf(ans);

  // cycles has all the cycles as list of blocks

  // TerminatorInst *Terminator = fi->getTerminator();

  // // Check if the terminator instruction has successors
  // if (Terminator->getNumSuccessors() > 0) {
  //     // Get the first successor basic block
  //     return ;
  //     dfs_cycle(Terminator->getSuccessor(0), fi, color, par,
  //     cyclenumber,cycles);
  // } else {
  //     // No successor found
  //     return ;
  // }

  // for (BasicBlock &BB : F) {

  // }
  return;
}

void Inst_Count(Function &F) {
  std::map<std::string, int> instCounter;
//   dbgs() << F.getName() << ": \n";

  for (BasicBlock &BB : F) {
    for (Instruction &I : BB) {
      if (instCounter.find(I.getOpcodeName()) == instCounter.end())
        instCounter[I.getOpcodeName()] = 1;
      else
        instCounter[I.getOpcodeName()] += 1;
    }
  }
//   for (auto &i : instCounter) {
//     dbgs() << i.first << ": " << i.second << "\n";
//   }
//   dbgs() << "\n";
}

struct AI : PassInfoMixin<AI> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
    // visitor(F);
    // Inst_Count(F);
    checkcycle(F);
    return PreservedAnalyses::all();
  }
  static bool isRequired() { return true; }
};

} // namespace
llvm::PassPluginLibraryInfo getAIPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "ArithmaticInt", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "arithmetic-intensity") {
                    FPM.addPass(AI());
                    return true;
                  }
                  return false;
                });
          }};
}
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getAIPluginInfo();
}
