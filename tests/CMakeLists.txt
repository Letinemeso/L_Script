file(GLOB_RECURSE Sources "./*.cpp")

if(MSVC)
    add_compile_options(/bigobj)
endif(MSVC)

add_executable(L_Script_Tests ${Sources})

target_include_directories(L_Script_Tests PUBLIC
                            "${PROJECT_SOURCE_DIR}"
                            "${PROJECT_SOURCE_DIR}/include"
                            "${PROJECT_SOURCE_DIR}/L_Utility/include"
                            "${PROJECT_SOURCE_DIR}/L_Variable/include"
                            "${PROJECT_SOURCE_DIR}/L_Script/include"
)


target_link_libraries(L_Script_Tests PUBLIC
    L_Utility
    L_Variable
    L_Script
)

if (UNIX)
    target_link_libraries(L_Script_Tests PUBLIC
        pthread
    )
endif (UNIX)
