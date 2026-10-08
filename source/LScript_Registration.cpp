#include <LScript_Registration.h>

#include <L_Script/L_Script_Registration_Manager.h>
#include <L_Script/L_Script.h>
#include <Python_Script/Python_Script_Engine.h>
#include <Python_Script/Python_Script_Import_Manager.h>
#include <Python_Script/Python_Script.h>

using namespace LScript;


void LScript::register_types(LV::Object_Constructor& _object_constructor)
{
    L_Script_Registration_Manager::instance().register_all();

    Python_Script_Engine::instance();  //  init
    Python_Script_Import_Manager::instance().import_all();

    _object_constructor.register_type<LScript::L_Script_Stub>();
    _object_constructor.register_type<LScript::Python_Script_Stub>();
}
