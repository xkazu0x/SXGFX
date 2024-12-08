#include "sx_renderer.h"

void
sx::renderer::create(sx::window *window, int32_t width, int32_t height) {
    m_window = window;
    m_width = width;
    m_height = height;
    
    m_bitmap_info = {};
    m_bitmap_info.bmiHeader.biSize = sizeof(m_bitmap_info.bmiHeader);
    m_bitmap_info.bmiHeader.biWidth = width;
    m_bitmap_info.bmiHeader.biHeight = height;
    m_bitmap_info.bmiHeader.biPlanes = 1;
    m_bitmap_info.bmiHeader.biBitCount = 32;
    m_bitmap_info.bmiHeader.biCompression = BI_RGB;

    m_memory_size = sizeof(uint32_t) * width * height;
    m_memory = VirtualAlloc(0,
                            m_memory_size,
                            MEM_RESERVE | MEM_COMMIT,
                            PAGE_READWRITE);
    m_pixels = (uint32_t *) m_memory;
}

void
sx::renderer::destroy() {
    VirtualFree(m_memory,
                m_memory_size,
                MEM_RELEASE);
}

void
sx::renderer::clear(uint32_t color) {
    for (int32_t i = 0; i < m_width * m_height; i++) {
        m_pixels[i] = color;
    }
}

void
sx::renderer::present() {
    StretchDIBits(m_window->get_device(),
                  0,
                  0,
                  m_window->width(),
                  m_window->height(),
                  0,
                  0,
                  m_width,
                  m_height,
                  m_memory,
                  &m_bitmap_info,
                  DIB_RGB_COLORS,
                  SRCCOPY);
}

void
sx::renderer::put_pixel(int32_t x, int32_t y, uint32_t color) {
    if (x >= m_width || y >= m_height || x < 0 || y < 0) return;
    m_pixels[x + ((m_height - 1) - y) * m_width] = color;
}

void
sx::renderer::fill_rect(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color) {
    for (int32_t yy = y; yy < height + y; yy++) {
        for (int32_t xx = x; xx < width + x; xx++) {
            put_pixel(xx, yy, color);
        }
    }
}
