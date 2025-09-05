#define UNICODE // Define UNICODE macro for wide character support
#define _UNICODE // Define _UNICODE macro for C runtime wide character support
#include <iostream> // Include standard input/output stream library
#include <windows.h> // Include Windows API header file
#include <wingdi.h> // Include Windows GDI header file

// Custom window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Set a pixel at coordinates (100, 100) to black color
        // SetPixel(hdc, 100, 100, RGB(0, 0, 0));
        HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0)); // Create a solid black brush
        RECT rect = { 50, 50, 200, 200 }; // Define a rectangle from (50,50) to (200,200)

        FillRect(hdc, &rect, brush); // Fill the rectangle with the black brush
        
        DeleteObject(brush); // Clean up the brush object

        EndPaint(hwnd, &ps);
    }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
        L"Learn to Program Windows", // Window title
        WS_OVERLAPPEDWINDOW, // Window style

        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, // Default position and size

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