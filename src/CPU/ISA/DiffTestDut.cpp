#include "../../../Include/DiffTestDut.hpp"

DiffTestDut::DiffTestDut(char *ref_so_file, long img_size, int port, BUS *InBUS, WEMUState *InWEMUState, ISA *InISA)
{
  this->BUSObj = InBUS;
  this->ISAObj = InISA;
  this->WEMUStateObj = InWEMUState;

  assert(ref_so_file != NULL);

  void *handle;
  handle = dlopen(ref_so_file, RTLD_LAZY);
  assert(handle);

  ref_difftest_memcpy = (void (*)(VAddr_t, void *, size_t, bool))dlsym(handle, "difftest_memcpy");
  assert(ref_difftest_memcpy);

  ref_difftest_regcpy = (void (*)(void *dut, bool direction))dlsym(handle, "difftest_regcpy");
  assert(ref_difftest_regcpy);

  ref_difftest_exec = (void (*)(uint64_t n))dlsym(handle, "difftest_exec");
  assert(ref_difftest_exec);

  ref_difftest_raise_intr = (void (*)(uint64_t NO))dlsym(handle, "difftest_raise_intr");
  assert(ref_difftest_raise_intr);

  void (*ref_difftest_init)(int) = (void (*)(int))dlsym(handle, "difftest_init");
  assert(ref_difftest_init);

  ref_difftest_init(port);
  ref_difftest_memcpy(RESET_VECTOR, this->BUSObj->MemoryObj->GuestToHost(RESET_VECTOR), img_size, DIFFTEST_TO_REF);
  ref_difftest_regcpy(&this->ISAObj->CPU_state, DIFFTEST_TO_REF);
}

void DiffTestDut::difftest_skip_ref()
{
  is_skip_ref = true;
  // If such an instruction is one of the instruction packing in QEMU
  // (see below), we end the process of catching up with QEMU's pc to
  // keep the consistent behavior in our best.
  // Note that this is still not perfect: if the packed instructions
  // already write some memory, and the incoming instruction in NEMU
  // will load that memory, we will encounter false negative. But such
  // situation is infrequent.
  skip_dut_nr_inst = 0;
}

void DiffTestDut::difftest_skip_dut(int nr_ref, int nr_dut)
{
  skip_dut_nr_inst += nr_dut;

  while (nr_ref-- > 0)
  {
    ref_difftest_exec(1);
  }
}

void DiffTestDut::difftest_step(VAddr_t pc, VAddr_t npc)
{
  Ref_CPU_state ref_r;

  if (skip_dut_nr_inst > 0)
  {
    ref_difftest_regcpy(&ref_r, DIFFTEST_TO_DUT);

    if (ref_r.pc == npc)
    {
      skip_dut_nr_inst = 0;
      checkregs(&ref_r, npc);
      return;
    }
    skip_dut_nr_inst--;
    if (skip_dut_nr_inst == 0)
      // printf("can not catch up with ref.pc =  %x at pc = " FMT_WORD, ref_r.pc, pc);
      return;
  }

  if (is_skip_ref)
  {
    // to skip the checking of an instruction, just copy the reg state to reference design
    ref_difftest_regcpy(&this->ISAObj->CPU_state, DIFFTEST_TO_REF);
    is_skip_ref = false;
    return;
  }
  ref_difftest_exec(1);

  ref_difftest_regcpy(&ref_r, DIFFTEST_TO_DUT);

  checkregs(&ref_r, pc);
}

void DiffTestDut::checkregs(Ref_CPU_state *ref, VAddr_t pc)
{
  if (!ISAObj->isa_difftest_checkregs(ref, pc))
  {
    WEMUStateObj->state = WEMU_ABORT;
    WEMUStateObj->halt_pc = pc;
    this->ISAObj->DesplayReg();
    }
}

void DiffTestDut::GetLocalCPUState()
{
  for (int i = 0; i < 32; i++)
  {
    this->Dut_CPU_stateObj.gpr[i] = this->ISAObj->Get_gpr(i);
  }
  this->Dut_CPU_stateObj.pc = this->ISAObj->Get_PC();
}