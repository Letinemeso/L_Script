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

    m_data = LV::Type_Manager::allocate(m_type, _data.size());
    LV::Type_Manager::parse(m_type, _data, m_data);
}

void Variable::setup(const std::string& _type, const Strings_Vector& _data)
{
    reset();

    m_type = _type;
    set_data(_data);
}

