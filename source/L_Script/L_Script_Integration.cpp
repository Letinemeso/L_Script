#include <L_Script/L_Script_Integration.h>

using namespace LScript;


namespace LScript
{
    void remove_const_if_present(std::string& _type)
    {
        constexpr const char* Const_Str = "const ";
        constexpr unsigned int Const_Str_Length = 6;

        if(_type.length() <= Const_Str_Length)
            return;

        if(std::memcmp(Const_Str, _type.c_str(), Const_Str_Length) != 0)
            return;

        _type = _type.substr(Const_Str_Length);
    }

    void remove_reference_if_present(std::string& _type)
    {
        if(_type[_type.size() - 1] == '&')
            _type.pop_back();
    }
}


std::string LScript::__extract_type(const std::string& _raw_type)
{
    std::string result = _raw_type;
    remove_const_if_present(result);
    remove_reference_if_present(result);
    return result;
}

LDS::Vector<std::string> LScript::__construct_args_string_vec(const std::string& _combined_types)
{
    LDS::Vector<std::string> result;

    for(unsigned int offset = 0; offset < _combined_types.size(); ++offset)
    {
        unsigned int limit = offset + 1;
        for(; limit < _combined_types.size(); ++limit)
        {
            if(_combined_types[limit] == ',')
                break;
        }

        std::string type_str = __extract_type( _combined_types.substr(offset, limit - offset) );
        result.push(LST::move(type_str));

        offset = limit + 1;
    }

    return result;
}



const std::string& LScript::__get_default_type(const std::string& _type)
{
    if(_type == "void")
        return _type;

    if(Builtins_Controller::instance().is_class_registered(_type))
        return _type;

    return LV::Type_Manager::get_default_type_name(_type);
}
