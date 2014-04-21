// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#include "emu.h"

// -----------------------------------------------------------------------------
JIT::~JIT()
{
  for (auto seg : segments)
  {
    if (seg.second != nullptr)
    {
      delete seg.second;
    }
  }

  segments.clear();
}

// -----------------------------------------------------------------------------
JIT::Segment& JIT::CreateSegment(uint16_t addr)
{
  Segment *seg = new Segment;
  seg->addr     = addr;
  seg->capacity = 0x1000; // 4Kb
  seg->size     = 0;
  seg->cycles   = 0;
  seg->mem      = (uint8_t*)mmap(NULL, seg->capacity, PROT_WRITE,
                                 MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);

  if (!seg->mem)
  {
    delete seg;
    throw std::runtime_error("Cannot allocate executable memory");
  }

  segments.insert(make_pair(addr, seg));
  return *seg;
}

// -----------------------------------------------------------------------------
void JIT::Execute(JIT::Segment &seg)
{
  seg.mem[0] = 0xC3;

  mprotect(seg.mem, seg.capacity, PROT_READ | PROT_EXEC);
  ((Exec)seg.mem)();
}
