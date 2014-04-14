// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#include "emu.h"

void
CPU::Start()
{
  PC = emu.mem.ReadWord(0xFFFC);
}

void
CPU::Tick()
{
  (this->*IFunTable[emu.mem.ReadWord(PC++)]) ();
}

void
CPU::I00_BRK()
{

}

void
CPU::I01_ORA()
{

}

void
CPU::I02_KIL()
{

}

void
CPU::I03_SLO()
{

}

void
CPU::I04_DOP()
{

}

void
CPU::I05_ORA()
{

}

void
CPU::I06_ASL()
{

}

void
CPU::I07_SLO()
{

}

void
CPU::I08_PHP()
{

}

void
CPU::I09_ORA()
{

}

void
CPU::I0A_ASL()
{

}

void
CPU::I0B_AAC()
{

}

void
CPU::I0C_TOP()
{

}

void
CPU::I0D_ORA()
{

}

void
CPU::I0E_ASL()
{

}

void
CPU::I0F_SLO()
{

}

void
CPU::I10_BPL()
{

}

void
CPU::I11_ORA()
{

}

void
CPU::I12_KIL()
{

}

void
CPU::I13_SLO()
{

}

void
CPU::I14_DOP()
{

}

void
CPU::I15_ORA()
{

}

void
CPU::I16_ASL()
{

}

void
CPU::I17_SLO()
{

}

void
CPU::I18_CLC()
{

}

void
CPU::I19_ORA()
{

}

void
CPU::I1A_NOP()
{

}

void
CPU::I1B_SLO()
{

}

void
CPU::I1C_TOP()
{

}

void
CPU::I1D_ORA()
{

}

void
CPU::I1E_ASL()
{

}

void
CPU::I1F_SLO()
{

}

void
CPU::I20_JSR()
{

}

void
CPU::I21_AND()
{

}

void
CPU::I22_KIL()
{

}

void
CPU::I23_RLA()
{

}

void
CPU::I24_BIT()
{

}

void
CPU::I25_AND()
{

}

void
CPU::I26_ROL()
{

}

void
CPU::I27_RLA()
{

}

void
CPU::I28_PLP()
{

}

void
CPU::I29_AND()
{

}

void
CPU::I2A_ROL()
{

}

void
CPU::I2B_AAC()
{

}

void
CPU::I2C_BIT()
{

}

void
CPU::I2D_AND()
{

}

void
CPU::I2E_ROL()
{

}

void
CPU::I2F_RLA()
{

}

void
CPU::I30_BMI()
{

}

void
CPU::I31_AND()
{

}

void
CPU::I32_KIL()
{

}

void
CPU::I33_RLA()
{

}

void
CPU::I34_DOP()
{

}

void
CPU::I35_AND()
{

}

void
CPU::I36_ROL()
{

}

void
CPU::I37_RLA()
{

}

void
CPU::I38_SEC()
{

}

void
CPU::I39_AND()
{

}

void
CPU::I3A_NOP()
{

}

void
CPU::I3B_RLA()
{

}

void
CPU::I3C_TOP()
{

}

void
CPU::I3D_AND()
{

}

void
CPU::I3E_ROL()
{

}

void
CPU::I3F_RLA()
{

}

void
CPU::I40_RTI()
{

}

void
CPU::I41_EOR()
{

}

void
CPU::I42_KIL()
{

}

void
CPU::I43_SRE()
{

}

void
CPU::I44_DOP()
{

}

void
CPU::I45_EOR()
{

}

void
CPU::I46_LSR()
{

}

void
CPU::I47_SRE()
{

}

void
CPU::I48_PHA()
{

}

void
CPU::I49_EOR()
{

}

void
CPU::I4A_LSR()
{

}

void
CPU::I4B_ASR()
{

}

void
CPU::I4C_JMP()
{

}

void
CPU::I4D_EOR()
{

}

void
CPU::I4E_LSR()
{

}

void
CPU::I4F_SRE()
{

}

void
CPU::I50_BVC()
{

}

void
CPU::I51_EOR()
{

}

void
CPU::I52_KIL()
{

}

void
CPU::I53_SRE()
{

}

void
CPU::I54_DOP()
{

}

void
CPU::I55_EOR()
{

}

void
CPU::I56_LSR()
{

}

void
CPU::I57_SRE()
{

}

void
CPU::I58_CLI()
{

}

void
CPU::I59_EOR()
{

}

void
CPU::I5A_NOP()
{

}

void
CPU::I5B_SRE()
{

}

void
CPU::I5C_TOP()
{

}

void
CPU::I5D_EOR()
{

}

void
CPU::I5E_LSR()
{

}

void
CPU::I5F_SRE()
{

}

void
CPU::I60_RTS()
{

}

void
CPU::I61_ADC()
{

}

void
CPU::I62_KIL()
{

}

void
CPU::I63_RRA()
{

}

void
CPU::I64_DOP()
{

}

void
CPU::I65_ADC()
{

}

void
CPU::I66_ROR()
{

}

void
CPU::I67_RRA()
{

}

void
CPU::I68_PLA()
{

}

void
CPU::I69_ADC()
{

}

void
CPU::I6A_ROR()
{

}

void
CPU::I6B_ARR()
{

}

void
CPU::I6C_JMP()
{

}

void
CPU::I6D_ADC()
{

}

void
CPU::I6E_ROR()
{

}

void
CPU::I6F_RRA()
{

}

void
CPU::I70_BVS()
{

}

void
CPU::I71_ADC()
{

}

void
CPU::I72_KIL()
{

}

void
CPU::I73_RRA()
{

}

void
CPU::I74_DOP()
{

}

void
CPU::I75_ADC()
{

}

void
CPU::I76_ROR()
{

}

void
CPU::I77_RRA()
{

}

void
CPU::I78_SEI()
{

}

void
CPU::I79_ADC()
{

}

void
CPU::I7A_NOP()
{

}

void
CPU::I7B_RRA()
{

}

void
CPU::I7C_TOP()
{

}

void
CPU::I7D_ADC()
{

}

void
CPU::I7E_ROR()
{

}

void
CPU::I7F_RRA()
{

}

void
CPU::I80_DOP()
{

}

void
CPU::I81_STA()
{

}

void
CPU::I82_DOP()
{

}

void
CPU::I83_AAX()
{

}

void
CPU::I84_STY()
{

}

void
CPU::I85_STA()
{

}

void
CPU::I86_STX()
{

}

void
CPU::I87_AAX()
{

}

void
CPU::I88_DEY()
{

}

void
CPU::I89_DOP()
{

}

void
CPU::I8A_TXA()
{

}

void
CPU::I8B_XAA()
{

}

void
CPU::I8C_STY()
{

}

void
CPU::I8D_STA()
{

}

void
CPU::I8E_STX()
{

}

void
CPU::I8F_AAX()
{

}

void
CPU::I90_BCC()
{

}

void
CPU::I91_STA()
{

}

void
CPU::I92_KIL()
{

}

void
CPU::I93_AXA()
{

}

void
CPU::I94_STY()
{

}

void
CPU::I95_STA()
{

}

void
CPU::I96_STX()
{

}

void
CPU::I97_AAX()
{

}

void
CPU::I98_TYA()
{

}

void
CPU::I99_STA()
{

}

void
CPU::I9A_TXS()
{

}

void
CPU::I9B_XAS()
{

}

void
CPU::I9C_SYA()
{

}

void
CPU::I9D_STA()
{

}

void
CPU::I9E_SXA()
{

}

void
CPU::I9F_AXA()
{

}

void
CPU::IA0_LDY()
{

}

void
CPU::IA1_LDA()
{

}

void
CPU::IA2_LDX()
{

}

void
CPU::IA3_LAX()
{

}

void
CPU::IA4_LDY()
{

}

void
CPU::IA5_LDA()
{

}

void
CPU::IA6_LDX()
{

}

void
CPU::IA7_LAX()
{

}

void
CPU::IA8_TAY()
{

}

void
CPU::IA9_LDA()
{

}

void
CPU::IAA_TAX()
{

}

void
CPU::IAB_ATX()
{

}

void
CPU::IAC_LDY()
{

}

void
CPU::IAD_LDA()
{

}

void
CPU::IAE_LDX()
{

}

void
CPU::IAF_LAX()
{

}

void
CPU::IB0_BCS()
{

}

void
CPU::IB1_LDA()
{

}

void
CPU::IB2_KIL()
{

}

void
CPU::IB3_LAX()
{

}

void
CPU::IB4_LDY()
{

}

void
CPU::IB5_LDA()
{

}

void
CPU::IB6_LDX()
{

}

void
CPU::IB7_LAX()
{

}

void
CPU::IB8_CLV()
{

}

void
CPU::IB9_LDA()
{

}

void
CPU::IBA_TSX()
{

}

void
CPU::IBB_LAR()
{

}

void
CPU::IBC_LDY()
{

}

void
CPU::IBD_LDA()
{

}

void
CPU::IBE_LDX()
{

}

void
CPU::IBF_LAX()
{

}

void
CPU::IC0_CPY()
{

}

void
CPU::IC1_CMP()
{

}

void
CPU::IC2_DOP()
{

}

void
CPU::IC3_DCP()
{

}

void
CPU::IC4_CPY()
{

}

void
CPU::IC5_CMP()
{

}

void
CPU::IC6_DEC()
{

}

void
CPU::IC7_DCP()
{

}

void
CPU::IC8_INY()
{

}

void
CPU::IC9_CMP()
{

}

void
CPU::ICA_DEX()
{

}

void
CPU::ICB_AXS()
{

}

void
CPU::ICC_CPY()
{

}

void
CPU::ICD_CMP()
{

}

void
CPU::ICE_DEC()
{

}

void
CPU::ICF_DCP()
{

}

void
CPU::ID0_BNE()
{

}

void
CPU::ID1_CMP()
{

}

void
CPU::ID2_KIL()
{

}

void
CPU::ID3_DCP()
{

}

void
CPU::ID4_DOP()
{

}

void
CPU::ID5_CMP()
{

}

void
CPU::ID6_DEC()
{

}

void
CPU::ID7_DCP()
{

}

void
CPU::ID8_CLD()
{

}

void
CPU::ID9_CMP()
{

}

void
CPU::IDA_NOP()
{

}

void
CPU::IDB_DCP()
{

}

void
CPU::IDC_TOP()
{

}

void
CPU::IDD_CMP()
{

}

void
CPU::IDE_DEC()
{

}

void
CPU::IDF_DCP()
{

}

void
CPU::IE0_CPX()
{

}

void
CPU::IE1_SBC()
{

}

void
CPU::IE2_DOP()
{

}

void
CPU::IE3_ISC()
{

}

void
CPU::IE4_CPX()
{

}

void
CPU::IE5_SBC()
{

}

void
CPU::IE6_INC()
{

}

void
CPU::IE7_ISC()
{

}

void
CPU::IE8_INX()
{

}

void
CPU::IE9_SBC()
{

}

void
CPU::IEA_NOP()
{

}

void
CPU::IEB_SBC()
{

}

void
CPU::IEC_CPX()
{

}

void
CPU::IED_SBC()
{

}

void
CPU::IEE_INC()
{

}

void
CPU::IEF_ISC()
{

}

void
CPU::IF0_BEQ()
{

}

void
CPU::IF1_SBC()
{

}

void
CPU::IF2_KIL()
{

}

void
CPU::IF3_ISC()
{

}

void
CPU::IF4_DOP()
{

}

void
CPU::IF5_SBC()
{

}

void
CPU::IF6_INC()
{

}

void
CPU::IF7_ISC()
{

}

void
CPU::IF8_SED()
{

}

void
CPU::IF9_SBC()
{

}

void
CPU::IFA_NOP()
{

}

void
CPU::IFB_ISC()
{

}

void
CPU::IFC_TOP()
{

}

void
CPU::IFD_SBC()
{

}

void
CPU::IFE_INC()
{

}

void
CPU::IFF_ISC()
{

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
