#pragma once

#include <Data_Structures/List.h>

#include <Script_Details/Context.h>
#include <Script_Details/Operations/Operation.h>


namespace LScript
{

    class Compound_Statement
    {
    private:
        Context m_context;

        using Operations_List = LDS::List<Operation*>;
        Operations_List m_operations;

    public:
        Compound_Statement();
        ~Compound_Statement();

    public:
        inline Context& context() { return m_context; }
        inline const Context& context() const { return m_context; }

    public:
        void add_operation(Operation* _operation);
        void clear_operations();

    public:
        [[nodiscard]] Variable* process();

    };

}
