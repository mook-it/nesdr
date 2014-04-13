#include <cstdlib>
#include <iostream>
#include <exception>
#include "emu.h"

int
main(int argc, char **argv)
{
  try
  {
    (Emulator()).Run();
    return EXIT_SUCCESS;
  }
  catch (std::exception& e)
  {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
