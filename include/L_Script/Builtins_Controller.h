#pragma once

#include <Data_Structures/Map.h>

#include <L_Script/Script_Details/Function.h>


namespace LScript
{

    class Builtins_Controller
    {
    public:
        using Argument_Types = LDS::Vector<std::string>;

    private:
        using Functions_Vec = LDS::Vector<Function*>;
        using Functions = LDS::Map<std::string, Function*>;
        using Functions_By_Name = LDS::Map<std::string, Functions_Vec>;;
        using Member_Functions = LDS::Map<std::string, Functions>;

        Functions m_global_functions;
        Functions_By_Name m_global_functions_by_name;
        Member_Functions m_member_functions;

    private:
        Builtins_Controller();
        ~Builtins_Controller();

        Builtins_Controller(const Builtins_Controller&) = delete;
        Builtins_Controller(Builtins_Controller&&) = delete;

    private:
        void M_register_default_global_functions();
        void M_register_default_int_functions();
        void M_register_default_unsigned_int_functions();
        void M_register_default_float_functions();
        void M_register_default_string_functions();
        void M_register_default_bool_functions();
        void M_register_default_vector_functions();

    private:
        bool M_function_arguments_match(const Function* _function, const Argument_Types& _arg_types) const;
        Function* M_find_matching_function_by_name(const std::string& _name, const Argument_Types& _arg_types) const;

    public:
        static inline Builtins_Controller& instance() { static Builtins_Controller s_instance; return s_instance; }

    public:
        std::string construct_function_name(const std::string& _name, const Function* _function, const std::string& _owner_type = {}) const;
        std::string construct_function_name(const std::string& _name, const Argument_Types& _arg_types, const std::string& _owner_type = {}) const;

    public:
        void register_global_function(const std::string& _name, Function* _function);
        void register_member_function(const std::string& _owner_type, const std::string& _name, Function* _function);

    public:
        Function* get_global_function(const std::string& _name, const Argument_Types& _arg_types) const;
        Function* get_member_function(const std::string& _owner_type, const std::string& _name, const Argument_Types& _arg_types) const;

    };

}
