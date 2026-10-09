cmake_minimum_required(VERSION 3.5)

project(L_Script LANGUAGES CXX)

find_package(Python 3.6 COMPONENTS Interpreter Development REQUIRED)

set(CMAKE_DISABLE_FIND_PACKAGE_pybind11 ON)
set(pybind11_FIND_QUIETLY TRUE)
add_subdirectory(Third_Party/pybind11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB_RECURSE L_Script_include "include/*.h")
add_custom_target(_L_Script_include SOURCES ${L_Script_include})

file(GLOB_RECURSE Sources "source/*.cpp")
add_library(L_Script STATIC ${Sources} )

target_link_libraries(L_Script
                      PRIVATE pybind11::embed
                      )

target_include_directories(L_Script PUBLIC
                            "${PROJECT_BINARY_DIR}" 
                            "${PROJECT_SOURCE_DIR}/include"
                            "${PROJECT_SOURCE_DIR}/Third_Party/pybind11/include"
                            ${Python_INCLUDE_DIRS}
                            "${PROJECT_SOURCE_DIR}/../L_Utility/include"
                            "${PROJECT_SOURCE_DIR}/../L_Utility/glm"
                            "${PROJECT_SOURCE_DIR}/../L_Variable/include"
                            )
