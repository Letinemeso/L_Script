#pragma once

#include <string>

#include <Data_Structures/Vector.h>


namespace LScript
{

    class Variable
    {
    private:
        bool m_temporary = false;

    public:
        Variable();
        virtual ~Variable();

    public:
        inline void mark_temporary(bool _value) { m_temporary = _value; }
        inline bool is_temporary() const { return m_temporary; }

    public:
        virtual void reset() = 0;
        virtual void set_data(void* _data) = 0;
        virtual void assign(Variable* _what) = 0;

    public:
        virtual const std::string& type() const = 0;
        virtual void* data() const = 0;
        virtual bool bindable() const = 0;

    };

}
