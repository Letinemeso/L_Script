#pragma once


namespace LScript
{

    class Operation
    {
    public:
        virtual ~Operation() { }

    public:
        virtual void process() = 0;

    };

}
