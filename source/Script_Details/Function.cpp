#include <Script_Details/Function.h>

#include <Script_Details/Variables/Variable_Container.h>
#include <Script_Details/Variables/Variable_Reference.h>

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

    m_should_stop_execution = false;

    for(unsigned int i = 0; i < _args.size(); ++i)
    {
        L_ASSERT(_args[i]->type() == m_expected_arguments_data[i].expected_type);

        Variable* parameter = _args[i];

        Variable* variable = nullptr;
        if(m_expected_arguments_data[i].reference)
            variable = new Variable_Reference;
        else
            variable = new Variable_Container;

        variable->assign(parameter);

        m_compound_statement.context().add_variable(m_expected_arguments_data[i].name, variable);
    }

    Variable* result = m_compound_statement.process();
    L_ASSERT( (!result && m_return_type == "void") || (result && (result->type() == m_return_type)) );

    m_compound_statement.context().clear();

    return result;
}
