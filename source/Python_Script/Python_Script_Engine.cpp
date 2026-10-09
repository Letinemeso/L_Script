#include <Python_Script/Python_Script_Engine.h>

#include <L_Debug/L_Debug.h>
#include <Stuff/Cast_Tools.h>
#include <Stuff/Math_Stuff.h>
#include <Data_Structures/Vector.h>
#include <Data_Structures/List.h>

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
        .def(pybind11::init<const glm::vec2&>())
        .def_readwrite("x", &glm::vec2::x)
        .def_readwrite("y", &glm::vec2::y)
        .def("__add__", [](const glm::vec2& _this, const glm::vec2& _other){ return _this + _other; })
        .def("__sub__", [](const glm::vec2& _this, const glm::vec2& _other){ return _this - _other; })
        .def("__mul__", [](const glm::vec2& _this, float _multiplier){ return _this * _multiplier; })
        .def("__truediv__", [](const glm::vec2& _this, float _divider){ return _this / _divider; })
        .def("__getitem__", [](const glm::vec2& _this, unsigned int _index){ L_ASSERT(_index < 2); return _this[_index]; })
        .def("__setitem__", [](glm::vec2& _this, unsigned int _index, float _value){ L_ASSERT(_index < 2); _this[_index] = _value; })
        .def("__repr__", [](const glm::vec2& _v){
            return "vec2(" + std::to_string(_v.x) + ", " + std::to_string(_v.y) + ")";
        });

    pybind11::class_<glm::vec3>(module, "vec3")
        .def(pybind11::init<float, float, float>())
        .def(pybind11::init<const glm::vec3&>())
        .def_readwrite("x", &glm::vec3::x)
        .def_readwrite("y", &glm::vec3::y)
        .def_readwrite("z", &glm::vec3::z)
        .def("__add__", [](const glm::vec3& a, const glm::vec3& b){ return a + b; })
        .def("__sub__", [](const glm::vec3& _this, const glm::vec3& _other){ return _this - _other; })
        .def("__mul__", [](const glm::vec3& _this, float _multiplier){ return _this * _multiplier; })
        .def("__truediv__", [](const glm::vec3& _this, float _divider){ return _this / _divider; })
        .def("__getitem__", [](const glm::vec3& _this, unsigned int _index){ L_ASSERT(_index < 3); return _this[_index]; })
        .def("__setitem__", [](glm::vec3& _this, unsigned int _index, float _value){ L_ASSERT(_index < 3); _this[_index] = _value; })
        .def("__repr__", [](const glm::vec3& _v){
            return "vec3(" + std::to_string(_v.x) + ", " + std::to_string(_v.y) + ", " + std::to_string(_v.z) + ")";
        });

    pybind11::class_<glm::quat>(module, "quat")
        .def(pybind11::init<>())
        .def(pybind11::init<float, float, float, float>())
        .def(pybind11::init<const glm::quat&>())
        .def_readwrite("x", &glm::quat::x)
        .def_readwrite("y", &glm::quat::y)
        .def_readwrite("z", &glm::quat::z)
        .def_readwrite("w", &glm::quat::w)
        .def("__repr__", [](const glm::quat& _q){
            return "quat(" + std::to_string(_q.x) + ", " + std::to_string(_q.y) + ", " + std::to_string(_q.z) + ", " + std::to_string(_q.w) + ")";
        });

    module.def("vector_length", pybind11::overload_cast<const glm::vec2&>(&LST::Math::vector_length));
    module.def("vector_length", pybind11::overload_cast<const glm::vec3&>(&LST::Math::vector_length));

    module.def("vector_length_squared", pybind11::overload_cast<const glm::vec2&>(&LST::Math::vector_length_squared));
    module.def("vector_length_squared", pybind11::overload_cast<const glm::vec3&>(&LST::Math::vector_length_squared));

    module.def("shrink_vector_to_1", pybind11::overload_cast<glm::vec2&>(&LST::Math::shrink_vector_to_1));
    module.def("shrink_vector_to_1", pybind11::overload_cast<glm::vec3&>(&LST::Math::shrink_vector_to_1));

    module.def("extend_vector_to_length", pybind11::overload_cast<glm::vec2&, float>(&LST::Math::extend_vector_to_length));
    module.def("extend_vector_to_length", pybind11::overload_cast<glm::vec3&, float>(&LST::Math::extend_vector_to_length));

    module.def("calculate_direction_vec", &LST::Math::calculate_direction_vec);

    module.def("calculate_distance", &LST::Math::calculate_distance);
    module.def("calculate_distance_squared", &LST::Math::calculate_distance_squared);

    module.def("dot_product", pybind11::overload_cast<const glm::vec2&, const glm::vec2&>(&LST::Math::dot_product));
    module.def("dot_product", pybind11::overload_cast<const glm::vec3&, const glm::vec3&>(&LST::Math::dot_product));

    module.def("cross_product", &LST::Math::cross_product);

    module.def("calculate_perpendicular", &LST::Math::calculate_perpendicular);

    module.def("rotate_vector", &LST::Math::rotate_vector);

    module.def("calculate_angles", pybind11::overload_cast<const glm::vec3&, const glm::vec3&>(&LST::Math::calculate_angles));
    module.def("calculate_angles", pybind11::overload_cast<const glm::quat&>(&LST::Math::calculate_angles));

    module.def("calculate_rotation_quaternion", pybind11::overload_cast<const glm::vec3&, const glm::vec3&>(&LST::Math::calculate_rotation_quaternion));
    module.def("calculate_rotation_quaternion", pybind11::overload_cast<const glm::vec3&>(&LST::Math::calculate_rotation_quaternion));

    module.def("random_number", &LST::Math::random_number);

    module.def("random_number_float", &LST::Math::random_number_float);

    module.def("random_number_float_normal_distribution", &LST::Math::random_number_float_normal_distribution);

    module.def("random_bool", pybind11::overload_cast<>(&LST::Math::random_bool));
    module.def("random_bool", pybind11::overload_cast<unsigned int, unsigned int>(&LST::Math::random_bool));

    module.def("random_vec2", &LST::Math::random_vec2);

    module.def("random_vec3", pybind11::overload_cast<float>(&LST::Math::random_vec3));
    module.def("random_vec3", pybind11::overload_cast<const glm::vec3&, const glm::vec3&>(&LST::Math::random_vec3));

    module.def("random_vec3_rotation", &LST::Math::random_vec3_rotation);
}

template <typename _Type>
void register_vector(pybind11::module_& _module, const std::string& _type_name)
{
    std::string container_name = "Vector_" + _type_name;

    using Vec_Type = LDS::Vector<_Type>;

    pybind11::class_<Vec_Type> class_object(_module, container_name.c_str());
    class_object.def(pybind11::init<>());
    class_object.def(pybind11::init<unsigned int>());
    class_object.def(pybind11::init<unsigned int, const _Type&>());
    class_object.def(pybind11::init<const Vec_Type&>());

    class_object.def("resize", &Vec_Type::resize);
    class_object.def("fill", &Vec_Type::fill);
    class_object.def("resize_and_fill", &Vec_Type::resize_and_fill);
    class_object.def("clear", &Vec_Type::clear);
    class_object.def("mark_empty", &Vec_Type::mark_empty);
    class_object.def("mark_full", &Vec_Type::mark_full);

    class_object.def("push", pybind11::overload_cast<const _Type&>(&Vec_Type::push));
    class_object.def("swap", pybind11::overload_cast<unsigned int, unsigned int>(&Vec_Type::swap));

    class_object.def("size", &Vec_Type::size);
    class_object.def("capacity", &Vec_Type::capacity);
    class_object.def("contains", &Vec_Type::contains);

    class_object.def("__getitem__", [](const Vec_Type& _this, unsigned int _index){ return _this[_index]; });
    class_object.def("__setitem__", [](Vec_Type& _this, unsigned int _index, const _Type& _value){ _this[_index] = _value; });
}

PYBIND11_EMBEDDED_MODULE(Containers, module)
{
    register_vector<int>(module, "int");
    register_vector<unsigned int>(module, "uint");
    register_vector<float>(module, "float");
    register_vector<bool>(module, "bool");
    register_vector<glm::vec2>(module, "vec2");
    register_vector<glm::vec3>(module, "vec3");
}

