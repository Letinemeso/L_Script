#include <Script_Details/Operations/While_Operation.h>

using namespace LScript;


While_Operation::While_Operation()
{

}

While_Operation::~While_Operation()
{
    delete m_condition_operation;
}



bool While_Operation::M_condition_met()
{
    L_ASSERT(m_condition_operation);

    Variable* result_variable = m_condition_operation->process();
    L_ASSERT(result_variable);
    L_ASSERT(result_variable->data());

    bool* result_as_bool = (bool*)result_variable->data();
    return *result_as_bool;
}



Variable* While_Operation::process()
{
    set_stop_required(false);

    Variable* result = nullptr;

    while(M_condition_met())
    {
        result = m_cycle_compound_statement.process();
        if(!m_cycle_compound_statement.stop_required())
            continue;

        set_stop_required(true);
        break;
    }

    return result;
}
