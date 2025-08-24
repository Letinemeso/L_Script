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

    private:
        std::string m_source_line;
        unsigned int m_source_line_number = 0;

    public:
        inline void set_context(Context* _ptr) { m_context = _ptr; }

        inline void set_variable_type(const std::string& _value) { m_variable_type = _value; }
        inline void set_variable_name(const std::string& _value) { m_variable_name = _value; }

        inline void set_debug_info(const std::string& _line, unsigned int _line_number) { m_source_line = _line; m_source_line_number = _line_number; }

    public:
        Variable* process() override;

    };

}
