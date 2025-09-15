#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "rendering/ParteeWindow.hpp"

namespace Rendering {

ParteeWindow::ParteeWindow(int width, int height) : width(width), height(height)
{
    createWindow();
}

ParteeWindow::~ParteeWindow()
{
    if (hdc && hwnd)
    {
        ReleaseDC(hwnd, hdc);
    }
}

void ParteeWindow::show() 
{
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
        if (m_renderCallback)
        {
            m_renderCallback();
        }
        
        // Small sleep to prevent 100% CPU usage
        Sleep(1);
    }
}

HWND ParteeWindow::getHWND() const { return hwnd; }
HDC ParteeWindow::getHDC() const { return hdc; }
int ParteeWindow::getWidth() const { return width; }
int ParteeWindow::getHeight() const { return height; }

void ParteeWindow::setRenderCallback(RenderCallback callback)
{
    m_renderCallback = callback;
}

void ParteeWindow::createWindow() 
{
    const wchar_t CLASS_NAME[] = L"Partee Window Class";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // Added styles for OpenGL
    wc.hbrBackground = NULL;  // No background for OpenGL

    RegisterClassW(&wc);

    // Store this pointer for the window
    hwnd = CreateWindowExW(
        0, CLASS_NAME, L"Partee Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, GetModuleHandle(NULL), this
    );
    if (!hwnd) {
        MessageBoxW(NULL, L"Failed to create window", L"Error", MB_OK);
    }

    hdc = GetDC(hwnd);
    if (!hdc)
    {
        MessageBoxW(NULL, L"Failed to get device context", L"Error", MB_OK);
        return;
    }
}

LRESULT CALLBACK ParteeWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ParteeWindow* window = nullptr;
    
    if (uMsg == WM_CREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = reinterpret_cast<ParteeWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    } else {
        window = reinterpret_cast<ParteeWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    
    switch (uMsg)
    {
    case WM_CREATE:
        return 0;
    case WM_SIZE:
        if (window) {
            // Resize OpenGL viewport
            glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

} // namespace Rendering