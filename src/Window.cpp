#include "Window.hpp"

namespace ParteeEngine
{
    Window::Window(const int width, const int height) : width(width), height(height)
    {
        createWindow();
    };

    Window::~Window() {}

    int Window::getHeight() const { return height; }
    int Window::getWidth() const { return width; }
    HWND Window::getHWND() const { return hwnd; }
    HDC Window::getHDC() const { return hdc; }

    void Window::show() {
        ShowWindow(hwnd, SW_SHOW);

        MSG msg = {};
        while (true)
        {
            // Process all pending messages
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                    return;
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            // Render continuously
            if (renderCallback)
            {
                renderCallback();
            }

            // Small sleep to prevent 100% CPU usage
            Sleep(1);
        }
    }

    void Window::setRenderCallback(RenderCallback callback)
    {
        renderCallback = callback;
    }

    void Window::createWindow() {
        const wchar_t CLASS_NAME[] = L"Partee Window Class";

        WNDCLASSW wc = {};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = CLASS_NAME;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Added styles for OpenGL
        wc.hbrBackground = NULL;                       // No background for OpenGL

        RegisterClassW(&wc);

        // Store this pointer for the window
        hwnd = CreateWindowExW(
            0, CLASS_NAME, L"Partee Window",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height,
            NULL, NULL, GetModuleHandle(NULL), this);

        if (hwnd == NULL) {
            return;
        }

        // Get device context for OpenGL
        hdc = GetDC(hwnd);
    }

    LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        Window* pThis = nullptr;

        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (Window*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        }
        else
        {
            pThis = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }

        if (pThis)
        {
            switch (uMsg)
            {
            case WM_CLOSE:
                DestroyWindow(hwnd);
                return 0;

            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;

            case WM_PAINT:
                {
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);
                    // Paint operations would go here
                    EndPaint(hwnd, &ps);
                }
                return 0;

            case WM_SIZE:
                // Handle window resize
                return 0;
            }
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
} // namespace ParteeEngine
