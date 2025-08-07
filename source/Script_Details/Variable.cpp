#include <Script_Details/Variable.h>

#include <Type_Manager.h>

using namespace LScript;


Variable::Variable()
{

}

Variable::~Variable()
{
    if(!m_data)
        return;

    LV::Type_Manager::clear(m_type, m_data);
}



void Variable::reset()
{
    if(m_data)
        LV::Type_Manager::clear(m_type, m_data);

    m_data = nullptr;
    m_raw_size = 0;
    m_type.clear();
}

void Variable::set_type(const std::string& _type)
{
    reset();
    m_type = _type;
}

void Variable::set_data(const Strings_Vector& _data)
{
    if(m_data)
        LV::Type_Manager::clear(m_type, m_data);

    LV::Type_Utility::Allocate_Result allocation_result = LV::Type_Manager::allocate(m_type, _data.size());
    m_data = allocation_result.ptr;
    m_raw_size = allocation_result.size;

    LV::Type_Manager::parse(m_type, _data, m_data);
}

void Variable::set_data(void* _data, unsigned int _raw_size)
{
    if(m_data)
        LV::Type_Manager::clear(m_type, m_data);

    m_data = _data;
    m_raw_size = _raw_size;
}

void Variable::setup(const std::string& _type, const Strings_Vector& _data)
{
    reset();

    m_type = _type;
    set_data(_data);
}

void Variable::shallow_copy(const Variable* _other)
{
    L_ASSERT(_other->m_type == m_type);

    if(m_data)
        LV::Type_Manager::clear(m_type, m_data);

    if(_other->m_raw_size == 0)
        return;

    m_raw_size = _other->m_raw_size;
    char* raw_array = new char[m_raw_size];
    m_data = (void*)raw_array;

    char* other_raw_array = (char*)_other->m_data;

    for(unsigned int i = 0; i < m_raw_size; ++i)
        raw_array[i] = other_raw_array[i];
}

