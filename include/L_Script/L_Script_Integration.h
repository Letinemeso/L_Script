#pragma once

#include <Data_Structures/Vector.h>
#include <Stuff/Arguments_Container.h>

#include <L_Script/Script.h>
#include <L_Script/Script_Details/Operations/Custom_Operation.h>
#include <L_Script/Integrated_Functions.h>


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
            return [_return_type_str](_Owner_Class* _owner_object, _Func_Type _func, _Args_Container_Type& _args_container)
            {
                _Return_Type return_value = _args_container.call_with_args(*_owner_object, _func);
                std::string default_return_type_name = LV::Type_Manager::get_default_type_name(_return_type_str);
                LV::Type_Utility::Allocate_Result allocate_result = LV::Type_Manager::allocate(default_return_type_name, 1);
                LScript::Variable_Container* return_container = new LScript::Variable_Container;
                return_container->set_type(default_return_type_name);
                return_container->set_data(allocate_result.ptr, allocate_result.size);
                LV::Type_Manager::copy(default_return_type_name, allocate_result.ptr, &return_value);
                return return_container;
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
            return [](_Owner_Class* _owner_object, _Func_Type _func, _Args_Container_Type& _args_container)
            {
                _args_container.call_with_args(*_owner_object, _func);
                return nullptr;
            };
        }
    };


    template <typename _Owner_Type, typename _Return_Type, typename _Function_Type>
    void register_l_script_function(const std::string& _owner_name, const std::string& _function_name,
                                    const std::string& _return_type, const LDS::Vector<std::string>& _arguments_types,
                                    _Function_Type _function)
    {
        using Arguments_Container_Type = decltype(LScript::__construct_args_container(_function));

        L_ASSERT(_arguments_types.size() == Arguments_Container_Type::arguments_amount());

        LST::Function<LScript::Variable*(_Owner_Type* _owner_object, _Function_Type _func, Arguments_Container_Type& _args_container)>
            construct_result = LScript::__Calling_Function_Construction_Helper<_Return_Type>().construct_calling_function<_Owner_Type, _Function_Type, Arguments_Container_Type>(_return_type);

        LScript::Function::Arguments_Data arguments_data;
        arguments_data.push({_owner_name, "this", true});
        for(unsigned int i = 0; i < _arguments_types.size(); ++i)
        {
            const std::string& default_type_name = LV::Type_Manager::get_default_type_name(_arguments_types[i]);
            arguments_data.push({default_type_name, "_" + std::to_string(arguments_data.size() - 1), true});
        }

        LScript::Function* function = new LScript::Function;
        function->set_return_type(_return_type == "void" ? _return_type : LV::Type_Manager::get_default_type_name(_return_type));
        LScript::Custom_Operation* call_scriptable_function_operation = new LScript::Custom_Operation;
        call_scriptable_function_operation->set_operation_logic([_owner_name, function, _function, construct_result]()
        {
            Arguments_Container_Type args_container;
            for(unsigned int i = 0; i < args_container.arguments_amount(); ++i)
            {
                void* arg_raw = nullptr;
                args_container.init_pointer(i, arg_raw);
                LScript::Variable* variable = function->compound_statement().context().get_variable("_" + std::to_string(i));
                L_ASSERT(variable);
                void* variable_raw = (void*)variable->data();
                LV::Type_Manager::copy(function->expected_arguments_data()[1].expected_type, arg_raw, variable_raw);
            }
            LScript::Variable* context_object_variable = function->compound_statement().context().get_variable("this");
            L_ASSERT(context_object_variable);
            L_ASSERT(_owner_name == context_object_variable->type());
            _Owner_Type* context_object = (_Owner_Type*)context_object_variable->data();
            LScript::Variable* return_variable = construct_result(context_object, _function, args_container);
            if(return_variable)
                function->compound_statement().context().add_variable("__result__", return_variable);
            return return_variable;
        });

        function->compound_statement().add_operation(call_scriptable_function_operation);
        function->set_expected_arguments_data(arguments_data);
        LScript::Integrated_Functions::instance().register_member_function(_owner_name, _function_name, function);
    }

}
