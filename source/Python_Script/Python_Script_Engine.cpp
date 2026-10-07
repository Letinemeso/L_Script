#include <Python_Script/Python_Script_Engine.h>

#include <pybind11/embed.h>

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

    m_hidden_interpreter = (void*) new pybind11::scoped_interpreter;

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
