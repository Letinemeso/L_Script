#pragma once

#include <Stuff/Arguments_Container.h>

#include <Script.h>
#include <Script_Details/Operations/Custom_Operation.h>
#include <Integrated_Functions.h>


//      -------------- USAGE EXAMPLE: --------------

// Test::Test()
// {
//     SCRIPTABLE_FUNCTIONS_INITIALIZATION_BEGIN;       //  whole registration block begin
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


template <typename _Owner_Type, typename _Return_Type, typename... _Arg_Types>
LST::Arguments_Container<_Arg_Types...> __construct_args_container(_Return_Type(_Owner_Type::*_func)(_Arg_Types...))
{
    return LST::Arguments_Container<_Arg_Types...>();
}


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
        std::string scriptable_function_owner_name = #OWNER_CLASS

#define SCRIPTABLE_FUNCTION_NAME(FUNCTION_NAME) \
        auto member_function = &Scriptable_Function_Owner::FUNCTION_NAME; \
        std::string member_function_name = #FUNCTION_NAME; \
        using Arguments_Container = decltype(__construct_args_container(member_function)); \
        LScript::Function::Arguments_Data arguments_data; \
        arguments_data.push({"Test", "this", true}); \
        LScript::Function* function = new LScript::Function; \
        LScript::Custom_Operation* call_scriptable_function_operation = new LScript::Custom_Operation; \
        call_scriptable_function_operation->set_operation_logic([scriptable_function_owner_name, function, member_function]() \
        { \
                Arguments_Container args_container; \
                for(unsigned int i = 0; i < args_container.arguments_amount(); ++i) \
            { \
                    char* arg_raw = nullptr; \
                    args_container.init_pointer(i, arg_raw); \
                    LScript::Variable* variable = function->compound_statement().context().get_variable("_" + std::to_string(i)); \
                    L_ASSERT(variable); \
                    char* variable_raw = (char*)variable->data(); \
                    for(unsigned int byte = 0; byte < variable->raw_size(); ++byte) \
                        arg_raw[byte] = variable_raw[byte]; \
            } \
                LScript::Variable* context_object_variable = function->compound_statement().context().get_variable("this"); \
                L_ASSERT(context_object_variable); \
                L_ASSERT(scriptable_function_owner_name == context_object_variable->type()); \
                Scriptable_Function_Owner* context_object = (Scriptable_Function_Owner*)context_object_variable->data(); \
                args_container.call_with_args(*context_object, member_function); \
                return nullptr; \
        })

#define SCRIPTABLE_FUNCTION_ARG(TYPE) \
        arguments_data.push({#TYPE, "_" + std::to_string(arguments_data.size() - 1), false})

#define SCRIPTABLE_FUNCTION_INITIALIZATION_END \
        L_ASSERT(arguments_data.size() == Arguments_Container::arguments_amount() + 1); \
        function->compound_statement().add_operation(call_scriptable_function_operation); \
        function->set_expected_arguments_data(arguments_data); \
        LScript::Integrated_Functions::instance().register_member_function(scriptable_function_owner_name, member_function_name, function); \
    }
