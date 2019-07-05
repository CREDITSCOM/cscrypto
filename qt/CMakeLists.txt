cmake_minimum_required(VERSION 2.8.11)

project(Cscrypto_Qt)

if(WIN32)
  set(GUI_TYPE WIN32)
endif(WIN32)

set(HEADERS
  include/mainwidget.hpp
  include/keygenwidget.hpp
  include/signwidget.hpp
  include/hashwidget.hpp
  include/cipherwidget.hpp
  include/passwordlineedit.hpp
  include/keyexchangewidget.hpp
  include/keylistmodel.hpp
  include/storagewidget.hpp)
qt5_wrap_cpp(MOC_FILES ${HEADERS})

qt5_add_resources(
  QRC_WRAPPERS cscrypto_qt.qrc
  OPTIONS -no-compress)

add_executable(${PROJECT_NAME} ${GUI_TYPE}
  src/main.cpp
  src/mainwidget.cpp
  src/keygenwidget.cpp
  src/hashwidget.cpp
  src/signwidget.cpp
  src/cipherwidget.cpp
  src/passwordlineedit.cpp
  src/keyexchangewidget.cpp
  src/keylistmodel.cpp
  src/storagewidget.cpp
  include/common.hpp
  include/utils.hpp
  ${HEADERS}
  ${MOC_FILES}
  ${QRC_WRAPPERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
  
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Sql cscrypto base58)