#pragma once

#include <string>

#include <Data_Structures/Vector.h>

#include <Script_Details/Variables/Variable.h>


namespace LScript
{

class Variable_Reference : public Variable
{
public:
    using Strings_Vector = LDS::Vector<std::string>;

private:
    Variable* m_referencee = nullptr;

public:
    Variable_Reference();
    ~Variable_Reference();

public:
    void reset() override;
    void set_data(const Strings_Vector& _data) override;
    void set_data(const std::string& _data) override;
    void set_data(void* _data, unsigned int _raw_size) override;
    void assign(Variable* _other) override;

public:
    const std::string& type() const override;
    void* data() const override;
    unsigned int raw_size() const override;

};

}
