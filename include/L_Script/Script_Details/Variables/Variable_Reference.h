#pragma once

#include <L_Script/Script_Details/Variables/Variable.h>


namespace LScript
{

    class Variable_Reference : public Variable
    {
    private:
        std::string m_type;
        void* m_data = nullptr;
        bool m_bindable = false;

    public:
        Variable_Reference();
        ~Variable_Reference();

    public:
        inline void set_bindable(bool _value) { m_bindable = _value; }

    public:
        void reset() override;
        void set_data(void* _data) override;
        void assign(const Variable* _other) override;

        void set_type(const std::string& _type);

    public:
        const std::string& type() const override;
        void* data() const override;
        bool bindable() const override;

        [[nodiscard]] Variable* construct_copy() const override;
    };

}
