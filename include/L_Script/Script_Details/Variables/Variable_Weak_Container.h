#pragma once

#include <L_Script/Script_Details/Variables/Variable.h>


namespace LScript
{

    class Variable_Weak_Container : public Variable
    {
    private:
        std::string m_type;
        void* m_data = nullptr;
        bool m_bindable = false;

    public:
        Variable_Weak_Container();
        ~Variable_Weak_Container();

    public:
        inline void set_bindable(bool _value) { m_bindable = _value; }

    public:
        void reset() override;
        void set_data(void* _data) override;
        void assign(Variable* _other) override;

        void set_type(const std::string& _type);

    public:
        const std::string& type() const override;
        void* data() const override;
        bool bindable() const override;
    };

}
