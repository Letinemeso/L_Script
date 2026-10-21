#pragma once

#include <L_Script/Script_Details/Operations/Operation.h>
#include <L_Script/Script_Details/Variables/Variable_Container.h>


namespace LScript
{

    class Return_Operation : public Operation
    {
    private:
        Operation* m_operation = nullptr;

    public:
        Return_Operation();
        ~Return_Operation();

    public:
        inline void set_operation(Operation* _ptr) { delete m_operation; m_operation = _ptr; }

    public:
        Variable* process(Context* _context) override;

    };

}
