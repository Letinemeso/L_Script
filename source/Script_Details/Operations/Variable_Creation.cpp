#include <Script_Details/Operations/Variable_Creation.h>

#include <Script_Details/Variables/Variable_Container.h>

using namespace LScript;


Variable* Variable_Creation::process()
{
    L_ASSERT(m_context);

    Variable_Container* variable = new Variable_Container;
    variable->set_type(m_variable_type);

    m_context->add_variable(m_variable_name, variable);

    return nullptr;
}
