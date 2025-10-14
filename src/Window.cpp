#include "Window.hpp"

namespace ParteeEngine
{
    Window::Window(const int width, const int height) : width(width), height(height)
    {
        createWindow();
    };

    Window::~Window() {
        // Clean up OpenGL context
        if (hglrc) {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(hglrc);
        }
        
        // Release device context
        if (hdc && hwnd) {
            ReleaseDC(hwnd, hdc);
        }
        
        // Destroy window
        if (hwnd) {
            DestroyWindow(hwnd);
        }
    }

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
                swapBuffers();
            }

            // Small sleep to prevent 100% CPU usage
            Sleep(16); // ~60 FPS
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
        
        // Set up OpenGL
        setupOpenGL();
    }

    void Window::setupOpenGL() {
        // Set up pixel format for OpenGL
        PIXELFORMATDESCRIPTOR pfd = {};
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;

        int pixelFormat = ChoosePixelFormat(hdc, &pfd);
        if (pixelFormat == 0) {
            // Handle error
            return;
        }

        if (!SetPixelFormat(hdc, pixelFormat, &pfd)) {
            // Handle error
            return;
        }

        // Create OpenGL rendering context
        hglrc = wglCreateContext(hdc);
        if (hglrc == NULL) {
            // Handle error
            return;
        }

        // Make the context current
        if (!wglMakeCurrent(hdc, hglrc)) {
            // Handle error
            return;
        }

        // Set up basic OpenGL state
        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        
        // Set clear color to dark blue
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
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

    void Window::swapBuffers() {
        SwapBuffers(hdc);
    }
} // namespace ParteeEngine
