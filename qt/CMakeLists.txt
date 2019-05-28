cmake_minimum_required(VERSION 2.8.11)

project(Cscrypto_Qt)

set(HEADERS
  include/mainwidget.hpp
  include/keygenwidget.hpp
  include/signwidget.hpp
  include/hashwidget.hpp
  include/cipherwidget.hpp)
qt5_wrap_cpp(MOC_FILES ${HEADERS})

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/mainwidget.cpp
  src/keygenwidget.cpp
  src/hashwidget.cpp
  src/signwidget.cpp
  src/cipherwidget.cpp
  include/common.hpp
  ${HEADERS}
  ${MOC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
  
target_link_libraries(${PROJECT_NAME} Qt5::Widgets cscrypto base58)
