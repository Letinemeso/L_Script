#pragma once

#include <Script_Details/Operations/Operation.h>
#include <Script_Details/Variables/Variable_Container.h>


namespace LScript
{

    class RValue_Getter : public Operation
    {
    private:
        Variable_Container m_variable;

    public:
        inline Variable_Container& variable() { return m_variable; }
        inline const Variable_Container& variable() const { return m_variable; }

    public:
        Variable* process() override;

    };

}
