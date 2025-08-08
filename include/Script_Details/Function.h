#pragma once

#include <Data_Structures/Vector.h>

#include <Script_Details/Variables/Variable.h>
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
            bool reference = false;
        };

        using Arguments = LDS::Vector<Variable*>;
        using Arguments_Data = LDS::Vector<Argument_Data>;

    private:
        std::string m_return_type = "void";
        Arguments_Data m_expected_arguments_data;

        Compound_Statement m_compound_statement;        

    public:
        Function();
        ~Function();

    public:
        inline void set_return_type(const std::string& _value) { m_return_type = _value; }
        inline void set_expected_arguments_data(const Arguments_Data& _args) { m_expected_arguments_data = _args; }
        inline void set_expected_arguments_data(Arguments_Data&& _args) { m_expected_arguments_data = (Arguments_Data&&)_args; }

        inline Compound_Statement& compound_statement() { return m_compound_statement; }
        inline const Compound_Statement& compound_statement() const { return m_compound_statement; }

        inline const Arguments_Data& expected_arguments_data() const { return m_expected_arguments_data; }

    public:
        [[nodiscard]] Variable* call(const Arguments& _args);

    };

}
