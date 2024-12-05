#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdint>
#include <string>

namespace sx {    
    class window {
    public:
        struct create_info {
            std::string title;
            uint32_t width;
            uint32_t height;
            bool resizable;
            bool fullscreen;
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
        //void create(std::string title, uint32_t width, uint32_t height, bool fullscreen);
        void create(create_info *create_info);
        void destroy();

        void show();
        void update();
        void close();
        
        bool is_active();
        uint32_t width();
        uint32_t height();        
        int8_t get_key(int32_t key_code);
        
        void change_display_mode();
        
        uint32_t *allocate_pixels(uint32_t width, uint32_t height);
        void free_pixels();
        void present_pixels();

        void clear(int32_t color);
        void put_pixel(int32_t x, int32_t y, int32_t color);
        
    private:
        static LRESULT process_message_setup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
        static LRESULT process_message_redirect(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

        LRESULT process_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
        
    private:
        HINSTANCE m_instance;
        ATOM m_atom;
        HWND m_handle;
        HDC m_device;

        BITMAPINFO m_bitmap_info;
        uint32_t m_memory_size;
        void *m_memory;
        
        uint32_t *m_pixels;
        uint32_t m_src_width;
        uint32_t m_src_height;
        
        window_info m_info;
        window_state m_state;
    };
}
