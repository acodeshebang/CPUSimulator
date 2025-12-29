#ifndef __CPUEXEC_H__
#define __CPUEXEC_H__
#include "ISA.hpp"
#include "Memory.hpp"
#include "BUS.hpp"
#include "WEMUState.hpp"

class CPU
{
private:
    DiffTestDut *DiffTestDutObj;
    WEMUState *WEMUStateObj;
    void exec_once();
    void execute(uint64_t n);

public:
    CPU(WEMUState *InWEMUState, ISA *InCPU, DiffTestDut *InDiffTestDutObj);
    void cpu_exec(uint64_t n);
    ISA *ISAObj;
    void statistic();
};

#endif