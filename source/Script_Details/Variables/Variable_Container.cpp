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

void Variable_Container::set_data(const Strings_Vector& _data)
{
    if(m_data)
        LV::Type_Manager::clear(m_type, m_data);

    LV::Type_Utility::Allocate_Result allocation_result = LV::Type_Manager::allocate(m_type, _data.size());
    m_data = allocation_result.ptr;
    m_raw_size = allocation_result.size;

    LV::Type_Manager::parse(m_type, _data, m_data);
}

void Variable_Container::set_data(const std::string& _data)
{
    if(m_data)
        LV::Type_Manager::clear(m_type, m_data);

    LV::Type_Utility::Allocate_Result allocation_result = LV::Type_Manager::allocate(m_type, 1);
    m_data = allocation_result.ptr;
    m_raw_size = allocation_result.size;

    Strings_Vector crutch(1);
    crutch.push(_data);

    LV::Type_Manager::parse(m_type, crutch, m_data);
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

    m_raw_size = _other->raw_size();
    char* raw_array = new char[m_raw_size];
    m_data = (void*)raw_array;

    char* other_raw_array = (char*)_other->data();

    for(unsigned int i = 0; i < m_raw_size; ++i)
        raw_array[i] = other_raw_array[i];
}


void Variable_Container::set_type(const std::string& _type)
{
    reset();
    m_type = _type;
}

void Variable_Container::setup(const std::string& _type, const Strings_Vector& _data)
{
    reset();

    m_type = _type;
    set_data(_data);
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
