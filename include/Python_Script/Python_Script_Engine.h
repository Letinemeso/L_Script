#pragma once


namespace LScript
{

    class Python_Script_Engine final
    {
    private:
        void* m_hidden_interpreter = nullptr;

    private:
        Python_Script_Engine();

        Python_Script_Engine(const Python_Script_Engine&) = delete;
        Python_Script_Engine(Python_Script_Engine&&) = delete;
        void operator=(const Python_Script_Engine&) = delete;
        void operator=(Python_Script_Engine&&) = delete;

    public:
        ~Python_Script_Engine();

    private:
        void M_register_default_functions();

    public:
        static Python_Script_Engine& instance() { static Python_Script_Engine s_instance; return s_instance; }

    };

}
