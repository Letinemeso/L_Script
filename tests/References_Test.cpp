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



class Reference_Test_Helper
{
public:
    int var_int;

public:
    void set_int(const int& _value) { var_int = _value; }
    int get_int() { return var_int; }
    int& get_int_ref() { return var_int; }

};


SCRIPTABLE_CLASS_BEGIN(Reference_Test_Helper)

INIT_SCRIPTABLE_FUNCTION(void, set_int, const int&)
INIT_SCRIPTABLE_FUNCTION(int, get_int)
INIT_SCRIPTABLE_FUNCTION(int&, get_int_ref)

SCRIPTABLE_CLASS_END



INIT_TEST(Script_Test__Reference_Usage)
{
    LScript::L_Script* script = __construct_script(R"(

void main()
{
    int& ref;
    ref_bind(ref, helper.get_int_ref());

    ref.set(123);
}

)");

    Reference_Test_Helper helper;
    script->set_context_object("Reference_Test_Helper", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(helper.get_int(), 123);
}

INIT_TEST(Script_Test__Reference_For_Reference)
{
    LScript::L_Script* script = __construct_script(R"(

void main()
{
    int& ref;
    ref_bind(ref, helper.get_int_ref());

    int& ref_ref;
    ref_bind(ref_ref, ref);

    ref_ref.set(123);
}

)");

    Reference_Test_Helper helper;
    script->set_context_object("Reference_Test_Helper", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(helper.get_int(), 123);
}

INIT_TEST(Script_Test__Reference_For_Reference_In_Function)
{
    LScript::L_Script* script = __construct_script(R"(

void test_ref(int& _ref)
{
    int& ref_ref;
    ref_bind(ref_ref, _ref);

    ref_ref.set(123);
}

void main()
{
    int& ref;
    ref_bind(ref, helper.get_int_ref());

    test_ref(ref);
}

)");

    Reference_Test_Helper helper;
    script->set_context_object("Reference_Test_Helper", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(helper.get_int(), 123);
}
