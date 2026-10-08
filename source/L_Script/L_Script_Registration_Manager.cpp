#include <L_Script/L_Script_Registration_Manager.h>

using namespace LScript;


L_Script_Registration_Manager::L_Script_Registration_Manager()
{

}



L_Script_Registration_Manager::~L_Script_Registration_Manager()
{
    for(unsigned int i = 0; i < m_helpers.size(); ++i)
        delete m_helpers[i];
}



void L_Script_Registration_Manager::register_all()
{
    for(unsigned int i = 0; i < m_helpers.size(); ++i)
    {
        L_Script_Registration_Helper* helper = m_helpers[i];
        helper->register_function();
        delete helper;
    }

    m_helpers.clear();
}
