#include <L_Script/Script_Details/Operations/Return_Operation.h>

using namespace LScript;


Return_Operation::Return_Operation()
{
    set_stop_required(true);
}

Return_Operation::~Return_Operation()
{
    delete m_operation;
}



Variable* Return_Operation::process(Context* _context)
{
    if(!m_operation)
        return nullptr;

    Variable* operation_result = m_operation->process(_context);
    if(operation_result->is_temporary())
        return operation_result;

    Variable* result = operation_result->construct_copy();
    result->mark_temporary(true);
    return result;
}
