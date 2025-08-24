#pragma once

#include <Integrated_Functions.h>
#include <Script_Details/Operations/Operation.h>
#include <Script_Details/Context.h>


namespace LScript
{

    class Call_Member_Function : public Operation
    {
    public:
        using Arguments_Getter_Operations = LDS::Vector<Operation*>;

    private:
        Context* m_context = nullptr;

        std::string m_function_name;

        Operation* m_owner_object_getter_operation = nullptr;
        Arguments_Getter_Operations m_arguments_getter_operations;

    private:
        std::string m_source_line;
        unsigned int m_source_line_number = 0;

    public:
        Call_Member_Function();
        ~Call_Member_Function();

    public:
        inline void set_context(Context* _ptr) { m_context = _ptr; }

        inline void set_function_name(const std::string& _value) { m_function_name = _value; }

        inline void set_owner_object_getter_operation(Operation* _operation) { delete m_owner_object_getter_operation; m_owner_object_getter_operation = _operation; }

        inline void set_debug_info(const std::string& _line, unsigned int _line_number) { m_source_line = _line; m_source_line_number = _line_number; }

    public:
        void clear_arguments_getter_operations();
        void set_arguments_getter_operations(Arguments_Getter_Operations&& _operations);

    public:
        Variable* process() override;

    };

}
