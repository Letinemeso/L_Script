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
    bool get_boolt() { return var_bool; }
    bool& get_bool_ref() { return var_bool; }

};


SCRIPTABLE_CLASS_BEGIN(Basic_Types_Test_Helper)

INIT_SCRIPTABLE_FUNCTION(const std::string&, test_func, const int&, const std::string&, unsigned int&, const float)

INIT_SCRIPTABLE_FUNCTION(void, set_int, int)

// SCRIPTABLE_FUNCTION_INITIALIZATION_BEGIN
// SCRIPTABLE_FUNCTION_RETURN_TYPE(void)
// SCRIPTABLE_FUNCTION_NAME(set_int)
// SCRIPTABLE_FUNCTION_ARG(int)
// SCRIPTABLE_FUNCTION_INITIALIZATION_END

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
