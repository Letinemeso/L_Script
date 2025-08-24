#pragma once

#include <Script.h>

#include <Script_Details/Function.h>
#include <Script_Details/Compound_Statement.h>
#include <Script_Details/Operations/RValue_Getter.h>


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
            While,
            Return,
        };

        enum class Expression_Goal
        {
            Unknown,
            Variable_Declaration,
            Function_Declaration,
            Member_Access,
            Function_Call,
        };

        struct Operation_Parse_Result
        {
            Operation* operation = nullptr;
            unsigned int offset_after = 0;
        };

        struct String_Borders
        {
            unsigned int begin = 0;
            unsigned int end = 0;
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
        unsigned int M_calculate_line_number(const std::string& _source, unsigned int _offset) const;
        std::string M_parse_line(const std::string& _source, unsigned int _offset) const;
        void M_print_debug_error_message(bool _condition, const std::string& _source, unsigned int _offset, unsigned int _marker_length, const std::string& _message) const;

    private:
        void M_parse_global_space(const std::string& _source) const;

        unsigned int M_parse_global_expression(const std::string& _source, unsigned int _offset, unsigned int _max_size = Unlimited_Size) const;
        unsigned int M_parse_global_variable_declaration(Context& _context, const std::string& _source, const std::string& _type, const std::string& _name, unsigned int _offset_after_name) const;
        unsigned int M_parse_function_declaration(const std::string& _source, const std::string& _type, const std::string& _name, unsigned int _offset_after_name, unsigned int _max_size) const;

        LDS::Vector<Function::Argument_Data> M_parse_function_arguments_data(const std::string& _source, unsigned int _begin, unsigned int _end) const;
        void M_parse_compound_statement(Compound_Statement& _compound_statement, const std::string& _owner_function_return_type, const std::string& _source, unsigned int _begin, unsigned int _end) const;
        [[nodiscard]] Operation_Parse_Result M_parse_dynamic_expression(Context& _context, const std::string& _owner_function_return_type, const std::string& _source, unsigned int _offset, unsigned int _max_size) const;
        [[nodiscard]] Operation_Parse_Result M_parse_dynamic_declaration(Context& _context, const std::string& _type, const std::string& _source, unsigned int _offset, unsigned int _max_size) const;
        [[nodiscard]] Operation_Parse_Result M_parse_operation_with_variable(Context& _context, const std::string& _name, const std::string& _source, unsigned int _offset, unsigned int _max_size) const;
        [[nodiscard]] Compiler::Operation_Parse_Result M_parse_function_call(Context& _context, const std::string& _name, const std::string& _source, unsigned int _offset, unsigned int _max_size) const;
        [[nodiscard]] LDS::Vector<Operation*> M_construct_argument_getter_operations(Context& _context, const std::string& _source, unsigned int _args_begin, unsigned int _args_end) const;
        LDS::Vector<std::string> M_parse_passed_arguments(const std::string& _source, unsigned int _begin, unsigned int _end) const;
        std::string M_deduce_rvalue_type(const std::string& _rvalue) const;
        [[nodiscard]] Operation_Parse_Result M_parse_return(Context& _context, const std::string& _expected_return_type, const std::string& _source, unsigned int _offset, unsigned int _max_size) const;
        [[nodiscard]] Operation_Parse_Result M_parse_if(Context& _context, const std::string& _owner_function_return_type, const std::string& _source, unsigned int _offset, unsigned int _max_size) const;
        [[nodiscard]] Operation_Parse_Result M_parse_while(Context& _context, const std::string& _owner_function_return_type, const std::string& _source, unsigned int _offset, unsigned int _max_size) const;

        [[nodiscard]] Operation_Parse_Result M_parse_subexpression(Context& _context, const std::string& _source, unsigned int _offset, unsigned int _max_size) const;
        [[nodiscard]] RValue_Getter* M_construct_rvalue_getter(const std::string& _value_as_string) const;

        std::string M_parse_first_word(const std::string& _source, unsigned int _offset, unsigned int _max_size) const;
        unsigned int M_skip_past_first_word(const std::string& _source, unsigned int _offset, unsigned int _max_size) const;
        void M_cull_empty_symbols(std::string& _from) const;

        unsigned int M_skip_until_symbol_met(const std::string& _source, const Acceptable_Symbols& _acceptable_symbols, bool _symbols_expected = true, unsigned int _offset = 0, unsigned int _max_size = Unlimited_Size) const;
        unsigned int M_skip_until_symbol_met_reverse(const std::string& _source, const Acceptable_Symbols& _acceptable_symbols, bool _symbols_expected, unsigned int _max, unsigned int _min) const;
        unsigned int M_skip_until_symbol_met(const std::string& _source, char _symbol, bool _symbols_expected = true, unsigned int _offset = 0, unsigned int _max_size = Unlimited_Size) const;
        unsigned int M_skip_until_closer(const std::string& _source, char _opener, char _closer, unsigned int _offset = 0, unsigned int _max_size = Unlimited_Size) const;
        unsigned int M_skip_past_semicolon(const std::string& _source, unsigned int _offset, unsigned int _max_size = Unlimited_Size) const;
        String_Borders M_calculate_arguments_borders(const std::string& _source, unsigned int _offset, unsigned int _max_size) const;
        String_Borders M_calculate_compound_statement_borders(const std::string& _source, unsigned int _offset, unsigned int _max_size) const;

        Expression_Type M_get_expression_type(const std::string& _expression) const;

        Expression_Goal M_function_or_variable_declaration(const std::string& _source, unsigned int _offset_after_name) const;
        Expression_Goal M_member_access_or_function_call(const std::string& _source, unsigned int _offset_after_name) const;
        bool M_is_existing_variable(const Context& _context, const std::string& _name) const;
        bool M_can_be_variable_name(const std::string& _name) const;

    public:
        void compile(const std::string& _source) const;

    };

}
