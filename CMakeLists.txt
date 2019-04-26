cmake_minimum_required(VERSION 3.10)

project(cscrypto)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC
  include/cscrypto/cscrypto.hpp
  include/cscrypto/cryptotypes.hpp
  include/cscrypto/memoryprotection.hpp
  include/cscrypto/privatekey.hpp
  include/cscrypto/cryptoconstants.hpp
  include/cscrypto/maincryptofunctions.hpp
  src/maincryptofunctions.cpp
  src/privatekey.cpp
  )

add_subdirectory(third-party)

target_link_libraries(${PROJECT_NAME} blake2 libsodium)
target_compile_definitions(${PROJECT_NAME} PUBLIC SODIUM_STATIC)

if(WIN32)
  target_compile_options(${PROJECT_NAME} PUBLIC /wd4324)
endif()
  
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libsodium/src/libsodium/include)
