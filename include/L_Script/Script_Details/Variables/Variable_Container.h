#pragma once

#include <string>

#include <Data_Structures/Vector.h>

#include <L_Script/Script_Details/Variables/Variable.h>


namespace LScript
{

    class Variable_Container : public Variable
    {
    private:
        std::string m_type;
        void* m_data = nullptr;

    public:
        Variable_Container();
        ~Variable_Container();

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
