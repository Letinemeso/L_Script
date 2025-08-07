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
        unsigned int m_raw_size = 0;

    public:
        Variable();
        ~Variable();

    public:
        void reset();
        void set_type(const std::string& _type);
        void set_data(const Strings_Vector& _data);
        void set_data(void* _data, unsigned int _raw_size);
        void setup(const std::string& _type, const Strings_Vector& _data);
        void shallow_copy(const Variable* _other);

    public:
        inline const std::string& type() const { return m_type; }
        inline void* data() const { return m_data; }

    };

}
