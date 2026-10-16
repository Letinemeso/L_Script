#include <L_Script/Script_Details/Variables/Variable_Weak_Container.h>

#include <Type_Manager.h>

using namespace LScript;


Variable_Weak_Container::Variable_Weak_Container()
{

}

Variable_Weak_Container::~Variable_Weak_Container()
{

}



void Variable_Weak_Container::reset()
{
    m_data = nullptr;
    m_type.clear();
}

void Variable_Weak_Container::set_data(void* _data)
{
    m_data = _data;
}

void Variable_Weak_Container::assign(Variable* _other)
{
    m_data = _other->data();
    m_type = _other->type();
}


void Variable_Weak_Container::set_type(const std::string& _type)
{
    reset();
    m_type = _type;
}



const std::string& Variable_Weak_Container::type() const
{
    return m_type;
}

void* Variable_Weak_Container::data() const
{
    return m_data;
}
