#pragma once

#include <pybind11/embed.h>

#include <Script.h>


namespace LScript
{

    class Python_Script : public Script
    {
    public:
        INIT_VARIABLE(LScript::Python_Script, LScript::Script)

    private:
        pybind11::object m_script;
        pybind11::dict m_context;

    public:
        Python_Script();
        ~Python_Script();

    public:
        inline void set_script(const pybind11::object& _script) { m_script = _script; }

    public:
        void set_context_object(const std::string& _type_as_string, const std::string& _name, void* _ptr) override;

    public:
        void run() override;

    };


    class Python_Script_Stub : public Script_Stub
    {
    public:
        INIT_VARIABLE(LScript::Python_Script_Stub, LScript::Script_Stub)

        INIT_FIELDS
        ADD_FIELD(std::string, script_name)
        ADD_FIELD(std::string, source_code)
        FIELDS_END

        OVERRIDE_ON_VALUES_ASSIGNED

    public:
        std::string script_name;
        std::string source_code;

    private:
        pybind11::object m_compiled_script;

    public:
        INIT_BUILDER_STUB(Python_Script)

    private:
        void recompile_script();

    };

}
