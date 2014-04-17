// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#include "emu.h"

void
CPU::Start()
{
  PC = 0xC000;//emu.mem.ReadWord(0xFFFC);
  S = 0xFD;
  P = 0x24;
}

void
CPU::Tick()
{
  uint8_t op;

  op = emu.mem.ReadByte(PC++);
  static int i;

  std::cerr << std::setw(4) << std::dec << i + 1 << " "
            << std::setw(4) << std::hex << PC - 1 << " "
            << std::setw(2) << (int)op
            << std::setw(6) << (int)A << " "
            << std::setw(2) << (int)X << " "
            << std::setw(2) << (int)Y << " "
            << std::setw(2) << (int)(P & 0xEF) << " "
            << std::setw(2) << (int)S << std::endl;
  ++i;
  (this->*(IFunTable[op])) ();
}

// -----------------------------------------------------------------------------
// Stack
// -----------------------------------------------------------------------------
void
CPU::PushWord(uint16_t word)
{
  emu.mem.WriteByte((0x100 | ((S - 0) & 0xFF)), (word & 0xFF00) >> 8);
  emu.mem.WriteByte((0x100 | ((S - 1) & 0xFF)), (word & 0x00FF) >> 0);
  S -= 2;
}

void
CPU::PushByte(uint8_t byte)
{
  emu.mem.WriteByte((0x100 | S--), byte);
}

uint16_t
CPU::PopWord()
{
  uint16_t word;

  word = emu.mem.ReadWord(0x100 | ((S + 1) & 0xFF));
  S += 2;

  return word;
}

uint8_t
CPU::PopByte()
{
  return emu.mem.ReadByte(0x100 | ++S);
}

// -----------------------------------------------------------------------------
// Memory Read
// -----------------------------------------------------------------------------
inline uint8_t
CPU::ReadImmediate(uint16_t &addr)
{
  addr = 0;
  return emu.mem.ReadByte(PC++);
}

inline uint8_t
CPU::ReadImmediate()
{
  return emu.mem.ReadByte(PC++);
}

inline uint8_t
CPU::ReadZeroPage(uint16_t &addr)
{
  addr = emu.mem.ReadByte(PC++);
  return emu.mem.ReadByteZeroPage(addr);
}

inline uint8_t
CPU::ReadAbsolute(uint16_t &addr)
{
  addr = emu.mem.ReadWord(PC);
  PC += 2;
  return emu.mem.ReadByte(addr);
}

inline uint8_t
CPU::ReadIndexedIndirectX(uint16_t &addr)
{
  addr = emu.mem.ReadWordZeroPage((emu.mem.ReadByte(PC++) + X) & 0xFF);
  return emu.mem.ReadByte(addr);
}

inline uint8_t
CPU::ReadIndexedIndirectY(uint16_t &addr)
{
  addr = emu.mem.ReadWordZeroPage((emu.mem.ReadByte(PC++) + Y) & 0xFF);
  return emu.mem.ReadByte(addr);
}

// -----------------------------------------------------------------------------
// Memory Write
// -----------------------------------------------------------------------------
inline void
CPU::WriteAbsolute(uint16_t addr, uint8_t v)
{
  if (!addr)
  {
    addr = emu.mem.ReadWord(PC);
    PC += 2;
  }

  emu.mem.WriteByte(addr, v);
}

void
CPU::WriteZeroPage(uint16_t addr, uint8_t v)
{
  emu.mem.WriteByte(addr ? addr : addr = emu.mem.ReadByte(PC++), v);
}

void
CPU::WriteZeroPageX(uint16_t addr, uint8_t v)
{
  if (!addr)
  {
    addr = (emu.mem.ReadByte(PC++) + X) & 0xFF;
  }

  emu.mem.WriteByte(addr, v);
}

void
CPU::WriteZeroPageY(uint16_t addr, uint8_t v)
{
  if (!addr)
  {
    addr = (emu.mem.ReadByte(PC++) + Y) & 0xFF;
  }

  emu.mem.WriteByte(addr, v);
}

void
CPU::WriteIndexedIndirectX(uint16_t addr, uint8_t v)
{
  if (!addr)
  {
    addr = emu.mem.ReadWordZeroPage((emu.mem.ReadByte(PC++) + X) & 0xFF);
  }

  emu.mem.WriteByte(addr, v);
}

void
CPU::WriteIndexedIndirectY(uint16_t addr, uint8_t v)
{
  if (!addr)
  {
    addr = emu.mem.ReadWordZeroPage((emu.mem.ReadByte(PC++) + Y) & 0xFF);
  }

  emu.mem.WriteByte(addr, v);
}


void
CPU::WriteIndirectIndexedX(uint16_t addr, uint8_t v)
{
  if (!addr)
  {
    addr = emu.mem.ReadWordZeroPage(emu.mem.ReadWord(PC++)) + X;
  }

  emu.mem.WriteByte(addr, v);
}

void
CPU::WriteIndirectIndexedY(uint16_t addr, uint8_t v)
{
  if (!addr)
  {
    addr = emu.mem.ReadWordZeroPage(emu.mem.ReadWord(PC++)) + Y;
  }

  emu.mem.WriteByte(addr, v);
}

// -----------------------------------------------------------------------------
// Other instructions
// -----------------------------------------------------------------------------
void CPU::I00_BRK() { std::cerr << "I00_BRK" << std::endl; }
void CPU::I01_ORA() { Bitwise<&CPU::ReadIndexedIndirectX, bit_or<uint8_t>>(); }
void CPU::I02_KIL() { std::cerr << "I02_KIL" << std::endl; }
void CPU::I03_SLO() { std::cerr << "I03_SLO" << std::endl; }
void CPU::I04_DOP() { std::cerr << "I04_DOP" << std::endl; }
void CPU::I05_ORA() { Bitwise<&CPU::ReadZeroPage, bit_or<uint8_t>>(); }
void CPU::I06_ASL() { std::cerr << "I06_ASL" << std::endl; }
void CPU::I07_SLO() { std::cerr << "I07_SLO" << std::endl; }
void CPU::I08_PHP() { PushByte(P | 0x10); }
void CPU::I09_ORA() { Bitwise<&CPU::ReadImmediate, bit_or<uint8_t>>(); }
void CPU::I0A_ASL() { ASL<&CPU::ReadA, &CPU::WriteA>(); }
void CPU::I0B_AAC() { std::cerr << "I0B_AAC" << std::endl; }
void CPU::I0C_TOP() { std::cerr << "I0C_TOP" << std::endl; }
void CPU::I0D_ORA() { std::cerr << "I0D_ORA" << std::endl; }
void CPU::I0E_ASL() { std::cerr << "I0E_ASL" << std::endl; }
void CPU::I0F_SLO() { std::cerr << "I0F_SLO" << std::endl; }
void CPU::I10_BPL() { Branch<&CPU::ReadN, false>(); }
void CPU::I11_ORA() { std::cerr << "I11_ORA" << std::endl; }
void CPU::I12_KIL() { std::cerr << "I12_KIL" << std::endl; }
void CPU::I13_SLO() { std::cerr << "I13_SLO" << std::endl; }
void CPU::I14_DOP() { std::cerr << "I14_DOP" << std::endl; }
void CPU::I15_ORA() { std::cerr << "I15_ORA" << std::endl; }
void CPU::I16_ASL() { std::cerr << "I16_ASL" << std::endl; }
void CPU::I17_SLO() { std::cerr << "I17_SLO" << std::endl; }
void CPU::I18_CLC() { SetFlag<&CPU::SetC, false>(); }
void CPU::I19_ORA() { std::cerr << "I19_ORA" << std::endl; }
void CPU::I1A_NOP() { std::cerr << "I1A_NOP" << std::endl; }
void CPU::I1B_SLO() { std::cerr << "I1B_SLO" << std::endl; }
void CPU::I1C_TOP() { std::cerr << "I1C_TOP" << std::endl; }
void CPU::I1D_ORA() { std::cerr << "I1D_ORA" << std::endl; }
void CPU::I1E_ASL() { std::cerr << "I1E_ASL" << std::endl; }
void CPU::I1F_SLO() { std::cerr << "I1F_SLO" << std::endl; }
void CPU::I20_JSR() { PushWord(PC + 1); PC = emu.mem.ReadWord(PC); }
void CPU::I21_AND() { Bitwise<&CPU::ReadIndexedIndirectX, bit_and<uint8_t>>(); }
void CPU::I22_KIL() { std::cerr << "I22_KIL" << std::endl; }
void CPU::I23_RLA() { std::cerr << "I23_RLA" << std::endl; }
void CPU::I24_BIT() { BIT<&CPU::ReadZeroPage>(); }
void CPU::I25_AND() { Bitwise<&CPU::ReadZeroPage, bit_and<uint8_t>>(); }
void CPU::I26_ROL() { std::cerr << "I26_ROL" << std::endl; }
void CPU::I27_RLA() { std::cerr << "I27_RLA" << std::endl; }
void CPU::I28_PLP() { P = (PopByte() & 0xEF) | 0x20; }
void CPU::I29_AND() { Bitwise<&CPU::ReadImmediate, bit_and<uint8_t>>(); }
void CPU::I2B_AAC() { std::cerr << "I2B_AAC" << std::endl; }
void CPU::I2C_BIT() { std::cerr << "I2C_BIT" << std::endl; }
void CPU::I2D_AND() { std::cerr << "I2D_AND" << std::endl; }
void CPU::I2E_ROL() { std::cerr << "I2E_ROL" << std::endl; }
void CPU::I2F_RLA() { std::cerr << "I2F_RLA" << std::endl; }
void CPU::I30_BMI() { Branch<&CPU::ReadN, true>(); }
void CPU::I31_AND() { std::cerr << "I31_AND" << std::endl; }
void CPU::I32_KIL() { std::cerr << "I32_KIL" << std::endl; }
void CPU::I33_RLA() { std::cerr << "I33_RLA" << std::endl; }
void CPU::I34_DOP() { std::cerr << "I34_DOP" << std::endl; }
void CPU::I35_AND() { std::cerr << "I35_AND" << std::endl; }
void CPU::I36_ROL() { std::cerr << "I36_ROL" << std::endl; }
void CPU::I37_RLA() { std::cerr << "I37_RLA" << std::endl; }
void CPU::I38_SEC() { SetFlag<&CPU::SetC, true>(); }
void CPU::I39_AND() { std::cerr << "I39_AND" << std::endl; }
void CPU::I3A_NOP() { std::cerr << "I3A_NOP" << std::endl; }
void CPU::I3B_RLA() { std::cerr << "I3B_RLA" << std::endl; }
void CPU::I3C_TOP() { std::cerr << "I3C_TOP" << std::endl; }
void CPU::I3D_AND() { std::cerr << "I3D_AND" << std::endl; }
void CPU::I3E_ROL() { std::cerr << "I3E_ROL" << std::endl; }
void CPU::I3F_RLA() { std::cerr << "I3F_RLA" << std::endl; }
void CPU::I40_RTI() { P = PopByte() | 0x20; PC = PopWord(); }
void CPU::I41_EOR() { Bitwise<&CPU::ReadIndexedIndirectX, bit_xor<uint8_t>>(); }
void CPU::I42_KIL() { std::cerr << "I42_KIL" << std::endl; }
void CPU::I43_SRE() { std::cerr << "I43_SRE" << std::endl; }
void CPU::I44_DOP() { std::cerr << "I44_DOP" << std::endl; }
void CPU::I45_EOR() { Bitwise<&CPU::ReadZeroPage, bit_xor<uint8_t>>(); }
void CPU::I46_LSR() { LSR<&CPU::ReadZeroPage, &CPU::WriteZeroPage>(); }
void CPU::I47_SRE() { std::cerr << "I47_SRE" << std::endl; }
void CPU::I48_PHA() { PushByte(A); }
void CPU::I49_EOR() { Bitwise<&CPU::ReadImmediate, bit_xor<uint8_t>>(); }
void CPU::I4A_LSR() { LSR<&CPU::ReadA, &CPU::WriteA>(); }
void CPU::I4B_ASR() { std::cerr << "I4B_ASR" << std::endl; }
void CPU::I4C_JMP() { PC = emu.mem.ReadWord(PC); }
void CPU::I4D_EOR() { std::cerr << "I4D_EOR" << std::endl; }
void CPU::I4E_LSR() { std::cerr << "I4E_LSR" << std::endl; }
void CPU::I4F_SRE() { std::cerr << "I4F_SRE" << std::endl; }
void CPU::I50_BVC() { Branch<&CPU::ReadV, false>(); }
void CPU::I51_EOR() { std::cerr << "I51_EOR" << std::endl; }
void CPU::I52_KIL() { std::cerr << "I52_KIL" << std::endl; }
void CPU::I53_SRE() { std::cerr << "I53_SRE" << std::endl; }
void CPU::I54_DOP() { std::cerr << "I54_DOP" << std::endl; }
void CPU::I55_EOR() { std::cerr << "I55_EOR" << std::endl; }
void CPU::I56_LSR() { std::cerr << "I56_LSR" << std::endl; }
void CPU::I57_SRE() { std::cerr << "I57_SRE" << std::endl; }
void CPU::I58_CLI() { SetFlag<&CPU::SetI, false>(); }
void CPU::I59_EOR() { std::cerr << "I59_EOR" << std::endl; }
void CPU::I5A_NOP() { std::cerr << "I5A_NOP" << std::endl; }
void CPU::I5B_SRE() { std::cerr << "I5B_SRE" << std::endl; }
void CPU::I5C_TOP() { std::cerr << "I5C_TOP" << std::endl; }
void CPU::I5D_EOR() { std::cerr << "I5D_EOR" << std::endl; }
void CPU::I5E_LSR() { std::cerr << "I5E_LSR" << std::endl; }
void CPU::I5F_SRE() { std::cerr << "I5F_SRE" << std::endl; }
void CPU::I60_RTS() { PC = PopWord() + 1; }
void CPU::I61_ADC() { ADC<&CPU::ReadIndexedIndirectX>(); }
void CPU::I62_KIL() { std::cerr << "I62_KIL" << std::endl; }
void CPU::I63_RRA() { std::cerr << "I63_RRA" << std::endl; }
void CPU::I64_DOP() { std::cerr << "I64_DOP" << std::endl; }
void CPU::I65_ADC() { ADC<&CPU::ReadZeroPage>(); }
void CPU::I66_ROR() { std::cerr << "I66_ROR" << std::endl; }
void CPU::I67_RRA() { std::cerr << "I67_RRA" << std::endl; }
void CPU::I68_PLA() { A = PopByte(); Z = A == 0; N = A & 0x80 ? 1 : 0; }
void CPU::I69_ADC() { ADC<&CPU::ReadImmediate>(); }
void CPU::I6A_ROR() { ROR<&CPU::ReadA, &CPU::WriteA>(); }
void CPU::I2A_ROL() { ROL<&CPU::ReadA, &CPU::WriteA>(); }
void CPU::I6B_ARR() { std::cerr << "I6B_ARR" << std::endl; }
void CPU::I6C_JMP() { std::cerr << "I6C_JMP" << std::endl; }
void CPU::I6D_ADC() { std::cerr << "I6D_ADC" << std::endl; }
void CPU::I6E_ROR() { std::cerr << "I6E_ROR" << std::endl; }
void CPU::I6F_RRA() { std::cerr << "I6F_RRA" << std::endl; }
void CPU::I70_BVS() { Branch<&CPU::ReadV, true>(); }
void CPU::I71_ADC() { std::cerr << "I71_ADC" << std::endl; }
void CPU::I72_KIL() { std::cerr << "I72_KIL" << std::endl; }
void CPU::I73_RRA() { std::cerr << "I73_RRA" << std::endl; }
void CPU::I74_DOP() { std::cerr << "I74_DOP" << std::endl; }
void CPU::I75_ADC() { std::cerr << "I75_ADC" << std::endl; }
void CPU::I76_ROR() { std::cerr << "I76_ROR" << std::endl; }
void CPU::I77_RRA() { std::cerr << "I77_RRA" << std::endl; }
void CPU::I78_SEI() { SetFlag<&CPU::SetI, true>(); }
void CPU::I79_ADC() { std::cerr << "I79_ADC" << std::endl; }
void CPU::I7A_NOP() { std::cerr << "I7A_NOP" << std::endl; }
void CPU::I7B_RRA() { std::cerr << "I7B_RRA" << std::endl; }
void CPU::I7C_TOP() { std::cerr << "I7C_TOP" << std::endl; }
void CPU::I7D_ADC() { std::cerr << "I7D_ADC" << std::endl; }
void CPU::I7E_ROR() { std::cerr << "I7E_ROR" << std::endl; }
void CPU::I7F_RRA() { std::cerr << "I7F_RRA" << std::endl; }
void CPU::I80_DOP() { std::cerr << "I80_DOP" << std::endl; }
void CPU::I81_STA() { Move<&CPU::ReadA, &CPU::WriteIndexedIndirectX, false>(); }
void CPU::I82_DOP() { std::cerr << "I82_DOP" << std::endl; }
void CPU::I83_AAX() { std::cerr << "I83_AAX" << std::endl; }
void CPU::I84_STY() { Move<&CPU::ReadY, &CPU::WriteZeroPage, false>(); }
void CPU::I85_STA() { Move<&CPU::ReadA, &CPU::WriteZeroPage, false>(); }
void CPU::I86_STX() { Move<&CPU::ReadX, &CPU::WriteZeroPage, false>(); }
void CPU::I87_AAX() { std::cerr << "I87_AAX" << std::endl; }
void CPU::I88_DEY() { IncDec<&CPU::ReadY, &CPU::WriteY, -1>(); }
void CPU::I89_DOP() { std::cerr << "I89_DOP" << std::endl; }
void CPU::I8A_TXA() { Move<&CPU::ReadX, &CPU::WriteA>(); }
void CPU::I8B_XAA() { std::cerr << "I8B_XAA" << std::endl; }
void CPU::I8C_STY() { Move<&CPU::ReadY, &CPU::WriteAbsolute, false>(); }
void CPU::I8D_STA() { Move<&CPU::ReadA, &CPU::WriteAbsolute, false>(); }
void CPU::I8E_STX() { Move<&CPU::ReadX, &CPU::WriteAbsolute, false>(); }
void CPU::I8F_AAX() { std::cerr << "I8F_AAX" << std::endl; }
void CPU::I90_BCC() { Branch<&CPU::ReadC, false>(); }
void CPU::I91_STA() { Move<&CPU::ReadA, &CPU::WriteIndirectIndexedY, false>(); }
void CPU::I92_KIL() { std::cerr << "I92_KIL" << std::endl; }
void CPU::I93_AXA() { std::cerr << "I93_AXA" << std::endl; }
void CPU::I94_STY() { std::cerr << "I94_STY" << std::endl; }
void CPU::I95_STA() { Move<&CPU::ReadA, &CPU::WriteZeroPageX, false>(); }
void CPU::I96_STX() { std::cerr << "I96_STX" << std::endl; }
void CPU::I97_AAX() { std::cerr << "I97_AAX" << std::endl; }
void CPU::I98_TYA() { Move<&CPU::ReadY, &CPU::WriteA>(); }
void CPU::I99_STA() { std::cerr << "I99_STA" << std::endl; }
void CPU::I9A_TXS() { Move<&CPU::ReadX, &CPU::WriteS, false>(); }
void CPU::I9B_XAS() { std::cerr << "I9B_XAS" << std::endl; }
void CPU::I9C_SYA() { std::cerr << "I9C_SYA" << std::endl; }
void CPU::I9D_STA() { Move<&CPU::ReadA, &CPU::WriteAbsolute, false>(); }
void CPU::I9E_SXA() { std::cerr << "I9E_SXA" << std::endl; }
void CPU::I9F_AXA() { std::cerr << "I9F_AXA" << std::endl; }
void CPU::IA0_LDY() { Move<&CPU::ReadImmediate, &CPU::WriteY>(); }
void CPU::IA1_LDA() { Move<&CPU::ReadIndexedIndirectX, &CPU::WriteA>(); }
void CPU::IA2_LDX() { Move<&CPU::ReadImmediate, &CPU::WriteX>(); }
void CPU::IA3_LAX() { std::cerr << "IA3_LAX" << std::endl; }
void CPU::IA4_LDY() { Move<&CPU::ReadZeroPage, &CPU::WriteY>(); }
void CPU::IA5_LDA() { Move<&CPU::ReadZeroPage, &CPU::WriteA>(); }
void CPU::IA6_LDX() { Move<&CPU::ReadZeroPage, &CPU::WriteX>(); }
void CPU::IA7_LAX() { std::cerr << "IA7_LAX" << std::endl; }
void CPU::IA8_TAY() { Move<&CPU::ReadA, &CPU::WriteY>(); }
void CPU::IA9_LDA() { Move<&CPU::ReadImmediate, &CPU::WriteA>(); }
void CPU::IAA_TAX() { Move<&CPU::ReadA, &CPU::WriteX>(); }
void CPU::IAB_ATX() { std::cerr << "IAB_ATX" << std::endl; }
void CPU::IAC_LDY() { Move<&CPU::ReadAbsolute, &CPU::WriteY>(); }
void CPU::IAD_LDA() { Move<&CPU::ReadAbsolute, &CPU::WriteA>(); }
void CPU::IAE_LDX() { Move<&CPU::ReadAbsolute, &CPU::WriteX>(); }
void CPU::IAF_LAX() { std::cerr << "IAF_LAX" << std::endl; }
void CPU::IB0_BCS() { Branch<&CPU::ReadC, true>(); }
void CPU::IB1_LDA() { std::cerr << "IB1_LDA" << std::endl; }
void CPU::IB2_KIL() { std::cerr << "IB2_KIL" << std::endl; }
void CPU::IB3_LAX() { std::cerr << "IB3_LAX" << std::endl; }
void CPU::IB4_LDY() { std::cerr << "IB4_LDY" << std::endl; }
void CPU::IB5_LDA() { std::cerr << "IB5_LDA" << std::endl; }
void CPU::IB6_LDX() { std::cerr << "IB6_LDX" << std::endl; }
void CPU::IB7_LAX() { std::cerr << "IB7_LAX" << std::endl; }
void CPU::IB8_CLV() { SetFlag<&CPU::SetV, false>(); }
void CPU::IB9_LDA() { std::cerr << "IB9_LDA" << std::endl; }
void CPU::IBA_TSX() { Move<&CPU::ReadS, &CPU::WriteX>(); }
void CPU::IBB_LAR() { std::cerr << "IBB_LAR" << std::endl; }
void CPU::IBC_LDY() { std::cerr << "IBC_LDY" << std::endl; }
void CPU::IBD_LDA() { std::cerr << "IBD_LDA" << std::endl; }
void CPU::IBE_LDX() { std::cerr << "IBE_LDX" << std::endl; }
void CPU::IBF_LAX() { std::cerr << "IBF_LAX" << std::endl; }
void CPU::IC0_CPY() { CMP<&CPU::ReadY, &CPU::ReadImmediate>(); }
void CPU::IC1_CMP() { CMP<&CPU::ReadA, &CPU::ReadIndexedIndirectX>(); }
void CPU::IC2_DOP() { std::cerr << "IC2_DOP" << std::endl; }
void CPU::IC3_DCP() { std::cerr << "IC3_DCP" << std::endl; }
void CPU::IC4_CPY() { CMP<&CPU::ReadY, &CPU::ReadZeroPage>(); }
void CPU::IC5_CMP() { CMP<&CPU::ReadA, &CPU::ReadZeroPage>(); }
void CPU::IC6_DEC() { IncDec<&CPU::ReadA, &CPU::WriteA, -1>(); }
void CPU::IC7_DCP() { std::cerr << "IC7_DCP" << std::endl; }
void CPU::IC8_INY() { IncDec<&CPU::ReadY, &CPU::WriteY, 1>(); }
void CPU::IC9_CMP() { CMP<&CPU::ReadA, &CPU::ReadImmediate>(); }
void CPU::ICA_DEX() { IncDec<&CPU::ReadX, &CPU::WriteX, -1>(); }
void CPU::ICB_AXS() { std::cerr << "ICB_AXS" << std::endl; }
void CPU::ICC_CPY() { std::cerr << "ICC_CPY" << std::endl; }
void CPU::ICD_CMP() { std::cerr << "ICD_CMP" << std::endl; }
void CPU::ICE_DEC() { std::cerr << "ICE_DEC" << std::endl; }
void CPU::ICF_DCP() { std::cerr << "ICF_DCP" << std::endl; }
void CPU::ID0_BNE() { Branch<&CPU::ReadZ, false>(); }
void CPU::ID1_CMP() { std::cerr << "ID1_CMP" << std::endl; }
void CPU::ID2_KIL() { std::cerr << "ID2_KIL" << std::endl; }
void CPU::ID3_DCP() { std::cerr << "ID3_DCP" << std::endl; }
void CPU::ID4_DOP() { std::cerr << "ID4_DOP" << std::endl; }
void CPU::ID5_CMP() { std::cerr << "ID5_CMP" << std::endl; }
void CPU::ID6_DEC() { std::cerr << "ID6_DEC" << std::endl; }
void CPU::ID7_DCP() { std::cerr << "ID7_DCP" << std::endl; }
void CPU::ID8_CLD() { SetFlag<&CPU::SetD, false>(); }
void CPU::ID9_CMP() { std::cerr << "ID9_CMP" << std::endl; }
void CPU::IDA_NOP() { std::cerr << "IDA_NOP" << std::endl; }
void CPU::IDB_DCP() { std::cerr << "IDB_DCP" << std::endl; }
void CPU::IDC_TOP() { std::cerr << "IDC_TOP" << std::endl; }
void CPU::IDD_CMP() { std::cerr << "IDD_CMP" << std::endl; }
void CPU::IDE_DEC() { std::cerr << "IDE_DEC" << std::endl; }
void CPU::IDF_DCP() { std::cerr << "IDF_DCP" << std::endl; }
void CPU::IE0_CPX() { CMP<&CPU::ReadX, &CPU::ReadImmediate>(); }
void CPU::IE1_SBC() { SBC<&CPU::ReadIndexedIndirectX>(); }
void CPU::IE2_DOP() { std::cerr << "IE2_DOP" << std::endl; }
void CPU::IE3_ISC() { std::cerr << "IE3_ISC" << std::endl; }
void CPU::IE4_CPX() { CMP<&CPU::ReadX, &CPU::ReadZeroPage>(); }
void CPU::IE5_SBC() { SBC<&CPU::ReadZeroPage>(); }
void CPU::IE6_INC() { IncDec<&CPU::ReadZeroPage, &CPU::WriteZeroPage, 1>(); }
void CPU::IE7_ISC() { std::cerr << "IE7_ISC" << std::endl; }
void CPU::IE8_INX() { IncDec<&CPU::ReadX, &CPU::WriteX, 1>(); }
void CPU::IE9_SBC() { SBC<&CPU::ReadImmediate>(); }
void CPU::IEA_NOP() { }
void CPU::IEB_SBC() { std::cerr << "IEB_SBC" << std::endl; }
void CPU::IEC_CPX() { std::cerr << "IEC_CPX" << std::endl; }
void CPU::IED_SBC() { std::cerr << "IED_SBC" << std::endl; }
void CPU::IEE_INC() { std::cerr << "IEE_INC" << std::endl; }
void CPU::IEF_ISC() { std::cerr << "IEF_ISC" << std::endl; }
void CPU::IF0_BEQ() { Branch<&CPU::ReadZ, true>(); }
void CPU::IF1_SBC() { std::cerr << "IF1_SBC" << std::endl; }
void CPU::IF2_KIL() { std::cerr << "IF2_KIL" << std::endl; }
void CPU::IF3_ISC() { std::cerr << "IF3_ISC" << std::endl; }
void CPU::IF4_DOP() { std::cerr << "IF4_DOP" << std::endl; }
void CPU::IF5_SBC() { std::cerr << "IF5_SBC" << std::endl; }
void CPU::IF6_INC() { std::cerr << "IF6_INC" << std::endl; }
void CPU::IF7_ISC() { std::cerr << "IF7_ISC" << std::endl; }
void CPU::IF8_SED() { SetFlag<&CPU::SetD, true>(); }
void CPU::IF9_SBC() { std::cerr << "IF9_SBC" << std::endl; }
void CPU::IFA_NOP() { std::cerr << "IFA_NOP" << std::endl; }
void CPU::IFB_ISC() { std::cerr << "IFB_ISC" << std::endl; }
void CPU::IFC_TOP() { std::cerr << "IFC_TOP" << std::endl; }
void CPU::IFD_SBC() { std::cerr << "IFD_SBC" << std::endl; }
void CPU::IFE_INC() { std::cerr << "IFE_INC" << std::endl; }
void CPU::IFF_ISC() { std::cerr << "IFF_ISC" << std::endl; }

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
