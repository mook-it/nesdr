// This file is part of the gbdyn project
// Licensing information can be found in the LICENSE file
// (C) 2013 Licker Nandor. All rights reserved.
#ifndef __INES_H__
#define __INES_H__

/**
 * iNES file reader
 */
class INes
{
public:
  /**
   * Creats a new iNES file reader
   */
  INes(const std::string &name)
    : name(name)
  {
    Read();
  }

  /**
   * Frees the iNES file
   */
  ~INes()
  {
  }

  /**
   * Prints information about the file to a stream
   */
  void Dump(std::ostream& os) const;

  /**
   * Retrieves the PRG rom
   */
  const std::vector<uint8_t>& GetPRGRom() const
  {
    return prgROMData;
  }

  /**
   * Retrievs the mapper number
   */
  uint8_t GetMapper() const
  {
    return mapper;
  }

private:
  /**
   * Reads the file into memory
   */
  void Read();

private:
  /// Version (iNES or iNES 2.0)
  enum
  {
    INES1,
    INES2
  } version;
  /// Mirroring
  enum
  {
    VERT,
    HORZ,
    QUAD
  } mirror;
  /// System type
  enum
  {
    NORMAL,
    VS_UNISYSTEM,
    PLAYCHOICE
  } systemType;
  /// SRAM battery flag
  bool hasBattery;
  /// Trainer flag
  bool hasTrainer;
  /// Mapper number
  uint8_t mapper;

  /// PRG ROM data
  std::vector<uint8_t> prgROMData;
  /// CHR ROM data
  std::vector<uint8_t> chrROMData;
  /// Name of the file
  std::string name;
};

#endif /*__INES_H__*/
