#pragma once

#include <Script_Details/Operations/Operation.h>
#include <Script_Details/Compound_Statement.h>


namespace LScript
{

    class If_Operation : public Operation
    {
    private:
        Operation* m_condition = nullptr;
        Compound_Statement m_success_compound_statement;
        Compound_Statement m_failure_compound_statement;

    public:
        If_Operation();
        ~If_Operation();

    public:
        inline void set_condition(Operation* _operation) { delete m_condition; m_condition = _operation; }

        inline Compound_Statement& success_compound_statement() { return m_success_compound_statement; }
        inline Compound_Statement& failure_compound_statement() { return m_failure_compound_statement; }

    private:
        bool M_check_condition(Operation* _condition) const;

    public:
        Variable* process() override;

    };

}
