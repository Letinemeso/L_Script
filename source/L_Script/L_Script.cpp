#include <L_Script/L_Script.h>

#include <L_Script/Compiler.h>
#include <L_Script/Script_Details/Variables/Variable_Reference.h>

using namespace LScript;


L_Script::L_Script()
{

}

L_Script::~L_Script()
{
    clear_functions();
}



void L_Script::set_context_object(const std::string& _type_as_string, const std::string& _name, void* _ptr)
{
    // Variable* variable = m_global_context.get_variable(_name);
    // if(variable)
    // {
    //     variable = m_global_context.extract_variable(_name);
    //     delete variable;
    // }

    Context_Objects_Map::Iterator co_it = m_context_objects.find(_name);
    if(co_it.is_ok())
        delete *co_it;
    else
        co_it = m_context_objects.insert_and_get_iterator(_name, nullptr);

    Variable_Reference* pointer_variable = new Variable_Reference;
    pointer_variable->set_type(_type_as_string);
    pointer_variable->set_data(_ptr);

    *co_it = pointer_variable;
}


void L_Script::register_function(const std::string& _name, Function* _function)
{
    L_ASSERT(!m_functions.find(_name).is_ok());

    m_functions.insert(_name, _function);
}

void L_Script::clear_functions()
{
    for(Functions_Map::Iterator it = m_functions.iterator(); !it.end_reached(); ++it)
        delete *it;
    m_functions.clear();
}


Function* L_Script::get_function(const std::string& _name) const
{
    Functions_Map::Const_Iterator maybe_function_it = m_functions.find(_name);
    if(maybe_function_it.is_ok())
        return *maybe_function_it;

    return nullptr;
}



void L_Script::run()
{
    for(Context_Objects_Map::Iterator it = m_context_objects.iterator(); !it.end_reached(); ++it)
        m_global_context.add_variable(it.key(), *it);

    Variable* initialization_result = m_initialization.process(&m_global_context);
    L_ASSERT(!initialization_result || !initialization_result->is_temporary());

    Function* main_func = get_function("main");
    L_ASSERT(main_func);

    Variable* main_result = main_func->call(&m_global_context, {});
    L_ASSERT(!main_result);

    m_global_context.clear();
}





ON_VALUES_ASSIGNED_IMPLEMENTATION(L_Script_Stub)
{
    update_source_code_str();
}



void L_Script_Stub::update_source_code_str()
{
    source_code_str.clear();
    for(unsigned int i = 0; i < source_code.size(); ++i)
        source_code_str += source_code[i];
}



BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(L_Script_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(L_Script_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    Compiler compiler;

    compiler.set_target(product);
    compiler.compile(source_code_str);
}
