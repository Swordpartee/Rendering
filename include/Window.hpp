#pragma once

#include <windows.h>
#include <functional>
#include <GL/gl.h>

namespace ParteeEngine {
    class Window {
        public:
            using RenderCallback = std::function<void()>;

            Window(const int width, const int height);
            ~Window();

            void show();
            void setRenderCallback(RenderCallback callback);
            void swapBuffers();

            HWND getHWND() const;
            HDC getHDC() const;
            int getWidth() const;
            int getHeight() const;

        private:
            const int width;
            const int height;

            HWND hwnd;
            HDC hdc;
            HGLRC hglrc; // OpenGL rendering context
            RenderCallback renderCallback;

            static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

            void createWindow();
            void setupOpenGL();
    };

} // namespace ParteeEngine