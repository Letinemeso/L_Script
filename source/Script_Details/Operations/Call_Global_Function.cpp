#include <Script_Details/Operations/Call_Global_Function.h>

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
    L_ASSERT(m_script);

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

    Function* function = m_script->get_function(m_function_name);
    if(!function)
        function = Integrated_Functions::instance().get_global_function(m_function_name, argument_types);

    L_DEBUG_FUNC_NOARG([&]()
    {
        if(function)
            return;

        L_LOG("LScript_Debug", "Error at line " + std::to_string(m_source_line_number) + ": ");
        L_LOG("LScript_Debug", "Calling unregistered function \"" + Integrated_Functions::instance().construct_function_name(m_function_name, argument_types) + "\"");
        L_LOG("LScript_Debug", m_source_line);
        L_LOG("LScript_Debug", std::string(m_source_line.size(), '^'));
        L_ASSERT(function);
    });

    return function->call(arguments);
}
