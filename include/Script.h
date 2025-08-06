#pragma once

#include <Variable_Base.h>
#include <Builder_Stub.h>

#include <Script_Details/Context.h>


namespace LScript
{

    class Script : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LScript::Script, LV::Variable_Base)

    private:
        Context m_global_context;

    public:
        inline Context& global_context() { return m_global_context; }
        inline const Context& global_context() const { return m_global_context; }

    public:
        // void run();

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
