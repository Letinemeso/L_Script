#pragma once

#include <string>

#include <Data_Structures/Vector.h>

#include <Script_Details/Variables/Variable.h>


namespace LScript
{

class Variable_Reference : public Variable
{
private:
    Variable* m_referencee = nullptr;

public:
    Variable_Reference();
    ~Variable_Reference();

public:
    void reset() override;
    void set_data(void* _data, unsigned int _raw_size) override;
    void assign(Variable* _other) override;

public:
    const std::string& type() const override;
    void* data() const override;
    unsigned int raw_size() const override;

};

}
