#include "sx_window.h"
#include "sx_renderer.h"

#include <cstdio>

#define WIDTH 240
#define HEIGHT 160
#define SCALE 4

int main() {
    sx::window::create_info window_create_info = {};
    window_create_info.title = "SUIXIDE GRAPHICS";
    window_create_info.width = static_cast<uint32_t>(WIDTH * SCALE);
    window_create_info.height = static_cast<uint32_t>(HEIGHT * SCALE);
    window_create_info.flags = SX_WINDOW_UNDEFINED;
    
    sx::window window;
    window.create(&window_create_info);
    window.show();

    sx::renderer renderer;
    renderer.create(&window, WIDTH, HEIGHT);

    int x = 0;
    int y = 0;
    
    while (window.is_active()) {
        window.update();
        if (window.get_key(VK_ESCAPE)) window.close();
        if (window.get_key(VK_F1)) window.change_display_mode();

        if (window.get_key(VK_LEFT)) x--;
        if (window.get_key(VK_RIGHT)) x++;
        if (window.get_key(VK_UP)) y--;
        if (window.get_key(VK_DOWN)) y++;
        
        renderer.clear(0x00);
        renderer.fill_rect(x, y, 16, 16, 0xFF00FF);
        renderer.present();
    }

    renderer.destroy();
    window.destroy();
    return 0;
}

// for (uint32_t y = 0; y < HEIGHT; y++) {
//     for (uint32_t x = 0; x < WIDTH; x++) {
//         int u = ((x << 8) / WIDTH) << 16;
//         int v = ((y << 8) / HEIGHT) << 8;

//         int color = u | v;
//         renderer.put_pixel(x, y, color);
//     }
// }
