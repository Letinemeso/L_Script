#include <L_Script/Script_Details/Variables/Variable_Container.h>

#include <Type_Manager.h>

using namespace LScript;


Variable_Container::Variable_Container()
{

}

Variable_Container::~Variable_Container()
{
    if(!m_data)
        return;

    LV::Type_Manager::clear(m_type, m_data);
}



void Variable_Container::reset()
{
    if(m_data)
        LV::Type_Manager::clear(m_type, m_data);

    m_data = nullptr;
    m_type.clear();
}

void Variable_Container::set_data(void* _data)
{
    if(m_data)
        LV::Type_Manager::clear(m_type, m_data);

    m_data = _data;
}

void Variable_Container::assign(const Variable* _other)
{
    if(m_data)
        LV::Type_Manager::clear(m_type, m_data);

    m_type = _other->type();

    if(!_other->data())
        return;

    LV::Type_Utility::Allocate_Result allocate_result = LV::Type_Manager::allocate(m_type, 1);

    m_data = allocate_result.ptr;

    LV::Type_Manager::copy(m_type, m_data, _other->data());
}


void Variable_Container::set_type(const std::string& _type)
{
    reset();
    m_type = _type;
}



const std::string& Variable_Container::type() const
{
    return m_type;
}

void* Variable_Container::data() const
{
    return m_data;
}

bool Variable_Container::bindable() const
{
    return false;
}


Variable* Variable_Container::construct_copy() const
{
    Variable_Container* result = new Variable_Container;
    result->assign(this);
    return result;
}
