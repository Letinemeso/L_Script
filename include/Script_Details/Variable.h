#pragma once

#include <string>

#include <Data_Structures/Vector.h>


namespace LScript
{

    class Variable
    {
    public:
        using Strings_Vector = LDS::Vector<std::string>;

    private:
        std::string m_type;
        void* m_data = nullptr;

    public:
        Variable();
        ~Variable();

    public:
        void reset();
        void set_data(const Strings_Vector& _data);
        void setup(const std::string& _type, const Strings_Vector& _data);

    public:
        inline const std::string& type() const { return m_type; }
        inline void* data() const { return m_data; }

    };

}
