#include "llvm/Transforms/Obfuscation/aVMP/aVMPDispatcher.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

using namespace llvm;

Function *buildSharedCallDispatcher(Module *M,
                                    ArrayRef<VMPDispatchTarget> Targets,
                                    GlobalVariable *SharedCodeSegAddr) {
    LLVMContext &Ctx = M->getContext();
    FunctionType *FuncTy = FunctionType::get(Type::getVoidTy(Ctx),
                                             {Type::getInt64Ty(Ctx)}, false);
    Function *Dispatcher = M->getFunction("vmp_shared_call_dispatch");
    if (!Dispatcher) {
        Dispatcher = Function::Create(FuncTy, GlobalValue::InternalLinkage,
                                      "vmp_shared_call_dispatch", M);
        Dispatcher->setSection(".AProtect.text");
    }
    if (!Dispatcher->empty()) {
        Dispatcher->deleteBody();
    }

    BasicBlock *Entry = BasicBlock::Create(Ctx, "entry", Dispatcher);
    BasicBlock *Exit = BasicBlock::Create(Ctx, "exit", Dispatcher);
    IRBuilder<> Builder(Entry);
    Argument *FuncId = Dispatcher->arg_begin();
    Value *CurrentCodeSeg = Builder.CreateLoad(Type::getInt64Ty(Ctx),
                                               SharedCodeSegAddr);

    BasicBlock *Next = nullptr;
    for (const VMPDispatchTarget &Target : Targets) {
        BasicBlock *CallBB = BasicBlock::Create(Ctx, "dispatch.call", Dispatcher);
        Next = BasicBlock::Create(Ctx, "dispatch.next", Dispatcher);
        Value *CodePtr = Builder.CreatePtrToInt(Target.CodeSeg,
                                                Type::getInt64Ty(Ctx));
        Value *Matches = Builder.CreateICmpEQ(CurrentCodeSeg, CodePtr);
        Builder.CreateCondBr(Matches, CallBB, Next);

        IRBuilder<> CallBuilder(CallBB);
        CallBuilder.CreateCall(Target.CallHandler, {FuncId});
        CallBuilder.CreateBr(Exit);

        Builder.SetInsertPoint(Next);
    }
    Builder.CreateBr(Exit);

    IRBuilder<> ExitBuilder(Exit);
    ExitBuilder.CreateRetVoid();
    return Dispatcher;
}
