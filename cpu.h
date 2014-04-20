// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#ifndef __CPU_H__
#define __CPU_H__

#define ROWL(r, a, b, c, d, e, f, g, h)\
  I(r,0,a) I(r,1,b) I(r,2,c) I(r,3,d) I(r,4,e) I(r,5,f) I(r,6,g) I(r,7,h)

#define ROWH(r, a, b, c, d, e, f, g, h)\
  I(r,8,a) I(r,9,b) I(r,A,c) I(r,B,d) I(r,C,e) I(r,D,f) I(r,E,g) I(r,F,h)

/**
 * Interpretative emulation of the Ricoh 2A03 CPU found in the NES
 */
class CPU
{
public:
  CPU(Emulator &emu)
    : emu(emu)
  {
  }

  /**
   * Executes the reset interrupt
   */
  void Start();

  /**
   * Executes a single instruction
   */
  void Tick();

private:
  /**
   * List of addressing modes
   */
  enum AddrMode
  {
    REG_A,
    REG_X,
    REG_Y,
    IMM,
    MEM_ZP,
    MEM_ZP_X,
    MEM_ZP_Y,
    MEM_ABS,
    MEM_ABS_X,
    MEM_ABS_Y,
    MEM_IDX_IND_X,
    MEM_IDX_IND_Y,
    MEM_IND_IDX_X,
    MEM_IND_IDX_Y
  };

  /**
   * Pointer to a function that can retrieve an argument from the
   * address encoded in the instruction
   */
  typedef uint8_t (CPU::*ReadFunc) (uint16_t &, bool &);

  /**
   * Pointer to a function that can stores a result at an address encoded
   * in the instruction
   */
  typedef void (CPU::*WriteFunc) (uint16_t, bool c, uint8_t);

  /**
   * Performs an instruction, reading in an operand and modifying it
   */
  typedef void (CPU::*InstrFunc) (uint8_t &M);

  /**
   * Wraps functions that read & write to the A register
   */
  template <AddrMode Mode>
  inline void Instr(InstrFunc func, bool read = true, bool write = true)
  {
    throw std::runtime_error("unimplemented");
  }

  /**
   * Conditional branch
   */
  inline void Branch(bool branch)
  {
    PC += branch ? (int8_t)ReadImmediate() : 1;
  }

  /**
   * Moves data between registers or memory
   */
  template<ReadFunc read, WriteFunc write, bool flags = true>
  inline void Move()
  {
    uint16_t addr;
    uint8_t M;
    bool c;

    M = (this->*read)(addr, c);

    if (flags)
    {
      Z = M == 0;
      N = M & 0x80 ? 1 : 0;
    }

    (this->*write)(addr, c, M);
  }

  /**
   * Bitwise or
   */
  inline void ORA(uint8_t &M)
  {
    A = A | M;
    Z = A == 0;
    N = A & 0x80 ? 1 : 0;
  }

  /**
   * Bitwise or
   */
  inline void AND(uint8_t &M)
  {
    A = A & M;
    Z = A == 0;
    N = A & 0x80 ? 1 : 0;
  }

  /**
   * Bitwise or
   */
  inline void EOR(uint8_t &M)
  {
    A = A ^ M;
    Z = A == 0;
    N = A & 0x80 ? 1 : 0;
  }

  /**
   * Bit test
   */
  inline void BIT(uint8_t &M)
  {
    __asm__
      ( "testb   $0x80, %3         \n\t"
        "setsb   %0                \n\t"
        "testb   $0x40, %3         \n\t"
        "setnzb  %1                \n\t"
        "testb   %3, %4            \n\t"
        "setzb   %2                \n\t"
      : "=m" (N), "=m" (V), "=m" (Z)
      : "r" (M), "m" (A)
      : "memory", "cc"
      );
  }

  /**
   * Arithmetic shift left
   */
  inline void ASL(uint8_t &M)
  {
    C = M & 0x80 ? 1 : 0;
    M <<= 1;
    Z = M == 0;
    N = M & 0x80 ? 1 : 0;
  }

  /**
   * Logical shift right
   */
  inline void LSR(uint8_t &M)
  {
    C = M & 0x01;
    M >>= 1;
    Z = M == 0;
    N = 0;
  }

  /**
   * Rotate Left
   */
  inline void ROL(uint8_t &M)
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

  /**
   * Rotate Right
   */
  inline void ROR(uint8_t &M)
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

  /**
   * Subtract 1 from memory without borrow
   */
  inline void DCP(uint8_t &M)
  {
    M = M - 1;

    Z = M == A;
    C = A >= M;
    N = ((A - M) & 0x80) ? 1 : 0;
  }

  /**
   * Increase memory by one & subtract from accumulator
   */
  inline void ISC(uint8_t &M)
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

  /**
   * M <<= 1, A = A | M
   */
  inline void SLO(uint8_t &M)
  {
    uint16_t r;

    C = M & 0x80 ? 1 : 0;
    M <<= 1;

    A = A | M;
    Z = A == 0;
    N = (A & 0x80) ? 1 : 0;
  }

  /**
   * Rotate M left, A = A & M
   */
  inline void RLA(uint8_t &M)
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

  /**
   * Shift M right, A = A ^ M
   */
  inline void SRE(uint8_t &M)
  {
    uint8_t newCarry;

    C = M & 0x01;
    M >>= 1;

    A = A ^ M;
    Z = A == 0;
    N = (A & 0x80) ? 1 : 0;
  }

  /**
   * Rotate M right, A = A + M
   */
  inline void RRA(uint8_t &M)
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

  /**
   * Increment or decrement by a constant
   */
  inline void INC(uint8_t &M)
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

  /**
   * Increment or decrement by a constant
   */
  inline void DEC(uint8_t &M)
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

  /**
   * Compare
   */
  template<ReadFunc fst, ReadFunc snd>
  inline void CMP()
  {
    uint16_t addr;
    bool c;

    __asm__
      ( "cmpb    %3, %4         \n\t"
        "setnbb  %0             \n\t"
        "setsb   %1             \n\t"
        "seteb   %2             \n\t"
      : "=m" (C), "=m" (N), "=m" (Z)
      : "Q" ((this->*snd)(addr, c)), "q" ((this->*fst)(addr, c))
      :
      );
  }

  /**
   * Load A & X from memory
   */
  inline void LAX(uint8_t &M)
  {
    __asm__
      ( "testb   %2, %2         \n\t"
        "setzb   %1             \n\t"
        "setsb   %0             \n\t"
      : "=m" (N), "=m" (Z)
      : "q" (A = X = M)
      );
  }

  /**
   * M = A & X
   */
  inline void AAX(uint8_t &M)
  {
    M = A & X;
  }

  /**
   * M = A & X & 0x07
   */
  inline void AXA(uint8_t &M)
  {
    M = A & X & 0x7;
  }

  /**
   * Add with carry
   */
  inline void ADC(uint8_t &M)
  {
    __asm__
      ( "movb    %1, %%al       \n\t"
        "addb    $0xFF, %%al    \n\t"     // Set the carry flag
        "movb    %5, %%al       \n\t"
        "adcb    %%al, %0       \n\t"
        "setcb   %1             \n\t"
        "setsb   %2             \n\t"
        "setob   %3             \n\t"
        "setzb   %4             \n\t"
      : "+m" (A), "+m" (C), "=m" (N), "=m" (V), "=m" (Z)
      : "g" (M)
      : "memory", "cc", "al"
      );
  }

  /**
   * Subtract with carry
   */
  inline void SBC(uint8_t &M)
  {
    __asm__
      ( "movb    %1, %%al       \n\t"
        "subb    $0x01, %%al    \n\t"      // Negate the carry flag
        "movb    %5, %%al       \n\t"
        "sbbb    %%al, %0       \n\t"
        "setncb  %1             \n\t"
        "setsb   %2             \n\t"
        "setob   %3             \n\t"
        "setzb   %4             \n\t"
      : "+m" (A), "+m" (C), "=m" (N), "=m" (V), "=m" (Z)
      : "g" (M)
      : "memory", "cc", "al"
      );
  }

private:

  /**
   * Builds up the value of the P register from individual flags
   */
  inline uint8_t GetP()
  {
    uint8_t P = 0x20;

    P |= C;
    P |= Z << 1;
    P |= I << 2;
    P |= D << 3;
    P |= V << 6;
    P |= N << 7;

    return P;
  }

  void PushWord(uint16_t word);
  void PushByte(uint8_t byte);
  uint16_t PopWord();
  uint8_t PopByte();

private:

  uint8_t ReadImmediate();
  uint8_t ReadImmediate(uint16_t &addr, bool &c);
  uint8_t ReadZeroPage(uint16_t &addr, bool &c);
  uint8_t ReadZeroPageX(uint16_t &addr, bool &c);
  uint8_t ReadZeroPageY(uint16_t &addr, bool &c);
  uint8_t ReadAbsolute(uint16_t &addr, bool &c);
  uint8_t ReadAbsoluteX(uint16_t &addr, bool &c);
  uint8_t ReadAbsoluteY(uint16_t &addr, bool &c);
  uint8_t ReadIndexedIndirectX(uint16_t &addr, bool &c);
  uint8_t ReadIndexedIndirectY(uint16_t &addr, bool &c);
  uint8_t ReadIndirectIndexedX(uint16_t &addr, bool &c);
  uint8_t ReadIndirectIndexedY(uint16_t &addr, bool &c);

  inline uint8_t ReadA(uint16_t &addr, bool &c)
  {
    addr = 0;
    c = false;
    return A;
  }

  inline uint8_t ReadX(uint16_t &addr, bool &c)
  {
    addr = 0;
    c = false;
    return X;
  }

  inline uint8_t ReadY(uint16_t &addr, bool &c)
  {
    addr = 0;
    c = false;
    return Y;
  }

  inline uint8_t ReadS(uint16_t &addr, bool &c)
  {
    addr = 0;
    c = false;
    return S;
  }

private:

  void WriteA(uint16_t addr, bool c, uint8_t v) { A = v; }
  void WriteX(uint16_t addr, bool c, uint8_t v) { X = v; }
  void WriteY(uint16_t addr, bool c, uint8_t v) { Y = v; }
  void WriteS(uint16_t addr, bool c, uint8_t v) { S = v; }

  void WriteZeroPage(uint16_t addr, bool c, uint8_t v);
  void WriteZeroPageX(uint16_t addr, bool c, uint8_t v);
  void WriteZeroPageY(uint16_t addr, bool c, uint8_t v);
  void WriteAbsolute(uint16_t addr, bool c, uint8_t v);
  void WriteAbsoluteX(uint16_t addr, bool c, uint8_t v);
  void WriteAbsoluteY(uint16_t addr, bool c, uint8_t v);
  void WriteIndexedIndirectX(uint16_t addr, bool c, uint8_t v);
  void WriteIndexedIndirectY(uint16_t addr, bool c, uint8_t v);
  void WriteIndirectIndexedX(uint16_t addr, bool c, uint8_t v);
  void WriteIndirectIndexedY(uint16_t addr, bool c, uint8_t v);

private:

  #define I(r, c, i) void I##r##c##_##i();
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
  #undef I

  // Dispatch table
  typedef void (CPU::*IFunPtr) ();
  static IFunPtr IFunTable[0x100];

private:
  // Program counter
  uint16_t PC;
  // Accumulator
  uint8_t A;
  // X index
  uint8_t X;
  // Y index
  uint8_t Y;
  // Stack pointer
  uint8_t S;
  // P0: Carry flag
  uint8_t C;
  // P1: Zero flag
  uint8_t Z;
  // P2: Interrupt priority level
  uint8_t I;
  // P3: BCD flag
  uint8_t D;
  // P4: Overflow flag
  uint8_t V;
  // P5: Negative flag
  uint8_t N;
  /// Emulator reference
  Emulator &emu;
  /// Cycle counter
  uint64_t cycles;
};

#endif /*__CPU_H__*/
