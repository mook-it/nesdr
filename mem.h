// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#ifndef __MEMORY_H__
#define __MEMORY_H__

/**
 * Memory management unit
 *
 * Even though the original NES had only 2kb of RAM, the memory module
 * uses the whole 64Kb address space in order to minimise the number
 * of instructions required to access a particular byte.
 */
class Memory
{
public:
  /**
   * The two ROM areas
   */
  enum ROMArea
  {
    LROM,
    HROM
  };

  /**
   * Initialises the memory subsystem
   */
  Memory(Emulator &emu)
    : emu(emu)
  {
  }

  /**
   * Reads a NES file which will be the source
   * for ROM
   */
  void LoadINes(const std::string &nes);

  /**
   * Loads a certain memory bank
   * @param bank Index of the ROM bank
   * @param rom  Destination
   */
  void LoadBank(uint16_t bank, const ROMArea &rom);

  /**
   * Reads a byte from memory
   * @param addr 16 bit address
   */
  uint8_t ReadByte(uint16_t addr);

  /**
   * Reads a 16 bit word from memory, little endian order
   */
  uint16_t ReadWord(uint16_t addr)
  {
    return ReadByte(addr) | (ReadByte(addr + 1) << 8);
  }

  /**
   * Writes a single byte to memory
   */
  void WriteByte(uint16_t addr, uint8_t byte);

private:
  /**
   * +-----------------+--------+-----------------------------+
   * + 0x0000 - 0x07FF | 0x0800 | Game RAM                    |
   * +-----------------+--------+-----------------------------+
   * + 0x0800 - 0x1FFF | 0x0800 | Mirrors of Game RAM         |
   * +-----------------+--------+-----------------------------+
   * + 0x2000 - 0x2007 | 0x0008 | IO Registers                |
   * +-----------------+--------+-----------------------------+
   * + 0x2008 - 0x3FFF | 0x1FF8 | Mirrors of IO Registers     |
   * +-----------------+--------+-----------------------------+
   * + 0x4000 - 0x401F | 0x0020 | IO Registers                |
   * +-----------------+--------+-----------------------------+
   * + 0x4020 - 0x5FFF | 0x1FD0 | Cartridge Expansion RAM     |
   * +-----------------+--------+-----------------------------+
   * + 0x6000 - 0x7FFF | 0x2000 | SRAM                        |
   * +-----------------+--------+-----------------------------+
   * + 0x8000 - 0xBFFF | 0x4000 | Lower ROM bank              |
   * +-----------------+--------+-----------------------------+
   * + 0xC000 - 0xFFF9 | 0x3FFA | Higher ROM bank             |
   * +-----------------+--------+-----------------------------+
   * + 0xFFFA - 0xFFFB | 0x0002 | NMI Handler                 |
   * +-----------------+--------+-----------------------------+
   * + 0xFFFC - 0xFFFD | 0x0002 | Reset Handler               |
   * +-----------------+--------+-----------------------------+
   * + 0xFFFE - 0xFFFF | 0x0002 | BRK Handler                 |
   * +-----------------+--------+-----------------------------+
   */
  union
  {
    uint8_t memory[0x10000];

    struct
    {
      /// Ram & mirrors
      uint8_t ram[0x2000];

      /// PPU Registers
      uint8_t rppu[0x2000];

      /// APU Registers
      uint8_t rapu[0x0020];

      /// Expansion ROM
      uint8_t erom[0x1FE0];

      /// High memory (0x6000 - 0xFFFF)
      union
      {
        uint8_t hmem[0xA000];

        struct
        {
          /// SRAM
          uint8_t sram[0x2000];

          /// Low ROM bank
          uint8_t lrom[0x4000];

          /// High ROM bank
          union
          {
            uint8_t hrom[0x4000];

            struct
            {
              uint8_t  __[0x3FFA];

              /// Non Maskable Interrupt (NMI)
              uint16_t nmi_handler;
              /// Power on reset handler
              uint16_t rst_handler;
              /// Address of Break (BRK instruction)
              uint16_t brk_handler;
            };
          };
        };
      };
    };
  };

  /// Cartridge file
  std::unique_ptr<INes> cart;

  /// Reference to the emulator
  Emulator& emu;
};

#endif /*__MEMORY_H__*/
