#pragma once

#include <Script_Details/Variables/Variable.h>


namespace LScript
{

    class Operation
    {
    private:
        bool m_stop_required = false;

    public:
        virtual ~Operation() { }

    public:
        inline void set_stop_required(bool _value) { m_stop_required = _value; }
        inline bool stop_required() const { return m_stop_required; }

    public:
        [[nodiscard]] virtual Variable* process() = 0;

    };

}
