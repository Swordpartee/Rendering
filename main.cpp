#define UNICODE // Define UNICODE macro for wide character support
#define _UNICODE // Define _UNICODE macro for C runtime wide character support
#include <iostream> // Include standard input/output stream library
#include <windows.h> // Include Windows API header file
#include <GL/gl.h> // Include OpenGL header file
#include <cstdlib>
#include <ctime>

#pragma comment(lib, "opengl32.lib") // Link OpenGL library

int pixelArray[800][600][3]; // Declare a 2D array to hold pixel data

// Global variables for OpenGL
static HDC g_hDC = NULL;
static HGLRC g_hGLRC = NULL;
static unsigned char* g_pixelBuffer = NULL;
static int g_width = 800, g_height = 600;
static HWND g_hwnd = NULL;
static GLuint g_textureID = 0;

// Initialize OpenGL context
bool InitOpenGL(HWND hwnd) {
    g_hDC = GetDC(hwnd);
    
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        24,  // color depth
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        16,  // depth buffer
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    
    int pixelFormat = ChoosePixelFormat(g_hDC, &pfd);
    if (!pixelFormat) return false;
    
    if (!SetPixelFormat(g_hDC, pixelFormat, &pfd)) return false;
    
    g_hGLRC = wglCreateContext(g_hDC);
    if (!g_hGLRC) return false;
    
    if (!wglMakeCurrent(g_hDC, g_hGLRC)) return false;
    
    // Initialize OpenGL settings
    glViewport(0, 0, g_width, g_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    
    // Create texture
    glGenTextures(1, &g_textureID);
    glBindTexture(GL_TEXTURE_2D, g_textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // Allocate pixel buffer
    g_pixelBuffer = new unsigned char[g_width * g_height * 4];
    
    // Fill with initial gradient
    for (int y = 0; y < g_height; ++y) {
        for (int x = 0; x < g_width; ++x) {
            int idx = (y * g_width + x) * 4;
            g_pixelBuffer[idx + 0] = (unsigned char)(x / (float)g_width * 255); // Red
            g_pixelBuffer[idx + 1] = (unsigned char)(y / (float)g_height * 255); // Green
            g_pixelBuffer[idx + 2] = (unsigned char)(255 * (1 - x / (float)g_width)); // Blue
            g_pixelBuffer[idx + 3] = 255; // Alpha
        }
    }
    
    return true;
}

void CleanupOpenGL() {
    if (g_pixelBuffer) {
        delete[] g_pixelBuffer;
        g_pixelBuffer = NULL;
    }
    if (g_textureID) {
        glDeleteTextures(1, &g_textureID);
        g_textureID = 0;
    }
    if (g_hGLRC) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(g_hGLRC);
        g_hGLRC = NULL;
    }
    if (g_hDC) {
        ReleaseDC(g_hwnd, g_hDC);
        g_hDC = NULL;
    }
}

void RenderFrame() {
    // Upload pixel buffer to GPU texture
    glBindTexture(GL_TEXTURE_2D, g_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_width, g_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, g_pixelBuffer);
    
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Render textured quad
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f); // Bottom-left
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 0.0f); // Bottom-right
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f); // Top-right
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 1.0f); // Top-left
    glEnd();
    
    // Swap buffers to display
    SwapBuffers(g_hDC);
}
// Custom window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE: {
            if (!InitOpenGL(hwnd)) {
                MessageBox(hwnd, L"Failed to initialize OpenGL", L"Error", MB_OK);
                return -1;
            }
            return 0;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            RenderFrame();
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            if (g_hGLRC) {
                glViewport(0, 0, width, height);
                RenderFrame();
            }
            return 0;
        }

        case WM_KEYDOWN: {
            // Example: Press space to update pixels
            if (wParam == VK_SPACE) {
                srand((unsigned int)time(NULL));
                // Modify pixels here - example: make random colors
                for (int i = 0; i < 1000; ++i) {
                    int x = rand() % g_width;
                    int y = rand() % g_height;
                    int idx = (y * g_width + x) * 4;
                    g_pixelBuffer[idx + 0] = rand() % 256; // Red
                    g_pixelBuffer[idx + 1] = rand() % 256; // Green
                    g_pixelBuffer[idx + 2] = rand() % 256; // Blue
                    g_pixelBuffer[idx + 3] = 255; // Alpha
                }
                RenderFrame(); // Immediately render the changes
            }
            return 0;
        }

        case WM_DESTROY:
            CleanupOpenGL();
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void repaint()
{
    for (int x = 0; x < 800; x++) {
        for (int y = 0; y < 600; y++) {
            // Create a horizontal gradient from blue to red
            float ratio = (float)x / 800.0f;
            pixelArray[x][y][0] = (int)(255 * ratio);          // Red increases from left to right
            pixelArray[x][y][1] = (int)(255 * (y / 600.0f));   // Green increases from top to bottom
            pixelArray[x][y][2] = (int)(255 * (1 - ratio));    // Blue decreases from left to right
        }
    }
}

// Function to update a single pixel in the buffer
void SetPixelInBuffer(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    if (g_pixelBuffer && x >= 0 && x < g_width && y >= 0 && y < g_height) {
        int idx = (y * g_width + x) * 4;
        g_pixelBuffer[idx + 0] = r; // Red
        g_pixelBuffer[idx + 1] = g; // Green
        g_pixelBuffer[idx + 2] = b; // Blue
        g_pixelBuffer[idx + 3] = 255; // Alpha
    }
}

// Function to trigger a repaint of the window
void RefreshWindow() {
    if (g_hwnd) {
        RenderFrame();
    }
}

int main() // Entry point of the program
{
    const wchar_t CLASS_NAME[] = L"Sample Window Class"; // Define window class name as wide string

    WNDCLASSW wc = {}; // Initialize window class structure
    wc.lpfnWndProc = WindowProc; // Set custom window procedure function
    wc.hInstance = GetModuleHandle(NULL); // Get handle to current module
    wc.lpszClassName = CLASS_NAME; // Set the class name

    RegisterClassW(&wc); // Register the window class with Windows (Unicode version)

    HWND hwnd = CreateWindowEx( // Create a window and store its handle
        0, // Optional window styles
        CLASS_NAME, // Window class name 
        L"OpenGL GPU Accelerated Rendering", // Window title
        WS_OVERLAPPEDWINDOW, // Window style

        CW_USEDEFAULT, CW_USEDEFAULT, g_width, g_height, // Position and size

        NULL, // Parent window handle
        NULL, // Menu handle
        GetModuleHandle(NULL), // Instance handle
        NULL // Additional application data
    );

    // Check if window creation failed
    if (hwnd == NULL)
    {
        return 1;
    }

    g_hwnd = hwnd; // Store window handle globally

    ShowWindow(hwnd, SW_SHOW); // Display the window

    // Interface message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0; // Return success code
}