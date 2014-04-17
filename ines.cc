// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#include "emu.h"

struct __attribute__((packed)) Header
{
  uint32_t magic;
  uint8_t prgROMSize;
  uint8_t chrROMSize;
  uint8_t flags6;
  uint8_t flags7;
  uint8_t prgRAMSize;
  uint8_t flags9;
  uint8_t flags10;
  uint8_t pad[5];
};

void
INes::Read()
{
  std::ifstream is;
  Header hdr;
  size_t size, chrROMSize, prgROMSize;

  is.open(name.c_str(), std::ios::binary | std::ios::ate);
  if (!is.is_open())
  {
    throw std::runtime_error("Cannot open '" + name + "'");
  }

  try
  {
    size = is.tellg();
    is.seekg(0, is.beg);
    is.read((char*)&hdr, sizeof(hdr));

    // Extract data
    hasBattery = hdr.flags6 & 0x02;
    hasTrainer = hdr.flags6 & 0x04;
    version = ((hdr.flags7 & 0x0C) == 0x08) ? INES2 : INES1;
    mapper = ((hdr.flags6 & 0xF0) >> 4) | (hdr.flags7 & 0xF0);

    switch (hdr.flags6 & 0x09)
    {
      case 0x0: mirror = VERT; break;
      case 0x1: mirror = HORZ; break;
      default:  mirror = QUAD; break;
    }

    chrROMSize = (size_t)hdr.chrROMSize << 13;
    prgROMSize = (size_t)hdr.prgROMSize << 14;

    // Sanity check
    if (hdr.magic != 0x1A53454E)
    {
      throw std::runtime_error("Bad magic '" + name + "'");
    }

    if (hdr.prgROMSize == 0)
    {
      throw std::runtime_error("Bad ROM size: '" + name + "'");
    }

    if (sizeof(hdr) + chrROMSize + prgROMSize != size)
    {
      throw std::runtime_error("Bad file size: '" + name + "'");
    }

    // Read contents
    prgROMData.resize(prgROMSize);
    is.read((char*)&prgROMData[0], prgROMData.size());
    chrROMData.resize(chrROMSize);
    is.read((char*)&chrROMData[0], chrROMData.size());

    is.close();
  }
  catch (...)
  {
    is.close();
    throw;
  }
}

void
INes::Dump(std::ostream& os) const
{
  os << "Version:      " << (version == INES1 ? "iNES 2.0" : "iNES") << "\n";
  os << "PRG ROM size: " << (prgROMData.size() >> 10) << "Kb \n";
  os << "CHR ROM size: " << (chrROMData.size() >> 10) << "Kb \n";
  os << "Battery:      " << (hasBattery ? "yes" : "no") << "\n";
  os << "Trainer:      " << (hasTrainer ? "yes" : "no") << "\n";
  os << std::hex;
  os << "Mapper:       0x" << (uint32_t)mapper << "\n";
  os << "Entry:        0x" << *((uint16_t*)&prgROMData[0x7FFC]) << "\n";
  os << std::dec;

  switch (mirror)
  {
    case HORZ: os << "Mirror:       Horizontal\n"; break;
    case VERT: os << "Mirror:       Vertical\n"; break;
    case QUAD: os << "Mirror:       4 screen\n"; break;
  }

  os << "\n";
}
