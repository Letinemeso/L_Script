#include <Script_Details/Operations/If_Operation.h>

using namespace LScript;


If_Operation::If_Operation()
{

}

If_Operation::~If_Operation()
{
    delete m_failure_compound_statement;
}



void If_Operation::add_case(Operation* _condition, Compound_Statement* _compound_statement)
{
    L_ASSERT(_condition);
    m_case_list.push_back({ _condition, _compound_statement });
}

void If_Operation::add_fail_case(Compound_Statement* _compound_statement)
{
    m_failure_compound_statement = _compound_statement;
}



bool If_Operation::M_check_condition(Operation* _condition) const
{
    Variable* condition_result = _condition->process();
    L_ASSERT(condition_result->type() == "bool");

    bool* result_as_bool = (bool*)condition_result->data();
    L_ASSERT(result_as_bool);
    return *result_as_bool;
}



Variable* If_Operation::process()
{
    L_ASSERT(m_case_list.size() > 0);

    set_stop_required(false);

    Compound_Statement* compound_statement = nullptr;
    for(Case_List::Iterator it = m_case_list.begin(); !it.end_reached(); ++it)
    {
        Case& current_case = *it;
        if(!M_check_condition(current_case.condition))
            continue;

        compound_statement = current_case.compound_statement;
        break;
    }

    if(!compound_statement)
        compound_statement = m_failure_compound_statement;

    if(!compound_statement)
        return nullptr;

    Variable* result = compound_statement->process();

    set_stop_required(compound_statement->stop_required());

    return result;
}
