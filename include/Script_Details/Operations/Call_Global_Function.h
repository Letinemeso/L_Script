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
        Function* m_function = nullptr;

        Arguments_Getter_Operations m_arguments_getter_operations;

    public:
        Call_Global_Function();
        ~Call_Global_Function();

    public:
        inline void set_function(Function* _ptr) { m_function = _ptr; }

    public:
        void clear_arguments_getter_operations();
        void set_arguments_getter_operations(Arguments_Getter_Operations&& _operations);

    public:
        Variable* process() override;

    };

}
