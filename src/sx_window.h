#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdint>
#include <string>

enum window_flags {
    SX_WINDOW_UNDEFINED = 0x00,
    SX_WINDOW_FULLSCREEN = 0x01,
    SX_WINDOW_RESIZABLE = 0x02,
};

namespace sx {    
    class window {
    public:        
        struct create_info {
            std::string title;
            uint32_t width;
            uint32_t height;
            uint32_t flags;
        };
        
    private:
        struct window_info {
            std::string title;
            uint32_t current_width;
            uint32_t current_height;
            uint32_t windowed_width;
            uint32_t windowed_height;
            uint32_t fullscreen_width;
            uint32_t fullscreen_height;
            uint32_t flags;
            int32_t xpos;
            int32_t ypos;
            bool fullscreen;
        };

        enum window_state {
            SX_WINDOW_STATE_HIDDEN = 0x00,
            SX_WINDOW_STATE_ACTIVE = 0x01,
            SX_WINDOW_STATE_CLOSED = 0x02,
        };
        
    public:
        void create(create_info *create_info);
        void destroy();

        void show();
        void update();
        void close();
        
        void change_display_mode();
        
        bool is_active();
        uint32_t width();
        uint32_t height();        
        int8_t get_key(int32_t key_code);
        HDC get_device();
        
    private:
        static LRESULT process_message_setup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
        static LRESULT process_message_redirect(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

        LRESULT process_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
        
    private:
        HINSTANCE m_instance;
        ATOM m_atom;
        HWND m_handle;
        HDC m_device;
        
        window_info m_info;
        window_state m_state;
    };
}
