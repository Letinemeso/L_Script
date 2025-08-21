#pragma once

#include <Data_Structures/Map.h>

#include <Script_Details/Function.h>


namespace LScript
{

    class Integrated_Functions
    {
    public:
        using Argument_Types = LDS::Vector<std::string>;

    private:
        using Functions = LDS::Map<std::string, Function*>;
        using Member_Functions = LDS::Map<std::string, Functions>;

        Functions m_global_functions;
        Member_Functions m_member_functions;

    private:
        Integrated_Functions();
        ~Integrated_Functions();

        Integrated_Functions(const Integrated_Functions&) = delete;
        Integrated_Functions(Integrated_Functions&&) = delete;

    private:
        void M_register_default_global_functions();
        void M_register_default_int_functions();
        void M_register_default_string_functions();

    public:
        static inline Integrated_Functions& instance() { static Integrated_Functions s_instance; return s_instance; }

    private:
        std::string M_construct_function_name(const std::string& _name, const Function* _function, const std::string& _owner_type = {}) const;
        std::string M_construct_function_name(const std::string& _name, const Argument_Types& _arg_types, const std::string& _owner_type = {}) const;

    public:
        void register_global_function(const std::string& _name, Function* _function);
        void register_member_function(const std::string& _owner_type, const std::string& _name, Function* _function);

    public:
        Function* get_global_function(const std::string& _name, const Argument_Types& _arg_types) const;
        Function* get_member_function(const std::string& _owner_type, const std::string& _name, const Argument_Types& _arg_types) const;

    };

}
