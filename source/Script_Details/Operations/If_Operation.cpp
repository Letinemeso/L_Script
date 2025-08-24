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

    L_DEBUG_FUNC_NOARG([&]()
    {
        auto calculate_condition_index = [&]()
        {
            unsigned int result = 0;
            for(Case_List::Const_Iterator it = m_case_list.begin(); !it.end_reached(); ++it)
            {
                if(it->condition == _condition)
                    break;
                ++result;
            }
            return result;
        };

        if(condition_result && condition_result->type() == "bool")
        {
            bool* result_as_bool = (bool*)condition_result->data();
            if(result_as_bool)
                return;

            unsigned int condition_index = calculate_condition_index();
            L_LOG("LScript_Debug", "Error at line " + std::to_string(m_source_line_numbers[condition_index]) + ": ");
            L_LOG("LScript_Debug", "Condition expression return value not initialized");
            L_LOG("LScript_Debug", m_source_lines[condition_index]);
            L_LOG("LScript_Debug", std::string(m_source_lines[condition_index].size(), '^'));

            L_ASSERT(false);
        }

        unsigned int condition_index = calculate_condition_index();
        L_LOG("LScript_Debug", "Error at line " + std::to_string(m_source_line_numbers[condition_index]) + ": ");
        L_LOG("LScript_Debug", "Condition expression does not return bool");
        L_LOG("LScript_Debug", m_source_lines[condition_index]);
        L_LOG("LScript_Debug", std::string(m_source_lines[condition_index].size(), '^'));

        L_ASSERT(false);
    });

    bool* result_as_bool = (bool*)condition_result->data();
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
