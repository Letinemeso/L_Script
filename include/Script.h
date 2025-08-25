#pragma once

#include <Variable_Base.h>
#include <Builder_Stub.h>

#include <Script_Details/Context.h>
#include <Script_Details/Function.h>


namespace LScript
{

    class Script : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LScript::Script, LV::Variable_Base)

    private:
        Context m_global_context;

        using Functions_Map = LDS::Map<std::string, Function*>;
        Functions_Map m_functions;

    public:
        Script();
        ~Script();

    public:
        inline Context& global_context() { return m_global_context; }
        inline const Context& global_context() const { return m_global_context; }

    public:
        void set_context_object(const std::string& _type_as_string, const std::string& _name, void* _ptr);

        void register_function(const std::string& _name, Function* _function);
        void clear_functions();

        Function* get_function(const std::string& _name) const;

    public:
        void run();

    };


    class Script_Stub : public LV::Builder_Stub
    {
    public:
        INIT_VARIABLE(LScript::Script_Stub, LV::Builder_Stub)

        INIT_FIELDS
        ADD_FIELD(std::string, source_code)
        FIELDS_END

    public:
        std::string source_code;

    public:
        INIT_BUILDER_STUB(Script)

    };

}
