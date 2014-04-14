// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#ifndef __CPU_H__
#define __CPU_H__

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

  void I00_BRK();
  void I01_ORA();
  void I02_KIL();
  void I03_SLO();
  void I04_DOP();
  void I05_ORA();
  void I06_ASL();
  void I07_SLO();
  void I08_PHP();
  void I09_ORA();
  void I0A_ASL();
  void I0B_AAC();
  void I0C_TOP();
  void I0D_ORA();
  void I0E_ASL();
  void I0F_SLO();

  void I10_BPL();
  void I11_ORA();
  void I12_KIL();
  void I13_SLO();
  void I14_DOP();
  void I15_ORA();
  void I16_ASL();
  void I17_SLO();
  void I18_CLC();
  void I19_ORA();
  void I1A_NOP();
  void I1B_SLO();
  void I1C_TOP();
  void I1D_ORA();
  void I1E_ASL();
  void I1F_SLO();

  void I20_JSR();
  void I21_AND();
  void I22_KIL();
  void I23_RLA();
  void I24_BIT();
  void I25_AND();
  void I26_ROL();
  void I27_RLA();
  void I28_PLP();
  void I29_AND();
  void I2A_ROL();
  void I2B_AAC();
  void I2C_BIT();
  void I2D_AND();
  void I2E_ROL();
  void I2F_RLA();

  void I30_BMI();
  void I31_AND();
  void I32_KIL();
  void I33_RLA();
  void I34_DOP();
  void I35_AND();
  void I36_ROL();
  void I37_RLA();
  void I38_SEC();
  void I39_AND();
  void I3A_NOP();
  void I3B_RLA();
  void I3C_TOP();
  void I3D_AND();
  void I3E_ROL();
  void I3F_RLA();

  void I40_RTI();
  void I41_EOR();
  void I42_KIL();
  void I43_SRE();
  void I44_DOP();
  void I45_EOR();
  void I46_LSR();
  void I47_SRE();
  void I48_PHA();
  void I49_EOR();
  void I4A_LSR();
  void I4B_ASR();
  void I4C_JMP();
  void I4D_EOR();
  void I4E_LSR();
  void I4F_SRE();

  void I50_BVC();
  void I51_EOR();
  void I52_KIL();
  void I53_SRE();
  void I54_DOP();
  void I55_EOR();
  void I56_LSR();
  void I57_SRE();
  void I58_CLI();
  void I59_EOR();
  void I5A_NOP();
  void I5B_SRE();
  void I5C_TOP();
  void I5D_EOR();
  void I5E_LSR();
  void I5F_SRE();

  void I60_RTS();
  void I61_ADC();
  void I62_KIL();
  void I63_RRA();
  void I64_DOP();
  void I65_ADC();
  void I66_ROR();
  void I67_RRA();
  void I68_PLA();
  void I69_ADC();
  void I6A_ROR();
  void I6B_ARR();
  void I6C_JMP();
  void I6D_ADC();
  void I6E_ROR();
  void I6F_RRA();

  void I70_BVS();
  void I71_ADC();
  void I72_KIL();
  void I73_RRA();
  void I74_DOP();
  void I75_ADC();
  void I76_ROR();
  void I77_RRA();
  void I78_SEI();
  void I79_ADC();
  void I7A_NOP();
  void I7B_RRA();
  void I7C_TOP();
  void I7D_ADC();
  void I7E_ROR();
  void I7F_RRA();

  void I80_DOP();
  void I81_STA();
  void I82_DOP();
  void I83_AAX();
  void I84_STY();
  void I85_STA();
  void I86_STX();
  void I87_AAX();
  void I88_DEY();
  void I89_DOP();
  void I8A_TXA();
  void I8B_XAA();
  void I8C_STY();
  void I8D_STA();
  void I8E_STX();
  void I8F_AAX();

  void I90_BCC();
  void I91_STA();
  void I92_KIL();
  void I93_AXA();
  void I94_STY();
  void I95_STA();
  void I96_STX();
  void I97_AAX();
  void I98_TYA();
  void I99_STA();
  void I9A_TXS();
  void I9B_XAS();
  void I9C_SYA();
  void I9D_STA();
  void I9E_SXA();
  void I9F_AXA();

  void IA0_LDY();
  void IA1_LDA();
  void IA2_LDX();
  void IA3_LAX();
  void IA4_LDY();
  void IA5_LDA();
  void IA6_LDX();
  void IA7_LAX();
  void IA8_TAY();
  void IA9_LDA();
  void IAA_TAX();
  void IAB_ATX();
  void IAC_LDY();
  void IAD_LDA();
  void IAE_LDX();
  void IAF_LAX();

  void IB0_BCS();
  void IB1_LDA();
  void IB2_KIL();
  void IB3_LAX();
  void IB4_LDY();
  void IB5_LDA();
  void IB6_LDX();
  void IB7_LAX();
  void IB8_CLV();
  void IB9_LDA();
  void IBA_TSX();
  void IBB_LAR();
  void IBC_LDY();
  void IBD_LDA();
  void IBE_LDX();
  void IBF_LAX();

  void IC0_CPY();
  void IC1_CMP();
  void IC2_DOP();
  void IC3_DCP();
  void IC4_CPY();
  void IC5_CMP();
  void IC6_DEC();
  void IC7_DCP();
  void IC8_INY();
  void IC9_CMP();
  void ICA_DEX();
  void ICB_AXS();
  void ICC_CPY();
  void ICD_CMP();
  void ICE_DEC();
  void ICF_DCP();

  void ID0_BNE();
  void ID1_CMP();
  void ID2_KIL();
  void ID3_DCP();
  void ID4_DOP();
  void ID5_CMP();
  void ID6_DEC();
  void ID7_DCP();
  void ID8_CLD();
  void ID9_CMP();
  void IDA_NOP();
  void IDB_DCP();
  void IDC_TOP();
  void IDD_CMP();
  void IDE_DEC();
  void IDF_DCP();

  void IE0_CPX();
  void IE1_SBC();
  void IE2_DOP();
  void IE3_ISC();
  void IE4_CPX();
  void IE5_SBC();
  void IE6_INC();
  void IE7_ISC();
  void IE8_INX();
  void IE9_SBC();
  void IEA_NOP();
  void IEB_SBC();
  void IEC_CPX();
  void IED_SBC();
  void IEE_INC();
  void IEF_ISC();

  void IF0_BEQ();
  void IF1_SBC();
  void IF2_KIL();
  void IF3_ISC();
  void IF4_DOP();
  void IF5_SBC();
  void IF6_INC();
  void IF7_ISC();
  void IF8_SED();
  void IF9_SBC();
  void IFA_NOP();
  void IFB_ISC();
  void IFC_TOP();
  void IFD_SBC();
  void IFE_INC();
  void IFF_ISC();

  // Dispatch table
  typedef void (CPU::*IFunPtr) ();
  static IFunPtr IFunTable[0x100];

private:
  // Accumulator
  uint8_t A;

  // X index
  uint8_t X;

  // Y index
  uint8_t Y;

  // Stack pointer
  uint8_t S;

  // Program counter
  uint16_t PC;

  // Processor flags
  union
  {
    struct
    {
      // Negative flag (bit 7)
      uint8_t N : 1;
      // Overflow flag
      uint8_t V : 1;
      // Always 1 if pushed
      uint8_t   : 1;
      // Only exists on the stack
      uint8_t B : 1;
      // BCD flag
      uint8_t D : 1;
      // Interrupt priority level
      uint8_t I : 1;
      // Zero flag
      uint8_t Z : 1;
      // Carry flag
      uint8_t C : 1;
    };

    uint8_t P;
  };

  /// Emulator reference
  Emulator &emu;
};

#endif /*__CPU_H__*/
