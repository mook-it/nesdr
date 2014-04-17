CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(nesdr)

SET(SOURCES apu.cc
            cpu.cc
            emu.cc
            ines.cc
            mem.cc
            ppu.cc
            mmc.cc
   )

SET(HEADERS apu.h
            cpu.h
            emu.h
            ines.h
            mem.h
            ppu.h
            mmc.h
   )

SET(LIBS
   )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

ADD_EXECUTABLE(nesdr ${SOURCES} ${HEADERS})
TARGET_LINK_LIBRARIES(nesdr ${LIBS})
