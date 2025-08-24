#include <Compiler.h>

#include <Data_Structures/AVL_Tree.h>

#include <Integrated_Functions.h>
#include <Script_Details/Operations/Operation.h>
#include <Script_Details/Operations/Variable_Creation.h>
#include <Script_Details/Operations/Extract_Variable.h>
#include <Script_Details/Operations/Call_Member_Function.h>
#include <Script_Details/Operations/Call_Global_Function.h>
#include <Script_Details/Operations/RValue_Getter.h>
#include <Script_Details/Operations/Custom_Operation.h>
#include <Script_Details/Operations/If_Operation.h>
#include <Script_Details/Operations/While_Operation.h>

using namespace LScript;

namespace LScript
{
    constexpr const char* String_Type_Name = "string";
    constexpr const char* Void_Type_Name = "void";
    constexpr const char* Bool_Type_Name = "bool";

    constexpr const char* If_Expression = "if";
    constexpr const char* Else_Expression = "else";
    constexpr const char* While_Expression = "while";
    constexpr const char* Return_Expression = "return";

    constexpr unsigned int Max_Symbols = 0xFF;

    Compiler::Acceptable_Symbols Empty_Symbols(Max_Symbols, false);
    Compiler::Acceptable_Symbols Variable_Name_Symbols(Max_Symbols, false);
    Compiler::Acceptable_Symbols Type_Name_Symbols(Max_Symbols, false);
    Compiler::Acceptable_Symbols Maybe_Value_Symbols(Max_Symbols, false);

    using Reserved_Keywords_Tree = LDS::AVL_Tree<std::string>;
    Reserved_Keywords_Tree Reserved_Keywords;

    constexpr const char* Debug_Log_Level = "LScript_Debug";
}



Compiler::Compiler()
{
    static bool s_initialized = false;
    if(s_initialized)
        return;
    s_initialized = true;

    Empty_Symbols[' '] = true;
    Empty_Symbols['\n'] = true;
    Empty_Symbols['\t'] = true;
    Empty_Symbols['\r'] = true;

    for(char i = 'a'; i <= 'z'; ++i)
        Variable_Name_Symbols[i] = true;
    for(char i = 'A'; i <= 'Z'; ++i)
        Variable_Name_Symbols[i] = true;
    for(char i = '0'; i <= '9'; ++i)
        Variable_Name_Symbols[i] = true;
    Variable_Name_Symbols['_'] = true;

    for(char i = 'a'; i <= 'z'; ++i)
        Type_Name_Symbols[i] = true;
    for(char i = 'A'; i <= 'Z'; ++i)
        Type_Name_Symbols[i] = true;
    for(char i = '0'; i <= '9'; ++i)
        Type_Name_Symbols[i] = true;
    Type_Name_Symbols['_'] = true;
    Type_Name_Symbols[':'] = true;
    Type_Name_Symbols['<'] = true;
    Type_Name_Symbols['>'] = true;
    Type_Name_Symbols['&'] = true;

    for(char i = 'a'; i <= 'z'; ++i)
        Maybe_Value_Symbols[i] = true;
    for(char i = 'A'; i <= 'Z'; ++i)
        Maybe_Value_Symbols[i] = true;
    for(char i = '0'; i <= '9'; ++i)
        Maybe_Value_Symbols[i] = true;
    Maybe_Value_Symbols['_'] = true;
    Maybe_Value_Symbols[':'] = true;
    Maybe_Value_Symbols['<'] = true;
    Maybe_Value_Symbols['>'] = true;
    Maybe_Value_Symbols['&'] = true;
    Maybe_Value_Symbols['-'] = true;

    Reserved_Keywords.insert(Void_Type_Name);
    Reserved_Keywords.insert(If_Expression);
    Reserved_Keywords.insert(While_Expression);
    Reserved_Keywords.insert(Return_Expression);
    Reserved_Keywords.insert("true");
    Reserved_Keywords.insert("false");

    L_CREATE_LOG_LEVEL(Debug_Log_Level);
}

Compiler::~Compiler()
{

}



unsigned int Compiler::M_calculate_line_number(const std::string& _source, unsigned int _offset) const
{
    unsigned int result = 0;
    for(unsigned int i = 0; i < _offset; ++i)
    {
        if(_source[i] == '\n')
            ++result;
    }
    return result;
}

std::string Compiler::M_parse_line(const std::string& _source, unsigned int _offset) const
{
    unsigned int line_offset = _offset;
    for(; line_offset > 1; --line_offset)
    {
        if(_source[line_offset] == '\n')
            break;
    }
    if(line_offset > 0)
        ++line_offset;

    unsigned int line_end = _offset;
    for(; line_end < _source.size(); ++line_end)
    {
        if(_source[line_end] == '\n')
            break;
    }
    return _source.substr(line_offset, line_end - line_offset);
}

void Compiler::M_print_debug_error_message(bool _condition, const std::string& _source, unsigned int _offset, unsigned int _marker_length, const std::string& _message) const
{
    if(_condition)
        return;

    unsigned int line_offset = _offset;
    for(; line_offset > 1; --line_offset)
    {
        if(_source[line_offset] == '\n')
            break;
    }
    ++line_offset;

    unsigned int line_end = _offset;
    for(; line_end < _source.size(); ++line_end)
    {
        if(_source[line_end] == '\n')
            break;
    }

    unsigned int line = M_calculate_line_number(_source, _offset);

    std::string line_str = _source.substr(line_offset, line_end - line_offset);
    std::string marker;
    marker.resize(line_str.size());

    if(_marker_length < Unlimited_Size)
    {
        for(unsigned int i = 0; i < line_str.size(); ++i)
            marker[i] = '-';
        unsigned int difference = _offset - line_offset;
        if(_marker_length > line_str.size() - difference)
            _marker_length = line_str.size() - difference;

        for(unsigned int i = 0; i < _marker_length; ++i)
            marker[difference + i] = '^';
    }
    else
    {
        for(unsigned int i = 0; i < line_str.size(); ++i)
            marker[i] = '^';
    }

    L_LOG(Debug_Log_Level, "Error at line " + std::to_string(line) + ": ");
    L_LOG(Debug_Log_Level, _message);
    L_LOG(Debug_Log_Level, line_str);
    L_LOG(Debug_Log_Level, marker);

    L_ASSERT(false);
}



void Compiler::M_parse_global_space(const std::string& _source) const
{
    unsigned int offset = 0;
    while(offset < _source.size())
    {
        offset = M_parse_global_expression(_source, offset);
    }
}


unsigned int Compiler::M_parse_global_expression(const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    if(_max_size == Unlimited_Size)
        _max_size = _source.size();

    std::string first_word = M_parse_first_word(_source, _offset, _max_size);
    if(first_word.size() == 0)
        return _max_size;
    unsigned int after_first_word_offset = M_skip_past_first_word(_source, _offset, _max_size);

    Expression_Type expression_type = M_get_expression_type(first_word);
    M_print_debug_error_message(expression_type == Expression_Type::Type_Name, _source, after_first_word_offset - first_word.size(), first_word.size(), "Only declarations available in global space");

    unsigned int second_word_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, after_first_word_offset);
    unsigned int after_second_word_offset = M_skip_until_symbol_met(_source, Variable_Name_Symbols, false, second_word_offset);
    M_print_debug_error_message(after_second_word_offset > second_word_offset, _source, second_word_offset, 1, "Expected name");

    unsigned int next_word_size = after_second_word_offset - second_word_offset;
    std::string second_word = _source.substr(second_word_offset, next_word_size);

    Expression_Goal expression_goal = M_function_or_variable_declaration(_source, after_second_word_offset);
    M_print_debug_error_message(expression_goal == Expression_Goal::Function_Declaration || expression_goal == Expression_Goal::Variable_Declaration,
                                _source, after_second_word_offset, Unlimited_Size, "Unknown expression");

    if(expression_goal == Expression_Goal::Function_Declaration)
        return M_parse_function_declaration(_source, first_word, second_word, after_second_word_offset, _max_size);
    else if(expression_goal == Expression_Goal::Variable_Declaration)
        return M_parse_global_variable_declaration(m_script_target->global_context(), _source, first_word, second_word, after_second_word_offset);

    return Unlimited_Size;
}

unsigned int Compiler::M_parse_global_variable_declaration(Context& _context, const std::string& _source, const std::string& _type, const std::string& _name, unsigned int _offset_after_name) const
{
    M_print_debug_error_message(_type != Void_Type_Name, _source, _offset_after_name, Unlimited_Size, "Unable to create variable with type \"void\"");
    M_print_debug_error_message(_context.get_local_variable(_name) == nullptr, _source, _offset_after_name - _name.size(), _name.size(), "Variable with name \"" + _name + "\" already exist");

    Variable_Container* variable = new Variable_Container;
    variable->set_type(_type);

    _context.add_variable(_name, variable);

    unsigned int semicolon_index = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset_after_name);
    M_print_debug_error_message(semicolon_index != Unlimited_Size, _source, _offset_after_name, 1, "Expected \";\"");
    M_print_debug_error_message(_source[semicolon_index] == ';', _source, semicolon_index, 1, "Expected \";\"");

    return semicolon_index + 1;
}

unsigned int Compiler::M_parse_function_declaration(const std::string& _source, const std::string& _type, const std::string& _name, unsigned int _offset_after_name, unsigned int _max_size) const
{
    String_Borders args_borders = M_calculate_arguments_borders(_source, _offset_after_name, _max_size);

    LDS::Vector<Function::Argument_Data> arguments_data = M_parse_function_arguments_data(_source, args_borders.begin, args_borders.end);

    String_Borders compound_statement_borders = M_calculate_compound_statement_borders(_source, args_borders.end + 1, _max_size);

    M_print_debug_error_message(m_script_target->get_function(_name) == nullptr, _source, _offset_after_name - _name.size(), _name.size(), "Function with name \"" + _name + "\" is already registered");

    Function* function = new Function;
    function->set_return_type(_type);
    function->set_expected_arguments_data(arguments_data);
    function->compound_statement().context().set_parent_context(&m_script_target->global_context());

    M_parse_compound_statement(function->compound_statement(), function->return_type(), _source, compound_statement_borders.begin, compound_statement_borders.end);

    m_script_target->register_function(_name, function);

    return compound_statement_borders.end + 1;
}


LDS::Vector<Function::Argument_Data> Compiler::M_parse_function_arguments_data(const std::string& _source, unsigned int _begin, unsigned int _end) const
{
    LDS::Vector<Function::Argument_Data> result;

    unsigned int offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, _begin, _end);
    if(offset + 1 >= _end)
        return result;

    while(offset < _end)
    {
        unsigned int type_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, offset, _end);
        unsigned int after_type_offset = M_skip_until_symbol_met(_source, Type_Name_Symbols, false, type_offset, _end);
        M_print_debug_error_message(after_type_offset > type_offset, _source, type_offset, 1, "Expected argument type");

        unsigned int name_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, after_type_offset, _end);
        unsigned int after_name_offset = M_skip_until_symbol_met(_source, Variable_Name_Symbols, false, name_offset, _end);
        if(after_name_offset == Unlimited_Size)
            after_name_offset = _end;
        M_print_debug_error_message(after_name_offset > name_offset, _source, name_offset, 1, "Expected argument name");

        std::string type = _source.substr(type_offset, after_type_offset - type_offset);
        std::string name = _source.substr(name_offset, after_name_offset - name_offset);
        bool reference = type[type.size() - 1] == '&';
        if(reference)
            type.pop_back();

        M_print_debug_error_message(LV::Type_Manager::type_is_registered(type), _source, type_offset, type.size(), "Unknown type \"" + type + "\"");

        result.push({type, name, reference});

        offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, after_name_offset, _end);
        if(offset >= _end)
            break;

        M_print_debug_error_message(_source[offset] == ',', _source, offset, 1, "Expected \",\"");
        ++offset;
    }

    return result;
}

void Compiler::M_parse_compound_statement(Compound_Statement& _compound_statement, const std::string& _owner_function_return_type, const std::string& _source, unsigned int _begin, unsigned int _end) const
{
    unsigned int offset = _begin;
    while(offset < _end)
    {
        Operation_Parse_Result parse_result = M_parse_dynamic_expression(_compound_statement.context(), _owner_function_return_type, _source, offset, _end);

        if(!parse_result.operation)
            break;

        _compound_statement.add_operation(parse_result.operation);
        offset = parse_result.offset_after;
    }
}

Compiler::Operation_Parse_Result Compiler::M_parse_dynamic_expression(Context& _context, const std::string& _owner_function_return_type, const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    if(_max_size == Unlimited_Size)
        _max_size = _source.size();

    std::string first_word = M_parse_first_word(_source, _offset, _max_size);
    if(first_word.size() == 0)
        return { nullptr, _max_size };
    unsigned int after_first_word_offset = M_skip_past_first_word(_source, _offset, _max_size);

    Expression_Type expression_type = M_get_expression_type(first_word);
    M_print_debug_error_message(expression_type != Expression_Type::Unknown, _source, _offset, Unlimited_Size, "Unknown expression");

    Operation_Parse_Result parse_result;

    if(expression_type == Expression_Type::If)
    {
        parse_result = M_parse_if(_context, _owner_function_return_type, _source, after_first_word_offset, _max_size);
    }
    else if(expression_type == Expression_Type::While)
    {
        parse_result = M_parse_while(_context, _owner_function_return_type, _source, after_first_word_offset, _max_size);
    }
    else if(expression_type == Expression_Type::Return)
    {
        parse_result = M_parse_return(_context, _owner_function_return_type, _source, after_first_word_offset, _max_size);
    }
    else if(expression_type == Expression_Type::Type_Name)
    {
        parse_result = M_parse_dynamic_declaration(_context, first_word, _source, after_first_word_offset, _max_size);
    }
    else if(expression_type == Expression_Type::Variable_Name)
    {
        Expression_Goal expression_goal = M_member_access_or_function_call(_source, after_first_word_offset);

        if(expression_goal == Expression_Goal::Member_Access)
            parse_result = M_parse_operation_with_variable(_context, first_word, _source, after_first_word_offset, _max_size);
        else if(expression_goal == Expression_Goal::Function_Call)
            parse_result = M_parse_function_call(_context, first_word, _source, after_first_word_offset, _max_size);

        parse_result.offset_after = M_skip_past_semicolon(_source, parse_result.offset_after, _max_size);
    }

    M_print_debug_error_message(parse_result.operation, _source, after_first_word_offset, Unlimited_Size, "Could not parse an expression");

    return parse_result;
}

Compiler::Operation_Parse_Result Compiler::M_parse_dynamic_declaration(Context& _context, const std::string& _type, const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    unsigned int name_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset);
    unsigned int after_name_offset = M_skip_until_symbol_met(_source, Variable_Name_Symbols, false, name_offset);
    M_print_debug_error_message(after_name_offset > name_offset, _source, name_offset, 1, "Expected name");

    unsigned int next_word_size = after_name_offset - name_offset;
    std::string name = _source.substr(name_offset, next_word_size);

    M_print_debug_error_message(M_function_or_variable_declaration(_source, after_name_offset) == Expression_Goal::Variable_Declaration,
                                _source, after_name_offset, 1, "Unable to declare a function inside a function");

    Variable_Creation* operation = new Variable_Creation;
    operation->set_context(&_context);
    operation->set_variable_type(_type);
    operation->set_variable_name(name);

    Operation_Parse_Result result;
    result.operation = operation;
    result.offset_after = M_skip_past_semicolon(_source, after_name_offset, _max_size);
    return result;
}

Compiler::Operation_Parse_Result Compiler::M_parse_operation_with_variable(Context& _context, const std::string& _name, const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    M_print_debug_error_message(_source[_offset] == '.', _source, _offset, 1, "Expected \".\"");

    unsigned int operation_offset = _offset + 1;
    unsigned int after_operation_offset = M_skip_until_symbol_met(_source, Variable_Name_Symbols, false, operation_offset, _max_size);
    L_ASSERT(after_operation_offset > operation_offset);
    std::string operation_name = _source.substr(operation_offset, after_operation_offset - operation_offset);

    String_Borders args_borders = M_calculate_arguments_borders(_source, after_operation_offset, _max_size);

    LDS::Vector<Operation*> argument_getters = M_construct_argument_getter_operations(_context, _source, args_borders.begin, args_borders.end);

    Extract_Variable* extract_variable_operation = new Extract_Variable;
    extract_variable_operation->set_context(&_context);
    extract_variable_operation->set_variable_name(_name);

    L_DEBUG_FUNC_NOARG([&]()
    {
        extract_variable_operation->set_debug_info(M_parse_line(_source, _offset), M_calculate_line_number(_source, _offset));
    });

    Call_Member_Function* call_member_function_operation = new Call_Member_Function;
    call_member_function_operation->set_context(&_context);
    call_member_function_operation->set_function_name(operation_name);
    call_member_function_operation->set_owner_object_getter_operation(extract_variable_operation);
    call_member_function_operation->set_arguments_getter_operations((LDS::Vector<Operation*>&&)argument_getters);

    L_DEBUG_FUNC_NOARG([&]()
    {
        call_member_function_operation->set_debug_info(M_parse_line(_source, _offset), M_calculate_line_number(_source, _offset));
    });

    Operation_Parse_Result result;
    result.operation = call_member_function_operation;
    result.offset_after = args_borders.end + 1;
    return result;
}

Compiler::Operation_Parse_Result Compiler::M_parse_function_call(Context& _context, const std::string& _name, const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    String_Borders args_borders = M_calculate_arguments_borders(_source, _offset, _max_size);

    LDS::Vector<Operation*> argument_getters = M_construct_argument_getter_operations(_context, _source, args_borders.begin, args_borders.end);

    Call_Global_Function* operation = new Call_Global_Function;
    operation->set_function_name(_name);
    operation->set_script(m_script_target);
    operation->set_arguments_getter_operations((LDS::Vector<Operation*>&&)argument_getters);

    L_DEBUG_FUNC_NOARG([&]()
    {
        operation->set_debug_info(M_parse_line(_source, _offset), M_calculate_line_number(_source, _offset));
    });

    Operation_Parse_Result result;
    result.operation = operation;
    result.offset_after = args_borders.end + 1;
    return result;
}

LDS::Vector<Operation*> Compiler::M_construct_argument_getter_operations(Context& _context, const std::string& _source, unsigned int _args_begin, unsigned int _args_end) const
{
    LDS::Vector<std::string> arguments = M_parse_passed_arguments(_source, _args_begin, _args_end);

    if(arguments.size() == 0)
        return {};

    LDS::Vector<Operation*> operations(arguments.size());

    for(unsigned int i = 0; i < arguments.size(); ++i)
    {
        std::string& argument = arguments[i];

        std::string first_word = M_parse_first_word(argument, 0, Unlimited_Size);
        unsigned int after_first_word_offset = M_skip_past_first_word(argument, 0, Unlimited_Size);

        Expression_Type expression_type = M_get_expression_type(first_word);

        if(expression_type == Expression_Type::Unknown)     //  treat this as a possible r-value
        {
            RValue_Getter* operation = M_construct_rvalue_getter(argument);
            operations.push(operation);
        }
        else if(expression_type == Expression_Type::Variable_Name)
        {
            Expression_Goal expression_goal = M_member_access_or_function_call(argument, after_first_word_offset);

            if(expression_goal == Expression_Goal::Unknown)
            {
                Extract_Variable* operation = new Extract_Variable;
                operation->set_context(&_context);
                operation->set_variable_name(argument);

                L_DEBUG_FUNC_NOARG([&]()
                {
                    operation->set_debug_info(M_parse_line(_source, 0), M_calculate_line_number(_source, 0));
                });

                operations.push(operation);
            }
            else if(expression_goal == Expression_Goal::Member_Access)
            {
                Operation_Parse_Result parse_result = M_parse_operation_with_variable(_context, first_word, argument, after_first_word_offset, Unlimited_Size);
                operations.push(parse_result.operation);
            }
            else if(expression_goal == Expression_Goal::Function_Call)
            {
                Operation_Parse_Result parse_result = M_parse_function_call(_context, first_word, argument, after_first_word_offset, Unlimited_Size);
                operations.push(parse_result.operation);
            }
        }
        else
        {
            M_print_debug_error_message(false, _source, _source.find(argument, _args_begin), argument.size(), "Unexpected expression type as an argument");
        }
    }

    return operations;
}

LDS::Vector<std::string> Compiler::M_parse_passed_arguments(const std::string& _source, unsigned int _begin, unsigned int _end) const
{
    unsigned int offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, _begin, _end);
    if(offset >= _end)
        return {};

    M_print_debug_error_message(_source[offset] != ',', _source, offset, 1, "Expected value");

    unsigned int amount = 1;

    unsigned int openers_met = 0;
    for(unsigned int i = offset; i < _end; ++i)
    {
        if(_source[i] == '(')
        {
            ++openers_met;
            continue;
        }

        if(_source[i] == ')')
        {
            M_print_debug_error_message(openers_met > 0, _source, i, Unlimited_Size, "Extra \")\"");
            --openers_met;
            continue;
        }

        if(_source[i] != ',' || openers_met > 0)
            continue;

        ++amount;
    }

    LDS::Vector<std::string> arguments(amount);

    openers_met = 0;
    for(unsigned int i = offset; i < _end; ++i)
    {
        if(_source[i] == '(')
        {
            ++openers_met;
            continue;
        }

        if(_source[i] == ')')
        {
            M_print_debug_error_message(openers_met > 0, _source, i, Unlimited_Size, "Extra \")\"");
            --openers_met;
            continue;
        }

        if(_source[i] != ',' || openers_met > 0)
            continue;

        std::string arg = _source.substr(offset, i - offset);
        M_cull_empty_symbols(arg);
        arguments.push(arg);

        offset = i + 1;
    }

    std::string arg = _source.substr(offset, _end - offset);
    M_cull_empty_symbols(arg);
    arguments.push(arg);

    return arguments;
}

std::string Compiler::M_deduce_rvalue_type(const std::string& _rvalue) const
{
    M_print_debug_error_message(_rvalue.size() > 0, _rvalue, 0, Unlimited_Size, "Trying to deduce value from empty string");

    if(_rvalue.size() >= 2)
    {
        if(_rvalue[0] == '\'' && _rvalue[_rvalue.size() - 1] == '\'')
            return String_Type_Name;
    }

    if(LV::Type_Manager::validate("int", _rvalue))
        return "int";

    if(LV::Type_Manager::validate("float", _rvalue))
        return "float";

    if(LV::Type_Manager::validate("bool", _rvalue))
        return "bool";

    M_print_debug_error_message(false, _rvalue, 0, Unlimited_Size, "Unable to deduce type for \"" + _rvalue + "\"");
    return "";
}

Compiler::Operation_Parse_Result Compiler::M_parse_return(Context& _context, const std::string& _expected_return_type, const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    unsigned int offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset, _max_size);
    M_print_debug_error_message(offset < _max_size, _source, offset, 1, "Expected expression or \";\"");

    if(_source[offset] == ';' && _expected_return_type == Void_Type_Name)
    {
        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([](){ return nullptr; });     //  do nothing, just return
        operation->set_stop_required(true);

        Operation_Parse_Result result;
        result.offset_after = offset + 1;
        result.operation = operation;
        return result;
    }

    Operation_Parse_Result result = M_parse_subexpression(_context, _source, offset, _max_size);
    result.offset_after = M_skip_past_semicolon(_source, result.offset_after, _max_size);

    result.operation->set_stop_required(true);

    return result;
}

Compiler::Operation_Parse_Result Compiler::M_parse_if(Context& _context, const std::string& _owner_function_return_type, const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    Operation_Parse_Result result;

    If_Operation* if_operation = new If_Operation;
    result.operation = if_operation;

    unsigned int conditions_amount = 0;
    while(true)
    {
        String_Borders args_borders = M_calculate_arguments_borders(_source, _offset, _max_size);
        Operation_Parse_Result condition_parse_result = M_parse_subexpression(_context, _source, args_borders.begin, args_borders.end);

        Compound_Statement* compound_statement = new Compound_Statement;
        compound_statement->context().set_parent_context(&_context);
        String_Borders compound_statement_borders = M_calculate_compound_statement_borders(_source, args_borders.end + 1, _max_size);
        M_parse_compound_statement(*compound_statement, _owner_function_return_type, _source, compound_statement_borders.begin, compound_statement_borders.end);

        if_operation->add_case(condition_parse_result.operation, compound_statement);

        L_DEBUG_FUNC_NOARG([&]()
        {
            if_operation->add_debug_info(M_parse_line(_source, _offset), M_calculate_line_number(_source, _offset));
        });

        std::string maybe_else = M_parse_first_word(_source, compound_statement_borders.end + 1, _max_size);
        if(maybe_else != Else_Expression)
        {
            result.offset_after = compound_statement_borders.end + 1;
            return result;
        }

        _offset = M_skip_past_first_word(_source, compound_statement_borders.end + 1, _max_size);

        std::string maybe_if = M_parse_first_word(_source, _offset, _max_size);
        if(maybe_if != If_Expression)
            break;

        _offset = M_skip_past_first_word(_source, _offset, _max_size);

        ++conditions_amount;
    }

    String_Borders compound_statement_borders = M_calculate_compound_statement_borders(_source, _offset, _max_size);
    Compound_Statement* compound_statement = new Compound_Statement;
    compound_statement->context().set_parent_context(&_context);
    M_parse_compound_statement(*compound_statement, _owner_function_return_type, _source, compound_statement_borders.begin, compound_statement_borders.end);

    if_operation->add_fail_case(compound_statement);

    result.offset_after = compound_statement_borders.end + 1;

    return result;
}

Compiler::Operation_Parse_Result Compiler::M_parse_while(Context& _context, const std::string& _owner_function_return_type, const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    Operation_Parse_Result result;

    While_Operation* while_operation = new While_Operation;
    while_operation->cycle_compound_statement().context().set_parent_context(&_context);
    result.operation = while_operation;

    L_DEBUG_FUNC_NOARG([&]()
    {
        while_operation->set_debug_info(M_parse_line(_source, _offset), M_calculate_line_number(_source, _offset));
    });

    String_Borders args_borders = M_calculate_arguments_borders(_source, _offset, _max_size);
    Operation_Parse_Result condition_parse_result = M_parse_subexpression(_context, _source, args_borders.begin, args_borders.end);
    while_operation->set_condition_operation(condition_parse_result.operation);

    String_Borders compound_statement_borders = M_calculate_compound_statement_borders(_source, args_borders.end + 1, _max_size);
    M_parse_compound_statement(while_operation->cycle_compound_statement(), _owner_function_return_type, _source, compound_statement_borders.begin, compound_statement_borders.end);
    result.offset_after = compound_statement_borders.end + 1;

    return result;
}


Compiler::Operation_Parse_Result Compiler::M_parse_subexpression(Context& _context, const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    std::string first_word = M_parse_first_word(_source, _offset, _max_size);
    M_print_debug_error_message(first_word.size() > 0, _source, _offset, Unlimited_Size, "Trying to parse subexpression from empty string");
    unsigned int after_first_word_offset = M_skip_past_first_word(_source, _offset, _max_size);

    Operation_Parse_Result result;

    Expression_Type expression_type = M_get_expression_type(first_word);
    if(expression_type == Expression_Type::Unknown)
    {
        RValue_Getter* operation = M_construct_rvalue_getter(first_word);

        result.operation = operation;
        result.offset_after = after_first_word_offset;
    }
    else if(expression_type == Expression_Type::Variable_Name)
    {
        Expression_Goal expression_goal = M_member_access_or_function_call(_source, after_first_word_offset);

        if(expression_goal == Expression_Goal::Unknown)
        {
            Extract_Variable* operation = new Extract_Variable;
            operation->set_context(&_context);
            operation->set_variable_name(first_word);

            L_DEBUG_FUNC_NOARG([&]()
            {
                operation->set_debug_info(M_parse_line(_source, _offset), M_calculate_line_number(_source, _offset));
            });

            result.operation = operation;
            result.offset_after = after_first_word_offset;
        }
        else if(expression_goal == Expression_Goal::Member_Access)
        {
            result = M_parse_operation_with_variable(_context, first_word, _source, after_first_word_offset, Unlimited_Size);
        }
        else if(expression_goal == Expression_Goal::Function_Call)
        {
            result = M_parse_function_call(_context, first_word, _source, after_first_word_offset, Unlimited_Size);
        }
    }

    M_print_debug_error_message(result.operation, _source, _offset, Unlimited_Size, "Unexpected subexpression type");
    return result;
}

RValue_Getter* Compiler::M_construct_rvalue_getter(const std::string& _value_as_string) const
{
    std::string deduced_type = M_deduce_rvalue_type(_value_as_string);

    LDS::Vector<std::string> strings_vector_crutch;

    if(deduced_type == String_Type_Name)
        strings_vector_crutch.push(_value_as_string.substr(1, _value_as_string.size() - 2));
    else
        strings_vector_crutch.push(_value_as_string);

    LV::Type_Utility::Allocate_Result allocation_result = LV::Type_Manager::allocate(deduced_type, 1);
    LV::Type_Manager::parse(deduced_type, strings_vector_crutch, allocation_result.ptr);

    RValue_Getter* operation = new RValue_Getter;
    operation->variable().set_type(deduced_type);
    operation->variable().set_data(allocation_result.ptr, allocation_result.size);

    return operation;
}


std::string Compiler::M_parse_first_word(const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    unsigned int first_word_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset, _max_size);
    if(first_word_offset == Unlimited_Size)
        return {};

    if(_source[first_word_offset] == '\'')
    {
        unsigned int string_end = M_skip_until_symbol_met(_source, '\'', true, first_word_offset + 1, _max_size);
        M_print_debug_error_message(string_end < _max_size, _source, first_word_offset, 1, "Could not find end of the string");
        return _source.substr(first_word_offset, string_end - first_word_offset);
    }

    unsigned int after_first_word_offset = M_skip_until_symbol_met(_source, Maybe_Value_Symbols, false, first_word_offset, _max_size);
    L_ASSERT(after_first_word_offset >= first_word_offset);     //  this shouldn't happen probably

    if(after_first_word_offset == first_word_offset)
        return {};

    return _source.substr(first_word_offset, after_first_word_offset - first_word_offset);
}

unsigned int Compiler::M_skip_past_first_word(const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    unsigned int first_word_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset, _max_size);
    if(first_word_offset == Unlimited_Size)
        return _max_size;

    if(_source[first_word_offset] == '\'')
    {
        unsigned int string_end = M_skip_until_symbol_met(_source, '\'', true, first_word_offset + 1, _max_size);
        M_print_debug_error_message(string_end < _max_size, _source, first_word_offset, 1, "Could not find end of the string");
        return string_end;
    }

    unsigned int after_first_word_offset = M_skip_until_symbol_met(_source, Type_Name_Symbols, false, first_word_offset, _max_size);
    L_ASSERT(after_first_word_offset >= first_word_offset);

    if(after_first_word_offset == first_word_offset)
        return _max_size;

    return after_first_word_offset;
}

void Compiler::M_cull_empty_symbols(std::string& _from) const
{
    unsigned int begin = M_skip_until_symbol_met(_from, Empty_Symbols, false, 0);
    L_ASSERT(begin < _from.size());

    unsigned int end = M_skip_until_symbol_met_reverse(_from, Empty_Symbols, false, _from.size() - 1, begin);

    _from = _from.substr(begin, end - begin + 1);
}


unsigned int Compiler::M_skip_until_symbol_met(const std::string& _source, const Acceptable_Symbols& _acceptable_symbols, bool _symbols_expected, unsigned int _offset, unsigned int _max_size) const
{
    if(_max_size == Unlimited_Size)
        _max_size = _source.size();

    for(unsigned int i = _offset; i < _max_size; ++i)
    {
        char current_char = _source[i];

        if(_acceptable_symbols[current_char] == _symbols_expected)
            return i;
    }

    return _max_size;
}

unsigned int Compiler::M_skip_until_symbol_met_reverse(const std::string& _source, const Acceptable_Symbols& _acceptable_symbols, bool _symbols_expected, unsigned int _max, unsigned int _min) const
{
    L_ASSERT(_max >= _min);

    for(int i = _max; i > _min; --i)
    {
        char current_char = _source[i];

        if(_acceptable_symbols[current_char] == _symbols_expected)
            return i;
    }

    return _min;
}

unsigned int Compiler::M_skip_until_symbol_met(const std::string& _source, char _symbol, bool _symbols_expected, unsigned int _offset, unsigned int _max_size) const
{
    if(_max_size == Unlimited_Size)
        _max_size = _source.size();

    for(unsigned int i = _offset; i < _max_size; ++i)
    {
        char current_char = _source[i];

        if((current_char == _symbol) == _symbols_expected)
            return i;
    }

    return Unlimited_Size;
}

unsigned int Compiler::M_skip_until_closer(const std::string& _source, char _opener, char _closer, unsigned int _offset, unsigned int _max_size) const
{
    if(_max_size == Unlimited_Size)
        _max_size = _source.size();

    unsigned int openers_met = 0;
    for(unsigned int i = _offset; i < _max_size; ++i)
    {
        char current_char = _source[i];

        if(current_char == _closer)
        {
            if(openers_met == 0)
                return i;

            --openers_met;
            continue;
        }

        if(current_char == _opener)
        {
            ++openers_met;
            continue;
        }
    }

    return Unlimited_Size;
}

unsigned int Compiler::M_skip_past_semicolon(const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    unsigned int semicolon_index = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset, _max_size);
    M_print_debug_error_message(semicolon_index != Unlimited_Size, _source, _offset, 1, "Expected \";\"");
    M_print_debug_error_message(_source[semicolon_index] == ';', _source, _offset, 1, "Expected \";\"");

    return semicolon_index + 1;
}

Compiler::String_Borders Compiler::M_calculate_arguments_borders(const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    String_Borders result;

    result.begin = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset, _max_size);
    M_print_debug_error_message(_source[result.begin] == '(', _source, result.begin, 1, "Expected \"(\"");
    ++result.begin;
    result.end = M_skip_until_closer(_source, '(', ')', result.begin, _max_size);
    M_print_debug_error_message(result.end < _max_size, _source, _offset, Unlimited_Size, "Closing \")\" not found");

    return result;
}

Compiler::String_Borders Compiler::M_calculate_compound_statement_borders(const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    String_Borders result;

    result.begin = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset, _max_size);
    M_print_debug_error_message(_source[result.begin] == '{', _source, result.begin, 1, "Expected \"{\"");
    ++result.begin;
    result.end = M_skip_until_closer(_source, '{', '}', result.begin, _max_size);
    M_print_debug_error_message(result.end < _max_size, _source, _offset, Unlimited_Size, "Closing \"}\" not found");

    return result;
}


Compiler::Expression_Type Compiler::M_get_expression_type(const std::string& _expression) const
{
    if(_expression == If_Expression)
        return Expression_Type::If;

    if(_expression == While_Expression)
        return Expression_Type::While;

    if(_expression == Return_Expression)
        return Expression_Type::Return;

    if(_expression == Void_Type_Name)
        return Expression_Type::Type_Name;
    if(LV::Type_Manager::type_is_registered(_expression))
        return Expression_Type::Type_Name;

    if(M_can_be_variable_name(_expression))
        return Expression_Type::Variable_Name;

    return Expression_Type::Unknown;
}


Compiler::Expression_Goal Compiler::M_function_or_variable_declaration(const std::string& _source, unsigned int _offset_after_name) const
{
    unsigned int next_symbol_index = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset_after_name);

    if(next_symbol_index == Unlimited_Size)
        return Expression_Goal::Unknown;

    char next_symbol = _source[next_symbol_index];

    if(next_symbol == '(')
        return Expression_Goal::Function_Declaration;
    if(next_symbol == ';')
        return Expression_Goal::Variable_Declaration;

    return Expression_Goal::Unknown;
}

Compiler::Expression_Goal Compiler::M_member_access_or_function_call(const std::string& _source, unsigned int _offset_after_name) const
{
    unsigned int next_symbol_index = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset_after_name);

    if(next_symbol_index == Unlimited_Size)
        return Expression_Goal::Unknown;

    char next_symbol = _source[next_symbol_index];

    if(next_symbol == '(')
        return Expression_Goal::Function_Call;
    if(next_symbol == '.')
        return Expression_Goal::Member_Access;

    return Expression_Goal::Unknown;
}

bool Compiler::M_is_existing_variable(const Context& _context, const std::string& _name) const
{
    return _context.get_variable(_name) != nullptr;
}

bool Compiler::M_can_be_variable_name(const std::string& _name) const
{
    if(Reserved_Keywords.find(_name).is_ok())
        return false;

    for(char c = '0'; c <= '9'; ++c)
    {
        if(_name[0] == c)
            return false;       //  variable name can't start with a digit
    }

    for(unsigned int i = 0; i < _name.size(); ++i)
    {
        char current_char = _name[i];
        if(!Variable_Name_Symbols[current_char])
            return false;
    }
    return true;
}



void Compiler::compile(const std::string& _source) const
{
    L_ASSERT(m_script_target);

    M_parse_global_space(_source);
}
