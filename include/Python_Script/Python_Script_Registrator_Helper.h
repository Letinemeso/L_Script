#pragma once

#include <pybind11/embed.h>

#include <Python_Script/Python_Script_Import_Manager.h>


namespace LScript
{

    template <typename _Owner_Class>
    class Python_Function_Registrator_Helper
    {
    public:
        virtual ~Python_Function_Registrator_Helper() { }

    public:
        virtual void register_function(pybind11::class_<_Owner_Class>& _pybind_registrator) const = 0;
    };

    template <typename _Owner_Class, typename _Function_Type>
    class Python_Function_Registrator_Helper_Typed final : public Python_Function_Registrator_Helper<_Owner_Class>
    {
    private:
        _Function_Type m_function;
        std::string m_name;

    public:
        Python_Function_Registrator_Helper_Typed(_Function_Type _function, const std::string& _name)
            : m_function(_function), m_name(_name)
        { }

    public:
        void register_function(pybind11::class_<_Owner_Class>& _pybind_registrator) const override
        {
            _pybind_registrator.def(m_name.c_str(), m_function);
        }

    };


    template <typename _Registered_Type, typename _Registrator_Class>
    pybind11::module_ register_python_module(const _Registrator_Class& _registrator, const std::string& _registered_type_name)
    {
        pybind11::module_ module = pybind11::module_::create_extension_module( _registered_type_name.c_str(), nullptr, new pybind11::module_::module_def() );
        pybind11::class_<_Registered_Type> pybind_registrator(module, _registered_type_name.c_str());
        for(unsigned int i = 0; i < _registrator.registration_helpers().size(); ++i)
            _registrator.registration_helpers()[i]->register_function(pybind_registrator);
        return module;
    }

}
