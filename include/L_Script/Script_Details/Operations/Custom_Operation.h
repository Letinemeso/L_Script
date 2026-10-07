#pragma once

#include <Stuff/Function_Wrapper.h>

#include <L_Script/Script_Details/Operations/Operation.h>
#include <L_Script/Script_Details/Function.h>


namespace LScript
{

    class Custom_Operation : public Operation
    {
    public:
        using Operation_Logic = LST::Function<Variable*()>;

    private:
        Operation_Logic m_operation_logic;

    public:
        inline void set_operation_logic(const Operation_Logic& _logic) { m_operation_logic = _logic; }

    public:
        Variable* process() override;

    };

}
