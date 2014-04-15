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

void
CPU::PushWord(uint16_t word)
{
  emu.mem.WriteByte((0x100 | ((S - 0) & 0xFF)), (word & 0xFF00) >> 8);
  emu.mem.WriteByte((0x100 | ((S - 1) & 0xFF)), (word & 0x00FF) >> 0);
  S -= 2;
}

uint16_t
CPU::PopWord()
{
  uint16_t word;

  word = emu.mem.ReadWord(0x100 | ((S + 1) & 0xFF));
  S += 2;

  return word;
}

// -----------------------------------------------------------------------------
// Flag helpers
// -----------------------------------------------------------------------------
#define SETZ(expr) Z = ((expr) != 0) ? 1 : 0;
#define SETN(expr) N = ((expr) != 0) ? 1 : 0;
#define SETV(expr) V = ((expr) != 0) ? 1 : 0;
#define SETC(expr) C = ((expr) != 0) ? 1 : 0;

#define CLEAR(name, flag)\
void \
CPU::name()\
{\
  flag = 0;\
}

CLEAR(I18_CLC, C);
CLEAR(I58_CLI, I);
CLEAR(IB8_CLV, V);
CLEAR(ID8_CLD, D);

#define SET(name, flag)\
void \
CPU::name()\
{\
  flag = 1;\
}

SET(I38_SEC, C);
SET(I78_SEI, I);
SET(IF8_SED, D);

// -----------------------------------------------------------------------------
// Branch instructions
// -----------------------------------------------------------------------------
#define BRANCH(name, flag)\
void \
CPU::name()\
{\
  PC = PC + 1 + (flag ? ((int8_t)emu.mem.ReadByte(PC)) : 0);\
}

BRANCH(I10_BPL, !N);
BRANCH(I30_BMI,  N);
BRANCH(I50_BVC, !V);
BRANCH(I70_BVS,  V);
BRANCH(I90_BCC, !C);
BRANCH(IB0_BCS,  C);
BRANCH(ID0_BNE, !Z);
BRANCH(IF0_BEQ,  Z);

// -----------------------------------------------------------------------------
// Store zero page
// -----------------------------------------------------------------------------
#define STOREZP(name, expr)\
void \
CPU::name()\
{\
  emu.mem.WriteByteZeroPage(emu.mem.ReadByte(PC++), expr);\
}

STOREZP(I84_STY, Y);
STOREZP(I85_STA, A);
STOREZP(I86_STX, X);

// -----------------------------------------------------------------------------
// Binary operators on immediate values
// -----------------------------------------------------------------------------
#define BINARY_IMM(name, op, reg)\
void \
CPU::name()\
{\
  reg = reg op emu.mem.ReadByte(PC++);\
  SETZ(reg == 0x00);\
  SETN(reg & 0x80);\
}

BINARY_IMM(I09_ORA, |, A);
BINARY_IMM(I29_AND, &, A);
BINARY_IMM(I49_EOR, ^, A);

// -----------------------------------------------------------------------------
// Stack operations
// -----------------------------------------------------------------------------
#define PUSH(name, reg)\
void \
CPU::name()\
{\
  emu.mem.WriteByte(S | 0x100, reg);\
  S = (S - 1) & 0xFF;\
}

PUSH(I08_PHP, P | 0x10);
PUSH(I48_PHA, A);

void
CPU::I28_PLP()
{
  S = (S + 1) & 0xFF;
  P = (emu.mem.ReadByte(S | 0x100) & 0xEF) | 0x20;
}


void
CPU::I68_PLA()
{
  S = (S + 1) & 0xFF;
  A = emu.mem.ReadByte(S | 0x100);
  SETZ(A == 0);
  SETN(A & 0x80);
}

// -----------------------------------------------------------------------------
// Other instructions
// -----------------------------------------------------------------------------
void
CPU::I00_BRK()
{
  std::cerr << "I00_BRK" << std::endl;
  exit(0);
}

void
CPU::I01_ORA()
{
  std::cerr << "I01_ORA" << std::endl;
  exit(0);
}

void
CPU::I02_KIL()
{
  std::cerr << "I02_KIL" << std::endl;
  exit(0);
}

void
CPU::I03_SLO()
{
  std::cerr << "I03_SLO" << std::endl;
  exit(0);
}

void
CPU::I04_DOP()
{
  std::cerr << "I04_DOP" << std::endl;
  exit(0);
}

void
CPU::I05_ORA()
{
  std::cerr << "I05_ORA" << std::endl;
  exit(0);
}

void
CPU::I06_ASL()
{
  std::cerr << "I06_ASL" << std::endl;
  exit(0);
}

void
CPU::I07_SLO()
{
  std::cerr << "I07_SLO" << std::endl;
  exit(0);
}

void
CPU::I0A_ASL()
{
  std::cerr << "I0A_ASL" << std::endl;
  exit(0);
}

void
CPU::I0B_AAC()
{
  std::cerr << "I0B_AAC" << std::endl;
  exit(0);
}

void
CPU::I0C_TOP()
{
  std::cerr << "I0C_TOP" << std::endl;
  exit(0);
}

void
CPU::I0D_ORA()
{
  std::cerr << "I0D_ORA" << std::endl;
  exit(0);
}

void
CPU::I0E_ASL()
{
  std::cerr << "I0E_ASL" << std::endl;
  exit(0);
}

void
CPU::I0F_SLO()
{
  std::cerr << "I0F_SLO" << std::endl;
  exit(0);
}

void
CPU::I11_ORA()
{
  std::cerr << "I11_ORA" << std::endl;
  exit(0);
}

void
CPU::I12_KIL()
{
  std::cerr << "I12_KIL" << std::endl;
  exit(0);
}

void
CPU::I13_SLO()
{
  std::cerr << "I13_SLO" << std::endl;
  exit(0);
}

void
CPU::I14_DOP()
{
  std::cerr << "I14_DOP" << std::endl;
  exit(0);
}

void
CPU::I15_ORA()
{
  std::cerr << "I15_ORA" << std::endl;
  exit(0);
}

void
CPU::I16_ASL()
{
  std::cerr << "I16_ASL" << std::endl;
  exit(0);
}

void
CPU::I17_SLO()
{
  std::cerr << "I17_SLO" << std::endl;
  exit(0);
}

void
CPU::I19_ORA()
{
  std::cerr << "I19_ORA" << std::endl;
  exit(0);
}

void
CPU::I1A_NOP()
{
  std::cerr << "I1A_NOP" << std::endl;
  exit(0);
}

void
CPU::I1B_SLO()
{
  std::cerr << "I1B_SLO" << std::endl;
  exit(0);
}

void
CPU::I1C_TOP()
{
  std::cerr << "I1C_TOP" << std::endl;
  exit(0);
}

void
CPU::I1D_ORA()
{
  std::cerr << "I1D_ORA" << std::endl;
  exit(0);
}

void
CPU::I1E_ASL()
{
  std::cerr << "I1E_ASL" << std::endl;
  exit(0);
}

void
CPU::I1F_SLO()
{
  std::cerr << "I1F_SLO" << std::endl;
  exit(0);
}

void
CPU::I20_JSR()
{
  PushWord(PC + 1);
  PC = emu.mem.ReadWord(PC);
}

void
CPU::I21_AND()
{
  std::cerr << "I21_AND" << std::endl;
  exit(0);
}

void
CPU::I22_KIL()
{
  std::cerr << "I22_KIL" << std::endl;
  exit(0);
}

void
CPU::I23_RLA()
{
  std::cerr << "I23_RLA" << std::endl;
  exit(0);
}

void
CPU::I24_BIT()
{
  uint8_t M;

  M = emu.mem.ReadByteZeroPage(emu.mem.ReadByte(PC++));
  SETZ((A & M) == 0);
  SETV(M & 0x40);
  SETN(M & 0x80);
}

void
CPU::I25_AND()
{
  std::cerr << "I25_AND" << std::endl;
  exit(0);
}

void
CPU::I26_ROL()
{
  std::cerr << "I26_ROL" << std::endl;
  exit(0);
}

void
CPU::I27_RLA()
{
  std::cerr << "I27_RLA" << std::endl;
  exit(0);
}

void
CPU::I2A_ROL()
{
  std::cerr << "I2A_ROL" << std::endl;
  exit(0);
}

void
CPU::I2B_AAC()
{
  std::cerr << "I2B_AAC" << std::endl;
  exit(0);
}

void
CPU::I2C_BIT()
{
  std::cerr << "I2C_BIT" << std::endl;
  exit(0);
}

void
CPU::I2D_AND()
{
  std::cerr << "I2D_AND" << std::endl;
  exit(0);
}

void
CPU::I2E_ROL()
{
  std::cerr << "I2E_ROL" << std::endl;
  exit(0);
}

void
CPU::I2F_RLA()
{
  std::cerr << "I2F_RLA" << std::endl;
  exit(0);
}

void
CPU::I31_AND()
{
  std::cerr << "I31_AND" << std::endl;
  exit(0);
}

void
CPU::I32_KIL()
{
  std::cerr << "I32_KIL" << std::endl;
  exit(0);
}

void
CPU::I33_RLA()
{
  std::cerr << "I33_RLA" << std::endl;
  exit(0);
}

void
CPU::I34_DOP()
{
  std::cerr << "I34_DOP" << std::endl;
  exit(0);
}

void
CPU::I35_AND()
{
  std::cerr << "I35_AND" << std::endl;
  exit(0);
}

void
CPU::I36_ROL()
{
  std::cerr << "I36_ROL" << std::endl;
  exit(0);
}

void
CPU::I37_RLA()
{
  std::cerr << "I37_RLA" << std::endl;
  exit(0);
}

void
CPU::I39_AND()
{
  std::cerr << "I39_AND" << std::endl;
  exit(0);
}

void
CPU::I3A_NOP()
{
  std::cerr << "I3A_NOP" << std::endl;
  exit(0);
}

void
CPU::I3B_RLA()
{
  std::cerr << "I3B_RLA" << std::endl;
  exit(0);
}

void
CPU::I3C_TOP()
{
  std::cerr << "I3C_TOP" << std::endl;
  exit(0);
}

void
CPU::I3D_AND()
{
  std::cerr << "I3D_AND" << std::endl;
  exit(0);
}

void
CPU::I3E_ROL()
{
  std::cerr << "I3E_ROL" << std::endl;
  exit(0);
}

void
CPU::I3F_RLA()
{
  std::cerr << "I3F_RLA" << std::endl;
  exit(0);
}

void
CPU::I40_RTI()
{
  std::cerr << "I40_RTI" << std::endl;
  exit(0);
}

void
CPU::I41_EOR()
{
  std::cerr << "I41_EOR" << std::endl;
  exit(0);
}

void
CPU::I42_KIL()
{
  std::cerr << "I42_KIL" << std::endl;
  exit(0);
}

void
CPU::I43_SRE()
{
  std::cerr << "I43_SRE" << std::endl;
  exit(0);
}

void
CPU::I44_DOP()
{
  std::cerr << "I44_DOP" << std::endl;
  exit(0);
}

void
CPU::I45_EOR()
{
  std::cerr << "I45_EOR" << std::endl;
  exit(0);
}

void
CPU::I46_LSR()
{
  std::cerr << "I46_LSR" << std::endl;
  exit(0);
}

void
CPU::I47_SRE()
{
  std::cerr << "I47_SRE" << std::endl;
  exit(0);
}

void
CPU::I4A_LSR()
{
  std::cerr << "I4A_LSR" << std::endl;
  exit(0);
}

void
CPU::I4B_ASR()
{
  std::cerr << "I4B_ASR" << std::endl;
  exit(0);
}

void
CPU::I4C_JMP()
{
  PC = emu.mem.ReadWord(PC);
}

void
CPU::I4D_EOR()
{
  std::cerr << "I4D_EOR" << std::endl;
  exit(0);
}

void
CPU::I4E_LSR()
{
  std::cerr << "I4E_LSR" << std::endl;
  exit(0);
}

void
CPU::I4F_SRE()
{
  std::cerr << "I4F_SRE" << std::endl;
  exit(0);
}

void
CPU::I51_EOR()
{
  std::cerr << "I51_EOR" << std::endl;
  exit(0);
}

void
CPU::I52_KIL()
{
  std::cerr << "I52_KIL" << std::endl;
  exit(0);
}

void
CPU::I53_SRE()
{
  std::cerr << "I53_SRE" << std::endl;
  exit(0);
}

void
CPU::I54_DOP()
{
  std::cerr << "I54_DOP" << std::endl;
  exit(0);
}

void
CPU::I55_EOR()
{
  std::cerr << "I55_EOR" << std::endl;
  exit(0);
}

void
CPU::I56_LSR()
{
  std::cerr << "I56_LSR" << std::endl;
  exit(0);
}

void
CPU::I57_SRE()
{
  std::cerr << "I57_SRE" << std::endl;
  exit(0);
}

void
CPU::I59_EOR()
{
  std::cerr << "I59_EOR" << std::endl;
  exit(0);
}

void
CPU::I5A_NOP()
{
  std::cerr << "I5A_NOP" << std::endl;
  exit(0);
}

void
CPU::I5B_SRE()
{
  std::cerr << "I5B_SRE" << std::endl;
  exit(0);
}

void
CPU::I5C_TOP()
{
  std::cerr << "I5C_TOP" << std::endl;
  exit(0);
}

void
CPU::I5D_EOR()
{
  std::cerr << "I5D_EOR" << std::endl;
  exit(0);
}

void
CPU::I5E_LSR()
{
  std::cerr << "I5E_LSR" << std::endl;
  exit(0);
}

void
CPU::I5F_SRE()
{
  std::cerr << "I5F_SRE" << std::endl;
  exit(0);
}

void
CPU::I60_RTS()
{
  PC = PopWord() + 1;
}

void
CPU::I61_ADC()
{
  std::cerr << "I61_ADC" << std::endl;
  exit(0);
}

void
CPU::I62_KIL()
{
  std::cerr << "I62_KIL" << std::endl;
  exit(0);
}

void
CPU::I63_RRA()
{
  std::cerr << "I63_RRA" << std::endl;
  exit(0);
}

void
CPU::I64_DOP()
{
  std::cerr << "I64_DOP" << std::endl;
  exit(0);
}

void
CPU::I65_ADC()
{
  std::cerr << "I65_ADC" << std::endl;
  exit(0);
}

void
CPU::I66_ROR()
{
  std::cerr << "I66_ROR" << std::endl;
  exit(0);
}

void
CPU::I67_RRA()
{
  std::cerr << "I67_RRA" << std::endl;
  exit(0);
}


void
CPU::I69_ADC()
{
  std::cerr << "I69_ADC" << std::endl;
  exit(0);
}

void
CPU::I6A_ROR()
{
  std::cerr << "I6A_ROR" << std::endl;
  exit(0);
}

void
CPU::I6B_ARR()
{
  std::cerr << "I6B_ARR" << std::endl;
  exit(0);
}

void
CPU::I6C_JMP()
{
  std::cerr << "I6C_JMP" << std::endl;
  exit(0);
}

void
CPU::I6D_ADC()
{
  std::cerr << "I6D_ADC" << std::endl;
  exit(0);
}

void
CPU::I6E_ROR()
{
  std::cerr << "I6E_ROR" << std::endl;
  exit(0);
}

void
CPU::I6F_RRA()
{
  std::cerr << "I6F_RRA" << std::endl;
  exit(0);
}

void
CPU::I71_ADC()
{
  std::cerr << "I71_ADC" << std::endl;
  exit(0);
}

void
CPU::I72_KIL()
{
  std::cerr << "I72_KIL" << std::endl;
  exit(0);
}

void
CPU::I73_RRA()
{
  std::cerr << "I73_RRA" << std::endl;
  exit(0);
}

void
CPU::I74_DOP()
{
  std::cerr << "I74_DOP" << std::endl;
  exit(0);
}

void
CPU::I75_ADC()
{
  std::cerr << "I75_ADC" << std::endl;
  exit(0);
}

void
CPU::I76_ROR()
{
  std::cerr << "I76_ROR" << std::endl;
  exit(0);
}

void
CPU::I77_RRA()
{
  std::cerr << "I77_RRA" << std::endl;
  exit(0);
}

void
CPU::I79_ADC()
{
  std::cerr << "I79_ADC" << std::endl;
  exit(0);
}

void
CPU::I7A_NOP()
{
  std::cerr << "I7A_NOP" << std::endl;
  exit(0);
}

void
CPU::I7B_RRA()
{
  std::cerr << "I7B_RRA" << std::endl;
  exit(0);
}

void
CPU::I7C_TOP()
{
  std::cerr << "I7C_TOP" << std::endl;
  exit(0);
}

void
CPU::I7D_ADC()
{
  std::cerr << "I7D_ADC" << std::endl;
  exit(0);
}

void
CPU::I7E_ROR()
{
  std::cerr << "I7E_ROR" << std::endl;
  exit(0);
}

void
CPU::I7F_RRA()
{
  std::cerr << "I7F_RRA" << std::endl;
  exit(0);
}

void
CPU::I80_DOP()
{
  std::cerr << "I80_DOP" << std::endl;
  exit(0);
}

void
CPU::I81_STA()
{
  std::cerr << "I81_STA" << std::endl;
  exit(0);
}

void
CPU::I82_DOP()
{
  std::cerr << "I82_DOP" << std::endl;
  exit(0);
}

void
CPU::I83_AAX()
{
  std::cerr << "I83_AAX" << std::endl;
  exit(0);
}

void
CPU::I87_AAX()
{
  std::cerr << "I87_AAX" << std::endl;
  exit(0);
}

void
CPU::I88_DEY()
{
  std::cerr << "I88_DEY" << std::endl;
  exit(0);
}

void
CPU::I89_DOP()
{
  std::cerr << "I89_DOP" << std::endl;
  exit(0);
}

void
CPU::I8A_TXA()
{
  std::cerr << "I8A_TXA" << std::endl;
  exit(0);
}

void
CPU::I8B_XAA()
{
  std::cerr << "I8B_XAA" << std::endl;
  exit(0);
}

void
CPU::I8C_STY()
{
  std::cerr << "I8C_STY" << std::endl;
  exit(0);
}

void
CPU::I8D_STA()
{
  emu.mem.WriteByte(emu.mem.ReadWord(PC += 2), A);
}

void
CPU::I8E_STX()
{
  emu.mem.WriteByte(emu.mem.ReadWord(PC += 2), X);
}

void
CPU::I8F_AAX()
{
  std::cerr << "I8F_AAX" << std::endl;
  exit(0);
}

void
CPU::I91_STA()
{
  emu.mem.WriteByte(emu.mem.ReadWordZeroPage(emu.mem.ReadWord(PC++)) + Y, A);
}

void
CPU::I92_KIL()
{
  std::cerr << "I92_KIL" << std::endl;
  exit(0);
}

void
CPU::I93_AXA()
{
  std::cerr << "I93_AXA" << std::endl;
  exit(0);
}

void
CPU::I94_STY()
{
  std::cerr << "I94_STY" << std::endl;
  exit(0);
}

void
CPU::I95_STA()
{
  emu.mem.WriteByte((emu.mem.ReadByte(PC++) + X) & 0xFF, A);
}

void
CPU::I96_STX()
{
  std::cerr << "I96_STX" << std::endl;
  exit(0);
}

void
CPU::I97_AAX()
{
  std::cerr << "I97_AAX" << std::endl;
  exit(0);
}

void
CPU::I98_TYA()
{
  std::cerr << "I98_TYA" << std::endl;
  exit(0);
}

void
CPU::I99_STA()
{
  std::cerr << "I99_STA" << std::endl;
  exit(0);
}

void
CPU::I9A_TXS()
{
  S = X;
}

void
CPU::I9B_XAS()
{
  std::cerr << "I9B_XAS" << std::endl;
  exit(0);
}

void
CPU::I9C_SYA()
{
  std::cerr << "I9C_SYA" << std::endl;
  exit(0);
}

void
CPU::I9D_STA()
{
  emu.mem.WriteByte(emu.mem.ReadWord(PC += 2) + X, A);
}

void
CPU::I9E_SXA()
{
  std::cerr << "I9E_SXA" << std::endl;
  exit(0);
}

void
CPU::I9F_AXA()
{
  std::cerr << "I9F_AXA" << std::endl;
  exit(0);
}

void
CPU::IA0_LDY()
{
  Y = emu.mem.ReadByte(PC++);
  Z = Y == 0;
  N = Y & 0x80;
}

void
CPU::IA1_LDA()
{
  std::cerr << "IA1_LDA" << std::endl;
  exit(0);
}

void
CPU::IA2_LDX()
{
  X = emu.mem.ReadByte(PC++);
  Z = X == 0;
  N = X & 0x80;
}

void
CPU::IA3_LAX()
{
  std::cerr << "IA3_LAX" << std::endl;
  exit(0);
}

void
CPU::IA4_LDY()
{
  std::cerr << "IA4_LDY" << std::endl;
  exit(0);
}

void
CPU::IA5_LDA()
{
  std::cerr << "IA5_LDA" << std::endl;
  exit(0);
}

void
CPU::IA6_LDX()
{
  std::cerr << "IA6_LDX" << std::endl;
  exit(0);
}

void
CPU::IA7_LAX()
{
  std::cerr << "IA7_LAX" << std::endl;
  exit(0);
}

void
CPU::IA8_TAY()
{
  std::cerr << "IA8_TAY" << std::endl;
  exit(0);
}

void
CPU::IA9_LDA()
{
  A = emu.mem.ReadByte(PC++);
  SETZ(A == 0);
  SETN(A & 0x80);
}

void
CPU::IAA_TAX()
{
  X = A;
  Z = X == 0;
  N = X & 0x80;
}

void
CPU::IAB_ATX()
{
  std::cerr << "IAB_ATX" << std::endl;
  exit(0);
}

void
CPU::IAC_LDY()
{
  std::cerr << "IAC_LDY" << std::endl;
  exit(0);
}

void
CPU::IAD_LDA()
{
  std::cerr << "IAD_LDA" << std::endl;
  exit(0);
}

void
CPU::IAE_LDX()
{
  std::cerr << "IAE_LDX" << std::endl;
  exit(0);
}

void
CPU::IAF_LAX()
{
  std::cerr << "IAF_LAX" << std::endl;
  exit(0);
}

void
CPU::IB1_LDA()
{
  std::cerr << "IB1_LDA" << std::endl;
  exit(0);
}

void
CPU::IB2_KIL()
{
  std::cerr << "IB2_KIL" << std::endl;
  exit(0);
}

void
CPU::IB3_LAX()
{
  std::cerr << "IB3_LAX" << std::endl;
  exit(0);
}

void
CPU::IB4_LDY()
{
  std::cerr << "IB4_LDY" << std::endl;
  exit(0);
}

void
CPU::IB5_LDA()
{
  std::cerr << "IB5_LDA" << std::endl;
  exit(0);
}

void
CPU::IB6_LDX()
{
  std::cerr << "IB6_LDX" << std::endl;
  exit(0);
}

void
CPU::IB7_LAX()
{
  std::cerr << "IB7_LAX" << std::endl;
  exit(0);
}

void
CPU::IB9_LDA()
{
  std::cerr << "IB9_LDA" << std::endl;
  exit(0);
}

void
CPU::IBA_TSX()
{
  X = S;
  Z = X == 0;
  N = X & 0x80;
}

void
CPU::IBB_LAR()
{
  std::cerr << "IBB_LAR" << std::endl;
  exit(0);
}

void
CPU::IBC_LDY()
{
  std::cerr << "IBC_LDY" << std::endl;
  exit(0);
}

void
CPU::IBD_LDA()
{
  std::cerr << "IBD_LDA" << std::endl;
  exit(0);
}

void
CPU::IBE_LDX()
{
  std::cerr << "IBE_LDX" << std::endl;
  exit(0);
}

void
CPU::IBF_LAX()
{
  std::cerr << "IBF_LAX" << std::endl;
  exit(0);
}

void
CPU::IC0_CPY()
{
  std::cerr << "IC0_CPY" << std::endl;
  exit(0);
}

void
CPU::IC1_CMP()
{
  std::cerr << "IC1_CMP" << std::endl;
  exit(0);
}

void
CPU::IC2_DOP()
{
  std::cerr << "IC2_DOP" << std::endl;
  exit(0);
}

void
CPU::IC3_DCP()
{
  std::cerr << "IC3_DCP" << std::endl;
  exit(0);
}

void
CPU::IC4_CPY()
{
  std::cerr << "IC4_CPY" << std::endl;
  exit(0);
}

void
CPU::IC5_CMP()
{
  std::cerr << "IC5_CMP" << std::endl;
  exit(0);
}

void
CPU::IC6_DEC()
{
  std::cerr << "IC6_DEC" << std::endl;
  exit(0);
}

void
CPU::IC7_DCP()
{
  std::cerr << "IC7_DCP" << std::endl;
  exit(0);
}

void
CPU::IC8_INY()
{
  Y = Y + 1;
  Z = Y == 0;
  N = Y & 0x80;
}

void
CPU::IC9_CMP()
{
  uint8_t M;

  M = emu.mem.ReadByte(PC++);
  SETC(A >= M);
  SETZ(A == M);
  SETN((A - M) & 0x80);
}

void
CPU::ICA_DEX()
{
  X = X - 1;
  Z = X == 0;
  N = X & 0x80;
}

void
CPU::ICB_AXS()
{
  std::cerr << "ICB_AXS" << std::endl;
  exit(0);
}

void
CPU::ICC_CPY()
{
  std::cerr << "ICC_CPY" << std::endl;
  exit(0);
}

void
CPU::ICD_CMP()
{
  std::cerr << "ICD_CMP" << std::endl;
  exit(0);
}

void
CPU::ICE_DEC()
{
  std::cerr << "ICE_DEC" << std::endl;
  exit(0);
}

void
CPU::ICF_DCP()
{
  std::cerr << "ICF_DCP" << std::endl;
  exit(0);
}

void
CPU::ID1_CMP()
{
  std::cerr << "ID1_CMP" << std::endl;
  exit(0);
}

void
CPU::ID2_KIL()
{
  std::cerr << "ID2_KIL" << std::endl;
  exit(0);
}

void
CPU::ID3_DCP()
{
  std::cerr << "ID3_DCP" << std::endl;
  exit(0);
}

void
CPU::ID4_DOP()
{
  std::cerr << "ID4_DOP" << std::endl;
  exit(0);
}

void
CPU::ID5_CMP()
{
  std::cerr << "ID5_CMP" << std::endl;
  exit(0);
}

void
CPU::ID6_DEC()
{
  std::cerr << "ID6_DEC" << std::endl;
  exit(0);
}

void
CPU::ID7_DCP()
{
  std::cerr << "ID7_DCP" << std::endl;
  exit(0);
}

void
CPU::ID9_CMP()
{
  std::cerr << "ID9_CMP" << std::endl;
  exit(0);
}

void
CPU::IDA_NOP()
{
  std::cerr << "IDA_NOP" << std::endl;
  exit(0);
}

void
CPU::IDB_DCP()
{
  std::cerr << "IDB_DCP" << std::endl;
  exit(0);
}

void
CPU::IDC_TOP()
{
  std::cerr << "IDC_TOP" << std::endl;
  exit(0);
}

void
CPU::IDD_CMP()
{
  std::cerr << "IDD_CMP" << std::endl;
  exit(0);
}

void
CPU::IDE_DEC()
{
  std::cerr << "IDE_DEC" << std::endl;
  exit(0);
}

void
CPU::IDF_DCP()
{
  std::cerr << "IDF_DCP" << std::endl;
  exit(0);
}

void
CPU::IE0_CPX()
{
  std::cerr << "IE0_CPX" << std::endl;
  exit(0);
}

void
CPU::IE1_SBC()
{
  std::cerr << "IE1_SBC" << std::endl;
  exit(0);
}

void
CPU::IE2_DOP()
{
  std::cerr << "IE2_DOP" << std::endl;
  exit(0);
}

void
CPU::IE3_ISC()
{
  std::cerr << "IE3_ISC" << std::endl;
  exit(0);
}

void
CPU::IE4_CPX()
{
  std::cerr << "IE4_CPX" << std::endl;
  exit(0);
}

void
CPU::IE5_SBC()
{
  std::cerr << "IE5_SBC" << std::endl;
  exit(0);
}

void
CPU::IE6_INC()
{
  uint8_t addr, M;

  addr = emu.mem.ReadByte(PC++);
  M = emu.mem.ReadByteZeroPage(addr) + 1;

  emu.mem.WriteByteZeroPage(addr, M);
  Z = M == 0;
  N = M & 0x80;
}

void
CPU::IE7_ISC()
{
  std::cerr << "IE7_ISC" << std::endl;
  exit(0);
}

void
CPU::IE8_INX()
{
  X = X + 1;
  Z = X == 0;
  N = X & 0x80;
}

void
CPU::IE9_SBC()
{
  std::cerr << "IE9_SBC" << std::endl;
  exit(0);
}

void
CPU::IEA_NOP()
{
}

void
CPU::IEB_SBC()
{
  std::cerr << "IEB_SBC" << std::endl;
  exit(0);
}

void
CPU::IEC_CPX()
{
  std::cerr << "IEC_CPX" << std::endl;
  exit(0);
}

void
CPU::IED_SBC()
{
  std::cerr << "IED_SBC" << std::endl;
  exit(0);
}

void
CPU::IEE_INC()
{
  std::cerr << "IEE_INC" << std::endl;
  exit(0);
}

void
CPU::IEF_ISC()
{
  std::cerr << "IEF_ISC" << std::endl;
  exit(0);
}

void
CPU::IF1_SBC()
{
  std::cerr << "IF1_SBC" << std::endl;
  exit(0);
}

void
CPU::IF2_KIL()
{
  std::cerr << "IF2_KIL" << std::endl;
  exit(0);
}

void
CPU::IF3_ISC()
{
  std::cerr << "IF3_ISC" << std::endl;
  exit(0);
}

void
CPU::IF4_DOP()
{
  std::cerr << "IF4_DOP" << std::endl;
  exit(0);
}

void
CPU::IF5_SBC()
{
  std::cerr << "IF5_SBC" << std::endl;
  exit(0);
}

void
CPU::IF6_INC()
{
  std::cerr << "IF6_INC" << std::endl;
  exit(0);
}

void
CPU::IF7_ISC()
{
  std::cerr << "IF7_ISC" << std::endl;
  exit(0);
}

void
CPU::IF9_SBC()
{
  std::cerr << "IF9_SBC" << std::endl;
  exit(0);
}

void
CPU::IFA_NOP()
{
  std::cerr << "IFA_NOP" << std::endl;
  exit(0);
}

void
CPU::IFB_ISC()
{
  std::cerr << "IFB_ISC" << std::endl;
  exit(0);
}

void
CPU::IFC_TOP()
{
  std::cerr << "IFC_TOP" << std::endl;
  exit(0);
}

void
CPU::IFD_SBC()
{
  std::cerr << "IFD_SBC" << std::endl;
  exit(0);
}

void
CPU::IFE_INC()
{
  std::cerr << "IFE_INC" << std::endl;
  exit(0);
}

void
CPU::IFF_ISC()
{
  std::cerr << "IFF_ISC" << std::endl;
  exit(0);
}

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
