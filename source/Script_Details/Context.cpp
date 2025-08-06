#include <Script_Details/Context.h>

using namespace LScript;


Context::Context()
{

}

Context::~Context()
{
    clear();
}



void Context::add_variable(const std::string& _name, Variable* _variable)
{
    L_ASSERT(!m_variables.find(_name).is_ok());

    m_variables.insert(_name, _variable);
}

Variable* Context::get_variable(const std::string& _name) const
{
    Variables_Map::Const_Iterator maybe_variable_it = m_variables.find(_name);
    if(maybe_variable_it.is_ok())
        return *maybe_variable_it;

    if(!m_parent_context)
        return nullptr;

    return m_parent_context->get_variable(_name);
}

Variable* Context::extract_variable(const std::string& _name)
{
    Variables_Map::Iterator variable_it = m_variables.find(_name);
    L_ASSERT(variable_it.is_ok());

    Variable* variable = *variable_it;

    m_variables.erase(variable_it);

    return variable;
}


void Context::clear()
{
    for(Variables_Map::Iterator it = m_variables.iterator(); !it.end_reached(); ++it)
        delete *it;
    m_variables.clear();
}
