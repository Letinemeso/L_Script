#include <Testing/Testing.h>

#include <L_Script/L_Script_Registration_Manager.h>

INIT_TEST_INITILIZATOR(LScript_Registration)

LScript::L_Script_Registration_Manager::instance().register_all();

TEST_INITILIZATOR_END

