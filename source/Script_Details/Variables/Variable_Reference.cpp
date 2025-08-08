#include <Script_Details/Variables/Variable_Reference.h>

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
    L_ASSERT(m_referencee);

    m_referencee->reset();
}

void Variable_Reference::set_data(const Strings_Vector& _data)
{
    L_ASSERT(m_referencee);

    m_referencee->set_data(_data);
}

void Variable_Reference::set_data(const std::string& _data)
{
    L_ASSERT(m_referencee);

    m_referencee->set_data(_data);
}

void Variable_Reference::set_data(void* _data, unsigned int _raw_size)
{
    L_ASSERT(m_referencee);

    m_referencee->set_data(_data, _raw_size);
}

void Variable_Reference::assign(Variable* _other)
{
    m_referencee = _other;
}



const std::string& Variable_Reference::type() const
{
    L_ASSERT(m_referencee);

    return m_referencee->type();
}

void* Variable_Reference::data() const
{
    L_ASSERT(m_referencee);

    return m_referencee->data();
}

unsigned int Variable_Reference::raw_size() const
{
    L_ASSERT(m_referencee);

    return m_referencee->raw_size();
}
