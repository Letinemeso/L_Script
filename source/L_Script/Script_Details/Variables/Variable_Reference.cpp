#include <L_Script/Script_Details/Variables/Variable_Reference.h>

#include <Type_Manager.h>

using namespace LScript;


Variable_Reference::Variable_Reference()
{

}

Variable_Reference::~Variable_Reference()
{

}



void Variable_Reference::reset()
{
    m_data = nullptr;
    m_type.clear();
}

void Variable_Reference::set_data(void* _data)
{
    m_data = _data;
}

void Variable_Reference::assign(const Variable* _other)
{
    m_data = _other->data();
    m_type = _other->type();
}


void Variable_Reference::set_type(const std::string& _type)
{
    reset();
    m_type = _type;
}



const std::string& Variable_Reference::type() const
{
    return m_type;
}

void* Variable_Reference::data() const
{
    return m_data;
}

bool Variable_Reference::bindable() const
{
    return m_bindable;
}


Variable* Variable_Reference::construct_copy() const
{
    Variable_Reference* result = new Variable_Reference;
    result->assign(this);
    return result;
}
