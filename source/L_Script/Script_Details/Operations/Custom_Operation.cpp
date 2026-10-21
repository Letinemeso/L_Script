#include <L_Script/Script_Details/Operations/Custom_Operation.h>

using namespace LScript;


Variable* Custom_Operation::process(Context* _context)
{
    L_ASSERT(m_operation_logic);

    return m_operation_logic(_context);
}
