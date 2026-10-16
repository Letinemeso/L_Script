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
    L_ASSERT(m_referencee);

    m_referencee->reset();
}

void Variable_Reference::set_data(void* _data)
{
    L_ASSERT(m_referencee);

    m_referencee->set_data(_data);
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
