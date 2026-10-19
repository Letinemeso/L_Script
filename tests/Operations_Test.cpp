#include <Testing/Testing.h>

#include <Stuff/Math_Stuff.h>

#include <L_Script/L_Script.h>
#include <L_Script/Debug/Variable_Instance_Counter.h>
#include <Scriptable_Functions_Registration.h>


namespace
{
    LScript::L_Script* __construct_script(const std::string& _source)
    {
        LScript::L_Script_Stub stub;
        stub.source_code = _source;

        return LScript::L_Script_Stub::construct_from(&stub);
    }
}



class Operations_Test_Helper
{
public:
    bool bool_var = false; // boulevard lol

public:
    void set_bool(const bool& _value) { bool_var = _value; }
    bool get_bool() { return bool_var; }
    bool& get_bool_ref() { return bool_var; }

};


SCRIPTABLE_CLASS_BEGIN(Operations_Test_Helper)

INIT_SCRIPTABLE_FUNCTION(void, set_bool, const bool&)
INIT_SCRIPTABLE_FUNCTION(bool, get_bool)
INIT_SCRIPTABLE_FUNCTION(bool&, get_bool_ref)

SCRIPTABLE_CLASS_END



INIT_TEST(Script_Test__Bool_Creation)
{
    LScript::L_Script* script = __construct_script(R"(

void main()
{
    bool var;
    var.set(true);

    helper.set_bool(var);
}

)");

    Operations_Test_Helper helper;
    script->set_context_object("Operations_Test_Helper", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(helper.get_bool(), true);
}

INIT_TEST(Script_Test__If_True)
{
    LScript::L_Script* script = __construct_script(R"(

void main()
{
    bool var;
    var.set(true);

    if(var.equals(true))
    {
        helper.set_bool(true);
    }
}

)");

    Operations_Test_Helper helper;
    script->set_context_object("Operations_Test_Helper", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(helper.get_bool(), true);
}

INIT_TEST(Script_Test__If_False)
{
    LScript::L_Script* script = __construct_script(R"(

void main()
{
    bool var;
    var.set(false);

    if(var.equals(true))
    {
        helper.set_bool(true);
    }
}

)");

    Operations_Test_Helper helper;
    script->set_context_object("Operations_Test_Helper", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(helper.get_bool(), false);
}

INIT_TEST(Script_Test__While)
{
    LScript::L_Script* script = __construct_script(R"(

void main()
{
    uint counter;
    counter.set(0);

    while(counter.less(10))
    {
        counter.add(1);
    }

    if(counter.equals(10))
    {
        helper.set_bool(true);
    }
}

)");

    Operations_Test_Helper helper;
    script->set_context_object("Operations_Test_Helper", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(helper.get_bool(), true);
}
