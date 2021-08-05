#ifndef COLORWIN_HPP_INCLUDED
#define COLORWIN_HPP_INCLUDED

#include <Windows.h>
#include <iostream>
#include <stack>

namespace colorwin
{
    enum CW_COLORS
    {
        red = FOREGROUND_RED | FOREGROUND_INTENSITY,
        yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        green = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        cyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        blue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        magenta = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY,
        white = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        gray = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        grey = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,

        dark_gray = FOREGROUND_INTENSITY,
        dark_grey = FOREGROUND_INTENSITY,
    };

    class color
    {
    public:
        color(CW_COLORS color) : m_color(color), m_console_handle(INVALID_HANDLE_VALUE)
        {
            CONSOLE_SCREEN_BUFFER_INFO console_info;
            m_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
            if (!GetConsoleScreenBufferInfo(m_console_handle, &console_info))
            {
                m_console_handle = GetStdHandle(STD_ERROR_HANDLE);
                if (!GetConsoleScreenBufferInfo(m_console_handle, &console_info)) // maybe standard output device has been redirected, try the standard error device
                {
                    m_console_handle = INVALID_HANDLE_VALUE;
                }
            }
        }

        ~color()
        {
            if (m_console_handle != INVALID_HANDLE_VALUE)
            {
                // Restore the previous color.
                SetConsoleTextAttribute(m_console_handle, get_color_stack().top());
                get_color_stack().pop();
            }
        }

    private:
        void change_color() const
        {
            if (m_console_handle == INVALID_HANDLE_VALUE)
                return; // Can't get console info, can't change color.
            CONSOLE_SCREEN_BUFFER_INFO console_info;
            GetConsoleScreenBufferInfo(m_console_handle, &console_info);
            // save the current attributes for restoration on destruction.
            get_color_stack().push(console_info.wAttributes);
            SetConsoleTextAttribute(m_console_handle, 0x0F & m_color | 0xf0 & console_info.wAttributes);    // save the background color
        }

        color(color&);
        color& operator=(color);

        static std::stack<WORD>& get_color_stack()
        {
            // Use this instead of static member to avoid multiply defined symbols.
            static std::stack<WORD> color_stack;
            return color_stack;
        }

        HANDLE m_console_handle;
        const CW_COLORS m_color;

        friend class withcolor;
        template<typename charT, typename traits> friend std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& lhs, colorwin::color const& rhs);
    };

    // Example usage : 
    //  {
    //      withcolor scoped(yellow);
    //      cout << "This is a yellow warning!\n";
    //      cout << "This is a second yellow warning!\n";
    //  }
    //  --  or  --
    //      withcolor(yellow).printf("This will be yellow\n");
    class withcolor
    {
    public:
        withcolor(CW_COLORS color) : m_color(color)
        {
            m_color.change_color();
        }

        int printf(const char* format, ...)
        {
            va_list vlist;
            va_start(vlist, format);
            int ret = vprintf(format, vlist);
            va_end(vlist);
            return ret;
        }

#if _MSC_VER >= 1400    // printf_s offered in Visual Studio 2005
        int printf_s(const char* format, ...)
        {
            va_list vlist;
            va_start(vlist, format);
            int ret = vprintf_s(format, vlist);
            va_end(vlist);
            return ret;
        }
#endif

    private:
        withcolor(withcolor&);
        withcolor& operator=(withcolor);

        color m_color;
    };

    // cout << color(red) -> operator<<(cout, colorwin::color(red))
    template<typename charT, typename traits> std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& lhs, colorwin::color const& rhs)
    {
        rhs.change_color();
        return lhs;
    }
}

#endif // COLORWIN_HPP_INCLUDED