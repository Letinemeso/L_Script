#include <Script_Details/Operations/Variable_Creation.h>

using namespace LScript;


Variable* Variable_Creation::process()
{
    L_ASSERT(m_context);

    Variable* variable = new Variable;
    variable->set_type(m_variable_type);

    m_context->add_variable(m_variable_name, variable);

    return nullptr;
}
