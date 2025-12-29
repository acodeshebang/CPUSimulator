#include "../../Include/CPU.hpp"

CPU::CPU(WEMUState * InWEMUState,ISA * InCPU,DiffTestDut *InDiffTestDutObj)
{
    this->ISAObj = InCPU;
    this->WEMUStateObj = InWEMUState;
    this->DiffTestDutObj = InDiffTestDutObj;
}

void CPU::exec_once()
{
    this->ISAObj->isa_exec_once();
    
    this->DiffTestDutObj->difftest_step(this->ISAObj->Get_PC(),this->ISAObj->Get_PC());
    
}
void CPU::execute(uint64_t n)
{
    for (; n > 0; n--)
    {
        exec_once();
        if (WEMUStateObj->state != WEMU_RUNNING)
            break;
    }
}
void CPU::cpu_exec(uint64_t n)
{
    switch (WEMUStateObj->state) {
        case WEMU_END: case WEMU_ABORT:
          printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
          return;
        default: WEMUStateObj->state = WEMU_RUNNING;
      }
    execute(n);
    switch (WEMUStateObj->state) {
        case WEMU_RUNNING: WEMUStateObj->state = WEMU_STOP; break;
    
        case WEMU_END: case WEMU_ABORT:
          printf("CPUSimulator: %s at pc = %x\n" ,
              (WEMUStateObj->state == WEMU_ABORT ? ANSI_FMT("ABORT", ANSI_FG_RED) :
               (WEMUStateObj->halt_ret == 0 ? ANSI_FMT("HIT GOOD TRAP", ANSI_FG_GREEN) :
                ANSI_FMT("HIT BAD TRAP", ANSI_FG_RED))),
                WEMUStateObj->halt_pc);
          
          // fall through
        case WEMU_QUIT: statistic();
      }
}


void CPU::statistic(){

}