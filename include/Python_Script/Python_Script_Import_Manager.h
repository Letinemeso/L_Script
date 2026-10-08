#pragma once

#include <Data_Structures/Map.h>
#include <Stuff/Function_Wrapper.h>

#include <pybind11/embed.h>


namespace LScript
{

    class Python_Script_Import_Manager
    {
    public:
        struct Module_Data
        {
            using Context_Object_Registration_Func = LST::Function<void(const std::string&, void*, pybind11::dict&)>;

            Context_Object_Registration_Func context_object_registration;
        };

    private:
        using Registered_Modules_Map = LDS::Map<std::string, Module_Data>;
        Registered_Modules_Map m_registered_modules;

    private:
        Python_Script_Import_Manager() { }

        Python_Script_Import_Manager(const Python_Script_Import_Manager&) = delete;
        Python_Script_Import_Manager(Python_Script_Import_Manager&&) = delete;
        void operator=(const Python_Script_Import_Manager&) = delete;
        void operator=(Python_Script_Import_Manager&&) = delete;

    public:
        inline static Python_Script_Import_Manager& instance() { static Python_Script_Import_Manager s_instance; return s_instance; }

    public:
        template <typename _Type>
        void register_module(const std::string& _module_name);

        void import_all();
        void register_context_object(const std::string& _context_variable_type, const std::string& _context_variable_name, void* _typeless_object, pybind11::dict& _script_context);

    };


    template <typename _Type>
    void Python_Script_Import_Manager::register_module(const std::string& _module_name)
    {
        L_ASSERT(!m_registered_modules.contains(_module_name));

        Module_Data data;
        data.context_object_registration = [](const std::string& _context_variable_name, void* _typeless_object, pybind11::dict& _script_context)
        {
            _Type* object = LST::raw_cast<_Type>(_typeless_object);
            _script_context[_context_variable_name.c_str()] = pybind11::cast( object, pybind11::return_value_policy::reference );
        };

        m_registered_modules.insert(_module_name, data);
    }

}
