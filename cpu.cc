// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#include "emu.h"

// -----------------------------------------------------------------------------
void CPU::Start()
{
  PC = emu.mem.ReadWord(0xFFFC);
  S = 0xFD;
  C = 0;
  Z = 0;
  I = 1;
  D = 0;
  V = 0;
  N = 0;
}

// -----------------------------------------------------------------------------
void CPU::Tick()
{
  (this->*(IFunTable[emu.mem.ReadByte(PC++)])) ();
}

// -----------------------------------------------------------------------------
inline void CPU::PushWord(uint16_t word)
{
  emu.mem.WriteByte((0x100 | ((S - 0) & 0xFF)), (word & 0xFF00) >> 8);
  emu.mem.WriteByte((0x100 | ((S - 1) & 0xFF)), (word & 0x00FF) >> 0);
  S -= 2;
}

// -----------------------------------------------------------------------------
inline void CPU::PushByte(uint8_t byte)
{
  emu.mem.WriteByte((0x100 | S--), byte);
}

// -----------------------------------------------------------------------------
inline uint16_t CPU::PopWord()
{
  uint16_t word;

  word = emu.mem.ReadWord(0x100 | ((S + 1) & 0xFF));
  S += 2;

  return word;
}

// -----------------------------------------------------------------------------
inline uint8_t CPU::PopByte()
{
  return emu.mem.ReadByte(0x100 | ++S);
}

// -----------------------------------------------------------------------------
template <>
inline void CPU::Instr<CPU::REG_A>(CPU::InstrFunc func, bool r, bool w)
{
  (this->*func) (A);
}

// -----------------------------------------------------------------------------
template <>
inline void CPU::Instr<CPU::REG_X>(CPU::InstrFunc func, bool r, bool w)
{
  (this->*func) (X);
}

// -----------------------------------------------------------------------------
template <>
inline void CPU::Instr<CPU::REG_Y>(CPU::InstrFunc func, bool r, bool w)
{
  (this->*func) (Y);
}

// -----------------------------------------------------------------------------
template <>
inline void CPU::Instr<CPU::IMM>(CPU::InstrFunc func, bool r, bool w)
{
  uint8_t M = emu.mem.ReadByte(PC++);
  (this->*func) (M);
}

// -----------------------------------------------------------------------------
template <>
inline void CPU::Instr<CPU::MEM_ZP>(CPU::InstrFunc func, bool r, bool w)
{
  uint8_t M, addr;

  if (r)
  {
    addr = emu.mem.ReadByte(PC++);
    M = emu.mem.ReadByteZeroPage(addr);
  }

  (this->*func) (M);

  if (w)
  {
    if (!r)
    {
      addr = emu.mem.ReadByte(PC++);
    }

    emu.mem.WriteByteZeroPage(addr, M);
  }
}

// -----------------------------------------------------------------------------
template <>
inline void CPU::Instr<CPU::MEM_ZP_X>(CPU::InstrFunc func, bool r, bool w)
{
  uint8_t M, addr;

  if (r)
  {
    addr = emu.mem.ReadByte(PC++) + X;
    M = emu.mem.ReadByteZeroPage(addr);
  }

  (this->*func) (M);

  if (w)
  {
    if (!r)
    {
      addr = emu.mem.ReadByte(PC++) + X;
    }

    emu.mem.WriteByteZeroPage(addr, M);
  }
}

// -----------------------------------------------------------------------------
template <>
inline void CPU::Instr<CPU::MEM_ZP_Y>(CPU::InstrFunc func, bool r, bool w)
{
  uint8_t M, addr;

  if (r)
  {
    addr = emu.mem.ReadByte(PC++) + Y;
    M = emu.mem.ReadByteZeroPage(addr);
  }

  (this->*func) (M);

  if (w)
  {
    if (!r)
    {
      addr = emu.mem.ReadByte(PC++) + Y;
    }

    emu.mem.WriteByteZeroPage(addr, M);
  }
}

// -----------------------------------------------------------------------------
template <>
inline void CPU::Instr<CPU::MEM_ABS>(CPU::InstrFunc func, bool r, bool w)
{
  uint8_t M;
  uint16_t addr;

  if (r)
  {
    addr = emu.mem.ReadWord(PC), PC += 2;
    M = emu.mem.ReadByte(addr);
  }

  (this->*func) (M);

  if (w)
  {
    if (!r)
    {
      addr = emu.mem.ReadWord(PC), PC += 2;
    }

    emu.mem.WriteByte(addr, M);
  }
}

// -----------------------------------------------------------------------------
template <>
inline void CPU::Instr<CPU::MEM_ABS_X>(CPU::InstrFunc func, bool r, bool w)
{
  uint8_t M;
  uint16_t addr;

  if (r)
  {
    addr = emu.mem.ReadWord(PC) + X, PC += 2;
    M = emu.mem.ReadByte(addr);
  }

  (this->*func) (M);

  if (w)
  {
    if (!r)
    {
      addr = emu.mem.ReadWord(PC) + X, PC += 2;
    }

    emu.mem.WriteByte(addr, M);
  }
}

// -----------------------------------------------------------------------------
template <>
inline void CPU::Instr<CPU::MEM_ABS_Y>(CPU::InstrFunc func, bool r, bool w)
{
  uint8_t M;
  uint16_t addr;

  if (r)
  {
    addr = emu.mem.ReadWord(PC) + Y, PC += 2;
    M = emu.mem.ReadByte(addr);
  }

  (this->*func) (M);

  if (w)
  {
    if (!r)
    {
      addr = emu.mem.ReadWord(PC) + Y, PC += 2;
    }

    emu.mem.WriteByte(addr, M);
  }
}

// -----------------------------------------------------------------------------
template <>
inline void CPU::Instr<CPU::MEM_IDX_IND_X>(CPU::InstrFunc func, bool r, bool w)
{
  uint8_t M;
  uint16_t addr;

  if (r)
  {
    addr = emu.mem.ReadWordZeroPage((emu.mem.ReadByte(PC++) + X) & 0xFF);
    M = emu.mem.ReadByte(addr);
  }

  (this->*func) (M);

  if (w)
  {
    if (!r)
    {
      addr = emu.mem.ReadWordZeroPage((emu.mem.ReadByte(PC++) + X) & 0xFF);
    }

    emu.mem.WriteByte(addr, M);
  }
}

// -----------------------------------------------------------------------------
template <>
inline void CPU::Instr<CPU::MEM_IDX_IND_Y>(CPU::InstrFunc func, bool r, bool w)
{
  uint8_t M;
  uint16_t addr;

  if (r)
  {
    addr = emu.mem.ReadWordZeroPage((emu.mem.ReadByte(PC++) + Y) & 0xFF);
    M = emu.mem.ReadByte(addr);
  }

  (this->*func) (M);

  if (w)
  {
    if (!r)
    {
      addr = emu.mem.ReadWordZeroPage((emu.mem.ReadByte(PC++) + Y) & 0xFF);
    }

    emu.mem.WriteByte(addr, M);
  }
}

// -----------------------------------------------------------------------------
template <>
inline void CPU::Instr<CPU::MEM_IND_IDX_X>(CPU::InstrFunc func, bool r, bool w)
{
  uint8_t M;
  uint16_t addr;

  if (r)
  {
    addr = emu.mem.ReadWordZeroPage(emu.mem.ReadWord(PC++)) + X;
    M = emu.mem.ReadByte(addr);
  }

  (this->*func) (M);

  if (w)
  {
    if (!r)
    {
      addr = emu.mem.ReadWordZeroPage(emu.mem.ReadWord(PC++)) + X;
    }

    emu.mem.WriteByte(addr, M);
  }
}

// -----------------------------------------------------------------------------
template <>
inline void CPU::Instr<CPU::MEM_IND_IDX_Y>(CPU::InstrFunc func, bool r, bool w)
{
  uint8_t M;
  uint16_t addr;

  if (r)
  {
    addr = emu.mem.ReadWordZeroPage(emu.mem.ReadWord(PC++)) + Y;
    M = emu.mem.ReadByte(addr);
  }

  (this->*func) (M);

  if (w)
  {
    if (!r)
    {
      addr = emu.mem.ReadWordZeroPage(emu.mem.ReadWord(PC++)) + Y;
    }

    emu.mem.WriteByte(addr, M);
  }
}

// -----------------------------------------------------------------------------
inline uint8_t CPU::ReadImmediate()
{
  return emu.mem.ReadByte(PC++);
}

// -----------------------------------------------------------------------------
inline uint8_t CPU::ReadImmediate(uint16_t &addr, bool &c)
{
  c = true;
  addr = 0;
  return emu.mem.ReadByte(PC++);
}

// -----------------------------------------------------------------------------
inline uint8_t CPU::ReadZeroPage(uint16_t &addr, bool &c)
{
  c = true;
  addr = emu.mem.ReadByte(PC++);
  return emu.mem.ReadByteZeroPage(addr);
}

// -----------------------------------------------------------------------------
inline uint8_t CPU::ReadZeroPageX(uint16_t &addr, bool &c)
{
  c = true;
  addr = (emu.mem.ReadByte(PC++) + X) & 0xFF;
  return emu.mem.ReadByteZeroPage(addr);
}

// -----------------------------------------------------------------------------
inline uint8_t CPU::ReadZeroPageY(uint16_t &addr, bool &c)
{
  c = true;
  addr = (emu.mem.ReadByte(PC++) + Y) & 0xFF;
  return emu.mem.ReadByteZeroPage(addr);
}

// -----------------------------------------------------------------------------
inline uint8_t CPU::ReadAbsolute(uint16_t &addr, bool &c)
{
  c = true;
  addr = emu.mem.ReadWord(PC), PC += 2;
  return emu.mem.ReadByte(addr);
}

// -----------------------------------------------------------------------------
inline uint8_t CPU::ReadAbsoluteX(uint16_t &addr, bool &c)
{
  c = true;
  addr = emu.mem.ReadWord(PC) + X, PC += 2;
  return emu.mem.ReadByte(addr);
}

// -----------------------------------------------------------------------------
inline uint8_t CPU::ReadAbsoluteY(uint16_t &addr, bool &c)
{
  c = true;
  addr = emu.mem.ReadWord(PC) + Y, PC += 2;
  return emu.mem.ReadByte(addr);
}

// -----------------------------------------------------------------------------
inline uint8_t CPU::ReadIndexedIndirectX(uint16_t &addr, bool &c)
{
  c = true;
  addr = emu.mem.ReadWordZeroPage((emu.mem.ReadByte(PC++) + X) & 0xFF);
  return emu.mem.ReadByte(addr);
}

// -----------------------------------------------------------------------------
inline uint8_t CPU::ReadIndexedIndirectY(uint16_t &addr, bool &c)
{
  c = true;
  addr = emu.mem.ReadWordZeroPage((emu.mem.ReadByte(PC++) + Y) & 0xFF);
  return emu.mem.ReadByte(addr);
}

// -----------------------------------------------------------------------------
inline uint8_t CPU::ReadIndirectIndexedX(uint16_t &addr, bool &c)
{
  c = true;
  addr = emu.mem.ReadWordZeroPage(emu.mem.ReadWord(PC++)) + X;
  return emu.mem.ReadByte(addr);
}

// -----------------------------------------------------------------------------
inline uint8_t CPU::ReadIndirectIndexedY(uint16_t &addr, bool &c)
{
  c = true;
  addr = emu.mem.ReadWordZeroPage(emu.mem.ReadWord(PC++)) + Y;
  return emu.mem.ReadByte(addr);
}


// -----------------------------------------------------------------------------
inline void CPU::WriteZeroPage(uint16_t addr, bool c, uint8_t v)
{
  emu.mem.WriteByte(c ? addr : addr = emu.mem.ReadByte(PC++), v);
}

// -----------------------------------------------------------------------------
inline void CPU::WriteZeroPageX(uint16_t addr, bool c, uint8_t v)
{
  emu.mem.WriteByte(c ? addr : addr = (emu.mem.ReadByte(PC++) + X) & 0xFF, v);
}

// -----------------------------------------------------------------------------
inline void CPU::WriteZeroPageY(uint16_t addr, bool c, uint8_t v)
{
  emu.mem.WriteByte(c ? addr : addr = (emu.mem.ReadByte(PC++) + Y) & 0xFF, v);
}

// -----------------------------------------------------------------------------
inline void CPU::WriteAbsolute(uint16_t addr, bool c, uint8_t v)
{
  if (!c)
  {
    addr = emu.mem.ReadWord(PC);
    PC += 2;
  }

  emu.mem.WriteByte(addr, v);
}

// -----------------------------------------------------------------------------
inline void CPU::WriteAbsoluteX(uint16_t addr, bool c, uint8_t v)
{
  if (!c)
  {
    addr = emu.mem.ReadWord(PC) + X, PC += 2;
  }

  emu.mem.WriteByte(addr, v);
}

// -----------------------------------------------------------------------------
inline void CPU::WriteAbsoluteY(uint16_t addr, bool c, uint8_t v)
{
  if (!c)
  {
    addr = emu.mem.ReadWord(PC) + Y, PC += 2;
  }

  emu.mem.WriteByte(addr, v);
}

// -----------------------------------------------------------------------------
inline void CPU::WriteIndexedIndirectX(uint16_t addr, bool c, uint8_t v)
{
  if (!c)
  {
    addr = emu.mem.ReadWordZeroPage((emu.mem.ReadByte(PC++) + X) & 0xFF);
  }

  emu.mem.WriteByte(addr, v);
}

// -----------------------------------------------------------------------------
inline void CPU::WriteIndexedIndirectY(uint16_t addr, bool c, uint8_t v)
{
  if (!c)
  {
    addr = emu.mem.ReadWordZeroPage((emu.mem.ReadByte(PC++) + Y) & 0xFF);
  }

  emu.mem.WriteByte(addr, v);
}

// -----------------------------------------------------------------------------
inline void CPU::WriteIndirectIndexedX(uint16_t addr, bool c, uint8_t v)
{
  if (!c)
  {
    addr = emu.mem.ReadWordZeroPage(emu.mem.ReadWord(PC++)) + X;
  }

  emu.mem.WriteByte(addr, v);
}

// -----------------------------------------------------------------------------
inline void CPU::WriteIndirectIndexedY(uint16_t addr, bool c, uint8_t v)
{
  if (!c)
  {
    addr = emu.mem.ReadWordZeroPage(emu.mem.ReadWord(PC++)) + Y;
  }

  emu.mem.WriteByte(addr, v);
}

// -----------------------------------------------------------------------------
void CPU::I09_ORA() { Instr<IMM>           (&CPU::ORA, true, false); }
void CPU::I05_ORA() { Instr<MEM_ZP>        (&CPU::ORA, true, false); }
void CPU::I15_ORA() { Instr<MEM_ZP_X>      (&CPU::ORA, true, false); }
void CPU::I0D_ORA() { Instr<MEM_ABS>       (&CPU::ORA, true, false); }
void CPU::I1D_ORA() { Instr<MEM_ABS_X>     (&CPU::ORA, true, false); }
void CPU::I19_ORA() { Instr<MEM_ABS_Y>     (&CPU::ORA, true, false); }
void CPU::I01_ORA() { Instr<MEM_IDX_IND_X> (&CPU::ORA, true, false); }
void CPU::I11_ORA() { Instr<MEM_IND_IDX_Y> (&CPU::ORA, true, false); }

// -----------------------------------------------------------------------------
void CPU::I29_AND() { Instr<IMM>           (&CPU::AND, true, false); }
void CPU::I25_AND() { Instr<MEM_ZP>        (&CPU::AND, true, false); }
void CPU::I35_AND() { Instr<MEM_ZP_X>      (&CPU::AND, true, false); }
void CPU::I2D_AND() { Instr<MEM_ABS>       (&CPU::AND, true, false); }
void CPU::I3D_AND() { Instr<MEM_ABS_X>     (&CPU::AND, true, false); }
void CPU::I39_AND() { Instr<MEM_ABS_Y>     (&CPU::AND, true, false); }
void CPU::I21_AND() { Instr<MEM_IDX_IND_X> (&CPU::AND, true, false); }
void CPU::I31_AND() { Instr<MEM_IND_IDX_Y> (&CPU::AND, true, false); }

// -----------------------------------------------------------------------------
void CPU::I49_EOR() { Instr<IMM>           (&CPU::EOR, true, false); }
void CPU::I45_EOR() { Instr<MEM_ZP>        (&CPU::EOR, true, false); }
void CPU::I55_EOR() { Instr<MEM_ZP_X>      (&CPU::EOR, true, false); }
void CPU::I4D_EOR() { Instr<MEM_ABS>       (&CPU::EOR, true, false); }
void CPU::I5D_EOR() { Instr<MEM_ABS_X>     (&CPU::EOR, true, false); }
void CPU::I59_EOR() { Instr<MEM_ABS_Y>     (&CPU::EOR, true, false); }
void CPU::I41_EOR() { Instr<MEM_IDX_IND_X> (&CPU::EOR, true, false); }
void CPU::I51_EOR() { Instr<MEM_IND_IDX_Y> (&CPU::EOR, true, false); }

// -----------------------------------------------------------------------------
void CPU::I69_ADC() { Instr<IMM>           (&CPU::ADC, true, false); }
void CPU::I65_ADC() { Instr<MEM_ZP>        (&CPU::ADC, true, false); }
void CPU::I75_ADC() { Instr<MEM_ZP_X>      (&CPU::ADC, true, false); }
void CPU::I6D_ADC() { Instr<MEM_ABS>       (&CPU::ADC, true, false); }
void CPU::I7D_ADC() { Instr<MEM_ABS_X>     (&CPU::ADC, true, false); }
void CPU::I79_ADC() { Instr<MEM_ABS_Y>     (&CPU::ADC, true, false); }
void CPU::I61_ADC() { Instr<MEM_IDX_IND_X> (&CPU::ADC, true, false); }
void CPU::I71_ADC() { Instr<MEM_IND_IDX_Y> (&CPU::ADC, true, false); }

// -----------------------------------------------------------------------------
void CPU::IE9_SBC() { Instr<IMM>           (&CPU::SBC, true, false); }
void CPU::IEB_SBC() { Instr<IMM>           (&CPU::SBC, true, false); }
void CPU::IE5_SBC() { Instr<MEM_ZP>        (&CPU::SBC, true, false); }
void CPU::IF5_SBC() { Instr<MEM_ZP_X>      (&CPU::SBC, true, false); }
void CPU::IED_SBC() { Instr<MEM_ABS>       (&CPU::SBC, true, false); }
void CPU::IFD_SBC() { Instr<MEM_ABS_X>     (&CPU::SBC, true, false); }
void CPU::IF9_SBC() { Instr<MEM_ABS_Y>     (&CPU::SBC, true, false); }
void CPU::IE1_SBC() { Instr<MEM_IDX_IND_X> (&CPU::SBC, true, false); }
void CPU::IF1_SBC() { Instr<MEM_IND_IDX_Y> (&CPU::SBC, true, false); }

// -----------------------------------------------------------------------------
void CPU::I0A_ASL() { Instr<REG_A>     (&CPU::ASL); }
void CPU::I06_ASL() { Instr<MEM_ZP>    (&CPU::ASL); }
void CPU::I16_ASL() { Instr<MEM_ZP_X>  (&CPU::ASL); }
void CPU::I0E_ASL() { Instr<MEM_ABS>   (&CPU::ASL); }
void CPU::I1E_ASL() { Instr<MEM_ABS_X> (&CPU::ASL); }

// -----------------------------------------------------------------------------
void CPU::I4A_LSR() { Instr<REG_A>     (&CPU::LSR); }
void CPU::I46_LSR() { Instr<MEM_ZP>    (&CPU::LSR); }
void CPU::I56_LSR() { Instr<MEM_ZP_X>  (&CPU::LSR); }
void CPU::I4E_LSR() { Instr<MEM_ABS>   (&CPU::LSR); }
void CPU::I5E_LSR() { Instr<MEM_ABS_X> (&CPU::LSR); }

// -----------------------------------------------------------------------------
void CPU::I2A_ROL() { Instr<REG_A>     (&CPU::ROL); }
void CPU::I26_ROL() { Instr<MEM_ZP>    (&CPU::ROL); }
void CPU::I36_ROL() { Instr<MEM_ZP_X>  (&CPU::ROL); }
void CPU::I2E_ROL() { Instr<MEM_ABS>   (&CPU::ROL); }
void CPU::I3E_ROL() { Instr<MEM_ABS_X> (&CPU::ROL); }

// -----------------------------------------------------------------------------
void CPU::I6A_ROR() { Instr<REG_A>     (&CPU::ROR); }
void CPU::I66_ROR() { Instr<MEM_ZP>    (&CPU::ROR); }
void CPU::I76_ROR() { Instr<MEM_ZP_X>  (&CPU::ROR); }
void CPU::I6E_ROR() { Instr<MEM_ABS>   (&CPU::ROR); }
void CPU::I7E_ROR() { Instr<MEM_ABS_X> (&CPU::ROR); }

// -----------------------------------------------------------------------------
void CPU::ICA_DEX() { Instr<REG_X>     (&CPU::DEC); }
void CPU::I88_DEY() { Instr<REG_Y>     (&CPU::DEC); }
void CPU::IC6_DEC() { Instr<MEM_ZP>    (&CPU::DEC); }
void CPU::ID6_DEC() { Instr<MEM_ZP_X>  (&CPU::DEC); }
void CPU::ICE_DEC() { Instr<MEM_ABS>   (&CPU::DEC); }
void CPU::IDE_DEC() { Instr<MEM_ABS_X> (&CPU::DEC); }

// -----------------------------------------------------------------------------
void CPU::IE8_INX() { Instr<REG_X>     (&CPU::INC); }
void CPU::IC8_INY() { Instr<REG_Y>     (&CPU::INC); }
void CPU::IE6_INC() { Instr<MEM_ZP>    (&CPU::INC); }
void CPU::IF6_INC() { Instr<MEM_ZP_X>  (&CPU::INC); }
void CPU::IEE_INC() { Instr<MEM_ABS>   (&CPU::INC); }
void CPU::IFE_INC() { Instr<MEM_ABS_X> (&CPU::INC); }

// -----------------------------------------------------------------------------
void CPU::IE7_ISC() { Instr<MEM_ZP>        (&CPU::ISC); }
void CPU::IF7_ISC() { Instr<MEM_ZP_X>      (&CPU::ISC); }
void CPU::IEF_ISC() { Instr<MEM_ABS>       (&CPU::ISC); }
void CPU::IFF_ISC() { Instr<MEM_ABS_X>     (&CPU::ISC); }
void CPU::IFB_ISC() { Instr<MEM_ABS_Y>     (&CPU::ISC); }
void CPU::IE3_ISC() { Instr<MEM_IDX_IND_X> (&CPU::ISC); }
void CPU::IF3_ISC() { Instr<MEM_IND_IDX_Y> (&CPU::ISC); }

// -----------------------------------------------------------------------------
void CPU::IC7_DCP() { Instr<MEM_ZP>        (&CPU::DCP); }
void CPU::ID7_DCP() { Instr<MEM_ZP_X>      (&CPU::DCP); }
void CPU::ICF_DCP() { Instr<MEM_ABS>       (&CPU::DCP); }
void CPU::IDF_DCP() { Instr<MEM_ABS_X>     (&CPU::DCP); }
void CPU::IDB_DCP() { Instr<MEM_ABS_Y>     (&CPU::DCP); }
void CPU::IC3_DCP() { Instr<MEM_IDX_IND_X> (&CPU::DCP); }
void CPU::ID3_DCP() { Instr<MEM_IND_IDX_Y> (&CPU::DCP); }

// -----------------------------------------------------------------------------
void CPU::I07_SLO() { Instr<MEM_ZP>        (&CPU::SLO); }
void CPU::I17_SLO() { Instr<MEM_ZP_X>      (&CPU::SLO); }
void CPU::I0F_SLO() { Instr<MEM_ABS>       (&CPU::SLO); }
void CPU::I1F_SLO() { Instr<MEM_ABS_X>     (&CPU::SLO); }
void CPU::I1B_SLO() { Instr<MEM_ABS_Y>     (&CPU::SLO); }
void CPU::I03_SLO() { Instr<MEM_IDX_IND_X> (&CPU::SLO); }
void CPU::I13_SLO() { Instr<MEM_IND_IDX_Y> (&CPU::SLO); }

// -----------------------------------------------------------------------------
void CPU::I27_RLA() { Instr<MEM_ZP>        (&CPU::RLA); }
void CPU::I37_RLA() { Instr<MEM_ZP_X>      (&CPU::RLA); }
void CPU::I2F_RLA() { Instr<MEM_ABS>       (&CPU::RLA); }
void CPU::I3F_RLA() { Instr<MEM_ABS_X>     (&CPU::RLA); }
void CPU::I3B_RLA() { Instr<MEM_ABS_Y>     (&CPU::RLA); }
void CPU::I23_RLA() { Instr<MEM_IDX_IND_X> (&CPU::RLA); }
void CPU::I33_RLA() { Instr<MEM_IND_IDX_Y> (&CPU::RLA); }

// -----------------------------------------------------------------------------
void CPU::I47_SRE() { Instr<MEM_ZP>        (&CPU::SRE); }
void CPU::I57_SRE() { Instr<MEM_ZP_X>      (&CPU::SRE); }
void CPU::I4F_SRE() { Instr<MEM_ABS>       (&CPU::SRE); }
void CPU::I5F_SRE() { Instr<MEM_ABS_X>     (&CPU::SRE); }
void CPU::I5B_SRE() { Instr<MEM_ABS_Y>     (&CPU::SRE); }
void CPU::I43_SRE() { Instr<MEM_IDX_IND_X> (&CPU::SRE); }
void CPU::I53_SRE() { Instr<MEM_IND_IDX_Y> (&CPU::SRE); }

// -----------------------------------------------------------------------------
void CPU::I67_RRA() { Instr<MEM_ZP>        (&CPU::RRA); }
void CPU::I77_RRA() { Instr<MEM_ZP_X>      (&CPU::RRA); }
void CPU::I6F_RRA() { Instr<MEM_ABS>       (&CPU::RRA); }
void CPU::I7F_RRA() { Instr<MEM_ABS_X>     (&CPU::RRA); }
void CPU::I7B_RRA() { Instr<MEM_ABS_Y>     (&CPU::RRA); }
void CPU::I63_RRA() { Instr<MEM_IDX_IND_X> (&CPU::RRA); }
void CPU::I73_RRA() { Instr<MEM_IND_IDX_Y> (&CPU::RRA); }

// -----------------------------------------------------------------------------
void CPU::IA7_LAX() { Instr<MEM_ZP>        (&CPU::LAX); }
void CPU::IB7_LAX() { Instr<MEM_ZP_X>      (&CPU::LAX); }
void CPU::IAF_LAX() { Instr<MEM_ABS>       (&CPU::LAX); }
void CPU::IBF_LAX() { Instr<MEM_ABS_Y>     (&CPU::LAX); }
void CPU::IA3_LAX() { Instr<MEM_IDX_IND_X> (&CPU::LAX); }
void CPU::IB3_LAX() { Instr<MEM_IND_IDX_Y> (&CPU::LAX); }

// -----------------------------------------------------------------------------
void CPU::I24_BIT() { Instr<MEM_ZP>  (&CPU::BIT, true, false); }
void CPU::I2C_BIT() { Instr<MEM_ABS> (&CPU::BIT, true, false); }

// -----------------------------------------------------------------------------
void CPU::I87_AAX() { Instr<MEM_ZP>        (&CPU::AAX); }
void CPU::I97_AAX() { Instr<MEM_ZP_Y>      (&CPU::AAX); }
void CPU::I83_AAX() { Instr<MEM_IDX_IND_X> (&CPU::AAX); }
void CPU::I8F_AAX() { Instr<MEM_ABS>       (&CPU::AAX); }

// -----------------------------------------------------------------------------
void CPU::I93_AXA() { Instr<MEM_IND_IDX_Y> (&CPU::AXA); }
void CPU::I9F_AXA() { Instr<MEM_ABS_Y>     (&CPU::AXA); }

// -----------------------------------------------------------------------------
void CPU::IC9_CMP() { CMP<&CPU::ReadA, &CPU::ReadImmediate>(); }
void CPU::IC5_CMP() { CMP<&CPU::ReadA, &CPU::ReadZeroPage>(); }
void CPU::ID5_CMP() { CMP<&CPU::ReadA, &CPU::ReadZeroPageX>(); }
void CPU::ICD_CMP() { CMP<&CPU::ReadA, &CPU::ReadAbsolute>(); }
void CPU::IDD_CMP() { CMP<&CPU::ReadA, &CPU::ReadAbsoluteX>(); }
void CPU::ID9_CMP() { CMP<&CPU::ReadA, &CPU::ReadAbsoluteY>(); }
void CPU::IC1_CMP() { CMP<&CPU::ReadA, &CPU::ReadIndexedIndirectX>(); }
void CPU::ID1_CMP() { CMP<&CPU::ReadA, &CPU::ReadIndirectIndexedY>(); }
void CPU::IC0_CPY() { CMP<&CPU::ReadY, &CPU::ReadImmediate>(); }
void CPU::IC4_CPY() { CMP<&CPU::ReadY, &CPU::ReadZeroPage>(); }
void CPU::ICC_CPY() { CMP<&CPU::ReadY, &CPU::ReadAbsolute>(); }
void CPU::IE0_CPX() { CMP<&CPU::ReadX, &CPU::ReadImmediate>(); }
void CPU::IE4_CPX() { CMP<&CPU::ReadX, &CPU::ReadZeroPage>(); }
void CPU::IEC_CPX() { CMP<&CPU::ReadX, &CPU::ReadAbsolute>(); }

// -----------------------------------------------------------------------------
void CPU::I81_STA() { Move<&CPU::ReadA, &CPU::WriteIndexedIndirectX, false>(); }
void CPU::I85_STA() { Move<&CPU::ReadA, &CPU::WriteZeroPage,         false>(); }
void CPU::I8D_STA() { Move<&CPU::ReadA, &CPU::WriteAbsolute,         false>(); }
void CPU::I91_STA() { Move<&CPU::ReadA, &CPU::WriteIndirectIndexedY, false>(); }
void CPU::I95_STA() { Move<&CPU::ReadA, &CPU::WriteZeroPageX,        false>(); }
void CPU::I99_STA() { Move<&CPU::ReadA, &CPU::WriteAbsoluteY,        false>(); }
void CPU::I9D_STA() { Move<&CPU::ReadA, &CPU::WriteAbsoluteX,        false>(); }
void CPU::I84_STY() { Move<&CPU::ReadY, &CPU::WriteZeroPage,         false>(); }
void CPU::I8C_STY() { Move<&CPU::ReadY, &CPU::WriteAbsolute,         false>(); }
void CPU::I94_STY() { Move<&CPU::ReadY, &CPU::WriteZeroPageX,        false>(); }
void CPU::I86_STX() { Move<&CPU::ReadX, &CPU::WriteZeroPage,         false>(); }
void CPU::I8E_STX() { Move<&CPU::ReadX, &CPU::WriteAbsolute,         false>(); }
void CPU::I96_STX() { Move<&CPU::ReadX, &CPU::WriteZeroPageY,        false>(); }

// -----------------------------------------------------------------------------
void CPU::IA1_LDA() { Move<&CPU::ReadIndexedIndirectX, &CPU::WriteA>(); }
void CPU::IA5_LDA() { Move<&CPU::ReadZeroPage,         &CPU::WriteA>(); }
void CPU::IA9_LDA() { Move<&CPU::ReadImmediate,        &CPU::WriteA>(); }
void CPU::IAD_LDA() { Move<&CPU::ReadAbsolute,         &CPU::WriteA>(); }
void CPU::IB1_LDA() { Move<&CPU::ReadIndirectIndexedY, &CPU::WriteA>(); }
void CPU::IB5_LDA() { Move<&CPU::ReadZeroPageX,        &CPU::WriteA>(); }
void CPU::IB9_LDA() { Move<&CPU::ReadAbsoluteY,        &CPU::WriteA>(); }
void CPU::IBD_LDA() { Move<&CPU::ReadAbsoluteX,        &CPU::WriteA>(); }
void CPU::IA0_LDY() { Move<&CPU::ReadImmediate,        &CPU::WriteY>(); }
void CPU::IA4_LDY() { Move<&CPU::ReadZeroPage,         &CPU::WriteY>(); }
void CPU::IAC_LDY() { Move<&CPU::ReadAbsolute,         &CPU::WriteY>(); }
void CPU::IB4_LDY() { Move<&CPU::ReadZeroPageX,        &CPU::WriteY>(); }
void CPU::IBC_LDY() { Move<&CPU::ReadAbsoluteX,        &CPU::WriteY>(); }
void CPU::IA2_LDX() { Move<&CPU::ReadImmediate,        &CPU::WriteX>(); }
void CPU::IA6_LDX() { Move<&CPU::ReadZeroPage,         &CPU::WriteX>(); }
void CPU::IAE_LDX() { Move<&CPU::ReadAbsolute,         &CPU::WriteX>(); }
void CPU::IB6_LDX() { Move<&CPU::ReadZeroPageY,        &CPU::WriteX>(); }
void CPU::IBE_LDX() { Move<&CPU::ReadAbsoluteY,        &CPU::WriteX>(); }

// -----------------------------------------------------------------------------
void CPU::I8A_TXA() { Move<&CPU::ReadX, &CPU::WriteA>(); }
void CPU::IAA_TAX() { Move<&CPU::ReadA, &CPU::WriteX>(); }
void CPU::I98_TYA() { Move<&CPU::ReadY, &CPU::WriteA>(); }
void CPU::IA8_TAY() { Move<&CPU::ReadA, &CPU::WriteY>(); }
void CPU::IBA_TSX() { Move<&CPU::ReadS, &CPU::WriteX>(); }
void CPU::I9A_TXS() { S = X; }

// -----------------------------------------------------------------------------
void CPU::IEA_NOP() { }

// -----------------------------------------------------------------------------
void CPU::I10_BPL() { Branch(!N); }
void CPU::I50_BVC() { Branch(!V); }
void CPU::I90_BCC() { Branch(!C); }
void CPU::ID0_BNE() { Branch(!Z); }
void CPU::I30_BMI() { Branch(N); }
void CPU::I70_BVS() { Branch(V); }
void CPU::IB0_BCS() { Branch(C); }
void CPU::IF0_BEQ() { Branch(Z); }

// -----------------------------------------------------------------------------
void CPU::I18_CLC() { C = false; }
void CPU::I58_CLI() { I = false; }
void CPU::IB8_CLV() { V = false; }
void CPU::ID8_CLD() { D = false; }
void CPU::I38_SEC() { C = true; }
void CPU::I78_SEI() { I = true; }
void CPU::IF8_SED() { D = true; }

// -----------------------------------------------------------------------------
void CPU::I1A_NOP() { }
void CPU::I3A_NOP() { }
void CPU::I5A_NOP() { }
void CPU::I7A_NOP() { }
void CPU::IDA_NOP() { }
void CPU::IFA_NOP() { }

// -----------------------------------------------------------------------------
void CPU::I14_DOP() { PC++; }
void CPU::I04_DOP() { PC++; }
void CPU::I34_DOP() { PC++; }
void CPU::I44_DOP() { PC++; }
void CPU::I54_DOP() { PC++; }
void CPU::I64_DOP() { PC++; }
void CPU::I74_DOP() { PC++; }
void CPU::I80_DOP() { PC++; }
void CPU::I82_DOP() { PC++; }
void CPU::I89_DOP() { PC++; }
void CPU::IC2_DOP() { PC++; }
void CPU::ID4_DOP() { PC++; }
void CPU::IE2_DOP() { PC++; }
void CPU::IF4_DOP() { PC++; }

// -----------------------------------------------------------------------------
void CPU::I0C_TOP() { PC += 2; }
void CPU::I1C_TOP() { PC += 2; }
void CPU::I3C_TOP() { PC += 2; }
void CPU::I5C_TOP() { PC += 2; }
void CPU::I7C_TOP() { PC += 2; }
void CPU::IDC_TOP() { PC += 2; }
void CPU::IFC_TOP() { PC += 2; }

// -----------------------------------------------------------------------------
void CPU::I02_KIL() { std::cout << "I02_KIL" << std::endl; exit(0); }
void CPU::I12_KIL() { std::cout << "I12_KIL" << std::endl; exit(0); }
void CPU::I22_KIL() { std::cout << "I22_KIL" << std::endl; exit(0); }
void CPU::I32_KIL() { std::cout << "I32_KIL" << std::endl; exit(0); }
void CPU::I42_KIL() { std::cout << "I42_KIL" << std::endl; exit(0); }
void CPU::I52_KIL() { std::cout << "I52_KIL" << std::endl; exit(0); }
void CPU::I62_KIL() { std::cout << "I62_KIL" << std::endl; exit(0); }
void CPU::I72_KIL() { std::cout << "I72_KIL" << std::endl; exit(0); }
void CPU::I92_KIL() { std::cout << "I92_KIL" << std::endl; exit(0); }
void CPU::IB2_KIL() { std::cout << "IB2_KIL" << std::endl; exit(0); }
void CPU::ID2_KIL() { std::cout << "ID2_KIL" << std::endl; exit(0); }
void CPU::IF2_KIL() { std::cout << "IF2_KIL" << std::endl; exit(0); }

// -----------------------------------------------------------------------------
void CPU::I00_BRK()
{
  PushWord(PC + 1);
  PushByte(GetP() | 0x10);
  I = 1;
  PC = emu.mem.ReadWord(0xFFFE);
}

// -----------------------------------------------------------------------------
void CPU::I20_JSR()
{
  PushWord(PC + 1); PC = emu.mem.ReadWord(PC);
}

// -----------------------------------------------------------------------------
void CPU::I60_RTS()
{
  PC = PopWord() + 1;
}

// -----------------------------------------------------------------------------
void CPU::I40_RTI()
{
  uint8_t P = PopByte();
  C = P & 0x01 ? 1 : 0;
  Z = P & 0x02 ? 1 : 0;
  I = P & 0x04 ? 1 : 0;
  D = P & 0x08 ? 1 : 0;
  V = P & 0x40 ? 1 : 0;
  N = P & 0x80 ? 1 : 0;
  PC = PopWord();
}

// -----------------------------------------------------------------------------
void CPU::I4C_JMP()
{
  PC = emu.mem.ReadWord(PC);
}

// -----------------------------------------------------------------------------
void CPU::I6C_JMP()
{
  uint16_t b0, b1;
  b0 = emu.mem.ReadWord(PC);
  b1 = (b0 & 0xFF00) | ((b0 + 1) & 0xFF);
  PC = emu.mem.ReadByte(b0) | (emu.mem.ReadByte(b1) << 8);
}

// -----------------------------------------------------------------------------
void CPU::I08_PHP()
{
  uint8_t P = 0x30;
  P |= C ? 0x01 : 0x00;
  P |= Z ? 0x02 : 0x00;
  P |= I ? 0x04 : 0x00;
  P |= D ? 0x08 : 0x00;
  P |= V ? 0x40 : 0x00;
  P |= N ? 0x80 : 0x00;
  PushByte(P);
}

// -----------------------------------------------------------------------------
void CPU::I28_PLP()
{
  uint8_t P = PopByte();
  C = P & 0x01 ? 1 : 0;
  Z = P & 0x02 ? 1 : 0;
  I = P & 0x04 ? 1 : 0;
  D = P & 0x08 ? 1 : 0;
  V = P & 0x40 ? 1 : 0;
  N = P & 0x80 ? 1 : 0;
}

// -----------------------------------------------------------------------------
void CPU::I48_PHA()
{
  PushByte(A);
}

// -----------------------------------------------------------------------------
void CPU::I68_PLA()
{
  A = PopByte();
  Z = A == 0;
  N = A & 0x80 ? 1 : 0;
}

// -----------------------------------------------------------------------------
void CPU::I0B_AAC()
{
  A = A & ReadImmediate();
  Z = A == 0;
  N = C = A & 0x80 ? 1 : 0;
}

// -----------------------------------------------------------------------------
void CPU::I2B_AAC()
{
  A = A & ReadImmediate();
  Z = A == 0;
  N = C = A & 0x80 ? 1 : 0;
}

// -----------------------------------------------------------------------------
void CPU::I4B_ASR()
{
  A = A & ReadImmediate();
  C = A & 0x01;
  A >>= 1;
  Z = A == 0;
  N = 0;
}

// -----------------------------------------------------------------------------
void CPU::I6B_ARR()
{
  A = A & ReadImmediate();
  A = (A >> 1) | (C << 7);
  N = A & 0x80 ? 1 : 0;
  Z = A == 0;
  C = A & 0x40 ? 1 : 0;
  V = C ^ (A & 0x20 ? 1 : 0);
}

// -----------------------------------------------------------------------------
void CPU::IAB_ATX()
{
  X = A = ReadImmediate();
  Z = X == 0;
  N = X & 0x80 ? 1 : 0;
}

// -----------------------------------------------------------------------------
void CPU::ICB_AXS()
{
  uint16_t r;

  r = (A & X) - ReadImmediate();

  C = !(r & 0xFF00);
  Z = (r & 0xFF) == 0;
  N = (r & 0x80) ? 1 : 0;

  X = r;
}

// -----------------------------------------------------------------------------
void CPU::I9C_SYA()
{
  uint16_t arg;
  uint8_t M;

  arg = emu.mem.ReadWord(PC);
  PC += 2;
  M = Y & (((arg & 0xFF00) >> 8) + 1);
  emu.mem.WriteByte(arg + X, M);
}

// -----------------------------------------------------------------------------
void CPU::I9E_SXA()
{
  uint16_t arg;
  uint8_t M;

  arg = emu.mem.ReadWord(PC);
  PC += 2;
  M = X & (((arg & 0xFF00) >> 8) + 1);
  emu.mem.WriteByte(arg + Y, M);
}

// -----------------------------------------------------------------------------
void CPU::I8B_XAA()
{
  std::cout << "I8B_XAA" << std::endl; exit(0);
}

// -----------------------------------------------------------------------------
void CPU::I9B_XAS()
{
  std::cout << "I9B_XAS" << std::endl; exit(0);
}

// -----------------------------------------------------------------------------
void CPU::IBB_LAR()
{
  std::cout << "IBB_LAR" << std::endl; exit(0);
}

// -----------------------------------------------------------------------------
#define I(r, c, i) [0x##r##c] = &CPU::I##r##c##_##i,
CPU::IFunPtr CPU::IFunTable[] =
{
  ROWL(0, BRK, ORA, KIL, SLO, DOP, ORA, ASL, SLO)
  ROWH(0, PHP, ORA, ASL, AAC, TOP, ORA, ASL, SLO)
  ROWL(1, BPL, ORA, KIL, SLO, DOP, ORA, ASL, SLO)
  ROWH(1, CLC, ORA, NOP, SLO, TOP, ORA, ASL, SLO)
  ROWL(2, JSR, AND, KIL, RLA, BIT, AND, ROL, RLA)
  ROWH(2, PLP, AND, ROL, AAC, BIT, AND, ROL, RLA)
  ROWL(3, BMI, AND, KIL, RLA, DOP, AND, ROL, RLA)
  ROWH(3, SEC, AND, NOP, RLA, TOP, AND, ROL, RLA)
  ROWL(4, RTI, EOR, KIL, SRE, DOP, EOR, LSR, SRE)
  ROWH(4, PHA, EOR, LSR, ASR, JMP, EOR, LSR, SRE)
  ROWL(5, BVC, EOR, KIL, SRE, DOP, EOR, LSR, SRE)
  ROWH(5, CLI, EOR, NOP, SRE, TOP, EOR, LSR, SRE)
  ROWL(6, RTS, ADC, KIL, RRA, DOP, ADC, ROR, RRA)
  ROWH(6, PLA, ADC, ROR, ARR, JMP, ADC, ROR, RRA)
  ROWL(7, BVS, ADC, KIL, RRA, DOP, ADC, ROR, RRA)
  ROWH(7, SEI, ADC, NOP, RRA, TOP, ADC, ROR, RRA)
  ROWL(8, DOP, STA, DOP, AAX, STY, STA, STX, AAX)
  ROWH(8, DEY, DOP, TXA, XAA, STY, STA, STX, AAX)
  ROWL(9, BCC, STA, KIL, AXA, STY, STA, STX, AAX)
  ROWH(9, TYA, STA, TXS, XAS, SYA, STA, SXA, AXA)
  ROWL(A, LDY, LDA, LDX, LAX, LDY, LDA, LDX, LAX)
  ROWH(A, TAY, LDA, TAX, ATX, LDY, LDA, LDX, LAX)
  ROWL(B, BCS, LDA, KIL, LAX, LDY, LDA, LDX, LAX)
  ROWH(B, CLV, LDA, TSX, LAR, LDY, LDA, LDX, LAX)
  ROWL(C, CPY, CMP, DOP, DCP, CPY, CMP, DEC, DCP)
  ROWH(C, INY, CMP, DEX, AXS, CPY, CMP, DEC, DCP)
  ROWL(D, BNE, CMP, KIL, DCP, DOP, CMP, DEC, DCP)
  ROWH(D, CLD, CMP, NOP, DCP, TOP, CMP, DEC, DCP)
  ROWL(E, CPX, SBC, DOP, ISC, CPX, SBC, INC, ISC)
  ROWH(E, INX, SBC, NOP, SBC, CPX, SBC, INC, ISC)
  ROWL(F, BEQ, SBC, KIL, ISC, DOP, SBC, INC, ISC)
  ROWH(F, SED, SBC, NOP, ISC, TOP, SBC, INC, ISC)
};
