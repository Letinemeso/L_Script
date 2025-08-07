#include <Integrated_Functions.h>

#include <Script_Details/Operations/Custom_Operation.h>

using namespace LScript;


Integrated_Functions::Integrated_Functions()
{

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



void Integrated_Functions::M_register_default_int_functions()
{
    //  set
    {
        Function* function = new Function;
        function->set_return_type("void");

        Function::Arguments_Data arguments_data(2);
        arguments_data.push({"int", "this"});
        arguments_data.push({"int", "_value"});

        Custom_Operation* operation = new Custom_Operation;
        operation->set_operation_logic([function]()->Variable*
        {
            Context& context = function->compound_statement().context();
            Variable* var_this = context.get_variable("this");
            Variable* var_value = context.get_variable("_value");
            L_ASSERT(var_this);
            L_ASSERT(var_value);
            L_ASSERT(var_this->type() == "int");
            L_ASSERT(var_value->type() == "int");

            if(var_this->data() == nullptr)
                var_this->set_data(new int, sizeof(int));

            int* this_raw_data = (int*)var_this->data();
            int* value_raw_data = (int*)var_value->data();

            L_ASSERT(this_raw_data);
            L_ASSERT(value_raw_data);

            *this_raw_data = *value_raw_data;

            return nullptr;
        });

        function->set_expected_arguments_data((Function::Arguments_Data&&)arguments_data);
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
