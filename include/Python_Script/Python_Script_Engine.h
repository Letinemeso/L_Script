#pragma once

#include <pybind11/embed.h>


namespace LScript
{

    class Python_Script_Engine final
    {
    private:
        pybind11::scoped_interpreter* m_interpreter = nullptr;

        pybind11::object m_builtins;
        pybind11::object m_default_executor;
        pybind11::object m_default_compiler;

    private:
        Python_Script_Engine();

        Python_Script_Engine(const Python_Script_Engine&) = delete;
        Python_Script_Engine(Python_Script_Engine&&) = delete;
        void operator=(const Python_Script_Engine&) = delete;
        void operator=(Python_Script_Engine&&) = delete;

    public:
        ~Python_Script_Engine();

    private:
        void M_register_default_functions();

    public:
        inline static Python_Script_Engine& instance() { static Python_Script_Engine s_instance; return s_instance; }

        inline const pybind11::object& get_builtins() const { return m_builtins; }

    public:
        pybind11::object compile_script(const std::string& _source, const std::string& _name) const;
        void run_script(const pybind11::object& _precompiled_script, const pybind11::dict& _script_context) const;

    };

}
