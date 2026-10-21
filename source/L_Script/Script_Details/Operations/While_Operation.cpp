#include <L_Script/Script_Details/Operations/While_Operation.h>

using namespace LScript;


While_Operation::While_Operation()
{

}

While_Operation::~While_Operation()
{
    delete m_condition_operation;
}



bool While_Operation::M_condition_met(Context* _context)
{
    L_ASSERT(m_condition_operation);

    Variable* result_variable = m_condition_operation->process(_context);

    L_DEBUG_FUNC_NOARG([&]()
    {
        if(result_variable && result_variable->type() == "bool")
        {
            bool* result_as_bool = (bool*)result_variable->data();
            if(result_as_bool)
                return;

            L_LOG("LScript_Debug", "Error at line " + std::to_string(m_source_line_number) + ": ");
            L_LOG("LScript_Debug", "Condition expression return value not initialized");
            L_LOG("LScript_Debug", m_source_line);
            L_LOG("LScript_Debug", std::string(m_source_line.size(), '^'));

            L_ASSERT(false);
        }

        L_LOG("LScript_Debug", "Error at line " + std::to_string(m_source_line_number) + ": ");
        L_LOG("LScript_Debug", "Condition expression does not return bool");
        L_LOG("LScript_Debug", m_source_line);
        L_LOG("LScript_Debug", std::string(m_source_line.size(), '^'));

        L_ASSERT(false);
    });

    bool result_as_bool = *(bool*)result_variable->data();

    if(result_variable->is_temporary())
        delete result_variable;

    return result_as_bool;
}



Variable* While_Operation::process(Context* _context)
{
    set_stop_required(false);

    Variable* result = nullptr;

    while(M_condition_met(_context))
    {
        Context local_context(_context);
        result = m_cycle_compound_statement.process(&local_context);
        if(!m_cycle_compound_statement.stop_required())
            continue;

        set_stop_required(true);
        break;
    }

    return result;
}
