#include <Script_Details/Operations/RValue_Getter.h>

using namespace LScript;


Variable* RValue_Getter::process()
{
    return &m_variable;
}
