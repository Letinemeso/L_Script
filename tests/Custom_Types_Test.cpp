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



class Ref_Test_Helper
{
public:
    int var_int = 0;

public:
    void set_int(int _value) { var_int = _value; }

};


SCRIPTABLE_CLASS_BEGIN(Ref_Test_Helper)

INIT_SCRIPTABLE_FUNCTION(void, set_int, int)

SCRIPTABLE_CLASS_END



class Custom_Types_Test_Helper
{
public:
    Ref_Test_Helper ref_test_helpers[3];

public:
    int var_int;

public:
    void set_int(const int& _value) { var_int = _value; }
    int get_int() { return var_int; }
    int& get_int_ref() { return var_int; }

    Custom_Types_Test_Helper& get_self() { return *this; }

    Ref_Test_Helper& get_ref_test_helper(unsigned int _index) { return ref_test_helpers[_index]; }

};


SCRIPTABLE_CLASS_BEGIN(Custom_Types_Test_Helper)

INIT_SCRIPTABLE_FUNCTION(void, set_int, const int&)
INIT_SCRIPTABLE_FUNCTION(int, get_int)
INIT_SCRIPTABLE_FUNCTION(int&, get_int_ref)

INIT_SCRIPTABLE_FUNCTION(Custom_Types_Test_Helper&, get_self)

INIT_SCRIPTABLE_FUNCTION(Ref_Test_Helper&, get_ref_test_helper, unsigned int)

SCRIPTABLE_CLASS_END



INIT_TEST(Script_Test__Custom_Type_Ref)
{
    LScript::L_Script* script = __construct_script(R"(

void main()
{
    Custom_Types_Test_Helper& ref;
    ref_bind(ref, helper.get_self());
    ref.set_int(64);
}

)");

    Custom_Types_Test_Helper helper;
    script->set_context_object("Custom_Types_Test_Helper", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(helper.get_int(), 64);
}

INIT_TEST(Script_Test__Custom_Type_Ref_To_Other_Class)
{
    LScript::L_Script* script = __construct_script(R"(

void main()
{
    Custom_Types_Test_Helper& ref;
    ref_bind(ref, helper.get_self());
    ref.set_int(643);

    Ref_Test_Helper& ref_test_helper;

    uint counter;
    counter.set(0);
    while(counter.less(3))
    {
        ref_bind(ref_test_helper, ref.get_ref_test_helper(counter));
        ref_test_helper.set_int(counter.to_int());
        counter.add(1);
    }
}

)");

    Custom_Types_Test_Helper helper;
    script->set_context_object("Custom_Types_Test_Helper", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(helper.get_int(), 643);
    EXPECT_EQUAL(helper.get_ref_test_helper(0).var_int, 0);
    EXPECT_EQUAL(helper.get_ref_test_helper(1).var_int, 1);
    EXPECT_EQUAL(helper.get_ref_test_helper(2).var_int, 2);
}
