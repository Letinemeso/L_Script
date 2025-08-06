#pragma once

#include <Script_Details/Variable.h>
#include <Script_Details/Context.h>


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

    private:
        Context m_context;

        LDS::Vector<Argument_Data> m_expected_arguments_data;

    public:
        Function();
        ~Function();

    public:
        inline void set_expected_arguments_data(const LDS::Vector<Argument_Data>& _args) { m_expected_arguments_data = _args; }

        inline Context& context() { return m_context; }
        inline const Context& context() const { return m_context; }

    public:
        void call(const LDS::Vector<Variable*>& _args);

    };

}
