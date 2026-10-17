#include <L_Script/Script_Details/Variables/Variable.h>

#include <L_Script/Debug/Variable_Instance_Counter.h>

using namespace LScript;


Variable::Variable()
{
#ifdef L_DEBUG
    Variable_Instance_Counter::instance().count_variable(this);

    // if(Variable_Instance_Counter::instance().get_variable_id(this) == 8)
    //     std::cout << "ass" << std::endl;

#endif
}

Variable::~Variable()
{
#ifdef L_DEBUG
    Variable_Instance_Counter::instance().remove_variable(this);
#endif
}
