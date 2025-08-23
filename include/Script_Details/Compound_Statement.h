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

        bool m_stop_required = false;

    public:
        Compound_Statement();
        Compound_Statement(Compound_Statement&& _from);
        void operator=(Compound_Statement&& _from);

        ~Compound_Statement();

    public:
        inline Context& context() { return m_context; }
        inline const Context& context() const { return m_context; }

        inline bool stop_required() const { return m_stop_required; }

    public:
        void add_operation(Operation* _operation);
        void clear_operations();

    public:
        [[nodiscard]] Variable* process();

    };

}
