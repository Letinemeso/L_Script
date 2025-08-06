#pragma once

#include <Variable_Base.h>
#include <Builder_Stub.h>


namespace LScript
{

    class Script : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LScript::Script, LV::Variable_Base)

    private:


    };


    class Script_Stub : public LV::Builder_Stub
    {
    public:
        INIT_VARIABLE(LScript::Script_Stub, LV::Builder_Stub)

    public:
        INIT_BUILDER_STUB(Script)

    };

}
