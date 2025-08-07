#include <Script_Details/Operations/Call_Member_Function.h>

using namespace LScript;


Call_Member_Function::Call_Member_Function()
{

}

Call_Member_Function::~Call_Member_Function()
{
    delete m_owner_object_getter_operation;
}



Variable* Call_Member_Function::process()
{
    L_ASSERT(m_context);
    L_ASSERT(m_owner_object_getter_operation);

    Variable* owner_object = m_owner_object_getter_operation->process();
    L_ASSERT(owner_object);

    std::cout << "Trying to call member function.\nowner object type: " << owner_object->type() << "\nmember function name: " << m_function_name << std::endl << std::endl;

    return nullptr;
}
