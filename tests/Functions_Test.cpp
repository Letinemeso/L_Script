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



class Functions_Test_Helper
{
public:
    bool bool_var = false; // boulevard lol

public:
    void set_bool(const bool& _value) { bool_var = _value; }
    bool get_bool() { return bool_var; }
    bool& get_bool_ref() { return bool_var; }

};


SCRIPTABLE_CLASS_BEGIN(Functions_Test_Helper)

INIT_SCRIPTABLE_FUNCTION(void, set_bool, const bool&)
INIT_SCRIPTABLE_FUNCTION(bool, get_bool)
INIT_SCRIPTABLE_FUNCTION(bool&, get_bool_ref)

SCRIPTABLE_CLASS_END



INIT_TEST(Script_Test__Function_Call)
{
    LScript::L_Script* script = __construct_script(R"(

void func()
{
    helper.set_bool(true);
}

void main()
{
    func();
}

)");

    Functions_Test_Helper helper;
    script->set_context_object("Functions_Test_Helper", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(helper.get_bool(), true);
}

INIT_TEST(Script_Test__Function_Call_With_Args)
{
    LScript::L_Script* script = __construct_script(R"(

void func(int _int_arg, float _float_arg)
{
    if(_int_arg.less(5))
    {
        return;
    }
    if(_float_arg.less(5.0))
    {
        return;
    }

    helper.set_bool(true);
}

void main()
{
    func(6, 6.0);
}

)");

    Functions_Test_Helper helper;
    script->set_context_object("Functions_Test_Helper", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(helper.get_bool(), true);
}

INIT_TEST(Script_Test__Function_Call_With_Reference)
{
    LScript::L_Script* script = __construct_script(R"(

void func(int& _var)
{
    _var.add(123);
}

void main()
{
    int var;
    var.set(0);

    func(var);

    if(var.equals(123))
    {
        helper.set_bool(true);
    }
}

)");

    Functions_Test_Helper helper;
    script->set_context_object("Functions_Test_Helper", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(helper.get_bool(), true);
}

INIT_TEST(Script_Test__Function_Double_Call)
{
    LScript::L_Script* script = __construct_script(R"(

void func(int& _var)
{
    _var.add(1);
}

void main()
{
    int var;
    var.set(0);

    func(var);
    func(var);

    if(var.equals(2))
    {
        helper.set_bool(true);
    }
}

)");

    Functions_Test_Helper helper;
    script->set_context_object("Functions_Test_Helper", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(helper.get_bool(), true);
}

INIT_TEST(Script_Test__Two_Functions_Call)
{
    LScript::L_Script* script = __construct_script(R"(

void func_2(int& _var)
{
    _var.add(12);
}

void func(int& _var)
{
    func_2(_var);
}

void main()
{
    int var;
    var.set(0);

    func(var);

    if(var.equals(12))
    {
        helper.set_bool(true);
    }
}

)");

    Functions_Test_Helper helper;
    script->set_context_object("Functions_Test_Helper", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(helper.get_bool(), true);
}

INIT_TEST(Script_Test__Function_Recursive_Call)
{
    LScript::L_Script* script = __construct_script(R"(

void func(int& _var)
{
    if(_var.equals(10))
    {
        return;
    }

    _var.add(1);
    func(_var);
}

void main()
{
    int var;
    var.set(0);

    func(var);

    if(var.equals(10))
    {
        helper.set_bool(true);
    }
}

)");

    Functions_Test_Helper helper;
    script->set_context_object("Functions_Test_Helper", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(helper.get_bool(), true);
}
