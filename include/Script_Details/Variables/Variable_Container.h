#pragma once

#include <string>

#include <Data_Structures/Vector.h>

#include <Script_Details/Variables/Variable.h>


namespace LScript
{

class Variable_Container : public Variable
{
public:
    using Strings_Vector = LDS::Vector<std::string>;

private:
    std::string m_type;
    void* m_data = nullptr;
    unsigned int m_raw_size = 0;

public:
    Variable_Container();
    ~Variable_Container();

public:
    void reset() override;
    void set_data(const Strings_Vector& _data) override;
    void set_data(const std::string& _data) override;
    void set_data(void* _data, unsigned int _raw_size) override;
    void assign(Variable* _other) override;

    void set_type(const std::string& _type);
    void setup(const std::string& _type, const Strings_Vector& _data);

public:
    const std::string& type() const override;
    void* data() const override;
    unsigned int raw_size() const override;

};

}
