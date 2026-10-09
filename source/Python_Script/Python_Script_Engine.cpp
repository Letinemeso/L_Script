#include <Python_Script/Python_Script_Engine.h>5

#include <L_Debug/L_Debug.h>
#include <Stuff/Cast_Tools.h>
#include <Stuff/Math_Stuff.h>

using namespace LScript;

namespace LScript
{
    constexpr const char* Log_Level_Name = "Python Script";
}


Python_Script_Engine::Python_Script_Engine()
{
    L_CREATE_LOG_LEVEL(LScript::Log_Level_Name);

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


namespace LScript
{
    void __assert(bool _condition)
    {
        L_ASSERT(_condition);
    }
    void __assert_with_info(bool _condition, const std::string& _info)
    {
        L_ASSERT_WITH_INFO(_condition, _info);
    }
}





PYBIND11_EMBEDDED_MODULE(Debug, module)
{
    module.def("L_ASSERT", LScript::__assert);
    module.def("L_ASSERT_WITH_INFO", LScript::__assert_with_info);
}


PYBIND11_EMBEDDED_MODULE(Math, module)
{
    pybind11::class_<glm::vec2>(module, "vec2")
    .def(pybind11::init<float, float>())
        .def_readwrite("x", &glm::vec2::x)
        .def_readwrite("y", &glm::vec2::y)
        .def("__add__", [](const glm::vec2& _this, const glm::vec2& _other){ return _this + _other; })
        .def("__sub__", [](const glm::vec2& _this, const glm::vec2& _other){ return _this - _other; })
        .def("__mul__", [](const glm::vec2& _this, float _multiplier){ return _this * _multiplier; })
        .def("__truediv__", [](const glm::vec2& _this, float _divider){ return _this / _divider; })
        .def("__getitem__", [](const glm::vec2& _this, unsigned int _index){ L_ASSERT(_index < 2); return _this[_index]; })
        .def("__setitem__", [](glm::vec2& _this, unsigned int _index, float _value){ L_ASSERT(_index < 2); _this[_index] = _value; })
        .def("__repr__", [](const glm::vec2& v){
            return "vec2(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
        });

    pybind11::class_<glm::vec3>(module, "vec3")
        .def(pybind11::init<float, float, float>())
        .def_readwrite("x", &glm::vec3::x)
        .def_readwrite("y", &glm::vec3::y)
        .def_readwrite("z", &glm::vec3::z)
        .def("__add__", [](const glm::vec3& a, const glm::vec3& b){ return a + b; })
        .def("__sub__", [](const glm::vec3& _this, const glm::vec3& _other){ return _this - _other; })
        .def("__mul__", [](const glm::vec3& _this, float _multiplier){ return _this * _multiplier; })
        .def("__truediv__", [](const glm::vec3& _this, float _divider){ return _this / _divider; })
        .def("__getitem__", [](const glm::vec3& _this, unsigned int _index){ L_ASSERT(_index < 3); return _this[_index]; })
        .def("__setitem__", [](glm::vec3& _this, unsigned int _index, float _value){ L_ASSERT(_index < 3); _this[_index] = _value; })
        .def("__repr__", [](const glm::vec3& v){
            return "vec3(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
        });

    module.def("shrink_vector_to_1", pybind11::overload_cast<glm::vec2&>(&LST::Math::shrink_vector_to_1));
    module.def("shrink_vector_to_1", pybind11::overload_cast<glm::vec3&>(&LST::Math::shrink_vector_to_1));
}

