#include <Script_Details/Compound_Statement.h>

using namespace LScript;


Compound_Statement::Compound_Statement()
{

}

Compound_Statement::Compound_Statement(Compound_Statement&& _from)
{
    m_context = (Context&&)_from.m_context;
    m_operations = (Operations_List&&)_from.m_operations;
}

void Compound_Statement::operator=(Compound_Statement&& _from)
{
    clear_operations();

    m_context = (Context&&)_from.m_context;
    m_operations = (Operations_List&&)_from.m_operations;
}


Compound_Statement::~Compound_Statement()
{
    clear_operations();
}



void Compound_Statement::add_operation(Operation* _operation)
{
    L_ASSERT(!m_operations.find(_operation).is_ok());
    m_operations.push_back(_operation);
}

void Compound_Statement::clear_operations()
{
    for(Operations_List::Iterator it = m_operations.begin(); !it.end_reached(); ++it)
        delete *it;
    m_operations.clear();
}



Variable* Compound_Statement::process()
{
    m_stop_required = false;

    Variable* result = nullptr;

    for(Operations_List::Iterator it = m_operations.begin(); !it.end_reached(); ++it)
    {
        Operation* operation = *it;
        result = operation->process();

        m_stop_required = operation->stop_required();
        if(m_stop_required)
            break;
    }

    return result;
}
