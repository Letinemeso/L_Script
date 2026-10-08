#include <Python_Script/Python_Script_Import_Manager.h>

#include <pybind11/embed.h>

using namespace LScript;


void Python_Script_Import_Manager::register_module(const std::string& _module_name)
{
    L_ASSERT(!m_registered_modules.contains(_module_name));
    m_registered_modules.push(_module_name);
}

void Python_Script_Import_Manager::import_all()
{
    for(unsigned int i = 0; i < m_registered_modules.size(); ++i)
        pybind11::module_::import(m_registered_modules[i].c_str());
}
