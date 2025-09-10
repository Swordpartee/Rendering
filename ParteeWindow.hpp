#include <windows.h>
#include <GL/glew.h>  // Include GLEW first, before other OpenGL headers
#include <GL/gl.h>
#include <GL/glu.h>

namespace Windowing {
    class ParteeWindow 
    {
        public:
            ParteeWindow(int width, int height);
            ~ParteeWindow();

            void show();
            void render();

            HWND getHWND() const;
            int getWidth() const;
            int getHeight() const;

        private:
            int width;
            int height;

            HWND hwnd;
            HDC hdc;
            HGLRC hrc;
            
            GLuint textureID;
            GLubyte *pixelData;
            GLuint shaderProgram;
            GLuint VAO, VBO;

            static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

            void createWindow();
            void setUpOGL(HWND hwnd);
    }; 
}