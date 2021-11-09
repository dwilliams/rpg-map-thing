cmake_minimum_required(VERSION 3.20)
project(map_thing)

set(CMAKE_CXX_STANDARD 20)

add_executable(map_thing main.cpp HexMap.cpp HexMap.h mainwindow.cpp mainwindow.h HexDetail.cpp HexDetail.h Token.cpp Token.h PlayerCharacterToken.cpp PlayerCharacterToken.h NonPlayerCharacterToken.cpp NonPlayerCharacterToken.h ItemToken.cpp ItemToken.h TrapToken.cpp TrapToken.h)
