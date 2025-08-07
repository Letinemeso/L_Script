#include <Script_Details/Operations/Extract_Variable.h>

using namespace LScript;


Variable* Extract_Variable::process()
{
    L_ASSERT(m_context);

    Variable* result = m_context->get_variable(m_variable_name);
    L_ASSERT(result);

    return result;
}
