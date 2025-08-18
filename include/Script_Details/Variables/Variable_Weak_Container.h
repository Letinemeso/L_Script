#pragma once

#include <Script_Details/Variables/Variable.h>


namespace LScript
{

    class Variable_Weak_Container : public Variable
    {
    private:
        std::string m_type;
        void* m_data = nullptr;
        unsigned int m_raw_size = 0;

    public:
        Variable_Weak_Container();
        ~Variable_Weak_Container();

    public:
        void reset() override;
        void set_data(void* _data, unsigned int _raw_size) override;
        void assign(Variable* _other) override;

        void set_type(const std::string& _type);

    public:
        const std::string& type() const override;
        void* data() const override;
        unsigned int raw_size() const override;
    };

}
