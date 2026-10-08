#include <Python_Script/Python_Script_Import_Manager.h>

#include <pybind11/embed.h>

using namespace LScript;


void Python_Script_Import_Manager::import_all()
{
    for(Registered_Modules_Map::Iterator it = m_registered_modules.iterator(); !it.end_reached(); ++it)
        pybind11::module_::import(it.key().c_str());
}

void Python_Script_Import_Manager::register_context_object(const std::string& _context_variable_type, const std::string& _context_variable_name, void* _typeless_object, pybind11::dict& _script_context)
{
    Registered_Modules_Map::Iterator module_it = m_registered_modules.find(_context_variable_type);
    L_ASSERT(module_it.is_ok());

    const Module_Data::Context_Object_Registration_Func& registration_func = module_it->context_object_registration;
    registration_func(_context_variable_name, _typeless_object, _script_context);
}
