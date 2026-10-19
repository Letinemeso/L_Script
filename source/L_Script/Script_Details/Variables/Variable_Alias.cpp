#include <L_Script/Script_Details/Variables/Variable_Alias.h>

#include <Type_Manager.h>

using namespace LScript;


Variable_Alias::Variable_Alias()
{

}

Variable_Alias::~Variable_Alias()
{

}



void Variable_Alias::reset()
{
    L_ASSERT(m_referenced_variable);

    m_referenced_variable->reset();
}

void Variable_Alias::set_data(void* _data)
{
    L_ASSERT(m_referenced_variable);

    m_referenced_variable->set_data(_data);
}

void Variable_Alias::assign(const Variable* _other)
{
    m_referenced_variable->assign(_other);
}



const std::string& Variable_Alias::type() const
{
    L_ASSERT(m_referenced_variable);

    return m_referenced_variable->type();
}

void* Variable_Alias::data() const
{
    L_ASSERT(m_referenced_variable);

    return m_referenced_variable->data();
}

bool Variable_Alias::bindable() const
{
    return m_referenced_variable->bindable();
}


Variable* Variable_Alias::construct_copy() const
{
    Variable_Alias* result = new Variable_Alias;
    result->set_referenced_variable(m_referenced_variable);
    return result;
}
