#pragma once

#include <string>

#include <Data_Structures/Vector.h>


namespace LScript
{

    class Variable
    {
    public:
        using Strings_Vector = LDS::Vector<std::string>;

    public:
        Variable() { }
        virtual ~Variable() { }

    public:
        virtual void reset() = 0;
        virtual void set_data(const Strings_Vector& _data) = 0;
        virtual void set_data(const std::string& _data) = 0;
        virtual void set_data(void* _data, unsigned int _raw_size) = 0;
        virtual void assign(Variable* _what) = 0;

    public:
        virtual const std::string& type() const = 0;
        virtual void* data() const = 0;
        virtual unsigned int raw_size() const = 0;

    };

}
