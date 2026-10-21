#include <L_Script/Script_Details/Operations/RValue_Getter.h>

using namespace LScript;


Variable* RValue_Getter::process(Context* _context)
{
    return &m_variable;
}
