cmake_minimum_required(VERSION 2.8.11)

project(Cscrypto_Qt)

add_executable(${PROJECT_NAME}
  main.cpp)
  
target_link_libraries(${PROJECT_NAME} Qt5::Widgets cscrypto)
