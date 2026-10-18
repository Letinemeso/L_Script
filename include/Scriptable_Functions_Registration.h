#pragma once

#include <L_Script/L_Script_Registration_Manager.h>
#include <Python_Script/Python_Script_Registrator_Helper.h>

//      -------------- USAGE EXAMPLE: --------------

/*
SCRIPTABLE_CLASS_BEGIN(Test)     // somewhere in cpp file

SCRIPTABLE_FUNCTION_INITIALIZATION_BEGIN;
SCRIPTABLE_FUNCTION_RETURN_TYPE(void);
SCRIPTABLE_FUNCTION_NAME(set_string);
SCRIPTABLE_FUNCTION_ARG(std::string);
SCRIPTABLE_FUNCTION_INITIALIZATION_END;

SCRIPTABLE_FUNCTION_INITIALIZATION_BEGIN;
SCRIPTABLE_FUNCTION_RETURN_TYPE(void);
SCRIPTABLE_FUNCTION_NAME(set_number);
SCRIPTABLE_FUNCTION_ARG(unsigned int);
SCRIPTABLE_FUNCTION_INITIALIZATION_END;

SCRIPTABLE_FUNCTION_INITIALIZATION_BEGIN;
SCRIPTABLE_FUNCTION_RETURN_TYPE(void);
SCRIPTABLE_FUNCTION_NAME(print_data);
SCRIPTABLE_FUNCTION_INITIALIZATION_END;

SCRIPTABLE_CLASS_END;
*/

//      --------------------------------------------


#define INIT_SCRIPTABLE_FUNCTION(_RETURN_TYPE, _FUNCTION_NAME, ...) \
    { \
        std::string return_type_str = #_RETURN_TYPE; \
        auto member_function = &Registered_Type::_FUNCTION_NAME; \
        using Member_Function_Type = decltype(member_function); \
        std::string member_function_name = #_FUNCTION_NAME; \
        m_registration_helpers.push( new LScript::Python_Function_Registrator_Helper_Typed<Registered_Type, Member_Function_Type>(member_function, member_function_name) ); \
        LDS::Vector<std::string> arguments_types = LScript::__construct_args_string_vec(#__VA_ARGS__); \
        LScript::L_Script_Registration_Manager::instance().queue_registration<Registered_Type, Member_Function_Type>(Registered_Type_Str, return_type_str, member_function_name, LST::move(arguments_types), member_function); \
    }


#define SCRIPTABLE_CLASS_BEGIN( CLASS_NAME ) \
    namespace __##CLASS_NAME##_Registration \
    { \
        using Registered_Type = CLASS_NAME; \
        constexpr const char* Registered_Type_Str = #CLASS_NAME; \
         \
        class __Registrator \
        { \
        public: \
            using Registration_Helpers_Vec = LDS::Vector<LScript::Python_Function_Registrator_Helper<Registered_Type>*>; \
         \
        private: \
            Registration_Helpers_Vec m_registration_helpers; \
         \
        public: \
            ~__Registrator() \
            { \
            for(unsigned int i = 0; i < m_registration_helpers.size(); ++i) \
                delete m_registration_helpers[i]; \
            } \
         \
            inline const Registration_Helpers_Vec& registration_helpers() const { return m_registration_helpers; } \
         \
            __Registrator() \
            { \
                static bool __scriptable_functions_initialized = false; \
                L_ASSERT(!__scriptable_functions_initialized); \
                __scriptable_functions_initialized = true;

#define SCRIPTABLE_CLASS_END \
            } \
        }; \
         \
        __Registrator registrator_instance; \
         \
        class __Pybind_Registrator \
        { \
            public: \
            __Pybind_Registrator() \
            { \
                LScript::Python_Script_Import_Manager::instance().register_module<Registered_Type>(Registered_Type_Str); \
                 \
                PyImport_AppendInittab(Registered_Type_Str, []()->PyObject* \
                { \
                    pybind11::module_ module = LScript::register_python_module<Registered_Type>( registrator_instance, Registered_Type_Str ); \
                    return module.ptr(); \
                }); \
            } \
        }; \
         \
        __Pybind_Registrator pybind_registrator_instance; \
    }
