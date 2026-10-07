#pragma once

#include <L_Script/L_Script_Integration.h>

//      -------------- USAGE EXAMPLE: --------------

// Test::Test()
// {
//     SCRIPTABLE_FUNCTIONS_INITIALIZATION_BEGIN;       //  whole registration block begin
//
//     SCRIPTABLE_FUNCTION_INITIALIZATION_BEGIN( Test );
//     SCRIPTABLE_FUNCTION_NAME(print_shit);
//     SCRIPTABLE_FUNCTION_ARG(int);
//     SCRIPTABLE_FUNCTION_ARG(bool);
//     SCRIPTABLE_FUNCTION_INITIALIZATION_END;
//
//     SCRIPTABLE_FUNCTION_INITIALIZATION_BEGIN( Test );
//     SCRIPTABLE_FUNCTION_NAME(print_number);
//     SCRIPTABLE_FUNCTION_ARG(int);
//     SCRIPTABLE_FUNCTION_INITIALIZATION_END;
//
//     SCRIPTABLE_FUNCTIONS_INITIALIZATION_END;         //  whole registration block end
// }

//      --------------------------------------------


#define SCRIPTABLE_FUNCTIONS_INITIALIZATION_BEGIN \
    static bool __scriptable_functions_initialized = false; \
    if(!__scriptable_functions_initialized) \
    {

#define SCRIPTABLE_FUNCTIONS_INITIALIZATION_END \
        __scriptable_functions_initialized = true; \
    }

#define SCRIPTABLE_FUNCTION_INITIALIZATION_BEGIN(OWNER_CLASS) \
    { \
        using Scriptable_Function_Owner = OWNER_CLASS; \
        std::string scriptable_function_owner_name = #OWNER_CLASS; \
        LDS::Vector<std::string> arguments_types;

#define SCRIPTABLE_FUNCTION_RETURN_TYPE(TYPE) \
        using Return_Type = TYPE; \
        std::string return_type_str = #TYPE;

#define SCRIPTABLE_FUNCTION_NAME(FUNCTION_NAME) \
        auto member_function = &Scriptable_Function_Owner::FUNCTION_NAME; \
        using Member_Function_Type = decltype(member_function); \
        std::string member_function_name = #FUNCTION_NAME;

#define SCRIPTABLE_FUNCTION_ARG(TYPE) \
        { \
            const std::string& default_type_name = LV::Type_Manager::get_default_type_name(#TYPE); \
            arguments_types.push(default_type_name); \
        }

#define SCRIPTABLE_FUNCTION_INITIALIZATION_END \
        LScript::register_l_script_function<Scriptable_Function_Owner, Return_Type, Member_Function_Type>(scriptable_function_owner_name, member_function_name, return_type_str, arguments_types, member_function); \
    }

