#pragma once

#include <Script_Details/Operations/Operation.h>
#include <Script_Details/Context.h>


namespace LScript
{

    class Variable_Creation : public Operation
    {
    private:
        Context* m_context = nullptr;

        std::string m_variable_type;
        std::string m_variable_name;

    public:
        inline void set_context(Context* _ptr) { m_context = _ptr; }

        inline void set_variable_type(const std::string& _value) { m_variable_type = _value; }
        inline void set_variable_name(const std::string& _value) { m_variable_name = _value; }

    public:
        void process() override;

    };

}
