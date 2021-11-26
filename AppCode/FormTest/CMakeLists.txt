cmake_minimum_required(VERSION 3.0.0)
project(FormTest VERSION 0.1.0)


find_package(fmt)
include_directories(. ../StringGenLib ${LIBFMT_INCLUDE_DIRS})


add_executable(${PROJECT_NAME} src/FormTest.cpp )

target_link_libraries(${PROJECT_NAME} fmt fcgi StringGenLib)
