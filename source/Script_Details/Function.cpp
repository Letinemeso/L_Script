#include <Script_Details/Function.h>

using namespace LScript;


Function::Function()
{

}

Function::~Function()
{

}



void Function::call(const LDS::Vector<Variable*>& _args)
{
    L_ASSERT(_args.size() == m_expected_arguments_data.size());

    for(unsigned int i = 0; i < _args.size(); ++i)
    {
        L_ASSERT(_args[i]->type() == m_expected_arguments_data[i].expected_type);

        Variable* parameter = _args[i];

        Variable* variable = new Variable;
        variable->set_type(_args[i]->type());
        variable->shallow_copy(parameter);

        m_context.add_variable(m_expected_arguments_data[i].name, variable);
    }

    m_context.clear();
}
