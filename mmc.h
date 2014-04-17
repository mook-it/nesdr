// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#ifndef __MMC_H__
#define __MMC_H__

/**
 * MMC0 Mapper
 */
class MMC0 : public Memory::Mapper
{
private:
  MMC0(Memory &mem, INes &cart);
  void Write(uint16_t addr, uint8_t value);

private:
  friend class Memory;
};

/**
 * MMC1 Mapper
 */
class MMC1 : public Memory::Mapper
{
private:
  MMC1(Memory &mem, INes &cart);
  void Write(uint16_t addr, uint8_t value);

  void UpdatePRG();
  void UpdateCHR();

private:
  friend class Memory;

  /// CHR Reg 0
  uint8_t CHR0;
  /// CHR Reg 1
  uint8_t CHR1;
  /// PRG Reg
  uint8_t PRG;
  /// Mirroring control
  uint8_t M;
  /// WRAM enable
  bool W;
  /// CHR mode
  bool C;
  /// PRG size
  bool P;
  /// Slot select
  bool S;
  /// Index of bit written
  size_t index;
  /// Temporary register
  uint8_t temp;
};

#endif
