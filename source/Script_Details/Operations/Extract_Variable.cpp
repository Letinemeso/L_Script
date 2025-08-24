#include <Script_Details/Operations/Extract_Variable.h>

using namespace LScript;


Variable* Extract_Variable::process()
{
    L_ASSERT(m_context);

    Variable* result = m_context->get_variable(m_variable_name);

    L_DEBUG_FUNC_NOARG([&]()
    {
        if(result)
            return;

        L_LOG("LScript_Debug", "Error at line " + std::to_string(m_source_line_number) + ": ");
        L_LOG("LScript_Debug", "Unknown variable name: \"" + m_variable_name + "\"");
        L_LOG("LScript_Debug", m_source_line);
        L_LOG("LScript_Debug", std::string(m_source_line.size(), '^'));
        L_ASSERT(result);
    });

    return result;
}
