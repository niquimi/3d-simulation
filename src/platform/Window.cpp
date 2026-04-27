#include "Window.hpp"
#include "../renderer/Renderer.hpp"

Window::Window(HINSTANCE hInstance, PhysicsWorld* w, Camera* c)
    : world(w), cam(c), running(true)
{
    const wchar_t CLASS_NAME[] = L"PhysicsWindow";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = Window::WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Physics Simulation",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL,
        NULL,
        hInstance,
        this
    );

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

    ShowWindow(hwnd, SW_SHOW);
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (!window) return DefWindowProc(hwnd, uMsg, wParam, lParam);

    window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (uMsg) {

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rc;
        GetClientRect(hwnd, &rc);
        int w = rc.right - rc.left;
        int h = rc.bottom - rc.top;

        // Crear buffer en memoria
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, w, h);
        HGDIOBJ oldBitmap = SelectObject(memDC, memBitmap);

        // Dibujar en el buffer
        clear(memDC, rc);
        drawCube(memDC, window->world->getCubeSize(), *window->cam, w, h);
        drawSphere(memDC, window->world->getSphere(), *window->cam, w, h);

        // Volcar buffer a pantalla de golpe
        BitBlt(hdc, 0, 0, w, h, memDC, 0, 0, SRCCOPY);

        // Limpiar
        SelectObject(memDC, oldBitmap);
        DeleteObject(memBitmap);
        DeleteDC(memDC);

        EndPaint(hwnd, &ps);
        return 0;
    }

    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Window::run() {
    MSG msg = {};

    LARGE_INTEGER freq, prev, current;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&prev);

    while (running) {

        // Procesar mensajes
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Delta time
        QueryPerformanceCounter(&current);
        float dt = (float)(current.QuadPart - prev.QuadPart) / freq.QuadPart;
        prev = current;

        // Update física
        world->update(dt);
        cam->updateBasis();

        // Redibujar
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
    }
}