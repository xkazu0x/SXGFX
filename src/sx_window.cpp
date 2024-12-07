#include "sx_window.h"

void
sx::window::create(create_info *create_info) {
    m_info.title = create_info->title;
    m_info.flags = create_info->flags;
    
    m_instance = GetModuleHandle(nullptr);

    WNDCLASSEXA window_class { };
    window_class.cbSize = sizeof(window_class);
    window_class.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
    window_class.lpfnWndProc = process_message_setup;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.hInstance = m_instance;
    window_class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
    window_class.hbrBackground = nullptr;
    window_class.lpszClassName = m_info.title.c_str();
    window_class.hIconSm = nullptr;

    m_atom = RegisterClassExA(&window_class);

    m_info.windowed_width = create_info->width;
    m_info.windowed_height = create_info->height;
    m_info.fullscreen_width = GetSystemMetrics(SM_CXSCREEN);
    m_info.fullscreen_height = GetSystemMetrics(SM_CYSCREEN);
    
    m_info.current_width = m_info.windowed_width;
    m_info.current_height = m_info.windowed_height;
    m_info.xpos = (m_info.fullscreen_width - m_info.current_width) / 2;
    m_info.ypos = (m_info.fullscreen_height - m_info.current_height) / 2;

    uint32_t window_ex_style = WS_EX_APPWINDOW;
    uint32_t window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
    
    if (create_info->flags & SX_WINDOW_FULLSCREEN) {
        m_info.current_width = m_info.fullscreen_width;
        m_info.current_height = m_info.fullscreen_height;
        m_info.xpos = 0;
        m_info.ypos = 0;
        m_info.fullscreen = true;
        
        window_style = WS_POPUP;
        
        DEVMODE screen_settings = {};
        screen_settings.dmSize = sizeof(screen_settings);
        screen_settings.dmPelsWidth = m_info.current_width;
        screen_settings.dmPelsHeight = m_info.current_height;
        screen_settings.dmBitsPerPel = 32;
        screen_settings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        ChangeDisplaySettings(&screen_settings, CDS_FULLSCREEN);       
    }
    
    if (create_info->flags & SX_WINDOW_RESIZABLE) {
        window_style |= WS_MAXIMIZEBOX;
        window_style |= WS_THICKFRAME;
    }
        
    RECT border_rect = { 0, 0, 0, 0 };
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);
        
    m_info.current_width += border_rect.right - border_rect.left;
    m_info.current_height += border_rect.bottom - border_rect.top;
    m_info.xpos += border_rect.left;
    m_info.ypos += border_rect.top;
    
    m_handle = CreateWindowExA(window_ex_style, MAKEINTATOM(m_atom),
                               m_info.title.c_str(), window_style,
                               m_info.xpos, m_info.ypos,
                               m_info.current_width, m_info.current_height,
                               nullptr, nullptr, m_instance, this);

    m_device = GetDC(m_handle);
}

void
sx::window::destroy() {
    DestroyWindow(m_handle);
    UnregisterClass(MAKEINTATOM(m_atom), m_instance);
}

void
sx::window::update() {
    MSG msg;
    while (PeekMessage(&msg, m_handle, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) break;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void
sx::window::show() {
    ShowWindow(m_handle, SW_SHOW);
    m_state = SX_WINDOW_STATE_ACTIVE;
}

void
sx::window::close() {
    if (m_info.flags & SX_WINDOW_FULLSCREEN) ChangeDisplaySettings(NULL, 0);
    m_state = SX_WINDOW_STATE_CLOSED;
}

bool
sx::window::is_active() {
    return m_state == SX_WINDOW_STATE_ACTIVE;
}

uint32_t
sx::window::width() {
    return m_info.current_width;
}

uint32_t
sx::window::height() {
    return m_info.current_height;
}

int8_t
sx::window::get_key(int32_t key_code) {
    return GetAsyncKeyState(key_code);
}

void
sx::window::change_display_mode() {
    uint32_t window_flags = SWP_SHOWWINDOW;
    LONG window_ex_style = GetWindowLong(m_handle, GWL_EXSTYLE);
    LONG window_style = GetWindowLong(m_handle, GWL_STYLE);
    
    if (m_info.flags & SX_WINDOW_FULLSCREEN && m_info.fullscreen) {
        window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
        if (m_info.flags & SX_WINDOW_RESIZABLE) {
            window_style |= WS_MAXIMIZEBOX;
            window_style |= WS_THICKFRAME;
        }
        
        SetWindowLong(m_handle, GWL_STYLE, window_style);

        m_info.current_width = m_info.windowed_width;
        m_info.current_height = m_info.windowed_height;
        m_info.xpos = (m_info.fullscreen_width - m_info.current_width) / 2;
        m_info.ypos = (m_info.fullscreen_height - m_info.current_height) / 2;
        m_info.fullscreen = false;

        RECT border_rect = { 0, 0, 0, 0 };
        AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);
        
        m_info.current_width += border_rect.right - border_rect.left;
        m_info.current_height += border_rect.bottom - border_rect.top;
        m_info.xpos += border_rect.left;
        m_info.ypos += border_rect.top;
        SetWindowPos(m_handle,
                     0,
                     m_info.xpos,
                     m_info.ypos,
                     m_info.current_width,
                     m_info.current_height,
                     window_flags);
    } else if (m_info.flags & SX_WINDOW_FULLSCREEN && !m_info.fullscreen){
        window_style = WS_POPUP;
        SetWindowLong(m_handle, GWL_STYLE, window_style);
        
        m_info.current_width = m_info.fullscreen_width;
        m_info.current_height = m_info.fullscreen_height;
        m_info.xpos = 0;
        m_info.ypos = 0;
        m_info.fullscreen = true;
        SetWindowPos(m_handle,
                     0,
                     m_info.xpos,
                     m_info.ypos,
                     m_info.current_width,
                     m_info.current_height,
                     window_flags);
    }
}

uint32_t*
sx::window::allocate_pixels(uint32_t width, uint32_t height) {
    m_src_width = width;
    m_src_height = height;

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
    return m_pixels;
}

void
sx::window::free_pixels() {
    VirtualFree(m_memory,
                m_memory_size,
                MEM_RELEASE);
}

void
sx::window::present_pixels() {
    StretchDIBits(m_device,
                  0,
                  0,
                  m_info.current_width,
                  m_info.current_height,
                  0,
                  0,
                  m_src_width,
                  m_src_height,
                  m_memory,
                  &m_bitmap_info,
                  DIB_RGB_COLORS,
                  SRCCOPY);
}

void
sx::window::clear(int32_t color) {
    for (uint32_t i = 0; i < m_src_width * m_src_height; i++) {
        m_pixels[i] = color;
    }
}

void
sx::window::put_pixel(int32_t x, int32_t y, int32_t color) {
    int32_t width = static_cast<int32_t>(m_src_width);
    int32_t height = static_cast<int32_t>(m_src_height);
    
    if (x >= 0 && y >= 0 && x < width && y < height) {
        m_pixels[x + (((height - 1) - y) * width)] = color;
    }
}

LRESULT
sx::window::process_message_setup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    if (msg == WM_NCCREATE) {
        auto *create_struct = (CREATESTRUCTA*)lparam;
        SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)create_struct->lpCreateParams);
        SetWindowLongPtrA(hwnd, GWLP_WNDPROC, (LONG_PTR)&process_message_redirect);

        return process_message_redirect(hwnd, msg, wparam, lparam);
    }

    return DefWindowProcA(hwnd, msg, wparam, lparam);
}

LRESULT
sx::window::process_message_redirect(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    auto *window = (sx::window*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
    return window->process_message(hwnd, msg, wparam, lparam);
}

LRESULT
sx::window::process_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
    case WM_CLOSE: {
        close();
        return 0;
    } break;
    case WM_SIZE: {
        m_info.current_width = LOWORD(lparam);
        m_info.current_height = HIWORD(lparam);
    } break;
    }

    return DefWindowProcA(hwnd, msg, wparam, lparam);
}
