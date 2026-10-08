#pragma once

#include <Variable_Base.h>
#include <Builder_Stub.h>

#include <L_Script/Script_Details/Context.h>
#include <L_Script/Script_Details/Function.h>


namespace LScript
{

    class Script : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LScript::Script, LV::Variable_Base)

    public:
        virtual void set_context_object(const std::string& _type_as_string, const std::string& _name, void* _ptr) = 0;

    public:
        virtual void run() = 0;

    };


    class Script_Stub : public LV::Builder_Stub
    {
    public:
        INIT_VARIABLE(LScript::Script_Stub, LV::Builder_Stub)

    public:
        INIT_NULL_BUILDER_STUB(Script)

    };

}
