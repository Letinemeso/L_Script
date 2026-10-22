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



class Geometry_Types_Test_Helper
{
public:
    glm::vec2 var;

public:
    void set(const glm::vec2& _value) { var = _value; }
    glm::vec2 get() { return var; }

};


SCRIPTABLE_CLASS_BEGIN(Geometry_Types_Test_Helper)

INIT_SCRIPTABLE_FUNCTION(void, set, const glm::vec2&)
INIT_SCRIPTABLE_FUNCTION(glm::vec2, get)

SCRIPTABLE_CLASS_END



INIT_TEST(Script_Test__Vec2_Operations)
{
    LScript::L_Script* script = __construct_script(R"(

void main()
{
    vec2 var;

    var.set(123.0, 321.0);

    var.x().add(7.0);
    var.y().subtract(21.0);

    var.add(1.0, 2.0);
    var.subtract(11.0, 32.0);

    var.divide(3.0);
    var.multiply(0.5);

    vec2 var2;
    var2.set(20.0, 45.0);
    var.add(var2);

    helper.set(var);
}

)");

    Geometry_Types_Test_Helper helper;
    script->set_context_object("Geometry_Types_Test_Helper", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(LST::Math::vecs_are_equal( helper.get(), {40, 90} ), true);
}
