#pragma once

#include <Data_Structures/Map.h>

#include <Script_Details/Variable.h>


namespace LScript
{

    class Context
    {
    public:
        using Variables_Map = LDS::Map<std::string, Variable*>;

    private:
        Variables_Map m_variables;

    public:
        Context();
        ~Context();

    public:
        inline const Variables_Map& variables() const { return m_variables; }

    public:
        void add_variable(const std::string& _name, Variable* _variable);
        Variable* get_variable(const std::string& _name) const;
        [[nodiscard]] Variable* extract_variable(const std::string& _name);

    };

}
