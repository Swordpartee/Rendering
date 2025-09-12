#pragma once

#include <windows.h>
#include <functional>

namespace Rendering {
    class ParteeWindow 
    {
        public:
            using RenderCallback = std::function<void()>;
            
            ParteeWindow(int width, int height);
            ~ParteeWindow();

            void show();
            void setRenderCallback(RenderCallback callback);

            HWND getHWND() const;
            HDC getHDC() const;
            int getWidth() const;
            int getHeight() const;

        private:
            int width;
            int height;

            HWND hwnd;
            HDC hdc;
            RenderCallback m_renderCallback;
            
            static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

            void createWindow();
    }; 
}