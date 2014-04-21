// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#include "emu.h"

// -----------------------------------------------------------------------------
JIT::JIT(Emulator &emu)
  : cpu(emu.cpu)
  , mem(emu.mem)
  , addr(0)
  , capacity(0)
  , size(0)
  , cycles(0)
  , prg(nullptr)
{
  Create();
}

// -----------------------------------------------------------------------------
JIT::~JIT()
{
}

// -----------------------------------------------------------------------------
void JIT::Create()
{
  capacity = 0x1000; // 4Kb
  size = 0;
  cycles = 0;

  if (!(prg = (uint8_t*)mmap(NULL, capacity, PROT_WRITE | PROT_READ,
                             MAP_ANONYMOUS | MAP_PRIVATE, 0, 0)))
  {
    throw std::runtime_error("Cannot allocate executable memory");
  }
}

// -----------------------------------------------------------------------------
void JIT::Execute()
{
  prg[0] = 0xC3;

  // Mark the code as executable, but not writeable
  mprotect(prg, capacity, PROT_READ | PROT_EXEC);

  cpu.Dump(std::cout);

  __asm__ __volatile__
    ( // Save work registers
      "pushq      %%rbx             \n\t"
      "pushq      %%rcx             \n\t"
      "pushq      %%rdx             \n\t"
      "pushq      %%rsi             \n\t"
      "pushq      %%rdi             \n\t"
      "pushq      %%r8              \n\t"
      "pushq      %%r9              \n\t"
      "pushq      %%r10             \n\t"
      "pushq      %%r11             \n\t"
      "pushq      %%r12             \n\t"
      "pushfq                       \n\t"

      // Transfer data to host
      "pushfw                       \n\t"
      "orb       %[V], %%cl         \n\t"
      "shlw      $4, %%cx           \n\t"
      "orb       %[N], %%cl         \n\t"
      "shlw      $1, %%cx           \n\t"
      "orb       %[Z], %%cl         \n\t"
      "shlw      $6, %%cx           \n\t"
      "orb       %[C], %%cl         \n\t"
      "andw      $0xF73E, (%%rsp)   \n\t"
      "orw       %%cx, (%%rsp)      \n\t"
      "popfw                        \n\t"

      // Execute the code
      "call      *%0                \n\t"

      // Retrieve data from host
      "setcb     %[C]               \n\t"
      "setzb     %[Z]               \n\t"
      "setob     %[V]               \n\t"
      "setsb     %[N]               \n\t"

      // Restore work registers
      "popfq                        \n\t"
      "popq       %%r12             \n\t"
      "popq       %%r11             \n\t"
      "popq       %%r10             \n\t"
      "popq       %%r9              \n\t"
      "popq       %%r8              \n\t"
      "popq       %%rdi             \n\t"
      "popq       %%rsi             \n\t"
      "popq       %%rdx             \n\t"
      "popq       %%rcx             \n\t"
      "popq       %%rbx             \n\t"
    :
    : "a" (prg)
    , [M] "m" (mem.memory)    // rsi
    , [S] "m" (cpu.S)     // rdi
    , [A] "m" (cpu.A)     // r8b
    , [X] "m" (cpu.X)     // r9b
    , [Y] "m" (cpu.Y)     // r10b
    , [I] "m" (cpu.I)     // r11b
    , [D] "m" (cpu.D)     // r12b
    , [C] "m" (cpu.C)     // EFLAGS
    , [Z] "m" (cpu.Z)     // EFLAGS
    , [V] "m" (cpu.V)     // EFLAGS
    , [N] "m" (cpu.N)     // EFLAGS
    : "memory"
    );

  cpu.Dump(std::cout);
}
