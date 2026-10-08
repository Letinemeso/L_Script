#include <Python_Script/Python_Script_Engine.h>5

#include <L_Debug/L_Debug.h>
#include <Stuff/Cast_Tools.h>

using namespace LScript;

namespace LScript
{
    constexpr const char* Log_Level_Name = "Python Script";
    constexpr const char* Global_Module_Name = "Engine";

    void print(const std::string& _what)
    {
        L_LOG(Log_Level_Name, _what);
    }
    void print(int _what)
    {
        L_LOG(Log_Level_Name, std::to_string(_what));
    }
    void print(unsigned int _what)
    {
        L_LOG(Log_Level_Name, std::to_string(_what));
    }
    void print(float _what)
    {
        L_LOG(Log_Level_Name, std::to_string(_what));
    }

}

PYBIND11_EMBEDDED_MODULE(Engine, m)
{
    m.def("log", pybind11::overload_cast<const std::string&>(&LScript::print));
    m.def("log", pybind11::overload_cast<int>(&LScript::print));
    m.def("log", pybind11::overload_cast<unsigned int>(&LScript::print));
    m.def("log", pybind11::overload_cast<float>(&LScript::print));
}

Python_Script_Engine::Python_Script_Engine()
{
    L_CREATE_LOG_LEVEL(Log_Level_Name);

    m_interpreter = new pybind11::scoped_interpreter;
    m_builtins = pybind11::module_::import("builtins");
    m_default_executor = m_builtins.attr("exec");
    m_default_compiler = m_builtins.attr("compile");

    M_register_default_functions();
}



Python_Script_Engine::~Python_Script_Engine()
{
    // interpreter deletion causes a crash, so fuck it

    // pybind11::scoped_interpreter* interpreter = LST::raw_cast<pybind11::scoped_interpreter>(m_hidden_interpreter);
    // delete interpreter;
}



void Python_Script_Engine::M_register_default_functions()
{

}



pybind11::object Python_Script_Engine::compile_script(const std::string& _source, const std::string& _name) const
{
#ifdef L_DEBUG
    try
    {
        pybind11::object result = m_default_compiler(_source, _name, "exec");
        return result;
    }
    catch(const std::exception& _exception)
    {
        L_ASSERT_WITH_INFO(false, std::string("[Script compilation crash] ") + _exception.what());
    }
#else
    pybind11::object result = m_default_compiler(_source, _name, "exec");
    return result;
#endif
}

void Python_Script_Engine::run_script(const pybind11::object& _precompiled_script, const pybind11::dict& _script_context) const
{
#ifdef L_DEBUG
    try
    {
        m_default_executor(_precompiled_script, _script_context);
    }
    catch(const std::exception& _exception)
    {
        L_ASSERT_WITH_INFO(false, std::string("[Script crash] ") + _exception.what());
    }
#else
    m_default_executor(_precompiled_script, _script_context);
#endif
}
