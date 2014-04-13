// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#ifndef __MEMORY_H__
#define __MEMORY_H__

/**
 * Memory management unit
 *
 * Memory layout:
 * +-----------------+--------+-----------------------------+
 * + 0x0000 - 0x07FF | 0x0800 | Game RAM                    |
 * +-----------------+--------+-----------------------------+
 * + 0x0800 - 0x17FF | 0x0800 | Mirrors of Game RAM         |
 * +-----------------+--------+-----------------------------+
 * + 0x2000 - 0x2007 | 0x0008 | IO Registers                |
 * +-----------------+--------+-----------------------------+
 * + 0x2008 - 0x3FFF | 0x1FF8 | Mirrors of IO Registers     |
 * +-----------------+--------+-----------------------------+
 * + 0x4000 - 0x401F | 0x0020 | IO Registers                |
 * +-----------------+--------+-----------------------------+
 * + 0x4020 - 0x5FFF | 0x0800 | Cartridge Expansion RAM     |
 * +-----------------+--------+-----------------------------+
 * + 0x6000 - 0x7FFF | 0x0800 | SRAM                        |
 * +-----------------+--------+-----------------------------+
 * + 0x8000 - 0xBFFF | 0x0800 | Lower ROM bank              |
 * +-----------------+--------+-----------------------------+
 * + 0xC000 - 0xFFF9 | 0x0800 | Higher ROM bank             |
 * +-----------------+--------+-----------------------------+
 * + 0xFFFA - 0xFFFB | 0x0002 | NMI Handler                 |
 * +-----------------+--------+-----------------------------+
 * + 0xFFFC - 0xFFFD | 0x0002 | Reset Handler               |
 * +-----------------+--------+-----------------------------+
 * + 0xFFFE - 0xFFFF | 0x0002 | BRK Handler                 |
 * +-----------------+--------+-----------------------------+
 *
 * http://www.fceux.com/web/help/fceux.html?NESRAMMappingFindingValues.html
 */
class Memory
{
public:
  Memory(Emulator *emu)
  {
  }

  uint8_t  ReadByte(uint16_t addr);
  uint16_t ReadWord(uint16_t addr);

  void WriteByte(uint16_t addr, uint8_t byte);
  void WriteWord(uint16_t addr, uint16_t word);

private:
  /// Game RAM
  uint8_t ram[0x800];

  /// Expansion ROM
  uint8_t erom[0x1FE0];

  /// High memory (0x6000 - )
  union
  {
    struct
    {
      /// SRAM
      uint8_t sram[0x2000];

      /// Low ROM bank
      uint8_t lrom[0x8000];

      /// High ROM bank
      union
      {
        uint8_t hrom[0x8000];

        struct
        {
          uint8_t  __[0x7FFA];

          /// Non Maskable Interrupt (NMI)
          uint16_t nmi_handler;
          /// Power on reset handler
          uint16_t rst_handler;
          /// Address of Break (BRK instruction)
          uint16_t brk_handler;
        };
      };
    };

    uint8_t hmem[0xA000];
  };
};

#endif /*__MEMORY_H__*/
