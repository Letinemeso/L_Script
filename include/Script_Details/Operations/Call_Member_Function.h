#pragma once

#include <Script_Details/Operations/Operation.h>
#include <Script_Details/Context.h>


namespace LScript
{

    class Call_Member_Function : public Operation
    {
    private:
        Context* m_context = nullptr;

        std::string m_function_name;

        Operation* m_owner_object_getter_operation = nullptr;

    public:
        Call_Member_Function();
        ~Call_Member_Function();

    public:
        inline void set_context(Context* _ptr) { m_context = _ptr; }

        inline void set_function_name(const std::string& _value) { m_function_name = _value; }

        inline void set_owner_object_getter_operation(Operation* _operation) { delete m_owner_object_getter_operation; m_owner_object_getter_operation = _operation; }

    public:
        Variable* process() override;

    };

}
