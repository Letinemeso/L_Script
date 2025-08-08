#pragma once

#include <Script_Details/Variables/Variable.h>


namespace LScript
{

    class Operation
    {
    public:
        virtual ~Operation() { }

    public:
        [[nodiscard]] virtual Variable* process() = 0;

    };

}
