#include <Script_Details/Function.h>

using namespace LScript;


Function::Function()
{

}

Function::~Function()
{

}



Variable* Function::call(const Arguments& _args)
{
    L_ASSERT(_args.size() == m_expected_arguments_data.size());

    for(unsigned int i = 0; i < _args.size(); ++i)
    {
        L_ASSERT(_args[i]->type() == m_expected_arguments_data[i].expected_type);

        Variable* parameter = _args[i];

        Variable* variable = new Variable;
        variable->set_type(_args[i]->type());
        variable->shallow_copy(parameter);

        m_compound_statement.context().add_variable(m_expected_arguments_data[i].name, variable);
    }

    Variable* result = m_compound_statement.process();
    L_ASSERT( (!result && m_return_type == "void") || (result && (result->type() == m_return_type)) );

    m_compound_statement.context().clear();

    return result;
}
