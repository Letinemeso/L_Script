#include <Script_Details/Operations/If_Operation.h>

using namespace LScript;


If_Operation::If_Operation()
{

}

If_Operation::~If_Operation()
{
    delete m_condition;
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
    L_ASSERT(m_condition);

    set_stop_required(false);

    bool condition_succeded = M_check_condition(m_condition);

    Compound_Statement* compound_statement = nullptr;
    if(condition_succeded)
        compound_statement = &m_success_compound_statement;
    else
        compound_statement = &m_failure_compound_statement;

    Variable* result = compound_statement->process();

    set_stop_required(compound_statement->stop_required());

    return result;
}
