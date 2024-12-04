#include "sx_window.h"
#include <cstdio>

#define WIDTH 80
#define HEIGHT 120
#define SCALE 7

#define WINDOW_WIDTH WIDTH * SCALE
#define WINDOW_HEIGHT HEIGHT * SCALE

int main() {
    sx::window::create_info window_create_info = {};
    window_create_info.title = "SUIXIDE GRAPHICS";
    window_create_info.width = WINDOW_WIDTH;
    window_create_info.height = WINDOW_HEIGHT;    
    window_create_info.resizable = true;
    window_create_info.fullscreen = false;
    
    sx::window window;
    window.create(&window_create_info);
    window.show();
    
    uint32_t *pixels = window.allocate_pixels(WIDTH, HEIGHT);
    
    uint32_t tick = 0;
    while (window.is_active()) {
        window.update();
        if (window.get_key(VK_ESCAPE)) {
            window.close();
        }

        tick++;
        for (uint32_t i = 0; i < WIDTH * HEIGHT; i++){
            pixels[i] = i + tick;
        }

        window.present_pixels();
    }

    window.free_pixels();
    window.destroy();
    return 0;
}
