cmake_minimum_required(VERSION 3.18)
project(cpptimeparse)

set(CMAKE_CXX_STANDARD 20)

set(PYBIND11_PYTHON_VERSION 3.6)

if(MSVC)
    add_definitions(/EHsc)
    add_definitions(/wd4514)
    add_definitions(/wd5045)
    add_definitions(/wd4868)

    add_definitions(/wd4626)
    add_definitions(/wd4820)
    add_definitions(/wd4625)
    add_definitions(/wd5026)
    add_definitions(/wd4244)
    add_definitions(/wd4365)
else()
    set(CMAKE_CXX_FLAGS "-Wall -fPIC")
endif()

find_package(pybind11 REQUIRED)

pybind11_add_module(${PROJECT_NAME} pywrap.cpp)

target_compile_definitions(${PROJECT_NAME} PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
target_include_directories(${PROJECT_NAME} PRIVATE ${PYBIND11_INCLUDE_DIRS})

add_executable(test test.cpp timeparse.h)
