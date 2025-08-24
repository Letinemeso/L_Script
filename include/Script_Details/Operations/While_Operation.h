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

    private:
        std::string m_source_line;
        unsigned int m_source_line_number = 0;

    public:
        While_Operation();
        ~While_Operation();

    public:
        inline void set_condition_operation(Operation* _condition) { delete m_condition_operation; m_condition_operation = _condition; }

        inline Compound_Statement& cycle_compound_statement() { return m_cycle_compound_statement; }

        inline void set_debug_info(const std::string& _line, unsigned int _line_number) { m_source_line = _line; m_source_line_number = _line_number; }

    private:
        bool M_condition_met();

    public:
        Variable* process() override;

    };

}
