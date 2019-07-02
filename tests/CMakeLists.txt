cmake_minimum_required(VERSION 3.10)

project(cscrypto_tests)

add_executable(${PROJECT_NAME}
  ciphertests.cpp
  keysderivationtests.cpp
  mainfuncstests.cpp
  mnemonictests.cpp
  main.cpp
  privatekeytests.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC cscrypto gtest)
