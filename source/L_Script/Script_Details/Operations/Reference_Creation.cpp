#include <L_Script/Script_Details/Operations/Reference_Creation.h>

#include <Type_Manager.h>

#include <L_Script/Script_Details/Variables/Variable_Reference.h>

using namespace LScript;


Variable* Reference_Creation::process()
{
    L_ASSERT(m_context);

    L_DEBUG_FUNC_NOARG([&]()
    {
        if(!m_context->get_local_variable(m_variable_name))
            return;

        L_LOG("LScript_Debug", "Error at line " + std::to_string(m_source_line_number) + ": ");
        L_LOG("LScript_Debug", "Variable \"" + m_variable_name + "\" already exists");
        L_LOG("LScript_Debug", m_source_line);
        L_LOG("LScript_Debug", std::string(m_source_line.size(), '^'));

        L_ASSERT(false);
    });

    Variable_Reference* variable = new Variable_Reference;
    variable->set_bindable(true);
    variable->set_type(m_variable_type);

    m_context->add_variable(m_variable_name, variable);

    return nullptr;
}
