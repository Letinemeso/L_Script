#include <Script_Details/Variables/Variable_Container.h>

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
    m_raw_size = 0;
    m_type.clear();
}

void Variable_Container::set_data(void* _data, unsigned int _raw_size)
{
    if(m_data)
        LV::Type_Manager::clear(m_type, m_data);

    m_data = _data;
    m_raw_size = _raw_size;
}

void Variable_Container::assign(Variable* _other)
{
    if(m_data)
        LV::Type_Manager::clear(m_type, m_data);

    m_type = _other->type();

    if(_other->raw_size() == 0)
        return;

    LV::Type_Utility::Allocate_Result allocate_result = LV::Type_Manager::allocate(m_type, 1);

    m_raw_size = allocate_result.size;
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

unsigned int Variable_Container::raw_size() const
{
    return m_raw_size;
}
