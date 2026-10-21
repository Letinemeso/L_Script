#include <L_Script/Script_Details/Operations/Call_Member_Function.h>

#include <L_Script/Script_Details/Function.h>

using namespace LScript;


Call_Member_Function::Call_Member_Function()
{

}

Call_Member_Function::~Call_Member_Function()
{
    delete m_owner_object_getter_operation;
    clear_arguments_getter_operations();
}



void Call_Member_Function::clear_arguments_getter_operations()
{
    for(unsigned int i = 0; i < m_arguments_getter_operations.size(); ++i)
        delete m_arguments_getter_operations[i];
    m_arguments_getter_operations.clear();
}

void Call_Member_Function::set_arguments_getter_operations(Arguments_Getter_Operations&& _operations)
{
    clear_arguments_getter_operations();
    m_arguments_getter_operations = (Arguments_Getter_Operations&&)_operations;
}



Variable* Call_Member_Function::process(Context* _context)
{
    L_ASSERT(m_owner_object_getter_operation);

    Variable* owner_object = m_owner_object_getter_operation->process(_context);

    L_DEBUG_FUNC_NOARG([&]()
    {
        if(owner_object && (!owner_object->bindable() || owner_object->data() != nullptr))
            return;

        L_LOG("LScript_Debug", "Error at line " + std::to_string(m_source_line_number) + ": ");

        if(!owner_object)
            L_LOG("LScript_Debug", "Unable to retrieve owner object while calling function \"" + m_function_name + "\"");
        else
            L_LOG("LScript_Debug", "Unable to call a function for a non-bound reference");

        L_LOG("LScript_Debug", m_source_line);
        L_LOG("LScript_Debug", std::string(m_source_line.size(), '^'));

        L_ASSERT(false);
    });

    unsigned int arguments_amount = m_arguments_getter_operations.size() + 1;

    Builtins_Controller::Argument_Types argument_types(arguments_amount);
    Function::Arguments arguments(arguments_amount);
    arguments.push(owner_object);
    argument_types.push(owner_object->type());
    for(unsigned int i = 0; i < m_arguments_getter_operations.size(); ++i)
    {
        Variable* argument = m_arguments_getter_operations[i]->process(_context);
        L_ASSERT(argument);

        arguments.push(argument);
        argument_types.push(argument->type());
    }

    Function* function = Builtins_Controller::instance().get_member_function(owner_object->type(), m_function_name, argument_types);

    L_DEBUG_FUNC_NOARG([&]()
    {
        if(function)
            return;

        L_LOG("LScript_Debug", "Error at line " + std::to_string(m_source_line_number) + ": ");
        L_LOG("LScript_Debug", "Calling unregistered function \"" + Builtins_Controller::instance().construct_function_name(m_function_name, argument_types, owner_object->type()) + "\"");
        L_LOG("LScript_Debug", m_source_line);
        L_LOG("LScript_Debug", std::string(m_source_line.size(), '^'));

        L_ASSERT(function);
    });

    Variable* result = function->call(_context, arguments);

    for(unsigned int i = 0; i < arguments.size(); ++i)
    {
        Variable* arg = arguments[i];
        if(arg->is_temporary())
            delete arg;
    }

    return result;
}
