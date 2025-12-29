#include "../../Include/SOC.hpp"

SOC::SOC(char* ref_so_file,WEMUState * InWEMUState,char * img_file,char * dtb_file,ISA * InCPU){
    
    this->BUSObj = new BUS(img_file,dtb_file);
    printf("wqeqweq  = %s\n",ref_so_file);
    this->DiffTestDutObj = new DiffTestDut(ref_so_file,BUSObj->MemoryObj->img_size,1234,BUSObj,InWEMUState,InCPU);
    this->CPUObj = new CPU(InWEMUState,InCPU,DiffTestDutObj);
    
}