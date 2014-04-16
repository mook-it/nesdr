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
CPU::WriteZeroPageX(uint16_t addr, uint8_t v)
{
  if (!addr)
  {
    addr = (emu.mem.ReadByte(PC) + X) & 0xFF;
    PC++;
  }

  emu.mem.WriteByte(addr, v);
}

void
CPU::WriteZeroPageY(uint16_t addr, uint8_t v)
{
  if (!addr)
  {
    addr = (emu.mem.ReadByte(PC) + Y) & 0xFF;
    PC++;
  }

  emu.mem.WriteByte(addr, v);
}


// -----------------------------------------------------------------------------
// Flag macros
// -----------------------------------------------------------------------------
#define SETZ(expr) Z = ((expr) != 0) ? 1 : 0;
#define SETN(expr) N = ((expr) != 0) ? 1 : 0;
#define SETV(expr) V = ((expr) != 0) ? 1 : 0;
#define SETC(expr) C = ((expr) != 0) ? 1 : 0;

#define ARG_ZEROPAGE\
   emu.mem.ReadByteZeroPage(emu.mem.ReadByte(PC++));
#define ARG_ZEROPAGE_X\
 emu.mem.ReadByteZeroPage((X + emu.mem.ReadByte(PC++)) & 0xFF);
#define ARG_ZEROPAGE_Y\
 emu.mem.ReadByteZeroPage((Y + emu.mem.ReadByte(PC++)) & 0xFF);
#define ARG_ABSOLUTE\
   emu.mem.ReadByte(emu.mem.ReadWord(PC)), PC += 2
#define ARG_ABSOLUTE_X\
   emu.mem.ReadByte(emu.mem.ReadWord(PC) + X), PC += 2

#define HEADER(name, code)\
void \
CPU::name()\
{\
  uint16_t addr;\
  uint8_t M;\
  code;\
}

#define UNDEF(name)\
HEADER(name, { std::cerr << #name << std::endl; exit(0); })

// -----------------------------------------------------------------------------
// Flags
// -----------------------------------------------------------------------------
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
// Bitwise operators
// -----------------------------------------------------------------------------
#define LOGIC_IMM(name, op, reg)\
void \
CPU::name()\
{\
  reg = reg op emu.mem.ReadByte(PC++);\
  SETZ(reg == 0x00);\
  SETN(reg & 0x80);\
}

LOGIC_IMM(I09_ORA, |, A);
LOGIC_IMM(I29_AND, &, A);
LOGIC_IMM(I49_EOR, ^, A);

// -----------------------------------------------------------------------------
// Logical operators
// -----------------------------------------------------------------------------

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
// Comparisons
// -----------------------------------------------------------------------------
#define CMP_IMM(name, reg)\
void \
CPU::name()\
{\
  uint8_t M = emu.mem.ReadByte(PC++);\
  SETZ(reg == M);\
  SETC(reg >= M)\
  SETN((reg - M) & 0x80);\
}

CMP_IMM(IC0_CPY, Y);
CMP_IMM(IC9_CMP, A);
CMP_IMM(IE0_CPX, X);

// -----------------------------------------------------------------------------
// Decrement/increment
// -----------------------------------------------------------------------------
#define DEC(name, reg)\
void \
CPU::name()\
{\
  reg -= 1;\
  SETZ(reg == 0);\
  SETN(reg & 0x80);\
}

DEC(I88_DEY, Y);
DEC(IC6_DEC, A);
DEC(ICA_DEX, X);


// -----------------------------------------------------------------------------
// Other instructions
// -----------------------------------------------------------------------------
UNDEF(I00_BRK);
UNDEF(I01_ORA);
UNDEF(I02_KIL);
UNDEF(I03_SLO);
UNDEF(I04_DOP);
UNDEF(I05_ORA);
UNDEF(I06_ASL);
UNDEF(I07_SLO);

void
CPU::I0A_ASL()
{
  SETC(A & 0x80);
  A <<= 1;
  SETZ(A == 0);
  SETN(A & 0x80);
}

UNDEF(I0B_AAC);
UNDEF(I0C_TOP);
UNDEF(I0D_ORA);
UNDEF(I0E_ASL);
UNDEF(I0F_SLO);
void CPU::I10_BPL() { Branch<&CPU::ReadN, false>(); };
UNDEF(I11_ORA);
UNDEF(I12_KIL);
UNDEF(I13_SLO);
UNDEF(I14_DOP);
UNDEF(I15_ORA);
UNDEF(I16_ASL);
UNDEF(I17_SLO);
UNDEF(I19_ORA);
UNDEF(I1A_NOP);
UNDEF(I1B_SLO);
UNDEF(I1C_TOP);
UNDEF(I1D_ORA);
UNDEF(I1E_ASL);
UNDEF(I1F_SLO);

void
CPU::I20_JSR()
{
  PushWord(PC + 1);
  PC = emu.mem.ReadWord(PC);
}

UNDEF(I21_AND);
UNDEF(I22_KIL);
UNDEF(I23_RLA);

void
CPU::I24_BIT()
{
  uint8_t M;

  M = emu.mem.ReadByteZeroPage(emu.mem.ReadByte(PC++));
  SETZ((A & M) == 0);
  SETV(M & 0x40);
  SETN(M & 0x80);
}

UNDEF(I25_AND);
UNDEF(I26_ROL);
UNDEF(I27_RLA);
UNDEF(I2B_AAC);
UNDEF(I2C_BIT);
UNDEF(I2D_AND);
UNDEF(I2E_ROL);
UNDEF(I2F_RLA);
void CPU::I30_BMI() { Branch<&CPU::ReadN, true>(); };
UNDEF(I31_AND);
UNDEF(I32_KIL);
UNDEF(I33_RLA);
UNDEF(I34_DOP);
UNDEF(I35_AND);
UNDEF(I36_ROL);
UNDEF(I37_RLA);
UNDEF(I39_AND);
UNDEF(I3A_NOP);
UNDEF(I3B_RLA);
UNDEF(I3C_TOP);
UNDEF(I3D_AND);
UNDEF(I3E_ROL);
UNDEF(I3F_RLA);

void
CPU::I40_RTI()
{
  P = PopByte() | 0x20;
  PC = PopWord();
}

UNDEF(I41_EOR);
UNDEF(I42_KIL);
UNDEF(I43_SRE);
UNDEF(I44_DOP);
UNDEF(I45_EOR);
UNDEF(I46_LSR);
UNDEF(I47_SRE);

void
CPU::I4A_LSR()
{
  SETC(A & 0x01);
  A >>= 1;
  SETZ(A == 0);
  SETN(0);
}
UNDEF(I4B_ASR);
void CPU::I4C_JMP() { PC = emu.mem.ReadWord(PC); }
UNDEF(I4D_EOR);
UNDEF(I4E_LSR);
UNDEF(I4F_SRE);
void CPU::I50_BVC() { Branch<&CPU::ReadV, false>(); };
UNDEF(I51_EOR);
UNDEF(I52_KIL);
UNDEF(I53_SRE);
UNDEF(I54_DOP);
UNDEF(I55_EOR);
UNDEF(I56_LSR);
UNDEF(I57_SRE);
UNDEF(I59_EOR);
UNDEF(I5A_NOP);
UNDEF(I5B_SRE);
UNDEF(I5C_TOP);
UNDEF(I5D_EOR);
UNDEF(I5E_LSR);
UNDEF(I5F_SRE);
void
CPU::I60_RTS()
{
  PC = PopWord() + 1;
}

UNDEF(I61_ADC);
UNDEF(I62_KIL);
UNDEF(I63_RRA);
UNDEF(I64_DOP);
UNDEF(I65_ADC);
UNDEF(I66_ROR);
UNDEF(I67_RRA);

void
CPU::I69_ADC()
{
  uint16_t r, M;

  M = emu.mem.ReadByte(PC++);
  r = M + A + C;

  SETC(r & 0xFF00);
  SETZ((r & 0xFF) == 0);
  SETN(r & 0x80);
  SETV((A & 0x80) == (M & 0x80) && (A & 0x80) != (r & 0x80));

  A = r;
}

void
CPU::I6A_ROR()
{
  ROR<&CPU::ReadA, &CPU::WriteA>();
}

void
CPU::I2A_ROL()
{
  ROL<&CPU::ReadA, &CPU::WriteA>();
}

UNDEF(I6B_ARR);
UNDEF(I6C_JMP);
UNDEF(I6D_ADC);
UNDEF(I6E_ROR);
UNDEF(I6F_RRA);
void CPU::I70_BVS() { Branch<&CPU::ReadV, true>(); };
UNDEF(I71_ADC);
UNDEF(I72_KIL);
UNDEF(I73_RRA);
UNDEF(I74_DOP);
UNDEF(I75_ADC);
UNDEF(I76_ROR);
UNDEF(I77_RRA);
UNDEF(I79_ADC);
UNDEF(I7A_NOP);
UNDEF(I7B_RRA);
UNDEF(I7C_TOP);
UNDEF(I7D_ADC);
UNDEF(I7E_ROR);
UNDEF(I7F_RRA);
UNDEF(I80_DOP);
UNDEF(I81_STA);
UNDEF(I82_DOP);
UNDEF(I83_AAX);
UNDEF(I87_AAX);
UNDEF(I89_DOP);
void CPU::I8A_TXA() { Move<&CPU::ReadX, &CPU::WriteA>(); }
UNDEF(I8B_XAA);
void CPU::I8C_STY() { Move<&CPU::ReadY, &CPU::WriteAbsolute, false>(); }
void CPU::I8D_STA() { Move<&CPU::ReadA, &CPU::WriteAbsolute, false>(); }
void CPU::I8E_STX() { Move<&CPU::ReadX, &CPU::WriteAbsolute, false>(); }
UNDEF(I8F_AAX);
void CPU::I90_BCC() { Branch<&CPU::ReadC, false>(); };
void
CPU::I91_STA()
{
  emu.mem.WriteByte(emu.mem.ReadWordZeroPage(emu.mem.ReadWord(PC++)) + Y, A);
}
UNDEF(I92_KIL);
UNDEF(I93_AXA);
UNDEF(I94_STY);
void CPU::I95_STA() { Move<&CPU::ReadA, &CPU::WriteZeroPageX, false>(); }
UNDEF(I96_STX);
UNDEF(I97_AAX);
void CPU::I98_TYA() { Move<&CPU::ReadY, &CPU::WriteA>(); }
UNDEF(I99_STA);
void CPU::I9A_TXS() { Move<&CPU::ReadX, &CPU::WriteS, false>(); }
UNDEF(I9B_XAS);
UNDEF(I9C_SYA);
void CPU::I9D_STA() { Move<&CPU::ReadA, &CPU::WriteAbsolute, false>(); }
UNDEF(I9E_SXA);
UNDEF(I9F_AXA);
void CPU::IA0_LDY() { Move<&CPU::ReadImmediate, &CPU::WriteY>(); }
UNDEF(IA1_LDA);
void CPU::IA2_LDX() { Move<&CPU::ReadImmediate, &CPU::WriteX>(); }
UNDEF(IA3_LAX);
void CPU::IA4_LDY() { Move<&CPU::ReadZeroPage, &CPU::WriteY>(); }
void CPU::IA5_LDA() { Move<&CPU::ReadZeroPage, &CPU::WriteA>(); }
void CPU::IA6_LDX() { Move<&CPU::ReadZeroPage, &CPU::WriteX>(); }
UNDEF(IA7_LAX);
void CPU::IA8_TAY() { Move<&CPU::ReadA, &CPU::WriteY>(); }
void CPU::IA9_LDA() { Move<&CPU::ReadImmediate, &CPU::WriteA>(); }
void CPU::IAA_TAX() { Move<&CPU::ReadA, &CPU::WriteX>(); }
UNDEF(IAB_ATX);
void CPU::IAC_LDY() { Move<&CPU::ReadAbsolute, &CPU::WriteY>(); }
void CPU::IAD_LDA() { Move<&CPU::ReadAbsolute, &CPU::WriteA>(); }
void CPU::IAE_LDX() { Move<&CPU::ReadAbsolute, &CPU::WriteX>(); }
UNDEF(IAF_LAX);
void CPU::IB0_BCS() { Branch<&CPU::ReadC, true>(); };
UNDEF(IB1_LDA);
UNDEF(IB2_KIL);
UNDEF(IB3_LAX);
UNDEF(IB4_LDY);
UNDEF(IB5_LDA);
UNDEF(IB6_LDX);
UNDEF(IB7_LAX);
UNDEF(IB9_LDA);
void CPU::IBA_TSX() { Move<&CPU::ReadS, &CPU::WriteX>(); }
UNDEF(IBB_LAR);
UNDEF(IBC_LDY);
UNDEF(IBD_LDA);
UNDEF(IBE_LDX);
UNDEF(IBF_LAX);
UNDEF(IC1_CMP);
UNDEF(IC2_DOP);
UNDEF(IC3_DCP);
UNDEF(IC4_CPY);
UNDEF(IC5_CMP);
UNDEF(IC7_DCP);
void
CPU::IC8_INY()
{
  Y = Y + 1;
  Z = Y == 0;
  N = Y & 0x80;
}
UNDEF(ICB_AXS);
UNDEF(ICC_CPY);
UNDEF(ICD_CMP);
UNDEF(ICE_DEC);
UNDEF(ICF_DCP);
void CPU::ID0_BNE() { Branch<&CPU::ReadZ, false>(); };
UNDEF(ID1_CMP);
UNDEF(ID2_KIL);
UNDEF(ID3_DCP);
UNDEF(ID4_DOP);
UNDEF(ID5_CMP);
UNDEF(ID6_DEC);
UNDEF(ID7_DCP);
UNDEF(ID9_CMP);
UNDEF(IDA_NOP);
UNDEF(IDB_DCP);
UNDEF(IDC_TOP);
UNDEF(IDD_CMP);
UNDEF(IDE_DEC);
UNDEF(IDF_DCP);
UNDEF(IE1_SBC);
UNDEF(IE2_DOP);
UNDEF(IE3_ISC);
UNDEF(IE4_CPX);
UNDEF(IE5_SBC);

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

UNDEF(IE7_ISC);

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
  uint16_t r, M;

  M = emu.mem.ReadByte(PC++);
  r = A - M - !C;

  SETC(!(r & 0xFF00));
  SETZ((r & 0xFF) == 0);
  SETN(r & 0x80);
  SETV((A & 0x80) != (M & 0x80) && (M & 0x80) == (r & 0x80));

  A = r;
}
void CPU::IEA_NOP() { }
UNDEF(IEB_SBC);
UNDEF(IEC_CPX);
UNDEF(IED_SBC);
UNDEF(IEE_INC);
UNDEF(IEF_ISC);
void CPU::IF0_BEQ() { Branch<&CPU::ReadZ, true>(); };
UNDEF(IF1_SBC);
UNDEF(IF2_KIL);
UNDEF(IF3_ISC);
UNDEF(IF4_DOP);
UNDEF(IF5_SBC);
UNDEF(IF6_INC);
UNDEF(IF7_ISC);
UNDEF(IF9_SBC);
UNDEF(IFA_NOP);
UNDEF(IFB_ISC);
UNDEF(IFC_TOP);
UNDEF(IFD_SBC);
UNDEF(IFE_INC);
UNDEF(IFF_ISC);

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
