#include <Script.h>

#include <Compiler.h>

using namespace LScript;


Script::Script()
{

}

Script::~Script()
{
    clear_functions();
}



void Script::register_function(const std::string& _name, Function* _function)
{
    L_ASSERT(!m_functions.find(_name).is_ok());

    m_functions.insert(_name, _function);
}

void Script::clear_functions()
{
    for(Functions_Map::Iterator it = m_functions.iterator(); !it.end_reached(); ++it)
        delete *it;
    m_functions.clear();
}


Function* Script::get_function(const std::string& _name) const
{
    Functions_Map::Const_Iterator maybe_function_it = m_functions.find(_name);
    if(maybe_function_it.is_ok())
        return *maybe_function_it;

    return nullptr;
}



void Script::run()
{
    Function* main_func = get_function("main");
    L_ASSERT(main_func);

    main_func->call({});
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Script_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Script_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    Compiler compiler;

    compiler.set_target(product);
    compiler.compile(source_code);
}
