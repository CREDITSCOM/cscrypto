cmake_minimum_required(VERSION 3.10)

project(cscrypto)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC
  include/cscrypto/cscrypto.hpp
  include/cscrypto/cryptotypes.hpp
  include/cscrypto/helpers.hpp
  include/cscrypto/memoryprotection.hpp
  include/cscrypto/privatekey.hpp
  include/cscrypto/cryptoconstants.hpp
  include/cscrypto/maincryptofunctions.hpp
  include/cscrypto/keyexchange.hpp
  include/cscrypto/keysderivation.hpp
  include/cscrypto/dictionary.hpp
  include/cscrypto/mnemonic.hpp
  include/cscrypto/cipher.hpp
  src/helpers.cpp
  src/maincryptofunctions.cpp
  src/privatekey.cpp
  src/keyexchange.cpp
  src/keysderivation.cpp
  src/dictionary_en.cpp
  src/mnemonic.cpp
  src/cipher.cpp)

add_subdirectory(third-party)

if(build_cscrypto_tests)
  add_subdirectory(tests)
endif()

target_link_libraries(${PROJECT_NAME} blake2 libsodium)
target_compile_definitions(${PROJECT_NAME} PUBLIC SODIUM_STATIC)

if(WIN32)
  target_compile_options(${PROJECT_NAME} PUBLIC /wd4324)
  target_compile_definitions(${PROJECT_NAME} PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()
  
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libsodium/src/libsodium/include)

find_package(Qt5Widgets)
find_package(Qt5Sql)
set(WITH_GUI ${Qt5Widgets_FOUND})
if(WITH_GUI)
  add_subdirectory(qt)
else()
  message(WARNING "The GUI part will not be built")
endif()
