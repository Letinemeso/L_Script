#pragma once

#include <Data_Structures/Map.h>

#include <Script_Details/Variables/Variable.h>


namespace LScript
{

    class Context
    {
    public:
        using Variables_Map = LDS::Map<std::string, Variable*>;

    private:
        const Context* m_parent_context = nullptr;

        Variables_Map m_variables;

    public:
        Context();
        Context(Context&& _from);
        void operator=(Context&& _from);

        ~Context();

    public:
        inline void set_parent_context(const Context* _ptr) { m_parent_context = _ptr; }

        inline const Context* parent_context() const { return m_parent_context; }
        inline const Variables_Map& variables() const { return m_variables; }

    public:
        void add_variable(const std::string& _name, Variable* _variable);
        Variable* get_variable(const std::string& _name) const;
        Variable* get_local_variable(const std::string& _name) const;
        [[nodiscard]] Variable* extract_variable(const std::string& _name);

        void clear();

    };

}
