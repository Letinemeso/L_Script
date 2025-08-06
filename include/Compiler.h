#pragma once

#include <Script.h>


namespace LScript
{

    class Compiler
    {
    private:
        static constexpr unsigned int Unlimited_Size = 0xFFFFFFFF;

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
        unsigned int M_skip_until_symbol_met(const std::string& _source, const Acceptable_Symbols& _acceptable_symbols, bool _symbols_expected = true, unsigned int _offset = 0, unsigned int _max_size = Unlimited_Size) const;

        std::string M_parse_word(const std::string& _source, unsigned int _offset, unsigned int _max_length) const;

    public:
        void compile(const std::string& _source) const;

    };

}
