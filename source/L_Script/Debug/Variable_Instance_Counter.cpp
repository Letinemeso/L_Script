#include <L_Script/Debug/Variable_Instance_Counter.h>

using namespace LScript;


void Variable_Instance_Counter::count_variable(const Variable* _ptr)
{
    L_ASSERT(!m_pointers.contains(_ptr));

    m_pointers.insert(_ptr, m_id_counter);

    ++m_id_counter;
    ++m_counter;
}

void Variable_Instance_Counter::remove_variable(const Variable* _ptr)
{
    L_ASSERT(m_pointers.contains(_ptr));

    m_pointers.erase( m_pointers.find(_ptr) );

    --m_counter;
}
