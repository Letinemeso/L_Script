#include <Testing/Testing.h>

#include <L_Script/L_Script.h>
#include <L_Script/Debug/Variable_Instance_Counter.h>
#include <Scriptable_Functions_Registration.h>


LScript::L_Script* __construct_script(const std::string& _source)
{
    LScript::L_Script_Stub stub;
    stub.source_code = _source;

    return LScript::L_Script_Stub::construct_from(&stub);
}



class Basic_Types_Test_Helper
{
public:
    int var_int;
    unsigned int var_uint;
    float var_float;
    std::string var_string;
    bool var_bool;

public:
    const std::string& test_func(const int& _ass, const std::string& _bass, unsigned int& _karass, const float _brass) { return var_string; }

    void set_int(const int& _value) { var_int = _value; }
    int get_int() { return var_int; }
    int& get_int_ref() { return var_int; }

    void set_uint(const unsigned int& _value) { var_uint = _value; }
    unsigned int get_uint() { return var_uint; }
    unsigned int& get_uint_ref() { return var_uint; }

    void set_float(const float& _value) { var_float = _value; }
    float get_float() { return var_float; }
    float& get_float_ref() { return var_float; }

    void set_string(const std::string& _value) { var_string = _value; }
    std::string get_string() { return var_string; }
    std::string& get_string_ref() { return var_string; }

    void set_bool(const bool& _value) { var_bool = _value; }
    bool get_bool() { return var_bool; }
    bool& get_bool_ref() { return var_bool; }

};


SCRIPTABLE_CLASS_BEGIN(Basic_Types_Test_Helper)

INIT_SCRIPTABLE_FUNCTION(void, set_int, const int&)
INIT_SCRIPTABLE_FUNCTION(int, get_int)
INIT_SCRIPTABLE_FUNCTION(int&, get_int_ref)

INIT_SCRIPTABLE_FUNCTION(void, set_uint, const unsigned int&)
INIT_SCRIPTABLE_FUNCTION(unsigned int, get_uint)
INIT_SCRIPTABLE_FUNCTION(unsigned int&, get_uint_ref)

INIT_SCRIPTABLE_FUNCTION(void, set_float, const float&)
INIT_SCRIPTABLE_FUNCTION(float, get_float)
INIT_SCRIPTABLE_FUNCTION(float&, get_float_ref)

INIT_SCRIPTABLE_FUNCTION(void, set_string, const std::string&)
INIT_SCRIPTABLE_FUNCTION(std::string, get_string)
INIT_SCRIPTABLE_FUNCTION(std::string&, get_string_ref)

INIT_SCRIPTABLE_FUNCTION(void, set_bool, const bool&)
INIT_SCRIPTABLE_FUNCTION(bool, get_bool)
INIT_SCRIPTABLE_FUNCTION(bool&, get_bool_ref)

SCRIPTABLE_CLASS_END




INIT_TEST(Script_Test__Empty)
{
    LScript::L_Script* script = __construct_script(R"(

void main()
{

}

)");

    script->run();
    delete script;
}

INIT_TEST(Script_Test__Variables_Creation)
{
    LScript::L_Script* script = __construct_script(R"(

void main()
{
    int var_int;
    uint var_uint;
    float var_float;
    string var_string;
    bool var_bool;

    Int_Vector var_int_vec;
    Uint_Vector var_uint_vec;
    Bool_Vector var_bool_vec;
    String_Vector var_string_vec;
    Float_Vector var_float_vec;
}

)");

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    script->run();
    delete script;
}

INIT_TEST(Script_Test__Int_Operations)
{
    LScript::L_Script* script = __construct_script(R"(

void main()
{
    int var;
    var.set(10);
    var.add(5);
    var.subtract(3);
    var.multiply(50);
    var.divide(10);
    var.mod(7);

    helper.set_int(var);
}

)");

    Basic_Types_Test_Helper helper;
    script->set_context_object("Basic_Types_Test_Helper", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(helper.get_int(), 4);
}
