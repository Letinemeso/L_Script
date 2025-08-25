#pragma once

#include <Stuff/Arguments_Container.h>

#include <Script.h>
#include <Script_Details/Operations/Custom_Operation.h>
#include <Integrated_Functions.h>


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


namespace LScript
{

    template <typename _Owner_Type, typename _Return_Type, typename... _Arg_Types>
    LST::Arguments_Container<_Arg_Types...> __construct_args_container(_Return_Type(_Owner_Type::*_func)(_Arg_Types...))
    {
        return LST::Arguments_Container<_Arg_Types...>();
    }

    template <typename _Owner_Type, typename _Return_Type, typename... _Arg_Types>
    LST::Arguments_Container<_Arg_Types...> __construct_args_container(_Return_Type(_Owner_Type::*_func)(_Arg_Types...) const)
    {
        return LST::Arguments_Container<_Arg_Types...>();
    }

    template <typename _Return_Type>
    class __Calling_Function_Construction_Helper
    {
    public:
        template <typename _Owner_Class, typename _Func_Type, typename _Args_Container_Type>
        LST::Function<LScript::Variable*(_Owner_Class* _owner_object, _Func_Type _func, _Args_Container_Type& _args_container)>
        construct_calling_function(const std::string& _return_type_str)
        {
            return [_return_type_str](_Owner_Class* _owner_object, _Func_Type _func, _Args_Container_Type& _args_container) \
            { \
                    _Return_Type return_value = _args_container.call_with_args(*_owner_object, _func); \
                    std::string default_return_type_name = LV::Type_Manager::get_default_type_name(_return_type_str); \
                    LV::Type_Utility::Allocate_Result allocate_result = LV::Type_Manager::allocate(default_return_type_name, 1); \
                    LScript::Variable_Container* return_container = new LScript::Variable_Container; \
                    return_container->set_type(default_return_type_name); \
                    return_container->set_data(allocate_result.ptr, allocate_result.size); \
                    LV::Type_Manager::copy(default_return_type_name, allocate_result.ptr, &return_value); \
                    return return_container; \
            };
        }
    };

    template <>
    class __Calling_Function_Construction_Helper<void>
    {
    public:
        template <typename _Owner_Class, typename _Func_Type, typename _Args_Container_Type>
        LST::Function<LScript::Variable*(_Owner_Class* _owner_object, _Func_Type _func, _Args_Container_Type& _args_container)>
        construct_calling_function(const std::string& _return_type_str)
        {
            return [](_Owner_Class* _owner_object, _Func_Type _func, _Args_Container_Type& _args_container)\
            {
                    _args_container.call_with_args(*_owner_object, _func);
                    return nullptr;
            };
        }
    };

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

#define SCRIPTABLE_FUNCTION_RETURN_TYPE(TYPE) \
        using Return_Type = TYPE; \
        std::string return_type_str = #TYPE;

#define SCRIPTABLE_FUNCTION_NAME(FUNCTION_NAME) \
        auto member_function = &Scriptable_Function_Owner::FUNCTION_NAME; \
        using Member_Function_Type = decltype(member_function); \
        std::string member_function_name = #FUNCTION_NAME; \
        using Arguments_Container = decltype(LScript::__construct_args_container(member_function)); \
        \
        LST::Function<LScript::Variable*(Scriptable_Function_Owner* _owner_object, Member_Function_Type _func, Arguments_Container& _args_container)> \
            construct_result = LScript::__Calling_Function_Construction_Helper<Return_Type>().construct_calling_function<Scriptable_Function_Owner, Member_Function_Type, Arguments_Container>(return_type_str); \
        \
        LScript::Function::Arguments_Data arguments_data; \
        arguments_data.push({scriptable_function_owner_name, "this", true}); \
        LScript::Function* function = new LScript::Function; \
        function->set_return_type(return_type_str == "void" ? return_type_str : LV::Type_Manager::get_default_type_name(return_type_str)); \
        LScript::Custom_Operation* call_scriptable_function_operation = new LScript::Custom_Operation; \
        call_scriptable_function_operation->set_operation_logic([scriptable_function_owner_name, function, member_function, construct_result]() \
        { \
                Arguments_Container args_container; \
                for(unsigned int i = 0; i < args_container.arguments_amount(); ++i) \
            { \
                    void* arg_raw = nullptr; \
                    args_container.init_pointer(i, arg_raw); \
                    LScript::Variable* variable = function->compound_statement().context().get_variable("_" + std::to_string(i)); \
                    L_ASSERT(variable); \
                    void* variable_raw = (void*)variable->data(); \
                    LV::Type_Manager::copy(function->expected_arguments_data()[1].expected_type, arg_raw, variable_raw); \
            } \
                LScript::Variable* context_object_variable = function->compound_statement().context().get_variable("this"); \
                L_ASSERT(context_object_variable); \
                L_ASSERT(scriptable_function_owner_name == context_object_variable->type()); \
                Scriptable_Function_Owner* context_object = (Scriptable_Function_Owner*)context_object_variable->data(); \
                LScript::Variable* return_variable = construct_result(context_object, member_function, args_container); \
                if(return_variable) \
                    function->compound_statement().context().add_variable("__result__", return_variable); \
                return return_variable; \
        })

#define SCRIPTABLE_FUNCTION_ARG(TYPE) \
        { \
            const std::string& default_type_name = LV::Type_Manager::get_default_type_name(#TYPE); \
            arguments_data.push({default_type_name, "_" + std::to_string(arguments_data.size() - 1), true}); \
        }

#define SCRIPTABLE_FUNCTION_INITIALIZATION_END \
        L_ASSERT(arguments_data.size() == Arguments_Container::arguments_amount() + 1); \
        function->compound_statement().add_operation(call_scriptable_function_operation); \
        function->set_expected_arguments_data(arguments_data); \
        LScript::Integrated_Functions::instance().register_member_function(scriptable_function_owner_name, member_function_name, function); \
    }

