#pragma once

#include <Script_Details/Operations/Operation.h>
#include <Script_Details/Compound_Statement.h>


namespace LScript
{

    class While_Operation : public Operation
    {
    private:
        Operation* m_condition_operation = nullptr;

        Compound_Statement m_cycle_compound_statement;

    public:
        While_Operation();
        ~While_Operation();

    public:
        inline void set_condition_operation(Operation* _condition) { delete m_condition_operation; m_condition_operation = _condition; }

        inline Compound_Statement& cycle_compound_statement() { return m_cycle_compound_statement; }

    private:
        bool M_condition_met();

    public:
        Variable* process() override;

    };

}
