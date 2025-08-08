#pragma once

#include <Script.h>

#include <Script_Details/Function.h>
#include <Script_Details/Compound_Statement.h>


namespace LScript
{

    class Compiler
    {
    private:
        static constexpr unsigned int Unlimited_Size = 0xFFFFFFFF;

        enum class Expression_Type
        {
            Unknown,
            Type_Name,
            Variable_Name,
            If,
            For,
            While,
            Return,
        };

        enum class Expression_Goal
        {
            Unknown,
            Variable_Declaration,
            Function_Declaration,
            Member_Acces,
            Function_Call,
        };

    public:
        using Acceptable_Symbols = LDS::Vector<bool>;

    private:
        Script* m_script_target = nullptr;

    public:
        Compiler();
        ~Compiler();

    public:
        inline void set_target(Script* _ptr) { m_script_target = _ptr; }

    private:
        void M_parse_global_space(const std::string& _source) const;

        unsigned int M_parse_global_expression(const std::string& _source, unsigned int _offset, unsigned int _max_size = Unlimited_Size) const;
        unsigned int M_parse_global_variable_declaration(Context& _context, const std::string& _source, const std::string& _type, const std::string& _name, unsigned int _offset_after_name) const;
        unsigned int M_parse_function_declaration(const std::string& _source, const std::string& _type, const std::string& _name, unsigned int _offset_after_name) const;

        LDS::Vector<Function::Argument_Data> M_parse_function_arguments_data(const std::string& _source, unsigned int _begin, unsigned int _end) const;
        void M_parse_function_body(Function& _function, const std::string& _source, unsigned int _begin, unsigned int _end) const;
        unsigned int M_parse_dynamic_expression(Compound_Statement& _compound_statement, const std::string& _source, unsigned int _offset, unsigned int _max_size) const;
        unsigned int M_parse_dynamic_declaration(Compound_Statement& _compound_statement, const std::string& _type, const std::string& _source, unsigned int _offset, unsigned int _max_size) const;
        unsigned int M_parse_operation_with_variable(Compound_Statement& _compound_statement, const std::string& _name, const std::string& _source, unsigned int _offset, unsigned int _max_size) const;
        unsigned int M_parse_function_call(Compound_Statement& _compound_statement, const std::string& _name, const std::string& _source, unsigned int _offset, unsigned int _max_size) const;
        LDS::Vector<Operation*> M_construct_argument_getter_operations(Context& _context, const std::string& _source, unsigned int _args_begin, unsigned int _args_end) const;
        LDS::Vector<std::string> M_parse_passed_arguments(const std::string& _source, unsigned int _begin, unsigned int _end) const;
        std::string M_deduce_rvalue_type(const std::string& _rvalue) const;

        void M_cull_empty_symbols(std::string& _from) const;

        unsigned int M_skip_until_symbol_met(const std::string& _source, const Acceptable_Symbols& _acceptable_symbols, bool _symbols_expected = true, unsigned int _offset = 0, unsigned int _max_size = Unlimited_Size) const;
        unsigned int M_skip_until_symbol_met(const std::string& _source, char _symbol, bool _symbols_expected = true, unsigned int _offset = 0, unsigned int _max_size = Unlimited_Size) const;
        unsigned int M_skip_until_closer(const std::string& _source, char _opener, char _closer, unsigned int _offset = 0, unsigned int _max_size = Unlimited_Size) const;
        unsigned int M_skip_past_semicolon(const std::string& _source, unsigned int _offset, unsigned int _max_size = Unlimited_Size) const;

        Expression_Type M_get_expression_type(const std::string& _expression) const;

        Expression_Goal M_function_or_variable_declaration(const std::string& _source, unsigned int _offset_after_name) const;
        Expression_Goal M_member_acces_or_function_call(const std::string& _source, unsigned int _offset_after_name) const;
        bool M_is_existing_variable(const Context& _context, const std::string& _name) const;
        bool M_can_be_variable_name(const std::string& _name) const;

    public:
        void compile(const std::string& _source) const;

    };

}
