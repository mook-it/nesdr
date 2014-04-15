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

CPU::IFunPtr CPU::IFunTable[] =
{
  [0x00] = &CPU::I00_BRK,
  [0x01] = &CPU::I01_ORA,
  [0x02] = &CPU::I02_KIL,
  [0x03] = &CPU::I03_SLO,
  [0x04] = &CPU::I04_DOP,
  [0x05] = &CPU::I05_ORA,
  [0x06] = &CPU::I06_ASL,
  [0x07] = &CPU::I07_SLO,
  [0x08] = &CPU::I08_PHP,
  [0x09] = &CPU::I09_ORA,
  [0x0A] = &CPU::I0A_ASL,
  [0x0B] = &CPU::I0B_AAC,
  [0x0C] = &CPU::I0C_TOP,
  [0x0D] = &CPU::I0D_ORA,
  [0x0E] = &CPU::I0E_ASL,
  [0x0F] = &CPU::I0F_SLO,
  [0x10] = &CPU::I10_BPL,
  [0x11] = &CPU::I11_ORA,
  [0x12] = &CPU::I12_KIL,
  [0x13] = &CPU::I13_SLO,
  [0x14] = &CPU::I14_DOP,
  [0x15] = &CPU::I15_ORA,
  [0x16] = &CPU::I16_ASL,
  [0x17] = &CPU::I17_SLO,
  [0x18] = &CPU::I18_CLC,
  [0x19] = &CPU::I19_ORA,
  [0x1A] = &CPU::I1A_NOP,
  [0x1B] = &CPU::I1B_SLO,
  [0x1C] = &CPU::I1C_TOP,
  [0x1D] = &CPU::I1D_ORA,
  [0x1E] = &CPU::I1E_ASL,
  [0x1F] = &CPU::I1F_SLO,
  [0x20] = &CPU::I20_JSR,
  [0x21] = &CPU::I21_AND,
  [0x22] = &CPU::I22_KIL,
  [0x23] = &CPU::I23_RLA,
  [0x24] = &CPU::I24_BIT,
  [0x25] = &CPU::I25_AND,
  [0x26] = &CPU::I26_ROL,
  [0x27] = &CPU::I27_RLA,
  [0x28] = &CPU::I28_PLP,
  [0x29] = &CPU::I29_AND,
  [0x2A] = &CPU::I2A_ROL,
  [0x2B] = &CPU::I2B_AAC,
  [0x2C] = &CPU::I2C_BIT,
  [0x2D] = &CPU::I2D_AND,
  [0x2E] = &CPU::I2E_ROL,
  [0x2F] = &CPU::I2F_RLA,
  [0x30] = &CPU::I30_BMI,
  [0x31] = &CPU::I31_AND,
  [0x32] = &CPU::I32_KIL,
  [0x33] = &CPU::I33_RLA,
  [0x34] = &CPU::I34_DOP,
  [0x35] = &CPU::I35_AND,
  [0x36] = &CPU::I36_ROL,
  [0x37] = &CPU::I37_RLA,
  [0x38] = &CPU::I38_SEC,
  [0x39] = &CPU::I39_AND,
  [0x3A] = &CPU::I3A_NOP,
  [0x3B] = &CPU::I3B_RLA,
  [0x3C] = &CPU::I3C_TOP,
  [0x3D] = &CPU::I3D_AND,
  [0x3E] = &CPU::I3E_ROL,
  [0x3F] = &CPU::I3F_RLA,
  [0x40] = &CPU::I40_RTI,
  [0x41] = &CPU::I41_EOR,
  [0x42] = &CPU::I42_KIL,
  [0x43] = &CPU::I43_SRE,
  [0x44] = &CPU::I44_DOP,
  [0x45] = &CPU::I45_EOR,
  [0x46] = &CPU::I46_LSR,
  [0x47] = &CPU::I47_SRE,
  [0x48] = &CPU::I48_PHA,
  [0x49] = &CPU::I49_EOR,
  [0x4A] = &CPU::I4A_LSR,
  [0x4B] = &CPU::I4B_ASR,
  [0x4C] = &CPU::I4C_JMP,
  [0x4D] = &CPU::I4D_EOR,
  [0x4E] = &CPU::I4E_LSR,
  [0x4F] = &CPU::I4F_SRE,
  [0x50] = &CPU::I50_BVC,
  [0x51] = &CPU::I51_EOR,
  [0x52] = &CPU::I52_KIL,
  [0x53] = &CPU::I53_SRE,
  [0x54] = &CPU::I54_DOP,
  [0x55] = &CPU::I55_EOR,
  [0x56] = &CPU::I56_LSR,
  [0x57] = &CPU::I57_SRE,
  [0x58] = &CPU::I58_CLI,
  [0x59] = &CPU::I59_EOR,
  [0x5A] = &CPU::I5A_NOP,
  [0x5B] = &CPU::I5B_SRE,
  [0x5C] = &CPU::I5C_TOP,
  [0x5D] = &CPU::I5D_EOR,
  [0x5E] = &CPU::I5E_LSR,
  [0x5F] = &CPU::I5F_SRE,
  [0x60] = &CPU::I60_RTS,
  [0x61] = &CPU::I61_ADC,
  [0x62] = &CPU::I62_KIL,
  [0x63] = &CPU::I63_RRA,
  [0x64] = &CPU::I64_DOP,
  [0x65] = &CPU::I65_ADC,
  [0x66] = &CPU::I66_ROR,
  [0x67] = &CPU::I67_RRA,
  [0x68] = &CPU::I68_PLA,
  [0x69] = &CPU::I69_ADC,
  [0x6A] = &CPU::I6A_ROR,
  [0x6B] = &CPU::I6B_ARR,
  [0x6C] = &CPU::I6C_JMP,
  [0x6D] = &CPU::I6D_ADC,
  [0x6E] = &CPU::I6E_ROR,
  [0x6F] = &CPU::I6F_RRA,
  [0x70] = &CPU::I70_BVS,
  [0x71] = &CPU::I71_ADC,
  [0x72] = &CPU::I72_KIL,
  [0x73] = &CPU::I73_RRA,
  [0x74] = &CPU::I74_DOP,
  [0x75] = &CPU::I75_ADC,
  [0x76] = &CPU::I76_ROR,
  [0x77] = &CPU::I77_RRA,
  [0x78] = &CPU::I78_SEI,
  [0x79] = &CPU::I79_ADC,
  [0x7A] = &CPU::I7A_NOP,
  [0x7B] = &CPU::I7B_RRA,
  [0x7C] = &CPU::I7C_TOP,
  [0x7D] = &CPU::I7D_ADC,
  [0x7E] = &CPU::I7E_ROR,
  [0x7F] = &CPU::I7F_RRA,
  [0x80] = &CPU::I80_DOP,
  [0x81] = &CPU::I81_STA,
  [0x82] = &CPU::I82_DOP,
  [0x83] = &CPU::I83_AAX,
  [0x84] = &CPU::I84_STY,
  [0x85] = &CPU::I85_STA,
  [0x86] = &CPU::I86_STX,
  [0x87] = &CPU::I87_AAX,
  [0x88] = &CPU::I88_DEY,
  [0x89] = &CPU::I89_DOP,
  [0x8A] = &CPU::I8A_TXA,
  [0x8B] = &CPU::I8B_XAA,
  [0x8C] = &CPU::I8C_STY,
  [0x8D] = &CPU::I8D_STA,
  [0x8E] = &CPU::I8E_STX,
  [0x8F] = &CPU::I8F_AAX,
  [0x90] = &CPU::I90_BCC,
  [0x91] = &CPU::I91_STA,
  [0x92] = &CPU::I92_KIL,
  [0x93] = &CPU::I93_AXA,
  [0x94] = &CPU::I94_STY,
  [0x95] = &CPU::I95_STA,
  [0x96] = &CPU::I96_STX,
  [0x97] = &CPU::I97_AAX,
  [0x98] = &CPU::I98_TYA,
  [0x99] = &CPU::I99_STA,
  [0x9A] = &CPU::I9A_TXS,
  [0x9B] = &CPU::I9B_XAS,
  [0x9C] = &CPU::I9C_SYA,
  [0x9D] = &CPU::I9D_STA,
  [0x9E] = &CPU::I9E_SXA,
  [0x9F] = &CPU::I9F_AXA,
  [0xA0] = &CPU::IA0_LDY,
  [0xA1] = &CPU::IA1_LDA,
  [0xA2] = &CPU::IA2_LDX,
  [0xA3] = &CPU::IA3_LAX,
  [0xA4] = &CPU::IA4_LDY,
  [0xA5] = &CPU::IA5_LDA,
  [0xA6] = &CPU::IA6_LDX,
  [0xA7] = &CPU::IA7_LAX,
  [0xA8] = &CPU::IA8_TAY,
  [0xA9] = &CPU::IA9_LDA,
  [0xAA] = &CPU::IAA_TAX,
  [0xAB] = &CPU::IAB_ATX,
  [0xAC] = &CPU::IAC_LDY,
  [0xAD] = &CPU::IAD_LDA,
  [0xAE] = &CPU::IAE_LDX,
  [0xAF] = &CPU::IAF_LAX,
  [0xB0] = &CPU::IB0_BCS,
  [0xB1] = &CPU::IB1_LDA,
  [0xB2] = &CPU::IB2_KIL,
  [0xB3] = &CPU::IB3_LAX,
  [0xB4] = &CPU::IB4_LDY,
  [0xB5] = &CPU::IB5_LDA,
  [0xB6] = &CPU::IB6_LDX,
  [0xB7] = &CPU::IB7_LAX,
  [0xB8] = &CPU::IB8_CLV,
  [0xB9] = &CPU::IB9_LDA,
  [0xBA] = &CPU::IBA_TSX,
  [0xBB] = &CPU::IBB_LAR,
  [0xBC] = &CPU::IBC_LDY,
  [0xBD] = &CPU::IBD_LDA,
  [0xBE] = &CPU::IBE_LDX,
  [0xBF] = &CPU::IBF_LAX,
  [0xC0] = &CPU::IC0_CPY,
  [0xC1] = &CPU::IC1_CMP,
  [0xC2] = &CPU::IC2_DOP,
  [0xC3] = &CPU::IC3_DCP,
  [0xC4] = &CPU::IC4_CPY,
  [0xC5] = &CPU::IC5_CMP,
  [0xC6] = &CPU::IC6_DEC,
  [0xC7] = &CPU::IC7_DCP,
  [0xC8] = &CPU::IC8_INY,
  [0xC9] = &CPU::IC9_CMP,
  [0xCA] = &CPU::ICA_DEX,
  [0xCB] = &CPU::ICB_AXS,
  [0xCC] = &CPU::ICC_CPY,
  [0xCD] = &CPU::ICD_CMP,
  [0xCE] = &CPU::ICE_DEC,
  [0xCF] = &CPU::ICF_DCP,
  [0xD0] = &CPU::ID0_BNE,
  [0xD1] = &CPU::ID1_CMP,
  [0xD2] = &CPU::ID2_KIL,
  [0xD3] = &CPU::ID3_DCP,
  [0xD4] = &CPU::ID4_DOP,
  [0xD5] = &CPU::ID5_CMP,
  [0xD6] = &CPU::ID6_DEC,
  [0xD7] = &CPU::ID7_DCP,
  [0xD8] = &CPU::ID8_CLD,
  [0xD9] = &CPU::ID9_CMP,
  [0xDA] = &CPU::IDA_NOP,
  [0xDB] = &CPU::IDB_DCP,
  [0xDC] = &CPU::IDC_TOP,
  [0xDD] = &CPU::IDD_CMP,
  [0xDE] = &CPU::IDE_DEC,
  [0xDF] = &CPU::IDF_DCP,
  [0xE0] = &CPU::IE0_CPX,
  [0xE1] = &CPU::IE1_SBC,
  [0xE2] = &CPU::IE2_DOP,
  [0xE3] = &CPU::IE3_ISC,
  [0xE4] = &CPU::IE4_CPX,
  [0xE5] = &CPU::IE5_SBC,
  [0xE6] = &CPU::IE6_INC,
  [0xE7] = &CPU::IE7_ISC,
  [0xE8] = &CPU::IE8_INX,
  [0xE9] = &CPU::IE9_SBC,
  [0xEA] = &CPU::IEA_NOP,
  [0xEB] = &CPU::IEB_SBC,
  [0xEC] = &CPU::IEC_CPX,
  [0xED] = &CPU::IED_SBC,
  [0xEE] = &CPU::IEE_INC,
  [0xEF] = &CPU::IEF_ISC,
  [0xF0] = &CPU::IF0_BEQ,
  [0xF1] = &CPU::IF1_SBC,
  [0xF2] = &CPU::IF2_KIL,
  [0xF3] = &CPU::IF3_ISC,
  [0xF4] = &CPU::IF4_DOP,
  [0xF5] = &CPU::IF5_SBC,
  [0xF6] = &CPU::IF6_INC,
  [0xF7] = &CPU::IF7_ISC,
  [0xF8] = &CPU::IF8_SED,
  [0xF9] = &CPU::IF9_SBC,
  [0xFA] = &CPU::IFA_NOP,
  [0xFB] = &CPU::IFB_ISC,
  [0xFC] = &CPU::IFC_TOP,
  [0xFD] = &CPU::IFD_SBC,
  [0xFE] = &CPU::IFE_INC,
  [0xFF] = &CPU::IFF_ISC
};
