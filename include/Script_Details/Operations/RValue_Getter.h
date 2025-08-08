#pragma once

#include <Script_Details/Operations/Operation.h>


namespace LScript
{

    class RValue_Getter : public Operation
    {
    private:
        Variable m_variable;

    public:
        inline Variable& variable() { return m_variable; }
        inline const Variable& variable() const { return m_variable; }

    public:
        Variable* process() override;

    };

}
