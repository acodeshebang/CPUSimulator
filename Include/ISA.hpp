#ifndef __ISA_H__
#define __ISA_H__
#include "common.hpp"
#include "Memory.hpp"
#include "DiffTestDut.hpp"
typedef struct
{
    Word_t gpr[32];
    VAddr_t pc;
    Word_t mepc;    // 存放触发异常的PC
    Word_t mstatus; // 存放处理器的状态
    Word_t mcause;  // 存放触发异常的原因
    Word_t mtvec;   // 存放异常入口地址
} CPU_stateType;
class ISA{

    public:
        virtual VAddr_t Get_PC()=0;
        virtual int isa_exec_once()=0;
        virtual void DesplayReg()=0;
        virtual void DesplayCSR()=0;
        virtual Word_t Get_gpr(int n)=0;
        virtual bool isa_difftest_checkregs(Ref_CPU_state *ref_r, VAddr_t pc)=0;
        CPU_stateType CPU_state;
       
        

};

#endif