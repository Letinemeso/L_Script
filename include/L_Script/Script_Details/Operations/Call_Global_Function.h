#pragma once

#include <Data_Structures/Vector.h>

#include <L_Script/Integrated_Functions.h>
#include <L_Script/Script_Details/Operations/Operation.h>
#include <L_Script/Script_Details/Function.h>
#include <L_Script/L_Script.h>


namespace LScript
{

    class Call_Global_Function : public Operation
    {
    public:
        using Arguments_Getter_Operations = LDS::Vector<Operation*>;

    private:
        std::string m_function_name;

        Arguments_Getter_Operations m_arguments_getter_operations;
        const L_Script* m_script = nullptr;

    private:
        std::string m_source_line;
        unsigned int m_source_line_number = 0;

    public:
        Call_Global_Function();
        ~Call_Global_Function();

    public:
        inline void set_function_name(const std::string& _value) { m_function_name = _value; }
        inline void set_script(const L_Script* _ptr) { m_script = _ptr; }

        inline void set_debug_info(const std::string& _line, unsigned int _line_number) { m_source_line = _line; m_source_line_number = _line_number; }

    public:
        void clear_arguments_getter_operations();
        void set_arguments_getter_operations(Arguments_Getter_Operations&& _operations);

    public:
        Variable* process() override;

    };

}
