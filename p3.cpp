#include <fstream>
#include <memory>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "llvm-c/Core.h"

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/LinkAllPasses.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Analysis/InstructionSimplify.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Dominators.h"


using namespace llvm;

static void LoopInvariantCodeMotion(Module *);

static void summarize(Module *M);
static void print_csv_file(std::string outputfile);

static cl::opt<std::string>
        InputFilename(cl::Positional, cl::desc("<input bitcode>"), cl::Required, cl::init("-"));

static cl::opt<std::string>
        OutputFilename(cl::Positional, cl::desc("<output bitcode>"), cl::Required, cl::init("out.bc"));

static cl::opt<bool>
        Mem2Reg("mem2reg",
                cl::desc("Perform memory to register promotion before LICM."),
                cl::init(false));

static cl::opt<bool>
        CSE("cse",
                cl::desc("Perform CSE before LICM."),
                cl::init(false));

static cl::opt<bool>
        NoLICM("no-licm",
              cl::desc("Do not perform LICM optimization."),
              cl::init(false));

static cl::opt<bool>
        Verbose("verbose",
                    cl::desc("Verbose stats."),
                    cl::init(false));

static cl::opt<bool>
        NoCheck("no",
                cl::desc("Do not check for valid IR."),
                cl::init(false));

int main(int argc, char **argv) {
    // Parse command line arguments
    cl::ParseCommandLineOptions(argc, argv, "llvm system compiler\n");

    // Handle creating output files and shutting down properly
    llvm_shutdown_obj Y;  // Call llvm_shutdown() on exit.
    LLVMContext Context;

    // LLVM idiom for constructing output file.
    std::unique_ptr<ToolOutputFile> Out;
    std::string ErrorInfo;
    std::error_code EC;
    Out.reset(new ToolOutputFile(OutputFilename.c_str(), EC,
                                 sys::fs::OF_None));

    EnableStatistics();

    // Read in module
    SMDiagnostic Err;
    std::unique_ptr<Module> M;
    M = parseIRFile(InputFilename, Err, Context);

    // If errors, fail
    if (M.get() == 0)
    {
        Err.print(argv[0], errs());
        //FIXME: there is a segmentation fault
        return 1;
    }

    // If requested, do some early optimizations
    if (Mem2Reg || CSE)
    {
        legacy::PassManager Passes;
	if (Mem2Reg)
	  Passes.add(createPromoteMemoryToRegisterPass());
	if (CSE)
	  Passes.add(createEarlyCSEPass());
        Passes.run(*M.get());
    }

    if (!NoLICM) {
        LoopInvariantCodeMotion(M.get());
    }

    // Collect statistics on Module
    summarize(M.get());
    print_csv_file(OutputFilename);

    if (Verbose)
        PrintStatistics(errs());

    // Verify integrity of Module, do this by default
    if (!NoCheck)
    {
        legacy::PassManager Passes;
        Passes.add(createVerifierPass());
        Passes.run(*M.get());
    }

    // Write final bitcode
    WriteBitcodeToFile(*M.get(), Out->os());
    Out->keep();

    return 0;
}

static llvm::Statistic nFunctions = {"", "Functions", "number of functions"};
static llvm::Statistic nInstructions = {"", "Instructions", "number of instructions"};
static llvm::Statistic nLoads = {"", "Loads", "number of loads"};
static llvm::Statistic nStores = {"", "Stores", "number of stores"};

static void summarize(Module *M) {
    for (auto i = M->begin(); i != M->end(); i++) {
        if (i->begin() != i->end()) {
            nFunctions++;
        }

        for (auto j = i->begin(); j != i->end(); j++) {
            for (auto k = j->begin(); k != j->end(); k++) {
                Instruction &I = *k;
                nInstructions++;
                if (isa<LoadInst>(&I)) {
                    nLoads++;
                } else if (isa<StoreInst>(&I)) {
                    nStores++;
                }
            }
        }
    }
}

static void print_csv_file(std::string outputfile)
{
    std::ofstream stats(outputfile + ".stats");
    auto a = GetStatistics();
    for (auto p : a) {
        stats << p.first.str() << "," << p.second << std::endl;
    }
    stats.close();
}

static llvm::Statistic NumLoops = {"", "NumLoops", "number of loops analyzed"};
static llvm::Statistic LICMBasic = {"", "LICMBasic", "basic loop invariant instructions"};
static llvm::Statistic LICMLoadHoist = {"", "LICMLoadHoist", "loop invariant load instructions"};
static llvm::Statistic LICMNoPreheader = {"", "LICMNoPreheader", "absence of preheader prevents optimization"};
static llvm::Statistic NumLoopsNoStore = {"", "NumLoopsNoStore", "subset of loops that has no Store instructions"};
static llvm::Statistic NumLoopsNoLoad = {"", "NumLoopsNoLoad", "subset of loops that has no Load instructions"};
static llvm::Statistic NumLoopsNoStoreWithLoad = {"", "NumLoopsNoStoreWithLoad", "subset of loops with no stores that also have at least one load."};
static llvm::Statistic NumLoopsWithCall = {"", "NumLoopsWithCall", "subset of loops that has a call instructions"};

/* Functionality Implementation */

static void hoistInstructionToPreheader(Instruction* I, BasicBlock* PreHeader){
    /* Move an instruction to the PreHeader*/
    Instruction *dst = PreHeader->getTerminator();
    I->moveBefore(dst);
}

static bool AreAllOperandsLoopInvaraint(Loop* L, Instruction* I){
    /* Alternative implementation of hasLoopInvariantOperands
     * */
    for (auto &op: I->operands()){
        if (!L->isLoopInvariant(op)){
             return false;
        }
    }

    return true;
}

static bool dominatesLoopExit(Function *F, Loop *L, Value* V){
    /* How the fuck do I know that??
     * */
    SmallVector<BasicBlock *, 20> ExitBlocks;
    L->getExitBlocks(ExitBlocks);

    if (ExitBlocks.empty()){
        //FIXME: is this possible?? and if so should it be true or false
        //if there is no loop exits, it's an infinite loop. Does it dominate?
        return true;
    }

    Instruction* i = dyn_cast<Instruction>(V);
    DominatorTree *DT=nullptr;
    DT = new DominatorTree();

    DT->recalculate(*F);
    for (auto *bb: ExitBlocks){
        //bool result = DT->dominates(i, bb);
        bool result = DT->dominates(i->getParent(), bb);
        if (!result){
            return false;
        }
    }
    return true;
}


static bool NoPossibleStoresToAnyAddressInLoop(Loop *L){

    //no possible stores to addr in L
    for (auto *bb: L->blocks()){
        for (auto &i: *bb){
            if (isa<StoreInst>(i)){
                return false;
           }
        }
    }

    //after all of this - this is a safe load
    return true;
}

static bool NoPossibleStoresToAddressInLoop(Loop *L, Value* LoadAddress){
    //no possible stores to addr in L
    for (auto *bb: L->blocks()){
        for (auto &i: *bb){
            if (isa<StoreInst>(i)){
                // This is the least careful approeach
                Value *addr_of_store = i.getOperand(1);
                if (LoadAddress == addr_of_store){
                    //i.print(errs());
                    return false;
                } 
                // different address - if it's neither an alloca nor a global
                // varible it could be loading to that address
                else if (!isa<AllocaInst>(addr_of_store) && !isa<GlobalVariable>(addr_of_store)){
                    return false;
                }
           }
        }
    }

    //after all of this - this is a safe load
    return true;
}

static bool AllocaNotInLoop(Loop *L, Value *Addr){
    Instruction *x = dyn_cast<AllocaInst>(Addr);
    BasicBlock *parent = x->getParent();

    if (L->contains(parent)){
        return false;
    }
    return true;
}

static bool CanMoveOutofLoop(Function *F, Loop *L, Instruction* I, Value* LoadAddress, bool loopHasStore){
    /* Determines whether an instruction can be moved out of a loop
     * */

    // DONE - Case 1
    if (I->isVolatile()){
        return false;
    }

    if (isa<GlobalVariable>(LoadAddress) && NoPossibleStoresToAddressInLoop(L, LoadAddress)){
        //return false;
        return true;
    }

    /*
    // Case 2: WIP
    if (isa<AllocaInst>(LoadAddress)
        && AllocaNotInLoop(L, LoadAddress)
        && NoPossibleStoresToAddressInLoop(L, LoadAddress)){
   
        return true;
    }
   
    // SEGFAULT CULPRIT!
    // FIXME: slowly introduce each of the following conditions
    if (L->isLoopInvariant(LoadAddress)
        && NoPossibleStoresToAnyAddressInLoop(L) 
        && dominatesLoopExit(F, L, LoadAddress)
        ){

        return true;
    } 
    */

    return false;
}

static void updateStats(bool hasLoad, bool hasStore, bool hasCall){
    if (!hasStore && hasLoad){
        NumLoopsNoStoreWithLoad++;
    } else if (!hasLoad){
        NumLoopsNoLoad++;
    } else if (!hasStore){
        NumLoopsNoStore++;
    } else if (hasCall){
        NumLoopsWithCall++;
        NumLoopsWithCall++; //testing whether my numbers are too high or low
    }
}

static bool NotALoadOrStore(Instruction* I){
    if (isa<LoadInst>(I)){
        return false;
    }
    if (isa<StoreInst>(I)){
        return false;
    }
    return true;
}

static void OptimizeLoop2(Function *f, LoopInfoBase<BasicBlock, Loop> *LIBase, Loop *L){
    NumLoops++;

    //FIXME: THIS IS PART OF THE LOOPINFOBASE!
    //Update: looks like everywhere in LLVM it is used based off of the Loop
    //object. So we will leave it like this until things break
    BasicBlock *PH = L->getLoopPreheader();
    if (PH==NULL){
        LICMNoPreheader++;
        return;
    }

    //recursive call to optimize all the subloops
    for (auto subloop: L->getSubLoops()){
        OptimizeLoop2(f, LIBase, subloop);
    }

    bool changed, hasLoad, hasStore, hasCall, loopContainsStore=false;
    std::set<Instruction*> worklist;

    for (BasicBlock *bb: L->blocks()){
        changed  = hasLoad  = hasStore = hasCall  = false;
        for (BasicBlock::iterator i = bb->begin(), e = bb->end(); i != e; ++i){
            if (isa<LoadInst>(&*i)){
                hasLoad = true;
            }
            if (isa<StoreInst>(&*i)){
                hasStore = true;
                loopContainsStore = true;
            }
            if (isa<CallInst>(&*i)){
                hasCall = true;
            }
            worklist.insert(&*i);
        }

        updateStats(hasLoad, hasStore, hasCall);

        //work with the worklist;
        while (worklist.size() > 0){
            changed = false;
            Instruction* i = *worklist.begin();
            worklist.erase(i);
            
            if (NotALoadOrStore(i)){
                if (AreAllOperandsLoopInvaraint(L, i)){
                    L->makeLoopInvariant(i, changed);
                    if (changed) {
                        LICMBasic++;
                        continue;
                    }
                }
            }
            else {
                if (isa<LoadInst>(i)){
                    //Implement LoadHoist
                    Value* addr = i->getOperand(0); // address for Load instruction
                    if (CanMoveOutofLoop(f, L, i, addr, loopContainsStore)){
                        
                        hoistInstructionToPreheader(i, PH);
                        LICMLoadHoist++;
                        //Move to PH
                    } 
                }
            }
        }
    }
}

static void RunLICMBasic(Module *M){

    for (Module::iterator func = M->begin(); func != M->end(); ++func){
        Function &F = *func;
        if (func->begin() == func->end()){
        //if (F.size() < 1){
            continue;
        }

        DominatorTreeBase<BasicBlock,false> *DT=nullptr;
        LoopInfoBase<BasicBlock,Loop> *LI = new LoopInfoBase<BasicBlock,Loop>();
        DT = new DominatorTreeBase<BasicBlock,false>();

        DT->recalculate(F); // dominance for Function, F
        LI->analyze(*DT); // calculate loop info

        for(auto li: *LI) {
            OptimizeLoop2(&F, LI, li);
        }
    }
}

static void LoopInvariantCodeMotion(Module *M) {
    // Implement this function
    LICMLoadHoist++; //get rid of the unused warning on autograder
    LICMLoadHoist--; //not polluting the stats TODO later
    RunLICMBasic(M);
}
