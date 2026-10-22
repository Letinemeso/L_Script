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



class Geometry_Types_Test_Helper_Vec3
{
public:
    glm::vec3 var;

public:
    void set(const glm::vec3& _value) { var = _value; }
    glm::vec3 get() { return var; }

};


SCRIPTABLE_CLASS_BEGIN(Geometry_Types_Test_Helper_Vec3)

INIT_SCRIPTABLE_FUNCTION(void, set, const glm::vec3&)
INIT_SCRIPTABLE_FUNCTION(glm::vec3, get)

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

INIT_TEST(Script_Test__Vec2_Copy_Operations)
{
    LScript::L_Script* script = __construct_script(R"(

void main()
{
    vec2 var;
    var.set(10.0, 10.0);

    helper.set(var);

    helper_plus.set(var.plus(2.0, 2.0));
    helper_minus.set(var.minus(2.0, 2.0));
    helper_multiplied_by.set(var.multiplied_by(2.0, 2.0));
    helper_divided_by.set(var.divided_by(2.0, 2.0));
}

)");

    Geometry_Types_Test_Helper helper;
    script->set_context_object("Geometry_Types_Test_Helper", "helper", &helper);
    Geometry_Types_Test_Helper helper_plus;
    script->set_context_object("Geometry_Types_Test_Helper", "helper_plus", &helper_plus);
    Geometry_Types_Test_Helper helper_minus;
    script->set_context_object("Geometry_Types_Test_Helper", "helper_minus", &helper_minus);
    Geometry_Types_Test_Helper helper_multiplied_by;
    script->set_context_object("Geometry_Types_Test_Helper", "helper_multiplied_by", &helper_multiplied_by);
    Geometry_Types_Test_Helper helper_divided_by;
    script->set_context_object("Geometry_Types_Test_Helper", "helper_divided_by", &helper_divided_by);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(LST::Math::vecs_are_equal( helper.get(), {10, 10} ), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal( helper_plus.get(), {12, 12} ), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal( helper_minus.get(), {8, 8} ), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal( helper_multiplied_by.get(), {20, 20} ), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal( helper_divided_by.get(), {5, 5} ), true);
}

INIT_TEST(Script_Test__Vec3_Operations)
{
    LScript::L_Script* script = __construct_script(R"(

void main()
{
    vec3 var;

    var.set(123.0, 321.0, 623.0);

    var.x().add(7.0);
    var.y().subtract(21.0);
    var.z().subtract(3.0);

    var.add(1.0, 2.0, -7.0);
    var.subtract(11.0, 32.0, 13.0);

    var.divide(3.0);
    var.multiply(0.5);

    vec3 var2;
    var2.set(20.0, 45.0, 100.0);
    var.add(var2);

    helper.set(var);
}

)");

    Geometry_Types_Test_Helper_Vec3 helper;
    script->set_context_object("Geometry_Types_Test_Helper_Vec3", "helper", &helper);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(LST::Math::vecs_are_equal( helper.get(), {40, 90, 200} ), true);
}

INIT_TEST(Script_Test__Vec3_Copy_Operations)
{
    LScript::L_Script* script = __construct_script(R"(

void main()
{
    vec3 var;
    var.set(10.0, 10.0, 10.0);

    helper.set(var);

    helper_plus.set(var.plus(2.0, 2.0, 2.0));
    helper_minus.set(var.minus(2.0, 2.0, 2.0));
    helper_multiplied_by.set(var.multiplied_by(2.0, 2.0, 2.0));
    helper_divided_by.set(var.divided_by(2.0, 2.0, 2.0));
}

)");

    Geometry_Types_Test_Helper_Vec3 helper;
    script->set_context_object("Geometry_Types_Test_Helper_Vec3", "helper", &helper);
    Geometry_Types_Test_Helper_Vec3 helper_plus;
    script->set_context_object("Geometry_Types_Test_Helper_Vec3", "helper_plus", &helper_plus);
    Geometry_Types_Test_Helper_Vec3 helper_minus;
    script->set_context_object("Geometry_Types_Test_Helper_Vec3", "helper_minus", &helper_minus);
    Geometry_Types_Test_Helper_Vec3 helper_multiplied_by;
    script->set_context_object("Geometry_Types_Test_Helper_Vec3", "helper_multiplied_by", &helper_multiplied_by);
    Geometry_Types_Test_Helper_Vec3 helper_divided_by;
    script->set_context_object("Geometry_Types_Test_Helper_Vec3", "helper_divided_by", &helper_divided_by);

    script->run();
    delete script;

    EXPECT_EQUAL(LScript::Variable_Instance_Counter::instance().counter(), 0);

    EXPECT_EQUAL(LST::Math::vecs_are_equal( helper.get(), {10, 10, 10} ), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal( helper_plus.get(), {12, 12, 12} ), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal( helper_minus.get(), {8, 8, 8} ), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal( helper_multiplied_by.get(), {20, 20, 20} ), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal( helper_divided_by.get(), {5, 5, 5} ), true);
}
