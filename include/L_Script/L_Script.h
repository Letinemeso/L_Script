#pragma once

#include <Script.h>
#include <L_Script/Script_Details/Context.h>
#include <L_Script/Script_Details/Function.h>


namespace LScript
{

    class L_Script : public Script
    {
    public:
        INIT_VARIABLE(LScript::L_Script, LScript::Script)

    private:
        Context m_global_context;

        using Functions_Map = LDS::Map<std::string, Function*>;
        Functions_Map m_functions;

    public:
        L_Script();
        ~L_Script();

    public:
        inline Context& global_context() { return m_global_context; }
        inline const Context& global_context() const { return m_global_context; }

    public:
        void set_context_object(const std::string& _type_as_string, const std::string& _name, void* _ptr) override;

        void register_function(const std::string& _name, Function* _function);
        void clear_functions();

        Function* get_function(const std::string& _name) const;

    public:
        void run() override;

    };


    class L_Script_Stub : public Script_Stub
    {
    public:
        INIT_VARIABLE(LScript::L_Script_Stub, LScript::Script_Stub)

        INIT_FIELDS
        ADD_FIELD(LDS::Vector<std::string>, source_code)
        FIELDS_END

        OVERRIDE_ON_VALUES_ASSIGNED

    public:
        LDS::Vector<std::string> source_code;

    public:
        std::string source_code_str;

    public:
        void update_source_code_str();

    public:
        INIT_BUILDER_STUB(L_Script)

    };

}
