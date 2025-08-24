#include <Integrated_Functions.h>

#include <Type_Manager.h>

#include <Script_Details/Operations/Custom_Operation.h>

using namespace LScript;


namespace LScript
{
    template<typename _Type>
    _Type* __extract_raw_data(Context& _context, const std::string& _variable_name)
    {
        Variable* var = _context.get_variable(_variable_name);
        L_ASSERT(var);

        _Type* raw_data = (_Type*)var->data();
        L_ASSERT(raw_data);

        return raw_data;
    }
}



Integrated_Functions::Integrated_Functions()
{
    M_register_default_global_functions();
    M_register_default_int_functions();
    M_register_default_unsigned_int_functions();
    M_register_default_float_functions();
    M_register_default_string_functions();
    M_register_default_bool_functions();
}

Integrated_Functions::~Integrated_Functions()
{
    for(Functions::Iterator it = m_global_functions.iterator(); !it.end_reached(); ++it)
        delete *it;

    for(Member_Functions::Iterator member_it = m_member_functions.iterator(); !member_it.end_reached(); ++member_it)
    {
        for(Functions::Iterator it = member_it->iterator(); !it.end_reached(); ++it)
            delete *it;
    }
}



void Integrated_Functions::M_register_default_global_functions()
{
    //  debug_print(int)
    {
        Function* function = new Function;

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"int", "_what", false});
        function->set_expected_arguments_data(arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            int* what_raw_data = __extract_raw_data<int>(function->compound_statement().context(), "_what");

            std::cout << "[debug_print] [int] " << *what_raw_data << std::endl;

            return nullptr;
        });
        function->compound_statement().add_operation(operation);

        register_global_function("debug_print", function);
    }

    //  debug_print(uint)
    {
        Function* function = new Function;

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "_what", false});
        function->set_expected_arguments_data(arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           unsigned int* what_raw_data = __extract_raw_data<unsigned int>(function->compound_statement().context(), "_what");

                                           std::cout << "[debug_print] [uint] " << *what_raw_data << std::endl;

                                           return nullptr;
                                       });
        function->compound_statement().add_operation(operation);

        register_global_function("debug_print", function);
    }

    //  debug_print(float)
    {
        Function* function = new Function;

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"float", "_what", false});
        function->set_expected_arguments_data(arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            float* what_raw_data = __extract_raw_data<float>(function->compound_statement().context(), "_what");

            std::cout << "[debug_print] [float] " << *what_raw_data << std::endl;

            return nullptr;
        });
        function->compound_statement().add_operation(operation);

        register_global_function("debug_print", function);
    }

    //  debug_print(string)
    {
        Function* function = new Function;

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"string", "_what", false});
        function->set_expected_arguments_data(arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            std::string* what_raw_data = __extract_raw_data<std::string>(function->compound_statement().context(), "_what");

            std::cout << "[debug_print] [string] " << *what_raw_data << std::endl;

            return nullptr;
        });
        function->compound_statement().add_operation(operation);

        register_global_function("debug_print", function);
    }
}

void Integrated_Functions::M_register_default_int_functions()
{
    //  set
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"int", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            Context& context = function->compound_statement().context();
            Variable* var_this = context.get_variable("this");
            L_ASSERT(var_this);
            L_ASSERT(var_this->type() == "int");

            if(var_this->data() == nullptr)
            {
                LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("int", 1);
                var_this->set_data(allocated_data.ptr, allocated_data.size);
            }

            int* value_raw_data = __extract_raw_data<int>(function->compound_statement().context(), "_value");
            int* this_raw_data = (int*)var_this->data();

            L_ASSERT(this_raw_data);

            *this_raw_data = *value_raw_data;

            return nullptr;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("int", "set", function);
    }

    //  add
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"int", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            int* this_raw_data = __extract_raw_data<int>(function->compound_statement().context(), "this");
            int* value_raw_data = __extract_raw_data<int>(function->compound_statement().context(), "_value");

            *this_raw_data += *value_raw_data;

            return nullptr;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("int", "add", function);
    }

    //  subtract
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"int", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            int* this_raw_data = __extract_raw_data<int>(function->compound_statement().context(), "this");
            int* value_raw_data = __extract_raw_data<int>(function->compound_statement().context(), "_value");

            *this_raw_data -= *value_raw_data;

            return nullptr;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("int", "subtract", function);
    }

    //  multiply
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"int", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            int* this_raw_data = __extract_raw_data<int>(function->compound_statement().context(), "this");
            int* value_raw_data = __extract_raw_data<int>(function->compound_statement().context(), "_value");

            *this_raw_data *= *value_raw_data;

            return nullptr;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("int", "multiply", function);
    }

    //  divide
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"int", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            int* this_raw_data = __extract_raw_data<int>(function->compound_statement().context(), "this");
            int* value_raw_data = __extract_raw_data<int>(function->compound_statement().context(), "_value");

            *this_raw_data /= *value_raw_data;

            return nullptr;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("int", "divide", function);
    }

    //  mod
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"int", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            int* this_raw_data = __extract_raw_data<int>(function->compound_statement().context(), "this");
            int* value_raw_data = __extract_raw_data<int>(function->compound_statement().context(), "_value");

            *this_raw_data %= *value_raw_data;

            return nullptr;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("int", "mod", function);
    }

    //  equals
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"int", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            Context& context = function->compound_statement().context();

            int* this_raw_data = __extract_raw_data<int>(context, "this");
            int* value_raw_data = __extract_raw_data<int>(context, "_value");

            Variable_Container* result_container = new Variable_Container;
            context.add_variable("result", result_container);
            result_container->set_type("bool");
            LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
            result_container->set_data(allocated_data.ptr, allocated_data.size);

            bool* result_raw_data = (bool*)result_container->data();
            L_ASSERT(result_raw_data);

            *result_raw_data = *this_raw_data == *value_raw_data;

            return result_container;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("int", "equals", function);
    }

    //  less
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"int", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            Context& context = function->compound_statement().context();

            int* this_raw_data = __extract_raw_data<int>(context, "this");
            int* value_raw_data = __extract_raw_data<int>(context, "_value");

            Variable_Container* result_container = new Variable_Container;
            context.add_variable("result", result_container);
            result_container->set_type("bool");
            LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
            result_container->set_data(allocated_data.ptr, allocated_data.size);

            bool* result_raw_data = (bool*)result_container->data();
            L_ASSERT(result_raw_data);

            *result_raw_data = *this_raw_data < *value_raw_data;

            return result_container;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("int", "less", function);
    }

    //  more
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"int", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            Context& context = function->compound_statement().context();

            int* this_raw_data = __extract_raw_data<int>(context, "this");
            int* value_raw_data = __extract_raw_data<int>(context, "_value");

            Variable_Container* result_container = new Variable_Container;
            context.add_variable("result", result_container);
            result_container->set_type("bool");
            LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
            result_container->set_data(allocated_data.ptr, allocated_data.size);

            bool* result_raw_data = (bool*)result_container->data();
            L_ASSERT(result_raw_data);

            *result_raw_data = *this_raw_data > *value_raw_data;

            return result_container;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("int", "more", function);
    }

    //  less_or_equal
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"int", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            Context& context = function->compound_statement().context();

            int* this_raw_data = __extract_raw_data<int>(context, "this");
            int* value_raw_data = __extract_raw_data<int>(context, "_value");

            Variable_Container* result_container = new Variable_Container;
            context.add_variable("result", result_container);
            result_container->set_type("bool");
            LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
            result_container->set_data(allocated_data.ptr, allocated_data.size);

            bool* result_raw_data = (bool*)result_container->data();
            L_ASSERT(result_raw_data);

            *result_raw_data = *this_raw_data <= *value_raw_data;

            return result_container;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("int", "less_or_equal", function);
    }

    //  more_or_equal
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"int", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            Context& context = function->compound_statement().context();

            int* this_raw_data = __extract_raw_data<int>(context, "this");
            int* value_raw_data = __extract_raw_data<int>(context, "_value");

            Variable_Container* result_container = new Variable_Container;
            context.add_variable("result", result_container);
            result_container->set_type("bool");
            LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
            result_container->set_data(allocated_data.ptr, allocated_data.size);

            bool* result_raw_data = (bool*)result_container->data();
            L_ASSERT(result_raw_data);

            *result_raw_data = *this_raw_data >= *value_raw_data;

            return result_container;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("int", "more_or_equal", function);
    }

    //  to_uint
    {
        Function* function = new Function;
        function->set_return_type("uint");

        Function::Arguments_Data arguments_data(1);
        arguments_data.push({"int", "this", true});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            Context& context = function->compound_statement().context();

            int* this_raw_data = __extract_raw_data<int>(context, "this");

            Variable_Container* result_container = new Variable_Container;
            context.add_variable("result", result_container);
            result_container->set_type("uint");
            LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("uint", 1);
            result_container->set_data(allocated_data.ptr, allocated_data.size);

            unsigned int* result_raw_data = (unsigned int*)result_container->data();
            L_ASSERT(result_raw_data);

            *result_raw_data = (unsigned int)(*this_raw_data);

            return result_container;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("int", "to_uint", function);
    }

    //  to_float
    {
        Function* function = new Function;
        function->set_return_type("float");

        Function::Arguments_Data arguments_data(1);
        arguments_data.push({"int", "this", true});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            Context& context = function->compound_statement().context();

            int* this_raw_data = __extract_raw_data<int>(context, "this");

            Variable_Container* result_container = new Variable_Container;
            context.add_variable("result", result_container);
            result_container->set_type("float");
            LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("float", 1);
            result_container->set_data(allocated_data.ptr, allocated_data.size);

            float* result_raw_data = (float*)result_container->data();
            L_ASSERT(result_raw_data);

            *result_raw_data = (float)(*this_raw_data);

            return result_container;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("int", "to_float", function);
    }

}

void Integrated_Functions::M_register_default_unsigned_int_functions()
{
    //  set
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"uint", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();
                                           Variable* var_this = context.get_variable("this");
                                           L_ASSERT(var_this);
                                           L_ASSERT(var_this->type() == "uint");

                                           if(var_this->data() == nullptr)
                                           {
                                               LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("uint", 1);
                                               var_this->set_data(allocated_data.ptr, allocated_data.size);
                                           }

                                           unsigned int* value_raw_data = __extract_raw_data<unsigned int>(function->compound_statement().context(), "_value");
                                           unsigned int* this_raw_data = (unsigned int*)var_this->data();

                                           L_ASSERT(this_raw_data);

                                           *this_raw_data = *value_raw_data;

                                           return nullptr;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "set", function);
    }

    //  set(int)
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();
                                           Variable* var_this = context.get_variable("this");
                                           L_ASSERT(var_this);
                                           L_ASSERT(var_this->type() == "uint");

                                           if(var_this->data() == nullptr)
                                           {
                                               LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("uint", 1);
                                               var_this->set_data(allocated_data.ptr, allocated_data.size);
                                           }

                                           unsigned int* this_raw_data = (unsigned int*)var_this->data();
                                           int* value_raw_data = __extract_raw_data<int>(function->compound_statement().context(), "_value");

                                           L_ASSERT(this_raw_data);

                                           *this_raw_data = *value_raw_data;

                                           return nullptr;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "set", function);
    }

    //  add
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"uint", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(function->compound_statement().context(), "this");
                                           unsigned int* value_raw_data = __extract_raw_data<unsigned int>(function->compound_statement().context(), "_value");

                                           *this_raw_data += *value_raw_data;

                                           return nullptr;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "add", function);
    }

    //  add(int)
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(function->compound_statement().context(), "this");
                                           int* value_raw_data = __extract_raw_data<int>(function->compound_statement().context(), "_value");

                                           *this_raw_data += *value_raw_data;

                                           return nullptr;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "add", function);
    }

    //  subtract
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"uint", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(function->compound_statement().context(), "this");
                                           unsigned int* value_raw_data = __extract_raw_data<unsigned int>(function->compound_statement().context(), "_value");

                                           *this_raw_data -= *value_raw_data;

                                           return nullptr;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "subtract", function);
    }

    //  subtract(int)
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(function->compound_statement().context(), "this");
                                           int* value_raw_data = __extract_raw_data<int>(function->compound_statement().context(), "_value");

                                           *this_raw_data -= *value_raw_data;

                                           return nullptr;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "subtract", function);
    }

    //  multiply
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"uint", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(function->compound_statement().context(), "this");
                                           unsigned int* value_raw_data = __extract_raw_data<unsigned int>(function->compound_statement().context(), "_value");

                                           *this_raw_data *= *value_raw_data;

                                           return nullptr;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "multiply", function);
    }

    //  multiply(int)
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(function->compound_statement().context(), "this");
                                           int* value_raw_data = __extract_raw_data<int>(function->compound_statement().context(), "_value");

                                           *this_raw_data *= *value_raw_data;

                                           return nullptr;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "multiply", function);
    }

    //  divide
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"uint", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(function->compound_statement().context(), "this");
                                           unsigned int* value_raw_data = __extract_raw_data<unsigned int>(function->compound_statement().context(), "_value");

                                           *this_raw_data /= *value_raw_data;

                                           return nullptr;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "divide", function);
    }

    //  divide(int)
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(function->compound_statement().context(), "this");
                                           int* value_raw_data = __extract_raw_data<int>(function->compound_statement().context(), "_value");

                                           *this_raw_data /= *value_raw_data;

                                           return nullptr;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "divide", function);
    }

    //  mod
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"uint", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(function->compound_statement().context(), "this");
                                           unsigned int* value_raw_data = __extract_raw_data<unsigned int>(function->compound_statement().context(), "_value");

                                           *this_raw_data %= *value_raw_data;

                                           return nullptr;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "mod", function);
    }

    //  equals
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"uint", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();

                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(context, "this");
                                           unsigned int* value_raw_data = __extract_raw_data<unsigned int>(context, "_value");

                                           Variable_Container* result_container = new Variable_Container;
                                           context.add_variable("result", result_container);
                                           result_container->set_type("bool");
                                           LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
                                           result_container->set_data(allocated_data.ptr, allocated_data.size);

                                           bool* result_raw_data = (bool*)result_container->data();
                                           L_ASSERT(result_raw_data);

                                           *result_raw_data = *this_raw_data == *value_raw_data;

                                           return result_container;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "equals", function);
    }

    //  equals(int)
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();

                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(context, "this");
                                           int* value_raw_data = __extract_raw_data<int>(context, "_value");

                                           Variable_Container* result_container = new Variable_Container;
                                           context.add_variable("result", result_container);
                                           result_container->set_type("bool");
                                           LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
                                           result_container->set_data(allocated_data.ptr, allocated_data.size);

                                           bool* result_raw_data = (bool*)result_container->data();
                                           L_ASSERT(result_raw_data);

                                           *result_raw_data = *this_raw_data == *value_raw_data;

                                           return result_container;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "equals", function);
    }

    //  less
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"uint", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();

                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(context, "this");
                                           unsigned int* value_raw_data = __extract_raw_data<unsigned int>(context, "_value");

                                           Variable_Container* result_container = new Variable_Container;
                                           context.add_variable("result", result_container);
                                           result_container->set_type("bool");
                                           LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
                                           result_container->set_data(allocated_data.ptr, allocated_data.size);

                                           bool* result_raw_data = (bool*)result_container->data();
                                           L_ASSERT(result_raw_data);

                                           *result_raw_data = *this_raw_data < *value_raw_data;

                                           return result_container;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "less", function);
    }

    //  less(int)
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();

                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(context, "this");
                                           int* value_raw_data = __extract_raw_data<int>(context, "_value");

                                           Variable_Container* result_container = new Variable_Container;
                                           context.add_variable("result", result_container);
                                           result_container->set_type("bool");
                                           LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
                                           result_container->set_data(allocated_data.ptr, allocated_data.size);

                                           bool* result_raw_data = (bool*)result_container->data();
                                           L_ASSERT(result_raw_data);

                                           *result_raw_data = *this_raw_data < *value_raw_data;

                                           return result_container;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "less", function);
    }

    //  more
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"uint", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();

                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(context, "this");
                                           unsigned int* value_raw_data = __extract_raw_data<unsigned int>(context, "_value");

                                           Variable_Container* result_container = new Variable_Container;
                                           context.add_variable("result", result_container);
                                           result_container->set_type("bool");
                                           LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
                                           result_container->set_data(allocated_data.ptr, allocated_data.size);

                                           bool* result_raw_data = (bool*)result_container->data();
                                           L_ASSERT(result_raw_data);

                                           *result_raw_data = *this_raw_data > *value_raw_data;

                                           return result_container;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "more", function);
    }

    //  more(int)
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();

                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(context, "this");
                                           int* value_raw_data = __extract_raw_data<int>(context, "_value");

                                           Variable_Container* result_container = new Variable_Container;
                                           context.add_variable("result", result_container);
                                           result_container->set_type("bool");
                                           LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
                                           result_container->set_data(allocated_data.ptr, allocated_data.size);

                                           bool* result_raw_data = (bool*)result_container->data();
                                           L_ASSERT(result_raw_data);

                                           *result_raw_data = *this_raw_data > *value_raw_data;

                                           return result_container;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "more", function);
    }

    //  less_or_equal
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"uint", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();

                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(context, "this");
                                           unsigned int* value_raw_data = __extract_raw_data<unsigned int>(context, "_value");

                                           Variable_Container* result_container = new Variable_Container;
                                           context.add_variable("result", result_container);
                                           result_container->set_type("bool");
                                           LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
                                           result_container->set_data(allocated_data.ptr, allocated_data.size);

                                           bool* result_raw_data = (bool*)result_container->data();
                                           L_ASSERT(result_raw_data);

                                           *result_raw_data = *this_raw_data <= *value_raw_data;

                                           return result_container;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "less_or_equal", function);
    }

    //  less_or_equal(int)
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();

                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(context, "this");
                                           int* value_raw_data = __extract_raw_data<int>(context, "_value");

                                           Variable_Container* result_container = new Variable_Container;
                                           context.add_variable("result", result_container);
                                           result_container->set_type("bool");
                                           LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
                                           result_container->set_data(allocated_data.ptr, allocated_data.size);

                                           bool* result_raw_data = (bool*)result_container->data();
                                           L_ASSERT(result_raw_data);

                                           *result_raw_data = *this_raw_data <= *value_raw_data;

                                           return result_container;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "less_or_equal", function);
    }

    //  more_or_equal
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"uint", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();

                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(context, "this");
                                           unsigned int* value_raw_data = __extract_raw_data<unsigned int>(context, "_value");

                                           Variable_Container* result_container = new Variable_Container;
                                           context.add_variable("result", result_container);
                                           result_container->set_type("bool");
                                           LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
                                           result_container->set_data(allocated_data.ptr, allocated_data.size);

                                           bool* result_raw_data = (bool*)result_container->data();
                                           L_ASSERT(result_raw_data);

                                           *result_raw_data = *this_raw_data >= *value_raw_data;

                                           return result_container;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "more_or_equal", function);
    }

    //  more_or_equal(int)
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"uint", "this", true});
        arguments_data.push({"int", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();

                                           unsigned int* this_raw_data = __extract_raw_data<unsigned int>(context, "this");
                                           int* value_raw_data = __extract_raw_data<int>(context, "_value");

                                           Variable_Container* result_container = new Variable_Container;
                                           context.add_variable("result", result_container);
                                           result_container->set_type("bool");
                                           LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
                                           result_container->set_data(allocated_data.ptr, allocated_data.size);

                                           bool* result_raw_data = (bool*)result_container->data();
                                           L_ASSERT(result_raw_data);

                                           *result_raw_data = *this_raw_data >= *value_raw_data;

                                           return result_container;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "more_or_equal", function);
    }

    //  to_int
    {
        Function* function = new Function;
        function->set_return_type("int");

        Function::Arguments_Data arguments_data(1);
        arguments_data.push({"uint", "this", true});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            Context& context = function->compound_statement().context();

            unsigned int* this_raw_data = __extract_raw_data<unsigned int>(context, "this");

            Variable_Container* result_container = new Variable_Container;
            context.add_variable("result", result_container);
            result_container->set_type("int");
            LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("int", 1);
            result_container->set_data(allocated_data.ptr, allocated_data.size);

            int* result_raw_data = (int*)result_container->data();
            L_ASSERT(result_raw_data);

            *result_raw_data = (int)(*this_raw_data);

            return result_container;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "to_int", function);
    }

    //  to_float
    {
        Function* function = new Function;
        function->set_return_type("float");

        Function::Arguments_Data arguments_data(1);
        arguments_data.push({"uint", "this", true});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            Context& context = function->compound_statement().context();

            float* this_raw_data = __extract_raw_data<float>(context, "this");

            Variable_Container* result_container = new Variable_Container;
            context.add_variable("result", result_container);
            result_container->set_type("float");
            LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("float", 1);
            result_container->set_data(allocated_data.ptr, allocated_data.size);

            float* result_raw_data = (float*)result_container->data();
            L_ASSERT(result_raw_data);

            *result_raw_data = (float)(*this_raw_data);

            return result_container;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("uint", "to_float", function);
    }

}

void Integrated_Functions::M_register_default_float_functions()
{
    //  set
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"float", "this", true});
        arguments_data.push({"float", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();
                                           Variable* var_this = context.get_variable("this");
                                           L_ASSERT(var_this);
                                           L_ASSERT(var_this->type() == "float");

                                           if(var_this->data() == nullptr)
                                           {
                                               LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("float", 1);
                                               var_this->set_data(allocated_data.ptr, allocated_data.size);
                                           }

                                           float* value_raw_data = __extract_raw_data<float>(function->compound_statement().context(), "_value");
                                           float* this_raw_data = (float*)var_this->data();

                                           L_ASSERT(this_raw_data);

                                           *this_raw_data = *value_raw_data;

                                           return nullptr;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("float", "set", function);
    }

    //  add
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"float", "this", true});
        arguments_data.push({"float", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           float* this_raw_data = __extract_raw_data<float>(function->compound_statement().context(), "this");
                                           float* value_raw_data = __extract_raw_data<float>(function->compound_statement().context(), "_value");

                                           *this_raw_data += *value_raw_data;

                                           return nullptr;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("float", "add", function);
    }

    //  subtract
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"float", "this", true});
        arguments_data.push({"float", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           float* this_raw_data = __extract_raw_data<float>(function->compound_statement().context(), "this");
                                           float* value_raw_data = __extract_raw_data<float>(function->compound_statement().context(), "_value");

                                           *this_raw_data -= *value_raw_data;

                                           return nullptr;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("float", "subtract", function);
    }

    //  multiply
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"float", "this", true});
        arguments_data.push({"float", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           float* this_raw_data = __extract_raw_data<float>(function->compound_statement().context(), "this");
                                           float* value_raw_data = __extract_raw_data<float>(function->compound_statement().context(), "_value");

                                           *this_raw_data *= *value_raw_data;

                                           return nullptr;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("float", "multiply", function);
    }

    //  divide
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"float", "this", true});
        arguments_data.push({"float", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           float* this_raw_data = __extract_raw_data<float>(function->compound_statement().context(), "this");
                                           float* value_raw_data = __extract_raw_data<float>(function->compound_statement().context(), "_value");

                                           *this_raw_data /= *value_raw_data;

                                           return nullptr;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("float", "divide", function);
    }

    //  equals
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"float", "this", true});
        arguments_data.push({"float", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();

                                           float* this_raw_data = __extract_raw_data<float>(context, "this");
                                           float* value_raw_data = __extract_raw_data<float>(context, "_value");

                                           Variable_Container* result_container = new Variable_Container;
                                           context.add_variable("result", result_container);
                                           result_container->set_type("bool");
                                           LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
                                           result_container->set_data(allocated_data.ptr, allocated_data.size);

                                           bool* result_raw_data = (bool*)result_container->data();
                                           L_ASSERT(result_raw_data);

                                           *result_raw_data = *this_raw_data == *value_raw_data;

                                           return result_container;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("float", "equals", function);
    }

    //  less
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"float", "this", true});
        arguments_data.push({"float", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();

                                           float* this_raw_data = __extract_raw_data<float>(context, "this");
                                           float* value_raw_data = __extract_raw_data<float>(context, "_value");

                                           Variable_Container* result_container = new Variable_Container;
                                           context.add_variable("result", result_container);
                                           result_container->set_type("bool");
                                           LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
                                           result_container->set_data(allocated_data.ptr, allocated_data.size);

                                           bool* result_raw_data = (bool*)result_container->data();
                                           L_ASSERT(result_raw_data);

                                           *result_raw_data = *this_raw_data < *value_raw_data;

                                           return result_container;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("float", "less", function);
    }

    //  more
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"float", "this", true});
        arguments_data.push({"float", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();

                                           float* this_raw_data = __extract_raw_data<float>(context, "this");
                                           float* value_raw_data = __extract_raw_data<float>(context, "_value");

                                           Variable_Container* result_container = new Variable_Container;
                                           context.add_variable("result", result_container);
                                           result_container->set_type("bool");
                                           LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
                                           result_container->set_data(allocated_data.ptr, allocated_data.size);

                                           bool* result_raw_data = (bool*)result_container->data();
                                           L_ASSERT(result_raw_data);

                                           *result_raw_data = *this_raw_data > *value_raw_data;

                                           return result_container;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("float", "more", function);
    }

    //  less_or_equal
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"float", "this", true});
        arguments_data.push({"float", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();

                                           float* this_raw_data = __extract_raw_data<float>(context, "this");
                                           float* value_raw_data = __extract_raw_data<float>(context, "_value");

                                           Variable_Container* result_container = new Variable_Container;
                                           context.add_variable("result", result_container);
                                           result_container->set_type("bool");
                                           LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
                                           result_container->set_data(allocated_data.ptr, allocated_data.size);

                                           bool* result_raw_data = (bool*)result_container->data();
                                           L_ASSERT(result_raw_data);

                                           *result_raw_data = *this_raw_data <= *value_raw_data;

                                           return result_container;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("float", "less_or_equal", function);
    }

    //  more_or_equal
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"float", "this", true});
        arguments_data.push({"float", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
                                       {
                                           Context& context = function->compound_statement().context();

                                           float* this_raw_data = __extract_raw_data<float>(context, "this");
                                           float* value_raw_data = __extract_raw_data<float>(context, "_value");

                                           Variable_Container* result_container = new Variable_Container;
                                           context.add_variable("result", result_container);
                                           result_container->set_type("bool");
                                           LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
                                           result_container->set_data(allocated_data.ptr, allocated_data.size);

                                           bool* result_raw_data = (bool*)result_container->data();
                                           L_ASSERT(result_raw_data);

                                           *result_raw_data = *this_raw_data >= *value_raw_data;

                                           return result_container;
                                       });
        function->compound_statement().add_operation(operation);

        register_member_function("float", "more_or_equal", function);
    }

    //  to_int
    {
        Function* function = new Function;
        function->set_return_type("int");

        Function::Arguments_Data arguments_data(1);
        arguments_data.push({"float", "this", true});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            Context& context = function->compound_statement().context();

            float* this_raw_data = __extract_raw_data<float>(context, "this");

            Variable_Container* result_container = new Variable_Container;
            context.add_variable("result", result_container);
            result_container->set_type("int");
            LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("int", 1);
            result_container->set_data(allocated_data.ptr, allocated_data.size);

            int* result_raw_data = (int*)result_container->data();
            L_ASSERT(result_raw_data);

            *result_raw_data = (int)(*this_raw_data);

            return result_container;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("float", "to_int", function);
    }

    //  to_uint
    {
        Function* function = new Function;
        function->set_return_type("uint");

        Function::Arguments_Data arguments_data(1);
        arguments_data.push({"float", "this", true});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            Context& context = function->compound_statement().context();

            float* this_raw_data = __extract_raw_data<float>(context, "this");

            Variable_Container* result_container = new Variable_Container;
            context.add_variable("result", result_container);
            result_container->set_type("uint");
            LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("uint", 1);
            result_container->set_data(allocated_data.ptr, allocated_data.size);

            unsigned int* result_raw_data = (unsigned int*)result_container->data();
            L_ASSERT(result_raw_data);

            *result_raw_data = (unsigned int)(*this_raw_data);

            return result_container;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("float", "to_uint", function);
    }

}

void Integrated_Functions::M_register_default_string_functions()
{
    //  set
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"string", "this", true});
        arguments_data.push({"string", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            Context& context = function->compound_statement().context();
            Variable* var_this = context.get_variable("this");
            L_ASSERT(var_this);
            L_ASSERT(var_this->type() == "string");

            if(var_this->data() == nullptr)
            {
                LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("string", 1);
                var_this->set_data(allocated_data.ptr, allocated_data.size);
            }

            std::string* this_raw_data = (std::string*)var_this->data();
            std::string* value_raw_data = __extract_raw_data<std::string>(context, "_value");

            L_ASSERT(this_raw_data);
            L_ASSERT(value_raw_data);

            *this_raw_data = *value_raw_data;

            return nullptr;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("string", "set", function);
    }

}

void Integrated_Functions::M_register_default_bool_functions()
{
    //  set
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"bool", "this", true});
        arguments_data.push({"bool", "_value", false});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            Context& context = function->compound_statement().context();
            Variable* var_this = context.get_variable("this");
            L_ASSERT(var_this);

            if(var_this->data() == nullptr)
            {
                LV::Type_Utility::Allocate_Result allocated_data = LV::Type_Manager::allocate("bool", 1);
                var_this->set_data(allocated_data.ptr, allocated_data.size);
            }

            bool* this_raw_data = (bool*)var_this->data();
            bool* value_raw_data = __extract_raw_data<bool>(context, "_value");

            L_ASSERT(this_raw_data);
            L_ASSERT(value_raw_data);

            *this_raw_data = *value_raw_data;

            return nullptr;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("bool", "set", function);
    }

    //  invert
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(1);
        arguments_data.push({"bool", "this", true});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            bool* this_raw_data = __extract_raw_data<bool>(function->compound_statement().context(), "this");

            *this_raw_data = !*this_raw_data;

            return nullptr;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("bool", "invert", function);
    }

    //  inverted
    {
        Function* function = new Function;
        function->set_return_type("bool");

        Function::Arguments_Data arguments_data(1);
        arguments_data.push({"bool", "this", true});
        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            Context& context = function->compound_statement().context();

            bool* this_raw_data = __extract_raw_data<bool>(function->compound_statement().context(), "this");

            Variable_Container* inverted_container = new Variable_Container;
            context.add_variable("inverted_this", inverted_container);

            bool* new_bool_data = new bool;
            *new_bool_data = !*this_raw_data;
            inverted_container->set_type("bool");
            inverted_container->set_data(new_bool_data, sizeof(bool));

            return inverted_container;
        });
        function->compound_statement().add_operation(operation);

        register_member_function("bool", "inverted", function);
    }

}



std::string Integrated_Functions::M_construct_function_name(const std::string& _name, const Function* _function, const std::string& _owner_type) const
{
    std::string result;

    if(_owner_type.size() > 0)
        result += _owner_type + "::";

    result += _name;
    result += '(';

    const Function::Arguments_Data& arguments_data = _function->expected_arguments_data();
    for(unsigned int i = 1; i < arguments_data.size(); ++i)
        result += arguments_data[i - 1].expected_type + ", ";
    if(arguments_data.size() > 0)
        result += arguments_data[arguments_data.size() - 1].expected_type;

    result += ')';

    return result;
}

std::string Integrated_Functions::M_construct_function_name(const std::string& _name, const Argument_Types& _arg_types, const std::string& _owner_type) const
{
    std::string result;

    if(_owner_type.size() > 0)
        result += _owner_type + "::";

    result += _name;
    result += '(';

    for(unsigned int i = 1; i < _arg_types.size(); ++i)
        result += _arg_types[i - 1] + ", ";
    if(_arg_types.size() > 0)
        result += _arg_types[_arg_types.size() - 1];

    result += ')';

    return result;
}



void Integrated_Functions::register_global_function(const std::string& _name, Function* _function)
{
    std::string full_name = M_construct_function_name(_name, _function);
    L_ASSERT(!m_global_functions.find(full_name).is_ok());

    m_global_functions.insert((std::string&&)full_name, _function);
}

void Integrated_Functions::register_member_function(const std::string& _owner_type, const std::string& _name, Function* _function)
{
    Member_Functions::Iterator functions_it = m_member_functions.find(_owner_type);
    if(!functions_it.is_ok())
        functions_it = m_member_functions.insert_and_get_iterator(_owner_type, {});

    Functions& functions = *functions_it;

    std::string full_name = M_construct_function_name(_name, _function, _owner_type);
    L_ASSERT(!functions.find(full_name).is_ok());

    functions.insert((std::string&&)full_name, _function);
}



Function* Integrated_Functions::get_global_function(const std::string& _name, const Argument_Types& _arg_types) const
{
    std::string full_name = M_construct_function_name(_name, _arg_types);
    Functions::Const_Iterator maybe_function_it = m_global_functions.find(full_name);
    if(maybe_function_it.is_ok())
        return *maybe_function_it;

    return nullptr;
}

Function* Integrated_Functions::get_member_function(const std::string& _owner_type, const std::string& _name, const Argument_Types& _arg_types) const
{
    Member_Functions::Const_Iterator functions_it = m_member_functions.find(_owner_type);
    if(!functions_it.is_ok())
        return nullptr;

    const Functions& functions = *functions_it;

    std::string full_name = M_construct_function_name(_name, _arg_types, _owner_type);
    Functions::Const_Iterator maybe_function_it = functions.find(full_name);
    if(maybe_function_it.is_ok())
        return *maybe_function_it;

    return nullptr;
}
