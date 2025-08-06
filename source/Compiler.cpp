#include <Compiler.h>

using namespace LScript;

namespace LScript
{
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



void Compiler::compile(const std::string& _source) const
{
    // Context& global_context = script->global_context();

    std::cout << M_skip_until_symbol_met(_source, Empty_Symbols, false) << std::endl;
}
