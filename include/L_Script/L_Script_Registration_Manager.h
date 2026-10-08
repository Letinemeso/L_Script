#pragma once

#include <L_Script/L_Script_Integration.h>


namespace LScript
{

    class L_Script_Registration_Manager
    {
    private:
        class L_Script_Registration_Helper
        {
        public:
            virtual ~L_Script_Registration_Helper() { }

        public:
            virtual void register_function() = 0;
        };

        template <typename _Owner_Class, typename _Return_Type, typename _Member_Function_Type>
        class L_Script_Registration_Helper_Typed final : public L_Script_Registration_Helper
        {
        private:
            std::string m_owner_class_name;
            std::string m_return_type_name;
            std::string m_member_function_name;
            LDS::Vector<std::string> m_arg_types;

            _Member_Function_Type m_function_ptr;

        public:
            L_Script_Registration_Helper_Typed(const std::string& _owner_class_name, const std::string& _return_type, const std::string& _member_function_name,
                                               LDS::Vector<std::string>&& _arg_types, _Member_Function_Type _function_ptr)
                : m_owner_class_name(_owner_class_name), m_return_type_name(_return_type), m_member_function_name(_member_function_name)
                , m_arg_types(LST::move(_arg_types)), m_function_ptr(_function_ptr)
            { }

        public:
            void register_function() override
            {
                LScript::register_l_script_function<_Owner_Class, _Return_Type, _Member_Function_Type>(m_owner_class_name, m_member_function_name, m_return_type_name, m_arg_types, m_function_ptr);
            }

        };

    private:
        using Helpers_Vec = LDS::Vector<L_Script_Registration_Helper*>;
        Helpers_Vec m_helpers;

    private:
        L_Script_Registration_Manager();

        L_Script_Registration_Manager(const L_Script_Registration_Manager&) = delete;
        L_Script_Registration_Manager(L_Script_Registration_Manager&&) = delete;
        void operator=(const L_Script_Registration_Manager&) = delete;
        void operator=(L_Script_Registration_Manager&&) = delete;

    public:
        ~L_Script_Registration_Manager();

    public:
        inline static L_Script_Registration_Manager& instance() { static L_Script_Registration_Manager s_instance; return s_instance; }

    public:
        template <typename _Owner_Class, typename _Return_Type, typename _Member_Function_Type>
        void queue_registration(const std::string& _owner_class_name, const std::string& _return_type, const std::string& _member_function_name,
                                LDS::Vector<std::string>&& _arg_types, _Member_Function_Type _function_ptr);

        void register_all();

    };


    template <typename _Owner_Class, typename _Return_Type, typename _Member_Function_Type>
    void L_Script_Registration_Manager::queue_registration(const std::string& _owner_class_name, const std::string& _return_type, const std::string& _member_function_name,
                                                           LDS::Vector<std::string>&& _arg_types, _Member_Function_Type _function_ptr)
    {
        m_helpers.push( new L_Script_Registration_Helper_Typed<_Owner_Class, _Return_Type, _Member_Function_Type>(_owner_class_name, _return_type, _member_function_name, LST::move(_arg_types), _function_ptr) );
    }

}
