#include <Compiler.h>

using namespace LScript;

namespace LScript
{
    constexpr const char* Void_Type_Name = "void";
    constexpr const char* If_Expression = "if";
    constexpr const char* For_Expression = "for";
    constexpr const char* While_Expression = "while";

    constexpr unsigned int Max_Symbols = 0xFF;

    Compiler::Acceptable_Symbols Empty_Symbols(Max_Symbols, false);
    Compiler::Acceptable_Symbols Variable_Name_Symbols(Max_Symbols, false);
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
}

Compiler::~Compiler()
{

}



void Compiler::M_parse_global_space(const std::string& _source) const
{
    Context& global_context = m_script_target->global_context();

    unsigned int offset = 0;
    while(offset < _source.size())
    {
        offset = M_parse_expression(global_context, _source, offset);
    }
}


unsigned int Compiler::M_parse_expression(Context& _context, const std::string& _source, unsigned int _offset, unsigned int _max_size) const
{
    if(_max_size == Unlimited_Size)
        _max_size = _source.size();

    unsigned int first_word_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, _offset, _max_size);
    if(first_word_offset == Unlimited_Size)
        return Unlimited_Size;

    unsigned int after_first_word_offset = M_skip_until_symbol_met(_source, Variable_Name_Symbols, false, first_word_offset, _max_size);
    L_ASSERT(after_first_word_offset > first_word_offset);

    unsigned int first_word_size = after_first_word_offset - first_word_offset;
    std::string first_word = _source.substr(first_word_offset, first_word_size);

    std::cout << "Parsed word: " << first_word << std::endl;

    Expression_Type expression_type = M_get_expression_type(first_word);

    if(expression_type == Expression_Type::Type_Name)
        std::cout << "It's a type name" << std::endl;
    else if(expression_type == Expression_Type::Unknown)
        std::cout << "Expression type unknown" << std::endl;

    unsigned int second_word_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, after_first_word_offset);
    unsigned int after_second_word_offset = M_skip_until_symbol_met(_source, Variable_Name_Symbols, false, second_word_offset);
    L_ASSERT(after_second_word_offset > second_word_offset);

    unsigned int next_word_size = after_second_word_offset - second_word_offset;
    std::string second_word = _source.substr(second_word_offset, next_word_size);

    std::cout << "Next parsed word: " << second_word << std::endl;

    Expression_Goal expression_goal = M_function_or_variable_declaration(_source, after_second_word_offset);
    L_ASSERT(expression_goal != Expression_Goal::Unknown);

    if(expression_goal == Expression_Goal::Function_Declaration)
    {
        std::cout << "It's a function declaration" << std::endl;
        return M_parse_function_declaration(_source, first_word, second_word, after_second_word_offset);
    }
    else if(expression_goal == Expression_Goal::Variable_Declaration)
    {
        std::cout << "It's a variable declaration" << std::endl;
        return M_parse_variable_declaration(_context, _source, first_word, second_word, after_second_word_offset);
    }

    return Unlimited_Size;
}

unsigned int Compiler::M_parse_variable_declaration(Context& _context, const std::string& _source, const std::string& _type, const std::string& _name, unsigned int _offset_after_name) const
{
    L_ASSERT(_type != Void_Type_Name);  //  variable type cannot be void

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

    unsigned int body_offset = M_skip_until_symbol_met(_source, Empty_Symbols, false, after_args_offset + 1);
    L_ASSERT(body_offset != Unlimited_Size);

    unsigned int after_body_offset = M_skip_until_closer(_source, '{', '}', body_offset + 1);
    L_ASSERT(after_body_offset != Unlimited_Size);

    return after_body_offset + 1;
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



void Compiler::compile(const std::string& _source) const
{
    L_ASSERT(m_script_target);

    M_parse_global_space(_source);
}
