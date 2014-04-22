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

  prg[0] = 0xC3;

  // Mark the code as executable, but not writeable
  mprotect(prg, capacity, PROT_READ | PROT_EXEC);
  cpu.S = 100;
}

// -----------------------------------------------------------------------------
void JIT::Execute()
{
  __asm__ __volatile__
    ( // Save registers & C, Z, N
      "movb      %[S], %%al          \n\t"
      "movb      %[A], %%ah          \n\t"
      "movb      %[X], %%bl          \n\t"
      "movb      %[Y], %%bh          \n\t"
      "movb      %[C], %%cl          \n\t"
      "movb      %[Z], %%ch          \n\t"
      "movb      %[N], %%dh          \n\t"

      // Save V, I, D on stack
      "subq      $4, %%rsp           \n\t"
      "movb      %[V], %%dl          \n\t"
      "movb      %%dl, -4(%%rsp)     \n\t"
      "movb      %[I], %%dl          \n\t"
      "movb      %%dl, -3(%%rsp)     \n\t"
      "movb      %[D], %%dl          \n\t"
      "movb      %%dl, -2(%%rsp)     \n\t"

      // Execute the code
      "movq      %[M], %%r8          \n\t"
      "callq     %[P]                \n\t"

      // Restore V, I, D
      "movb      -2(%%rsp), %%dl     \n\t"
      "movb      %%dl, %[D]          \n\t"
      "movb      -3(%%rsp), %%dl     \n\t"
      "movb      %%dl, %[I]          \n\t"
      "movb      -4(%%rsp), %%dl     \n\t"
      "movb      %%dl, %[V]          \n\t"
      "addq      $4, %%rsp           \n\t"

      // Restore registers & C, Z, N
      "movb      %%dh, %[N]          \n\t"
      "movb      %%ch, %[Z]          \n\t"
      "movb      %%cl, %[C]          \n\t"
      "movb      %%bh, %[Y]          \n\t"
      "movb      %%bl, %[X]          \n\t"
      "movb      %%ah, %[A]          \n\t"
      "movb      %%al, %[S]          \n\t"
    : [S] "=m" (cpu.S)       // al
    , [A] "=m" (cpu.A)       // ah
    , [X] "=m" (cpu.X)       // bl
    , [Y] "=m" (cpu.Y)       // bh
    , [C] "=m" (cpu.C)       // cl
    , [Z] "=m" (cpu.Z)       // ch
    , [N] "=m" (cpu.N)       // dl
    , [V] "=r" (cpu.V)       // -12(%rsp)
    , [I] "=r" (cpu.I)       // -11(%rsp)
    , [D] "=r" (cpu.D)       // -10(%rsp)
    : [P] "m" (prg)
    , [M] "m" (mem.memory)   // r8
    : "memory", "cc", "rax", "rbx", "rcx", "rdx", "r8"
    );
}
