#include "sx_window.h"
#include <cstdio>

#define WIDTH 320
#define HEIGHT 240
#define SCALE 3

int main() {
    sx::window::create_info window_create_info = {};
    window_create_info.title = "SUIXIDE GRAPHICS";
    window_create_info.width = static_cast<uint32_t>(WIDTH * SCALE);
    window_create_info.height = static_cast<uint32_t>(HEIGHT * SCALE);
    window_create_info.flags = SX_WINDOW_UNDEFINED;
    
    sx::window window;
    window.create(&window_create_info);
    window.show();
    
    uint32_t *pixels = window.allocate_pixels(WIDTH, HEIGHT);

    while (window.is_active()) {
        window.update();
        if (window.get_key(VK_ESCAPE)) window.close();
        if (window.get_key(VK_F1)) window.change_display_mode();

        window.clear(0);
        for (uint32_t y = 0; y < HEIGHT; y++) {
            for (uint32_t x = 0; x < WIDTH; x++) {
                int u = ((x << 8) / WIDTH) << 16;
                int v = ((y << 8) / HEIGHT) << 8;
                
                int color = u | v;
                pixels[x + y * WIDTH] = color;
            }
        }
        window.present_pixels();
    }

    window.free_pixels();
    window.destroy();
    return 0;
}
