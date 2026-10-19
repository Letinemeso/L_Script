#include <L_Script/Debug/Variable_Instance_Counter.h>

#include <string>

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


void Variable_Instance_Counter::log_registered_variables_ids() const
{
    for(Pointers_Map::Const_Iterator it = m_pointers.iterator(); !it.end_reached(); ++it)
    {
        L_LOG(Log_Level, std::to_string(*it));
    }
}
