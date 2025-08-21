#include <Compiler.h>

#include <Data_Structures/AVL_Tree.h>

#include <Integrated_Functions.h>
#include <Script_Details/Operations/Operation.h>
#include <Script_Details/Operations/Variable_Creation.h>
#include <Script_Details/Operations/Extract_Variable.h>
#include <Script_Details/Operations/Call_Member_Function.h>
#include <Script_Details/Operations/Call_Global_Function.h>
#include <Script_Details/Operations/RValue_Getter.h>

using namespace LScript;

namespace LScript
{
    constexpr const char* Void_Type_Name = "void";
    constexpr const char* If_Expression = "if";
    constexpr const char* For_Expression = "for";
    constexpr const char* While_Expression = "while";
    constexpr const char* Return_Expression = "return";

    constexpr unsigned int Max_Symbols = 0xFF;

    Compiler::Acceptable_Symbols Empty_Symbols(Max_Symbols, false);
    Compiler::Acceptable_Symbols Variable_Name_Symbols(Max_Symbols, false);
    Compiler::Acceptable_Symbols Type_Name_Symbols(Max_Symbols, false);

    using Reserved_Keywords_Tree = LDS::AVL_Tree<std::string>;
    Reserved_Keywords_Tree Reserved_Keywords;
}



Compiler::Compiler()
{
    static bool acceptable_symbols_initialized = false;
    if(acceptable_symbols_initialized)
        return;

    acceptable_symbols_initialized = true;

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

    Reserved_Keywords.insert(Void_Type_Name);
    Reserved_Keywords.insert(If_Expression);
    Reserved_Keywords.insert(For_Expression);
    Reserved_Keywords.insert(While_Expression);
    Reserved_Keywords.insert(Return_Expression);
    Reserved_Keywords.insert("true");
    Reserved_Keywords.insert("false");
}

Compiler::~Compiler()
{

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
    L_ASSERT(expression_type == Expression_Type::Type_Name);

    unsigned int second_word_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, after_first_word_offset);
    unsigned int after_second_word_offset = M_skip_until_symbol_met(_source, Variable_Name_Symbols, false, second_word_offset);
    L_ASSERT(after_second_word_offset > second_word_offset);

    unsigned int next_word_size = after_second_word_offset - second_word_offset;
    std::string second_word = _source.substr(second_word_offset, next_word_size);

    Expression_Goal expression_goal = M_function_or_variable_declaration(_source, after_second_word_offset);
    L_ASSERT(expression_goal == Expression_Goal::Function_Declaration || expression_goal == Expression_Goal::Variable_Declaration);

    if(expression_goal == Expression_Goal::Function_Declaration)
        return M_parse_function_declaration(_source, first_word, second_word, after_second_word_offset);
    else if(expression_goal == Expression_Goal::Variable_Declaration)
        return M_parse_global_variable_declaration(m_script_target->global_context(), _source, first_word, second_word, after_second_word_offset);

    return Unlimited_Size;
}

unsigned int Compiler::M_parse_global_variable_declaration(Context& _context, const std::string& _source, const std::string& _type, const std::string& _name, unsigned int _offset_after_name) const
{
    L_ASSERT(_type != Void_Type_Name);  //  variable type cannot be void
    L_ASSERT(_context.get_local_variable(_name) == nullptr);

    Variable_Container* variable = new Variable_Container;
    variable->set_type(_type);

    _context.add_variable(_name, variable);

    unsigned int semicolon_index = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset_after_name);
    L_ASSERT(semicolon_index != Unlimited_Size);
    L_ASSERT(_source[semicolon_index] == ';');

    return semicolon_index + 1;
}

unsigned int Compiler::M_parse_function_declaration(const std::string& _source, const std::string& _type, const std::string& _name, unsigned int _offset_after_name) const
{
    unsigned int args_offset = M_skip_until_symbol_met(_source, '(', true, _offset_after_name);
    L_ASSERT(args_offset != Unlimited_Size);

    unsigned int after_args_offset = M_skip_until_closer(_source, '(', ')', args_offset + 1);
    L_ASSERT(after_args_offset != Unlimited_Size);

    LDS::Vector<Function::Argument_Data> arguments_data = M_parse_function_arguments_data(_source, args_offset + 1, after_args_offset);

    unsigned int body_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, after_args_offset + 1);
    L_ASSERT(body_offset != Unlimited_Size);
    L_ASSERT(_source[body_offset] == '{');

    unsigned int after_body_offset = M_skip_until_closer(_source, '{', '}', body_offset + 1);
    L_ASSERT(after_body_offset != Unlimited_Size);

    L_ASSERT(m_script_target->get_function(_name) == nullptr);

    Function* function = new Function;
    function->set_return_type(_type);
    function->set_expected_arguments_data(arguments_data);
    function->compound_statement().context().set_parent_context(&m_script_target->global_context());

    M_parse_function_body(*function, _source, body_offset + 1, after_body_offset);

    m_script_target->register_function(_name, function);

    return after_body_offset + 1;
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
        L_ASSERT(after_type_offset > type_offset);

        unsigned int name_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, after_type_offset, _end);
        unsigned int after_name_offset = M_skip_until_symbol_met(_source, Variable_Name_Symbols, false, name_offset, _end);
        if(after_name_offset == Unlimited_Size)
            after_name_offset = _end;
        L_ASSERT(after_name_offset > name_offset);

        std::string type = _source.substr(type_offset, after_type_offset - type_offset);
        std::string name = _source.substr(name_offset, after_name_offset - name_offset);
        bool reference = type[type.size() - 1] == '&';
        if(reference)
            type.pop_back();

        L_ASSERT(LV::Type_Manager::type_is_registered(type));

        result.push({type, name, reference});

        offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, after_name_offset, _end);
        if(offset >= _end)
            break;

        L_ASSERT(_source[offset] == ',');
        ++offset;
    }

    return result;
}

void Compiler::M_parse_function_body(Function& _function, const std::string& _source, unsigned int _begin, unsigned int _end) const
{
    unsigned int offset = _begin;
    while(offset < _end)
    {
        offset = M_parse_dynamic_expression(_function, _source, offset, _end);
    }
}

unsigned int Compiler::M_parse_dynamic_expression(Function& _function, const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    if(_max_size == Unlimited_Size)
        _max_size = _source.size();

    std::string first_word = M_parse_first_word(_source, _offset, _max_size);
    if(first_word.size() == 0)
        return _max_size;
    unsigned int after_first_word_offset = M_skip_past_first_word(_source, _offset, _max_size);

    Expression_Type expression_type = M_get_expression_type(first_word);
    L_ASSERT(expression_type != Expression_Type::Unknown);

    if(expression_type == Expression_Type::If)
    {
        // return M_parse_if();
    }
    if(expression_type == Expression_Type::For)
    {
        // return M_parse_for();
    }
    if(expression_type == Expression_Type::While)
    {
        // return M_parse_while();
    }
    if(expression_type == Expression_Type::Return)
    {
        return M_parse_return(_function.compound_statement(), _function.return_type(), _source, after_first_word_offset, _max_size);
    }
    if(expression_type == Expression_Type::Type_Name)
    {
        return M_parse_dynamic_declaration(_function.compound_statement(), first_word, _source, after_first_word_offset, _max_size);
    }
    if(expression_type == Expression_Type::Variable_Name)
    {
        Expression_Goal expression_goal = M_member_access_or_function_call(_source, after_first_word_offset);

        Operation_Parse_Result parse_result;
        if(expression_goal == Expression_Goal::Member_Access)
            parse_result = M_parse_operation_with_variable(_function.compound_statement().context(), first_word, _source, after_first_word_offset, _max_size);
        else if(expression_goal == Expression_Goal::Function_Call)
            parse_result = M_parse_function_call(_function.compound_statement().context(), first_word, _source, after_first_word_offset, _max_size);

        L_ASSERT(parse_result.operation);

        _function.compound_statement().add_operation(parse_result.operation);

        return M_skip_past_semicolon(_source, parse_result.offset_after, _max_size);;
    }

    return Unlimited_Size;
}

unsigned int Compiler::M_parse_dynamic_declaration(Compound_Statement& _compound_statement, const std::string& _type, const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    unsigned int name_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset);
    unsigned int after_name_offset = M_skip_until_symbol_met(_source, Variable_Name_Symbols, false, name_offset);
    L_ASSERT(after_name_offset > name_offset);

    unsigned int next_word_size = after_name_offset - name_offset;
    std::string name = _source.substr(name_offset, next_word_size);

    L_ASSERT(M_function_or_variable_declaration(_source, after_name_offset) == Expression_Goal::Variable_Declaration);

    Variable_Creation* operation = new Variable_Creation;
    operation->set_context(&_compound_statement.context());
    operation->set_variable_type(_type);
    operation->set_variable_name(name);

    _compound_statement.add_operation(operation);

    return M_skip_past_semicolon(_source, after_name_offset, _max_size);
}

Compiler::Operation_Parse_Result Compiler::M_parse_operation_with_variable(Context& _context, const std::string& _name, const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    L_ASSERT(_source[_offset] == '.');

    unsigned int operation_offset = _offset + 1;
    unsigned int after_operation_offset = M_skip_until_symbol_met(_source, Variable_Name_Symbols, false, operation_offset, _max_size);
    L_ASSERT(after_operation_offset > operation_offset);
    std::string operation_name = _source.substr(operation_offset, after_operation_offset - operation_offset);

    unsigned int arguments_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, after_operation_offset, _max_size);
    L_ASSERT(_source[arguments_offset] == '(');
    ++arguments_offset;
    unsigned int after_arguments_offset = M_skip_until_closer(_source, '(', ')', arguments_offset, _max_size);
    L_ASSERT(after_arguments_offset < Unlimited_Size);

    LDS::Vector<Operation*> argument_getters = M_construct_argument_getter_operations(_context, _source, arguments_offset, after_arguments_offset);

    Extract_Variable* extract_variable_operation = new Extract_Variable;
    extract_variable_operation->set_context(&_context);
    extract_variable_operation->set_variable_name(_name);

    Call_Member_Function* call_member_function_operation = new Call_Member_Function;
    call_member_function_operation->set_context(&_context);
    call_member_function_operation->set_function_name(operation_name);
    call_member_function_operation->set_owner_object_getter_operation(extract_variable_operation);
    call_member_function_operation->set_arguments_getter_operations((LDS::Vector<Operation*>&&)argument_getters);

    Operation_Parse_Result result;
    result.operation = call_member_function_operation;
    result.offset_after = after_arguments_offset + 1;
    return result;
}

Compiler::Operation_Parse_Result Compiler::M_parse_function_call(Context& _context, const std::string& _name, const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    unsigned int arguments_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset, _max_size);
    L_ASSERT(_source[arguments_offset] == '(');
    ++arguments_offset;
    unsigned int after_arguments_offset = M_skip_until_closer(_source, '(', ')', arguments_offset, _max_size);
    L_ASSERT(after_arguments_offset < Unlimited_Size);

    LDS::Vector<Operation*> argument_getters = M_construct_argument_getter_operations(_context, _source, arguments_offset, after_arguments_offset);

    Call_Global_Function* operation = new Call_Global_Function;
    operation->set_function_name(_name);
    operation->set_script(m_script_target);
    operation->set_arguments_getter_operations((LDS::Vector<Operation*>&&)argument_getters);

    Operation_Parse_Result result;
    result.operation = operation;
    result.offset_after = after_arguments_offset + 1;
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
            std::string deduced_type = M_deduce_rvalue_type(argument);
            if(deduced_type == "std::string")
                argument = argument.substr(1, argument.size() - 2);

            RValue_Getter* operation = new RValue_Getter;
            M_init_variable_container(deduced_type, argument, operation->variable());
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
            L_ASSERT(false);    //  remove later!
        }
    }

    return operations;
}

LDS::Vector<std::string> Compiler::M_parse_passed_arguments(const std::string& _source, unsigned int _begin, unsigned int _end) const
{
    unsigned int offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, _begin, _end);
    if(offset >= _end)
        return {};

    L_ASSERT(_source[offset] != ',');

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
            L_ASSERT(openers_met > 0);
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
            L_ASSERT(openers_met > 0);
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
    L_ASSERT(_rvalue.size() > 0);

    if(_rvalue.size() >= 2)
    {
        if(_rvalue[0] == '\'' && _rvalue[_rvalue.size() - 1] == '\'')
            return "std::string";
    }

    if(LV::Type_Manager::validate("int", _rvalue))
        return "int";

    if(LV::Type_Manager::validate("float", _rvalue))
        return "float";

    if(LV::Type_Manager::validate("bool", _rvalue))
        return "bool";

    L_ASSERT(false && "could not deduce type");
    return "";
}

unsigned int Compiler::M_parse_return(Compound_Statement& _compound_statement, const std::string& _expected_return_type, const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    unsigned int offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset, _max_size);
    L_ASSERT(offset < _max_size);
    if(_source[offset] == ';' && _expected_return_type == Void_Type_Name)
        return _max_size;

    unsigned int semicolon_index = M_skip_until_symbol_met(_source, ';', true, offset, _max_size);
    L_ASSERT(offset < _max_size);

    unsigned int expression_size = semicolon_index - offset;
    std::string expression = _source.substr(offset, expression_size);
    M_cull_empty_symbols(expression);

    std::string first_word = M_parse_first_word(_source, _offset, _max_size);
    if(first_word.size() == 0)
        return _max_size;
    unsigned int after_first_word_offset = M_skip_past_first_word(_source, _offset, _max_size);

    Expression_Type expression_type = M_get_expression_type(first_word);
    L_ASSERT(expression_type != Expression_Type::Type_Name);

    if(expression_type == Expression_Type::Unknown)
    {
        std::string deduced_type = M_deduce_rvalue_type(first_word);

        L_ASSERT(deduced_type == _expected_return_type);

        if(deduced_type == "std::string")
            first_word = first_word.substr(1, first_word.size() - 2);

        RValue_Getter* operation = new RValue_Getter;
        M_init_variable_container(deduced_type, first_word, operation->variable());
        operation->set_stop_required(true);
        _compound_statement.add_operation(operation);
    }
    else if(expression_type == Expression_Type::Variable_Name)
    {
        Extract_Variable* operation = new Extract_Variable;
        operation->set_context(&_compound_statement.context());
        operation->set_variable_name(first_word);
        operation->set_stop_required(true);
        _compound_statement.add_operation(operation);
    }

    return M_skip_past_semicolon(_source, after_first_word_offset, _max_size);
}


void Compiler::M_init_variable_container(const std::string& _type, const std::string& _value_as_string, Variable_Container& _variable) const
{
    LV::Type_Utility::Allocate_Result allocation_result = LV::Type_Manager::allocate(_type, 1);

    LDS::Vector<std::string> strings_vector_crutch;
    strings_vector_crutch.push(_value_as_string);

    LV::Type_Manager::parse(_type, strings_vector_crutch, allocation_result.ptr);

    _variable.set_type(_type);
    _variable.set_data(allocation_result.ptr, allocation_result.size);
}


std::string Compiler::M_parse_first_word(const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    unsigned int first_word_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset, _max_size);
    if(first_word_offset == Unlimited_Size)
        return {};

    if(_source[first_word_offset] == '\'')
    {
        unsigned int string_end = M_skip_until_symbol_met(_source, '\'', true, first_word_offset + 1, _max_size);
        L_ASSERT(string_end > first_word_offset);
        L_ASSERT(string_end < _max_size);
        return _source.substr(first_word_offset, string_end - first_word_offset);
    }

    unsigned int after_first_word_offset = M_skip_until_symbol_met(_source, Type_Name_Symbols, false, first_word_offset, _max_size);
    L_ASSERT(after_first_word_offset > first_word_offset);

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
        L_ASSERT(string_end > first_word_offset);
        L_ASSERT(string_end < _max_size);
        return string_end;
    }

    unsigned int after_first_word_offset = M_skip_until_symbol_met(_source, Type_Name_Symbols, false, first_word_offset, _max_size);
    L_ASSERT(after_first_word_offset > first_word_offset);

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

    return Unlimited_Size;
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
    L_ASSERT(semicolon_index != Unlimited_Size);
    L_ASSERT(_source[semicolon_index] == ';');

    return semicolon_index + 1;
}


Compiler::Expression_Type Compiler::M_get_expression_type(const std::string& _expression) const
{
    if(_expression == If_Expression)
        return Expression_Type::If;

    if(_expression == For_Expression)
        return Expression_Type::For;

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
