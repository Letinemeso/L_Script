#pragma once

#include <string>

#include <Data_Structures/Vector.h>

#include <L_Script/Script_Details/Variables/Variable.h>


namespace LScript
{

    class Variable_Reference : public Variable
    {
    private:
        Variable* m_referenced_variable = nullptr;

    public:
        Variable_Reference();
        ~Variable_Reference();

    public:
        inline void set_referenced_variable(Variable* _ptr) { m_referenced_variable = _ptr; }

    public:
        void reset() override;
        void set_data(void* _data) override;
        void assign(Variable* _other) override;

    public:
        const std::string& type() const override;
        void* data() const override;
        bool bindable() const override;

    };

}
