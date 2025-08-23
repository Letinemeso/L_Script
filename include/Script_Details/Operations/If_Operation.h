#pragma once

#include <Script_Details/Operations/Operation.h>
#include <Script_Details/Compound_Statement.h>


namespace LScript
{

    class If_Operation : public Operation
    {
    private:
        struct Case
        {
            Operation* condition = nullptr;
            Compound_Statement* compound_statement = nullptr;

            Case(Operation* _condition, Compound_Statement* _compound_statement)
            {
                condition = _condition;
                compound_statement = _compound_statement;
            }

            Case(Case& _other) = delete;

            Case(Case&& _from)
            {
                condition = _from.condition;
                _from.condition = nullptr;
                compound_statement = _from.compound_statement;
                _from.compound_statement = nullptr;
            }

            ~Case()
            {
                delete condition;
                delete compound_statement;
            }
        };
        using Case_List = LDS::List<Case>;

        Case_List m_case_list;
        Compound_Statement* m_failure_compound_statement = nullptr;

    public:
        If_Operation();
        ~If_Operation();

    public:
        void add_case(Operation* _condition, Compound_Statement* _compound_statement);
        void add_fail_case(Compound_Statement* _compound_statement);

    private:
        bool M_check_condition(Operation* _condition) const;

    public:
        Variable* process() override;

    };

}
