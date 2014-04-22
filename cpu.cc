// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#include "emu.h"

// -----------------------------------------------------------------------------
template <CPU::AddrMode ReadMode, CPU::AddrMode WriteMode>
inline void CPU::Instr(CPU::InstrFunc instr)
{
  uint16_t addr;
  uint8_t M;

  // Decode the address if it is encoded in the instruction
  switch (std::max(ReadMode, WriteMode))
  {
    case MEM_ZP:
      addr = emu.mem.ReadByte(PC++);
      break;
    case MEM_ZP_X:
      addr = emu.mem.ReadByte(PC++) + X;
      break;
    case MEM_ZP_Y:
      addr = emu.mem.ReadByte(PC++) + Y;
      break;
    case MEM_ABS:
      addr = emu.mem.ReadWord(PC), PC += 2;
      break;
    case MEM_ABS_X:
      addr = emu.mem.ReadWord(PC) + X, PC += 2;
      break;
    case MEM_ABS_Y:
      addr = emu.mem.ReadWord(PC) + Y, PC += 2;
      break;
    case MEM_IDX_X:
      addr = emu.mem.ReadWordZeroPage((emu.mem.ReadByte(PC++) + X) & 0xFF);
      break;
    case MEM_IDX_Y:
      addr = emu.mem.ReadWordZeroPage((emu.mem.ReadByte(PC++) + Y) & 0xFF);
      break;
    case MEM_IND_X:
      addr = emu.mem.ReadWordZeroPage(emu.mem.ReadWord(PC++)) + X;
      break;
    case MEM_IND_Y:
      addr = emu.mem.ReadWordZeroPage(emu.mem.ReadWord(PC++)) + Y;
      break;
    default:
      addr = 0;
      break;
  }

  // Read in the input operand
  switch (ReadMode)
  {
    case NOP:
      break;
    case REG_A:
      M = A;
      break;
    case REG_X:
      M = X;
      break;
    case REG_Y:
      M = Y;
      break;
    case REG_S:
      M = S;
      break;
    case IMM:
      M = emu.mem.ReadByte(PC++);
      break;
    case MEM_ZP: case MEM_ZP_X: case MEM_ZP_Y:
      M = emu.mem.ReadByteZeroPage(addr);
      break;
    default:
      M = emu.mem.ReadByte(addr);
      break;
  }

  // Run the instruction
  (this->*instr) (M);

  // Write back the result
  switch (WriteMode)
  {
    case NOP:
      break;
    case REG_A:
      A = M;
      break;
    case REG_X:
      X = M;
      break;
    case REG_Y:
      Y = M;
      break;
    case REG_S:
      S = M;
      break;
    case IMM:
      break;
    case MEM_ZP: case MEM_ZP_X: case MEM_ZP_Y:
      emu.mem.WriteByteZeroPage(addr, M);
      break;
    default:
      emu.mem.WriteByte(addr, M);
      break;
  }
}

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
void CPU::Dump(std::ostream &os)
{
  os << std::hex;

  os << "PC: " << std::setw(4) << PC << std::endl;
  os << "SP: " << std::setw(4) << (int)S << std::endl;

  os << " A: " << std::setw(4) << (int)A << std::endl;
  os << " X: " << std::setw(4) << (int)X << std::endl;
  os << " Y: " << std::setw(4) << (int)Y << std::endl;

  os << "C:" << (int)C << "  ";
  os << "Z:" << (int)Z << "  ";
  os << "I:" << (int)I << "  ";
  os << "D:" << (int)D << "  ";
  os << "V:" << (int)V << "  ";
  os << "N:" << (int)N << std::endl;

  os << std::endl;
}

// -----------------------------------------------------------------------------
inline void CPU::PushByte(uint8_t byte)
{
  emu.mem.WriteByteStack(S--, byte);
}

// -----------------------------------------------------------------------------
inline void CPU::PushWord(uint16_t word)
{
  emu.mem.WriteByteStack(S--, (word & 0xFF00) >> 8);
  emu.mem.WriteByteStack(S--, (word & 0x00FF) >> 0);
}

// -----------------------------------------------------------------------------
inline uint8_t CPU::PopByte()
{
  return emu.mem.ReadByteStack(++S);
}

// -----------------------------------------------------------------------------
inline uint16_t CPU::PopWord()
{
  uint8_t wl, wh;
  wl = emu.mem.ReadByteStack(++S);
  wh = emu.mem.ReadByteStack(++S);
  return wl | (wh << 8);
}

// -----------------------------------------------------------------------------
inline uint8_t CPU::GetP()
{
  uint8_t P = 0x20;
  P |= C << 0;
  P |= Z << 1;
  P |= I << 2;
  P |= D << 3;
  P |= V << 6;
  P |= N << 7;
  return P;
}

// -----------------------------------------------------------------------------
inline void CPU::SetP(uint8_t P)
{
  C = (P & 0x01) >> 0;
  Z = (P & 0x02) >> 1;
  I = (P & 0x04) >> 2;
  D = (P & 0x08) >> 3;
  V = (P & 0x40) >> 6;
  N = (P & 0x80) >> 7;
}

// -----------------------------------------------------------------------------
inline void CPU::AAX(uint8_t &M)
{
  M = A & X;
}

// -----------------------------------------------------------------------------
inline void CPU::ADC(uint8_t &M)
{
  __asm__
    ( "movb    %[C], %%al     \n\t"
      "addb    $0xFF, %%al    \n\t"
      "movb    %[M], %%al     \n\t"
      "adcb    %%al, %[A]     \n\t"
      "setcb   %[C]           \n\t"
      "setsb   %[N]           \n\t"
      "setob   %[V]           \n\t"
      "setzb   %[Z]           \n\t"
    : [A] "+m" (A)
    , [C] "+m" (C)
    , [N] "=m" (N)
    , [V] "=m" (V)
    , [Z] "=m" (Z)
    : [M]  "g" (M)
    : "memory", "cc", "al"
    );
}

// -----------------------------------------------------------------------------
inline void CPU::AXA(uint8_t &M)
{
  M = A & X & 0x7;
}

// -----------------------------------------------------------------------------
inline void CPU::Bcc(uint8_t M)
{
  PC += M ? (int8_t)emu.mem.ReadByte(PC++) : 1;
}

// -----------------------------------------------------------------------------
inline void CPU::LDM(uint8_t &M)
{
  __asm__
    ( "testb     %[M], %[M]     \n\t"
      "setzb     %[Z]           \n\t"
      "setsb     %[N]           \n\t"
    : [Z] "=m" (Z)
    , [N] "=m" (N)
    : [M]  "r" (M)
    : "memory", "cc"
    );
}

// -----------------------------------------------------------------------------
inline void CPU::STM(uint8_t &M)
{
}

// -----------------------------------------------------------------------------
inline void CPU::ORA(uint8_t &M)
{
  __asm__
    ( "orb       %[A], %[M]     \n\t"
      "setzb     %[Z]           \n\t"
      "setsb     %[N]           \n\t"
    : [Z] "=m" (Z)
    , [N] "=m" (N)
    , [M] "+r" (M)
    : [A]  "m" (A)
    : "memory", "cc"
    );
}

// -----------------------------------------------------------------------------
inline void CPU::AND(uint8_t &M)
{
  __asm__
    ( "andb      %[A], %[M]     \n\t"
      "setzb     %[Z]           \n\t"
      "setsb     %[N]           \n\t"
    : [Z] "=m" (Z)
    , [N] "=m" (N)
    , [M] "+r" (M)
    : [A]  "m" (A)
    : "memory", "cc"
    );
}

// -----------------------------------------------------------------------------
inline void CPU::EOR(uint8_t &M)
{
  __asm__
    ( "xorb      %[A], %[M]     \n\t"
      "setzb     %[Z]           \n\t"
      "setsb     %[N]           \n\t"
    : [Z] "=m" (Z)
    , [N] "=m" (N)
    , [M] "+r" (M)
    : [A]  "m" (A)
    : "memory", "cc"
    );
}

// -----------------------------------------------------------------------------
inline void CPU::BIT(uint8_t &M)
{
  __asm__
    ( "testb   $0x80, %[M]       \n\t"
      "setsb   %[N]              \n\t"
      "testb   $0x40, %[M]       \n\t"
      "setnzb  %[V]              \n\t"
      "testb   %[M], %[A]        \n\t"
      "setzb   %[Z]              \n\t"
    : [N] "=m" (N)
    , [V] "=m" (V)
    , [Z] "=m" (Z)
    : [M]  "r" (M)
    , [A]  "m" (A)
    : "memory", "cc"
    );
}

// -----------------------------------------------------------------------------
inline void CPU::ASL(uint8_t &M)
{
  __asm__
    ( "salb    %[M]             \n\t"
      "setcb   %[C]             \n\t"
      "setzb   %[Z]             \n\t"
      "setsb   %[N]             \n\t"
    : [C] "=m" (C)
    , [Z] "=m" (Z)
    , [N] "=m" (N)
    , [M] "+g" (M)
    );
}

// -----------------------------------------------------------------------------
inline void CPU::LSR(uint8_t &M)
{
  __asm__
    ( "shrb    %[M]             \n\t"
      "setcb   %[C]             \n\t"
      "setzb   %[Z]             \n\t"
      "setsb   %[N]             \n\t"
    : [C] "=m" (C)
    , [Z] "=m" (Z)
    , [N] "=m" (N)
    , [M] "+g" (M)
    );
}

// -----------------------------------------------------------------------------
inline void CPU::ROL(uint8_t &M)
{
  __asm__
    ( "movb    %1, %%dl       \n\t"
      "addb    $0xFF, %%dl    \n\t"
      "rclb    $1, %0         \n\t"
      "setcb   %1             \n\t"
      "testb   $0xFF, %0      \n\t"
      "setzb   %2             \n\t"
      "setsb   %3             \n\t"
    : "+g" (M), "=m" (C), "=m" (Z), "=m" (N)
    :
    : "memory", "cc", "dl"
    );
}

// -----------------------------------------------------------------------------
inline void CPU::ROR(uint8_t &M)
{
  __asm__
    ( "movb    %1, %%dl       \n\t"
      "addb    $0xFF, %%dl    \n\t"
      "rcrb    $1, %0         \n\t"
      "setcb   %1             \n\t"
      "testb   $0xFF, %0      \n\t"
      "setzb   %2             \n\t"
      "setsb   %3             \n\t"
    : "+g" (M), "=m" (C), "=m" (Z), "=m" (N)
    :
    : "memory", "cc", "dl"
    );
}

// -----------------------------------------------------------------------------
inline void CPU::DCP(uint8_t &M)
{
  M = M - 1;

  Z = M == A;
  C = A >= M;
  N = ((A - M) & 0x80) ? 1 : 0;
}

// -----------------------------------------------------------------------------
inline void CPU::ISC(uint8_t &M)
{
  uint16_t r;

  M = M + 1;
  r = A - M - !C;

  C = !(r & 0xFF00);
  Z = (r & 0xFF) == 0;
  N = (r & 0x80) ? 1 : 0;
  V = (A & 0x80) != (M & 0x80) && (M & 0x80) == (r & 0x80);
  A = r;
}

// -----------------------------------------------------------------------------
inline void CPU::SLO(uint8_t &M)
{
  uint16_t r;

  C = M & 0x80 ? 1 : 0;
  M <<= 1;

  A = A | M;
  Z = A == 0;
  N = (A & 0x80) ? 1 : 0;
}

// -----------------------------------------------------------------------------
inline void CPU::RLA(uint8_t &M)
{
  uint16_t r;
  uint8_t newCarry;

  newCarry = M & 0x80 ? 1 : 0;
  M = (M << 1) | C;
  C = newCarry;

  A = A & M;
  Z = A == 0;
  N = (A & 0x80) ? 1 : 0;
}

// -----------------------------------------------------------------------------
inline void CPU::SRE(uint8_t &M)
{
  uint8_t newCarry;

  C = M & 0x01;
  M >>= 1;

  A = A ^ M;
  Z = A == 0;
  N = (A & 0x80) ? 1 : 0;
}

// -----------------------------------------------------------------------------
inline void CPU::RRA(uint8_t &M)
{
  uint16_t r;
  uint8_t newCarry;

  newCarry = M & 0x01;
  M = (M >> 1) | (C << 7);

  r = M + A + newCarry;

  C = r & 0xFF00 ? 1 : 0;
  Z = (r & 0xFF) == 0;
  N = r & 0x80 ? 1 : 0;
  V = (A & 0x80) == (M & 0x80) && (A & 0x80) != (r & 0x80);

  A = r;
}

// -----------------------------------------------------------------------------
inline void CPU::INC(uint8_t &M)
{
  __asm__
    ( "incb    %0             \n\t"
      "setsb   %1             \n\t"
      "setzb   %2             \n\t"
    :  "+q" (M), "=m" (N), "=m" (Z)
    :
    : "memory", "cc"
    );
}

// -----------------------------------------------------------------------------
inline void CPU::DEC(uint8_t &M)
{
  __asm__
    ( "decb    %0             \n\t"
      "setsb   %1             \n\t"
      "setzb   %2             \n\t"
    :  "+q" (M), "=m" (N), "=m" (Z)
    :
    : "memory", "cc"
    );
}

// -----------------------------------------------------------------------------
inline void CPU::CMP(uint8_t &M)
{
  __asm__
    ( "cmpb    %3, %4         \n\t"
      "setnbb  %0             \n\t"
      "setsb   %1             \n\t"
      "seteb   %2             \n\t"
    : "=m" (C), "=m" (N), "=m" (Z)
    : "Q" (M), "m" (A)
    : "memory", "cc"
    );
}

// -----------------------------------------------------------------------------
inline void CPU::CPX(uint8_t &M)
{
  __asm__
    ( "cmpb    %3, %4         \n\t"
      "setnbb  %0             \n\t"
      "setsb   %1             \n\t"
      "seteb   %2             \n\t"
    : "=m" (C), "=m" (N), "=m" (Z)
    : "Q" (M), "m" (X)
    : "memory", "cc"
    );
}

// -----------------------------------------------------------------------------
inline void CPU::CPY(uint8_t &M)
{
  __asm__
    ( "cmpb    %[M], %[Y]     \n\t"
      "setnbb  %[C]           \n\t"
      "setsb   %[N]           \n\t"
      "seteb   %[Z]           \n\t"
    : [C] "=m" (C)
    , [N] "=m" (N)
    , [Z] "=m" (Z)
    : [M]  "Q" (M)
    , [Y]  "m" (Y)
    : "memory", "cc"
    );
}

// -----------------------------------------------------------------------------
inline void CPU::LAX(uint8_t &M)
{
  __asm__-------------------
void CPU::IBB_LAR() { Instr<MEM_ABS_Y, NOP> (&CPU::LAR); }
    ( "testb   %[M], %[M]     \n\t"
      "setzb   %[Z]           \n\t"
      "setsb   %[N]           \n\t"
    : [N] "=m" (N)
    , [Z] "=m" (Z)
    : [M] "q" (A = X = M)
    );
}

// -----------------------------------------------------------------------------
void CPU::SBC(uint8_t &M)
{
  __asm__
    ( "movb    %[C], %%al     \n\t"
      "subb    $0x01, %%al    \n\t"      // Negate the carry flag
      "movb    %[M], %%al     \n\t"
      "sbbb    %%al, %[A]     \n\t"
      "setncb  %[C]           \n\t"
      "setsb   %[N]           \n\t"
      "setob   %[V]           \n\t"
      "setzb   %[Z]           \n\t"
    : [A] "+m" (A)
    , [C] "+m" (C)
    , [N] "=m" (N)
    , [V] "=m" (V)
    , [Z] "=m" (Z)
    : [M]  "g" (M)
    : "memory", "cc", "al"
    );
}

// -----------------------------------------------------------------------------
inline void CPU::LAR(uint8_t &M)
{
  A = X = S = S & M;
  Z = A == 0;
  N = A & 0x80 ? 1 : 0;
}

// -----------------------------------------------------------------------------
void CPU::I09_ORA() { Instr<IMM,       REG_A> (&CPU::ORA); }
void CPU::I05_ORA() { Instr<MEM_ZP,    REG_A> (&CPU::ORA); }
void CPU::I15_ORA() { Instr<MEM_ZP_X,  REG_A> (&CPU::ORA); }
void CPU::I0D_ORA() { Instr<MEM_ABS,   REG_A> (&CPU::ORA); }
void CPU::I1D_ORA() { Instr<MEM_ABS_X, REG_A> (&CPU::ORA); }
void CPU::I19_ORA() { Instr<MEM_ABS_Y, REG_A> (&CPU::ORA); }
void CPU::I01_ORA() { Instr<MEM_IDX_X, REG_A> (&CPU::ORA); }
void CPU::I11_ORA() { Instr<MEM_IND_Y, REG_A> (&CPU::ORA); }
void CPU::I29_AND() { Instr<IMM,       REG_A> (&CPU::AND); }
void CPU::I25_AND() { Instr<MEM_ZP,    REG_A> (&CPU::AND); }
void CPU::I35_AND() { Instr<MEM_ZP_X,  REG_A> (&CPU::AND); }
void CPU::I2D_AND() { Instr<MEM_ABS,   REG_A> (&CPU::AND); }
void CPU::I3D_AND() { Instr<MEM_ABS_X, REG_A> (&CPU::AND); }
void CPU::I39_AND() { Instr<MEM_ABS_Y, REG_A> (&CPU::AND); }
void CPU::I21_AND() { Instr<MEM_IDX_X, REG_A> (&CPU::AND); }
void CPU::I31_AND() { Instr<MEM_IND_Y, REG_A> (&CPU::AND); }
void CPU::I49_EOR() { Instr<IMM,       REG_A> (&CPU::EOR); }
void CPU::I45_EOR() { Instr<MEM_ZP,    REG_A> (&CPU::EOR); }
void CPU::I55_EOR() { Instr<MEM_ZP_X,  REG_A> (&CPU::EOR); }
void CPU::I4D_EOR() { Instr<MEM_ABS,   REG_A> (&CPU::EOR); }
void CPU::I5D_EOR() { Instr<MEM_ABS_X, REG_A> (&CPU::EOR); }
void CPU::I59_EOR() { Instr<MEM_ABS_Y, REG_A> (&CPU::EOR); }
void CPU::I41_EOR() { Instr<MEM_IDX_X, REG_A> (&CPU::EOR); }
void CPU::I51_EOR() { Instr<MEM_IND_Y, REG_A> (&CPU::EOR); }
void CPU::IA1_LDA() { Instr<MEM_IDX_X, REG_A> (&CPU::LDM); }
void CPU::IA5_LDA() { Instr<MEM_ZP,    REG_A> (&CPU::LDM); }
void CPU::IA9_LDA() { Instr<IMM,       REG_A> (&CPU::LDM); }
void CPU::IAD_LDA() { Instr<MEM_ABS,   REG_A> (&CPU::LDM); }
void CPU::IB1_LDA() { Instr<MEM_IND_Y, REG_A> (&CPU::LDM); }
void CPU::IB5_LDA() { Instr<MEM_ZP_X,  REG_A> (&CPU::LDM); }
void CPU::IB9_LDA() { Instr<MEM_ABS_Y, REG_A> (&CPU::LDM); }
void CPU::IBD_LDA() { Instr<MEM_ABS_X, REG_A> (&CPU::LDM); }
void CPU::IA0_LDY() { Instr<IMM,       REG_Y> (&CPU::LDM); }
void CPU::IA4_LDY() { Instr<MEM_ZP,    REG_Y> (&CPU::LDM); }
void CPU::IAC_LDY() { Instr<MEM_ABS,   REG_Y> (&CPU::LDM); }
void CPU::IB4_LDY() { Instr<MEM_ZP_X,  REG_Y> (&CPU::LDM); }
void CPU::IBC_LDY() { Instr<MEM_ABS_X, REG_Y> (&CPU::LDM); }
void CPU::IA2_LDX() { Instr<IMM,       REG_X> (&CPU::LDM); }
void CPU::IA6_LDX() { Instr<MEM_ZP,    REG_X> (&CPU::LDM); }
void CPU::IAE_LDX() { Instr<MEM_ABS,   REG_X> (&CPU::LDM); }
void CPU::IB6_LDX() { Instr<MEM_ZP_Y,  REG_X> (&CPU::LDM); }
void CPU::IBE_LDX() { Instr<MEM_ABS_Y, REG_X> (&CPU::LDM); }

// -----------------------------------------------------------------------------
void CPU::I0A_ASL() { Instr<REG_A>     (&CPU::ASL); }
void CPU::I06_ASL() { Instr<MEM_ZP>    (&CPU::ASL); }
void CPU::I16_ASL() { Instr<MEM_ZP_X>  (&CPU::ASL); }
void CPU::I0E_ASL() { Instr<MEM_ABS>   (&CPU::ASL); }
void CPU::I1E_ASL() { Instr<MEM_ABS_X> (&CPU::ASL); }
void CPU::I4A_LSR() { Instr<REG_A>     (&CPU::LSR); }
void CPU::I46_LSR() { Instr<MEM_ZP>    (&CPU::LSR); }
void CPU::I56_LSR() { Instr<MEM_ZP_X>  (&CPU::LSR); }
void CPU::I4E_LSR() { Instr<MEM_ABS>   (&CPU::LSR); }
void CPU::I5E_LSR() { Instr<MEM_ABS_X> (&CPU::LSR); }
void CPU::I2A_ROL() { Instr<REG_A>     (&CPU::ROL); }
void CPU::I26_ROL() { Instr<MEM_ZP>    (&CPU::ROL); }
void CPU::I36_ROL() { Instr<MEM_ZP_X>  (&CPU::ROL); }
void CPU::I2E_ROL() { Instr<MEM_ABS>   (&CPU::ROL); }
void CPU::I3E_ROL() { Instr<MEM_ABS_X> (&CPU::ROL); }
void CPU::I6A_ROR() { Instr<REG_A>     (&CPU::ROR); }
void CPU::I66_ROR() { Instr<MEM_ZP>    (&CPU::ROR); }
void CPU::I76_ROR() { Instr<MEM_ZP_X>  (&CPU::ROR); }
void CPU::I6E_ROR() { Instr<MEM_ABS>   (&CPU::ROR); }
void CPU::I7E_ROR() { Instr<MEM_ABS_X> (&CPU::ROR); }
void CPU::ICA_DEX() { Instr<REG_X>     (&CPU::DEC); }
void CPU::I88_DEY() { Instr<REG_Y>     (&CPU::DEC); }
void CPU::IC6_DEC() { Instr<MEM_ZP>    (&CPU::DEC); }
void CPU::ID6_DEC() { Instr<MEM_ZP_X>  (&CPU::DEC); }
void CPU::ICE_DEC() { Instr<MEM_ABS>   (&CPU::DEC); }
void CPU::IDE_DEC() { Instr<MEM_ABS_X> (&CPU::DEC); }
void CPU::IE8_INX() { Instr<REG_X>     (&CPU::INC); }
void CPU::IC8_INY() { Instr<REG_Y>     (&CPU::INC); }
void CPU::IE6_INC() { Instr<MEM_ZP>    (&CPU::INC); }
void CPU::IF6_INC() { Instr<MEM_ZP_X>  (&CPU::INC); }
void CPU::IEE_INC() { Instr<MEM_ABS>   (&CPU::INC); }
void CPU::IFE_INC() { Instr<MEM_ABS_X> (&CPU::INC); }
void CPU::IE7_ISC() { Instr<MEM_ZP>    (&CPU::ISC); }
void CPU::IF7_ISC() { Instr<MEM_ZP_X>  (&CPU::ISC); }
void CPU::IEF_ISC() { Instr<MEM_ABS>   (&CPU::ISC); }
void CPU::IFF_ISC() { Instr<MEM_ABS_X> (&CPU::ISC); }
void CPU::IFB_ISC() { Instr<MEM_ABS_Y> (&CPU::ISC); }
void CPU::IE3_ISC() { Instr<MEM_IDX_X> (&CPU::ISC); }
void CPU::IF3_ISC() { Instr<MEM_IND_Y> (&CPU::ISC); }
void CPU::IC7_DCP() { Instr<MEM_ZP>    (&CPU::DCP); }
void CPU::ID7_DCP() { Instr<MEM_ZP_X>  (&CPU::DCP); }
void CPU::ICF_DCP() { Instr<MEM_ABS>   (&CPU::DCP); }
void CPU::IDF_DCP() { Instr<MEM_ABS_X> (&CPU::DCP); }
void CPU::IDB_DCP() { Instr<MEM_ABS_Y> (&CPU::DCP); }
void CPU::IC3_DCP() { Instr<MEM_IDX_X> (&CPU::DCP); }
void CPU::ID3_DCP() { Instr<MEM_IND_Y> (&CPU::DCP); }
void CPU::I07_SLO() { Instr<MEM_ZP>    (&CPU::SLO); }
void CPU::I17_SLO() { Instr<MEM_ZP_X>  (&CPU::SLO); }
void CPU::I0F_SLO() { Instr<MEM_ABS>   (&CPU::SLO); }
void CPU::I1F_SLO() { Instr<MEM_ABS_X> (&CPU::SLO); }
void CPU::I1B_SLO() { Instr<MEM_ABS_Y> (&CPU::SLO); }
void CPU::I03_SLO() { Instr<MEM_IDX_X> (&CPU::SLO); }
void CPU::I13_SLO() { Instr<MEM_IND_Y> (&CPU::SLO); }
void CPU::I27_RLA() { Instr<MEM_ZP>    (&CPU::RLA); }
void CPU::I37_RLA() { Instr<MEM_ZP_X>  (&CPU::RLA); }
void CPU::I2F_RLA() { Instr<MEM_ABS>   (&CPU::RLA); }
void CPU::I3F_RLA() { Instr<MEM_ABS_X> (&CPU::RLA); }
void CPU::I3B_RLA() { Instr<MEM_ABS_Y> (&CPU::RLA); }
void CPU::I23_RLA() { Instr<MEM_IDX_X> (&CPU::RLA); }
void CPU::I33_RLA() { Instr<MEM_IND_Y> (&CPU::RLA); }
void CPU::I47_SRE() { Instr<MEM_ZP>    (&CPU::SRE); }
void CPU::I57_SRE() { Instr<MEM_ZP_X>  (&CPU::SRE); }
void CPU::I4F_SRE() { Instr<MEM_ABS>   (&CPU::SRE); }
void CPU::I5F_SRE() { Instr<MEM_ABS_X> (&CPU::SRE); }
void CPU::I5B_SRE() { Instr<MEM_ABS_Y> (&CPU::SRE); }
void CPU::I43_SRE() { Instr<MEM_IDX_X> (&CPU::SRE); }
void CPU::I53_SRE() { Instr<MEM_IND_Y> (&CPU::SRE); }
void CPU::I67_RRA() { Instr<MEM_ZP>    (&CPU::RRA); }
void CPU::I77_RRA() { Instr<MEM_ZP_X>  (&CPU::RRA); }
void CPU::I6F_RRA() { Instr<MEM_ABS>   (&CPU::RRA); }
void CPU::I7F_RRA() { Instr<MEM_ABS_X> (&CPU::RRA); }
void CPU::I7B_RRA() { Instr<MEM_ABS_Y> (&CPU::RRA); }
void CPU::I63_RRA() { Instr<MEM_IDX_X> (&CPU::RRA); }
void CPU::I73_RRA() { Instr<MEM_IND_Y> (&CPU::RRA); }
void CPU::IA7_LAX() { Instr<MEM_ZP>    (&CPU::LAX); }
void CPU::IB7_LAX() { Instr<MEM_ZP_Y>  (&CPU::LAX); }
void CPU::IAF_LAX() { Instr<MEM_ABS>   (&CPU::LAX); }
void CPU::IBF_LAX() { Instr<MEM_ABS_Y> (&CPU::LAX); }
void CPU::IA3_LAX() { Instr<MEM_IDX_X> (&CPU::LAX); }
void CPU::IB3_LAX() { Instr<MEM_IND_Y> (&CPU::LAX); }
void CPU::I87_AAX() { Instr<MEM_ZP>    (&CPU::AAX); }
void CPU::I97_AAX() { Instr<MEM_ZP_Y>  (&CPU::AAX); }
void CPU::I83_AAX() { Instr<MEM_IDX_X> (&CPU::AAX); }
void CPU::I8F_AAX() { Instr<MEM_ABS>   (&CPU::AAX); }
void CPU::I93_AXA() { Instr<MEM_IND_Y> (&CPU::AXA); }
void CPU::I9F_AXA() { Instr<MEM_ABS_Y> (&CPU::AXA); }

// -----------------------------------------------------------------------------
void CPU::I81_STA() { Instr<REG_A, MEM_IDX_X> (&CPU::STM); }
void CPU::I85_STA() { Instr<REG_A, MEM_ZP>    (&CPU::STM); }
void CPU::I8D_STA() { Instr<REG_A, MEM_ABS>   (&CPU::STM); }
void CPU::I91_STA() { Instr<REG_A, MEM_IND_Y> (&CPU::STM); }
void CPU::I95_STA() { Instr<REG_A, MEM_ZP_X>  (&CPU::STM); }
void CPU::I99_STA() { Instr<REG_A, MEM_ABS_Y> (&CPU::STM); }
void CPU::I9D_STA() { Instr<REG_A, MEM_ABS_X> (&CPU::STM); }
void CPU::I84_STY() { Instr<REG_Y, MEM_ZP>    (&CPU::STM); }
void CPU::I8C_STY() { Instr<REG_Y, MEM_ABS>   (&CPU::STM); }
void CPU::I94_STY() { Instr<REG_Y, MEM_ZP_X>  (&CPU::STM); }
void CPU::I86_STX() { Instr<REG_X, MEM_ZP>    (&CPU::STM); }
void CPU::I8E_STX() { Instr<REG_X, MEM_ABS>   (&CPU::STM); }
void CPU::I96_STX() { Instr<REG_X, MEM_ZP_Y>  (&CPU::STM); }

// -----------------------------------------------------------------------------
void CPU::IC9_CMP() { Instr<IMM,       NOP> (&CPU::CMP); }
void CPU::IC5_CMP() { Instr<MEM_ZP,    NOP> (&CPU::CMP); }
void CPU::ID5_CMP() { Instr<MEM_ZP_X,  NOP> (&CPU::CMP); }
void CPU::ICD_CMP() { Instr<MEM_ABS,   NOP> (&CPU::CMP); }
void CPU::IDD_CMP() { Instr<MEM_ABS_X, NOP> (&CPU::CMP); }
void CPU::ID9_CMP() { Instr<MEM_ABS_Y, NOP> (&CPU::CMP); }
void CPU::IC1_CMP() { Instr<MEM_IDX_X, NOP> (&CPU::CMP); }
void CPU::ID1_CMP() { Instr<MEM_IND_Y, NOP> (&CPU::CMP); }
void CPU::IC0_CPY() { Instr<IMM,       NOP> (&CPU::CPY); }
void CPU::IC4_CPY() { Instr<MEM_ZP,    NOP> (&CPU::CPY); }
void CPU::ICC_CPY() { Instr<MEM_ABS,   NOP> (&CPU::CPY); }
void CPU::IE0_CPX() { Instr<IMM,       NOP> (&CPU::CPX); }
void CPU::IE4_CPX() { Instr<MEM_ZP,    NOP> (&CPU::CPX); }
void CPU::IEC_CPX() { Instr<MEM_ABS,   NOP> (&CPU::CPX); }
void CPU::I24_BIT() { Instr<MEM_ZP,    NOP> (&CPU::BIT); }
void CPU::I2C_BIT() { Instr<MEM_ABS,   NOP> (&CPU::BIT); }
void CPU::I69_ADC() { Instr<IMM,       NOP> (&CPU::ADC); }
void CPU::I65_ADC() { Instr<MEM_ZP,    NOP> (&CPU::ADC); }
void CPU::I75_ADC() { Instr<MEM_ZP_X,  NOP> (&CPU::ADC); }
void CPU::I6D_ADC() { Instr<MEM_ABS,   NOP> (&CPU::ADC); }
void CPU::I7D_ADC() { Instr<MEM_ABS_X, NOP> (&CPU::ADC); }
void CPU::I79_ADC() { Instr<MEM_ABS_Y, NOP> (&CPU::ADC); }
void CPU::I61_ADC() { Instr<MEM_IDX_X, NOP> (&CPU::ADC); }
void CPU::I71_ADC() { Instr<MEM_IND_Y, NOP> (&CPU::ADC); }
void CPU::IE9_SBC() { Instr<IMM,       NOP> (&CPU::SBC); }
void CPU::IEB_SBC() { Instr<IMM,       NOP> (&CPU::SBC); }
void CPU::IE5_SBC() { Instr<MEM_ZP,    NOP> (&CPU::SBC); }
void CPU::IF5_SBC() { Instr<MEM_ZP_X,  NOP> (&CPU::SBC); }
void CPU::IED_SBC() { Instr<MEM_ABS,   NOP> (&CPU::SBC); }
void CPU::IFD_SBC() { Instr<MEM_ABS_X, NOP> (&CPU::SBC); }
void CPU::IF9_SBC() { Instr<MEM_ABS_Y, NOP> (&CPU::SBC); }
void CPU::IE1_SBC() { Instr<MEM_IDX_X, NOP> (&CPU::SBC); }
void CPU::IF1_SBC() { Instr<MEM_IND_Y, NOP> (&CPU::SBC); }

// -----------------------------------------------------------------------------
void CPU::I8A_TXA() { Instr<REG_X, REG_A> (&CPU::LDM); }
void CPU::IAA_TAX() { Instr<REG_A, REG_X> (&CPU::LDM); }
void CPU::I98_TYA() { Instr<REG_Y, REG_A> (&CPU::LDM); }
void CPU::IA8_TAY() { Instr<REG_A, REG_Y> (&CPU::LDM); }
void CPU::IBA_TSX() { Instr<REG_S, REG_X> (&CPU::LDM); }
void CPU::I9A_TXS() { Instr<REG_X, REG_S> (&CPU::STM); }

// -----------------------------------------------------------------------------
void CPU::IEA_NOP() { }

// -----------------------------------------------------------------------------
void CPU::I10_BPL() { Bcc(!N); }
void CPU::I50_BVC() { Bcc(!V); }
void CPU::I90_BCC() { Bcc(!C); }
void CPU::ID0_BNE() { Bcc(!Z); }
void CPU::I30_BMI() { Bcc(N); }
void CPU::I70_BVS() { Bcc(V); }
void CPU::IB0_BCS() { Bcc(C); }
void CPU::IF0_BEQ() { Bcc(Z); }

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
  PushWord(PC + 1);
  PC = emu.mem.ReadWord(PC);
}

// -----------------------------------------------------------------------------
void CPU::I60_RTS()
{
  PC = PopWord() + 1;
}

// -----------------------------------------------------------------------------
void CPU::I40_RTI()
{
  SetP(PopByte());
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
  PushByte(GetP() | 0x10);
}

// -----------------------------------------------------------------------------
void CPU::I28_PLP()
{
  SetP(PopByte());
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
  A = A & emu.mem.ReadByte(PC++);
  Z = A == 0;
  N = C = A & 0x80 ? 1 : 0;
}

// -----------------------------------------------------------------------------
void CPU::I2B_AAC()
{
  A = A & emu.mem.ReadByte(PC++);
  Z = A == 0;
  N = C = A & 0x80 ? 1 : 0;
}

// -----------------------------------------------------------------------------
void CPU::I4B_ASR()
{
  A = A & emu.mem.ReadByte(PC++);
  C = A & 0x01;
  A >>= 1;
  Z = A == 0;
  N = 0;
}

// -----------------------------------------------------------------------------
void CPU::I6B_ARR()
{
  A = A & emu.mem.ReadByte(PC++);
  A = (A >> 1) | (C << 7);
  N = A & 0x80 ? 1 : 0;
  Z = A == 0;
  C = A & 0x40 ? 1 : 0;
  V = C ^ (A & 0x20 ? 1 : 0);
}

// -----------------------------------------------------------------------------
void CPU::IAB_ATX()
{
  X = A = emu.mem.ReadByte(PC++);
  Z = X == 0;
  N = X & 0x80 ? 1 : 0;
}

// -----------------------------------------------------------------------------
void CPU::ICB_AXS()
{
  uint16_t r;

  r = (A & X) - emu.mem.ReadByte(PC++);

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

  if ((arg & 0xFF) + X > 0xFF)
  {
    arg = M;
  }

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

  if ((arg & 0xFF) + Y > 0xFF)
  {
    arg = M;
  }

  emu.mem.WriteByte(arg + Y, M);
}

// -----------------------------------------------------------------------------
void CPU::I8B_XAA()
{
  A = X & emu.mem.ReadByte(PC++);
  Z = A == 0;
  N = A & 0x80 ? 1 : 0;
}

// -----------------------------------------------------------------------------
void CPU::I9B_XAS()
{
  std::cout << "I9B_XAS" << std::endl; exit(0);
}

// -----------------------------------------------------------------------------
void CPU::IBB_LAR() { Instr<MEM_ABS_Y, NOP> (&CPU::LAR); }

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
