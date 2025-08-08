#include <Script_Details/Operations/Call_Global_Function.h>

#include <Integrated_Functions.h>

using namespace LScript;


Call_Global_Function::Call_Global_Function()
{

}

Call_Global_Function::~Call_Global_Function()
{
    clear_arguments_getter_operations();
}



void Call_Global_Function::clear_arguments_getter_operations()
{
    for(unsigned int i = 0; i < m_arguments_getter_operations.size(); ++i)
        delete m_arguments_getter_operations[i];
    m_arguments_getter_operations.clear();
}

void Call_Global_Function::set_arguments_getter_operations(Arguments_Getter_Operations&& _operations)
{
    clear_arguments_getter_operations();
    m_arguments_getter_operations = (Arguments_Getter_Operations&&)_operations;
}



Variable* Call_Global_Function::process()
{
    unsigned int arguments_amount = m_arguments_getter_operations.size();

    Integrated_Functions::Argument_Types argument_types(arguments_amount);
    Function::Arguments arguments(arguments_amount);
    for(unsigned int i = 0; i < m_arguments_getter_operations.size(); ++i)
    {
        Variable* argument = m_arguments_getter_operations[i]->process();
        L_ASSERT(argument);

        arguments.push(argument);
        argument_types.push(argument->type());
    }

    std::cout << "Trying to call global function: " << m_function_name << std::endl << std::endl;

    Function* function = Integrated_Functions::instance().get_global_function(m_function_name, argument_types);
    L_ASSERT(function);

    return function->call(arguments);
}
