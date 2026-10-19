#pragma once

#include <Data_Structures/Map.h>


namespace LScript
{

    class Variable;


    class Variable_Instance_Counter
    {
    public:
        static constexpr const char* Log_Level = "Variable_Memory";

        using Pointers_Map = LDS::Map<const Variable*, unsigned int>;

    private:
        Pointers_Map m_pointers;

        unsigned int m_counter = 0;
        unsigned int m_id_counter = 0;

    private:
        Variable_Instance_Counter() { }

        Variable_Instance_Counter(const Variable_Instance_Counter&) = delete;
        Variable_Instance_Counter(Variable_Instance_Counter&&) = delete;
        void operator=(const Variable_Instance_Counter&) = delete;
        void operator=(Variable_Instance_Counter&&) = delete;

    public:
        inline static Variable_Instance_Counter& instance() { static Variable_Instance_Counter s_instance; return s_instance; }

        inline unsigned int counter() const { return m_counter; }
        inline const Pointers_Map& registered_variables() const { return m_pointers; }

        inline unsigned int get_variable_id(const Variable* _ptr) const { return *m_pointers.find(_ptr); }

    public:
        void count_variable(const Variable* _ptr);
        void remove_variable(const Variable* _ptr);

        void log_registered_variables_ids() const;

    };

}
