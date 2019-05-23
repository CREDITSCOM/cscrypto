cmake_minimum_required(VERSION 2.8.11)

project(Cscrypto_Qt)

set(HEADERS
  mainwidget.hpp
  keygenwidget.hpp
  signwidget.hpp
  hashwidget.hpp
  cipherwidget.hpp)
qt5_wrap_cpp(MOC_FILES ${HEADERS})

add_executable(${PROJECT_NAME}
  main.cpp
  mainwidget.cpp
  keygenwidget.cpp
  hashwidget.cpp
  signwidget.cpp
  cipherwidget.cpp
  ${HEADERS}
  ${MOC_FILES})
  
target_link_libraries(${PROJECT_NAME} Qt5::Widgets cscrypto)
