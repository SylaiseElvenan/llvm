#include "llvm/ADT/Statistic.h"
#include "llvm/Pass.h"
#include "llvm/Analysis/Passes.h"

#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/Instructions.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/InstIterator.h"

using namespace llvm;

#define DEBUG_TYPE "BranchInstCount"

STATISTIC(NumCondBranch, "Number of conditional branches in the program");
STATISTIC(NumUncondBranch, "Number of unconditional branches in the program");
STATISTIC(NumEqBranch, "Number of conditional branches whose comparison type is equal test");
STATISTIC(NumGTBranch, "Number of conditional branches whose comparison type is greater than test");
STATISTIC(NumLTBranch, "Number of conditional branches whose comparison type is less than test");

namespace {
	class BranchInstCount : public FunctionPass {

	public:
		static char ID;
		BranchInstCount() : FunctionPass(ID) {}

		virtual bool runOnFunction(Function &F) {
			for (BasicBlock &BB : F) {
				for (Instruction &I : BB) {

					BranchInst* BI = dyn_cast<BranchInst>(&I);

					if (BI && BI->isConditional()) {

						NumCondBranch++;

						CmpInst* CI = dyn_cast<CmpInst>(BI->getCondition());

						if (CI) {
							switch (CI->getPredicate()) {

							case CmpInst::ICMP_EQ:
							case CmpInst::FCMP_OEQ:
							case CmpInst::FCMP_UEQ:
								NumEqBranch++;
								break;

							case CmpInst::ICMP_SGT:
							case CmpInst::ICMP_UGT:
							case CmpInst::ICMP_SGE:
							case CmpInst::ICMP_UGE:
							case CmpInst::FCMP_OGT:
							case CmpInst::FCMP_UGT:
							case CmpInst::FCMP_OGE:
							case CmpInst::FCMP_UGE:
								NumGTBranch++;
								break;

							case CmpInst::ICMP_SLT:
							case CmpInst::ICMP_ULT:
							case CmpInst::ICMP_SLE:
							case CmpInst::ICMP_ULE:
							case CmpInst::FCMP_OLT:
							case CmpInst::FCMP_ULT:
							case CmpInst::FCMP_OLE:
							case CmpInst::FCMP_ULE:
								NumLTBranch++;
								break;

							default:
								break;
							}
						}
					}
					else if (BI && BI->isUnconditional()) {
						NumUncondBranch++;
					}
				}
			}

			errs() << "NumUncondBranch = " << NumUncondBranch << "\n";
			errs() << "NumCondBranch = " << NumCondBranch << "\n";
			errs() << "NumEqBranch = " << NumEqBranch << "\n";
			errs() << "NumGTBranch = " << NumGTBranch << "\n";
			errs() << "NumLTBranch = " << NumLTBranch << "\n";

			return false;
		}

		virtual void getAnalysisUsage(AnalysisUsage &AU) const {
			AU.setPreservesAll();
		}
	};
}

char BranchInstCount::ID = 0;
static RegisterPass<BranchInstCount> X("branchinstcount", "BranchInstCount Pass", false, true);
