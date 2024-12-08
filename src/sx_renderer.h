#pragma once

#include "sx_window.h"
#include <cstdint>

namespace sx {
    class renderer {
    public:
        void create(sx::window *window, int32_t width, int32_t height);
        void destroy();
        void clear(uint32_t color);
        void present();
        void put_pixel(int32_t x, int32_t y, uint32_t color);
        void fill_rect(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color);
        
    private:
        sx::window *m_window;
        int32_t m_width;
        int32_t m_height;
        BITMAPINFO m_bitmap_info;
        uint32_t m_memory_size;
        void *m_memory;
        uint32_t *m_pixels;
    };
}
