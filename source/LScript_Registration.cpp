#include <LScript_Registration.h>

#include <Script.h>

using namespace LScript;


void LScript::register_types(LV::Object_Constructor& _object_constructor)
{
    _object_constructor.register_type<LScript::Script>();
}
