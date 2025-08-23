#include <Script_Details/Operations/If_Operation.h>

using namespace LScript;


If_Operation::If_Operation()
{

}

If_Operation::~If_Operation()
{
    delete m_condition;
}



bool If_Operation::M_chech_condition(Operation* _condition) const
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

    if(!M_chech_condition(m_condition))
        return nullptr;

    Variable* result = m_success_compound_statement.process();

    set_stop_required(m_success_compound_statement.stop_required());

    return result;
}
