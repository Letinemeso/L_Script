#pragma once

#include <Data_Structures/Vector.h>


namespace LScript
{

    class Python_Script_Import_Manager
    {
    private:
        using Registered_Modules_Vec = LDS::Vector<std::string>;
        Registered_Modules_Vec m_registered_modules;

    private:
        Python_Script_Import_Manager() { }

        Python_Script_Import_Manager(const Python_Script_Import_Manager&) = delete;
        Python_Script_Import_Manager(Python_Script_Import_Manager&&) = delete;
        void operator=(const Python_Script_Import_Manager&) = delete;
        void operator=(Python_Script_Import_Manager&&) = delete;

    public:
        inline static Python_Script_Import_Manager& instance() { static Python_Script_Import_Manager s_instance; return s_instance; }

    public:
        void register_module(const std::string& _module_name);
        void import_all();

    };

}
