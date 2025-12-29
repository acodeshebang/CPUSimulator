#ifndef __SOC_H__
#define __SOC_H__
#include "Memory.hpp"
#include "CPU.hpp"
#include "BUS.hpp"
#include "DiffTestDut.hpp"
class SOC{
    private:
        DiffTestDut * DiffTestDutObj;
    public:
    
        CPU * CPUObj;
        BUS * BUSObj;
        WEMUState * WEMUStateObj;
        SOC(char* ref_so_file,WEMUState * InWEMUState,char * img_file,char * dtb_file,ISA * InCPU);
        
};

#endif