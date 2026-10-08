#include <Python_Script/Python_Script.h>

#include <Python_Script/Python_Script_Import_Manager.h>
#include <Python_Script/Python_Script_Engine.h>

using namespace LScript;


Python_Script::Python_Script()
{
    m_context["__builtins__"] = Python_Script_Engine::instance().get_builtins();
}

Python_Script::~Python_Script()
{

}



void Python_Script::set_context_object(const std::string& _type_as_string, const std::string& _name, void* _ptr)
{
    Python_Script_Import_Manager::instance().register_context_object(_type_as_string, _name, _ptr, m_context);
}



void Python_Script::run()
{
    L_ASSERT(pybind11::isinstance<pybind11::object>(m_script));
    L_ASSERT(pybind11::isinstance(m_script, pybind11::module_::import("types").attr("CodeType")));

    Python_Script_Engine::instance().run_script(m_script, m_context);
}





ON_VALUES_ASSIGNED_IMPLEMENTATION(Python_Script_Stub)
{
    recompile_script();
}



BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Python_Script_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Python_Script_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_script(m_compiled_script);
}



void Python_Script_Stub::recompile_script()
{
    L_ASSERT(source_code.size() > 0);

    m_compiled_script = Python_Script_Engine::instance().compile_script(source_code, script_name);
}

