#include <pybind11/embed.h>

#include <L_Debug/L_Debug.h>

#include <vec2.hpp>
#include <vec3.hpp>
#include <Stuff/Math_Stuff.h>


namespace LScript
{

    constexpr const char* Log_Level_Name = "Python Script";
    constexpr const char* Global_Module_Name = "Engine";

    void __log(const std::string& _what)
    {
        L_LOG(Log_Level_Name, _what);
    }
    void __log(int _what)
    {
        L_LOG(Log_Level_Name, std::to_string(_what));
    }
    void __log(unsigned int _what)
    {
        L_LOG(Log_Level_Name, std::to_string(_what));
    }
    void __log(float _what)
    {
        L_LOG(Log_Level_Name, std::to_string(_what));
    }

    void __assert(bool _condition)
    {
        L_ASSERT(_condition);
    }
    void __assert_with_info(bool _condition, const std::string& _info)
    {
        L_ASSERT(_condition);
    }
}


class __Log_Level_Registrator
{
public:
    __Log_Level_Registrator()
    {
        L_CREATE_LOG_LEVEL(LScript::Log_Level_Name);
    }
} llr_instance;


PYBIND11_EMBEDDED_MODULE(Engine, module)
{
    module.def("assert", LScript::__assert);
    module.def("assert_with_info", LScript::__assert_with_info);
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
            return "vec3(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
        });

    module.def("shrink_vector_to_1", pybind11::overload_cast<glm::vec2&>(&LST::Math::shrink_vector_to_1));
    module.def("shrink_vector_to_1", pybind11::overload_cast<glm::vec3&>(&LST::Math::shrink_vector_to_1));
}
