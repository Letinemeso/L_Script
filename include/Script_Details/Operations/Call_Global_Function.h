#pragma once

#include <Data_Structures/Vector.h>

#include <Script_Details/Operations/Operation.h>
#include <Script_Details/Function.h>


namespace LScript
{

    class Call_Global_Function : public Operation
    {
    public:
        using Arguments_Getter_Operations = LDS::Vector<Operation*>;

    private:
        std::string m_function_name;

        Arguments_Getter_Operations m_arguments_getter_operations;

    public:
        Call_Global_Function();
        ~Call_Global_Function();

    public:
        inline void set_function_name(const std::string& _value) { m_function_name = _value; }

    public:
        void clear_arguments_getter_operations();
        void set_arguments_getter_operations(Arguments_Getter_Operations&& _operations);

    public:
        Variable* process() override;

    };

}
