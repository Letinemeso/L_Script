#include <Compiler.h>

#include <Script_Details/Operations/Operation.h>
#include <Script_Details/Operations/Variable_Creation.h>
#include <Script_Details/Operations/Extract_Variable.h>
#include <Script_Details/Operations/Call_Member_Function.h>

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

    unsigned int first_word_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset, _max_size);
    if(first_word_offset == Unlimited_Size)
        return Unlimited_Size;

    unsigned int after_first_word_offset = M_skip_until_symbol_met(_source, Type_Name_Symbols, false, first_word_offset, _max_size);
    L_ASSERT(after_first_word_offset > first_word_offset);

    unsigned int first_word_size = after_first_word_offset - first_word_offset;
    std::string first_word = _source.substr(first_word_offset, first_word_size);

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

    Variable* variable = new Variable;
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
        unsigned int after_type_offset = M_skip_until_symbol_met(_source, Variable_Name_Symbols, false, type_offset, _end);
        L_ASSERT(after_type_offset > type_offset);

        unsigned int name_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, after_type_offset, _end);
        unsigned int after_name_offset = M_skip_until_symbol_met(_source, Variable_Name_Symbols, false, name_offset, _end);
        if(after_name_offset == Unlimited_Size)
            after_name_offset = _end;
        L_ASSERT(after_name_offset > name_offset);

        std::string type = _source.substr(type_offset, after_type_offset - type_offset);
        std::string name = _source.substr(name_offset, after_name_offset - name_offset);

        L_ASSERT(LV::Type_Manager::type_is_registered(type));

        result.push({type, name});

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
        offset = M_parse_dynamic_expression(_function.compound_statement(), _source, offset, _end);
    }
}

unsigned int Compiler::M_parse_dynamic_expression(Compound_Statement& _compound_statement, const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    if(_max_size == Unlimited_Size)
        _max_size = _source.size();

    unsigned int first_word_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset, _max_size);
    if(first_word_offset == Unlimited_Size)
        return Unlimited_Size;

    unsigned int after_first_word_offset = M_skip_until_symbol_met(_source, Type_Name_Symbols, false, first_word_offset, _max_size);
    L_ASSERT(after_first_word_offset > first_word_offset);

    unsigned int first_word_size = after_first_word_offset - first_word_offset;
    std::string first_word = _source.substr(first_word_offset, first_word_size);

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
    if(expression_type == Expression_Type::Type_Name)
    {
        return M_parse_dynamic_declaration(_compound_statement, first_word, _source, after_first_word_offset, _max_size);
    }
    if(expression_type == Expression_Type::Existing_Variable)
    {
        return M_parse_operation_with_variable(_compound_statement, first_word, _source, after_first_word_offset, _max_size);
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

unsigned int Compiler::M_parse_operation_with_variable(Compound_Statement& _compound_statement, const std::string& _name, const std::string& _source, unsigned int _offset, unsigned int _max_size) const
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
    L_ASSERT(after_operation_offset < Unlimited_Size);

    // std::cout << "Called " << _name << '.' << operation_name << std::endl;

    Extract_Variable* extract_variable_operation = new Extract_Variable;
    extract_variable_operation->set_context(&_compound_statement.context());
    extract_variable_operation->set_variable_name(_name);

    Call_Member_Function* call_member_function_operation = new Call_Member_Function;
    call_member_function_operation->set_context(&_compound_statement.context());
    call_member_function_operation->set_function_name(operation_name);

    call_member_function_operation->set_owner_object_getter_operation(extract_variable_operation);

    _compound_statement.add_operation(call_member_function_operation);

    return M_skip_past_semicolon(_source, after_arguments_offset + 1, _max_size);
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

    if(_expression == Void_Type_Name)
        return Expression_Type::Type_Name;
    if(LV::Type_Manager::type_is_registered(_expression))
        return Expression_Type::Type_Name;

    if(m_script_target->get_function(_expression))
        return Expression_Type::Existing_Function;

    if(M_can_be_variable_name(_expression))
        return Expression_Type::Existing_Variable;

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

bool Compiler::M_is_existing_variable(const Context& _context, const std::string& _name) const
{
    return _context.get_variable(_name) != nullptr;
}

bool Compiler::M_can_be_variable_name(const std::string& _name) const
{
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
