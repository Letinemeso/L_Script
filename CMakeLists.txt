cmake_minimum_required(VERSION 3.5)

project(L_Script LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB_RECURSE L_Script_include "include/*.h")
add_custom_target(_L_Script_include SOURCES ${L_Script_include})

file(GLOB_RECURSE Sources "source/*.cpp")
add_library(L_Script STATIC ${Sources} )

target_include_directories(L_Script PUBLIC
                            "${PROJECT_BINARY_DIR}" 
                            "${PROJECT_SOURCE_DIR}/include"
                            "${PROJECT_SOURCE_DIR}/../L_Utility/include"
                            "${PROJECT_SOURCE_DIR}/../L_Variable/include"
                            )
