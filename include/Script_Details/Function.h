#pragma once

#include <Data_Structures/Vector.h>

#include <Script_Details/Variable.h>
#include <Script_Details/Context.h>
#include <Script_Details/Compound_Statement.h>
#include <Script_Details/Operations/Operation.h>


namespace LScript
{

    class Function
    {
    public:
        struct Argument_Data
        {
            std::string expected_type;
            std::string name;
        };

        using Arguments = LDS::Vector<Variable*>;

    private:
        std::string m_return_type;
        LDS::Vector<Argument_Data> m_expected_arguments_data;

        Compound_Statement m_compound_statement;        

    public:
        Function();
        ~Function();

    public:
        inline void set_return_type(const std::string& _value) { m_return_type = _value; }
        inline void set_expected_arguments_data(const LDS::Vector<Argument_Data>& _args) { m_expected_arguments_data = _args; }

        inline Compound_Statement& compound_statement() { return m_compound_statement; }
        inline const Compound_Statement& compound_statement() const { return m_compound_statement; }

    public:
        [[nodiscard]] Variable* call(const Arguments& _args);

    };

}
