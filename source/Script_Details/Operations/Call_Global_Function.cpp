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
    L_ASSERT(m_function);
    L_ASSERT(m_function->expected_arguments_data().size() == m_arguments_getter_operations.size());

    unsigned int arguments_amount = m_arguments_getter_operations.size();

    Function::Arguments arguments(arguments_amount, nullptr);
    for(unsigned int i = 0; i < arguments_amount; ++i)
    {
        Variable* argument = m_arguments_getter_operations[i]->process();
        L_ASSERT(argument);
        L_ASSERT(argument->type() == m_function->expected_arguments_data()[i].expected_type);

        arguments[i] = argument;
    }

    return m_function->call(arguments);
}
