#include <Script.h>

#include <Compiler.h>

using namespace LScript;


//





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Script_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Script_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    Compiler compiler;

    compiler.set_target(product);
    compiler.compile(source_code);
}
