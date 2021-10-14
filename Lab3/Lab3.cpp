// Lab3.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab3.h"
#include <windows.h> 
#include <tchar.h> 
#include <string.h> 
#include "math.h"
#include <stdlib.h> 
#define FIRST_TIMER 1 
#define MAX_LOADSTRING 100 
// Глобальные переменные: 
HINSTANCE hInst; // текущий экземпляр 
WCHAR szTitle[MAX_LOADSTRING]; // Текст строки заголовка 
WCHAR szWindowClass[MAX_LOADSTRING]; // имя класса главного окна
static HWND BtnPlace;
static HWND BtnMove;
static HWND BtnClear;
int x, y;
int nTimerID;
bool flag1 = true, flag2 = false, flag3 = false, flag4 = false, flag5 = false;
int dir = 1; // направление движения изображения. 
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
void drawHouse(HDC, int, int);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // Инициализация глобальных строк 
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    // Выполнить инициализацию приложения: 
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB3));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}
// ФУНКЦИЯ: MyRegisterClass() 
// ЦЕЛЬ: Регистрирует класс окна. 
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_LAB3));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB3);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}
// ФУНКЦИЯ: InitInstance(HINSTANCE, int) 
// ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно 
// КОММЕНТАРИИ: 
// В этой функции маркер экземпляра сохраняется в глобальной переменной, а также 
// создается и выводится главное окно программы. 
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной 
    HWND hWnd = CreateWindowW(szWindowClass, L"Paint", WS_OVERLAPPEDWINDOW, 250, 200, 900, 500, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd)
    {
        return FALSE;
    }
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    // Создаем кнопки Place, Move и Clear 
    BtnPlace = CreateWindowW(L"button", L"Place", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 10, 80, 30, hWnd, (HMENU)10000, hInstance, nullptr);
    ShowWindow(BtnPlace, SW_SHOWNORMAL);
    BtnMove = CreateWindowW(L"button", L"Move", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 90, 10, 80, 30, hWnd, (HMENU)10000, hInstance, nullptr);
    ShowWindow(BtnMove, SW_SHOWNORMAL);
    BtnClear = CreateWindowW(L"button", L"Clear", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 170, 10, 80, 30, hWnd, (HMENU)10000, hInstance, nullptr);
    ShowWindow(BtnClear, SW_SHOWNORMAL);
    nTimerID = SetTimer(hWnd, FIRST_TIMER, 200, NULL);
    return TRUE;
}
// ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM) 
// ЦЕЛЬ: Обрабатывает сообщения в главном окне. 
// WM_COMMAND - обработать меню приложения 
// WM_PAINT - Отрисовка главного окна 
// WM_DESTROY - отправить сообщение о выходе и вернуться 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hWnd);
    int width = 100, height = 100;
    switch (message)
    {
    case WM_TIMER:
    {
        if (wParam == FIRST_TIMER)
        {
            if (flag5)
            {
                InvalidateRect(hWnd, NULL, TRUE);
                UpdateWindow(hWnd);
                drawHouse(hdc, width, height);
                UpdateWindow(hWnd);
                if (x > 760 || x < 10)
                {
                    dir *= -1;
                }
                x += dir * 10;
            }
        }
        break;
    case WM_COMMAND:
    {
        if (lParam == (LPARAM)BtnPlace) // если нажали на кнопку 		{
            if (flag1)
            {
                flag4 = true;
            }
            else
            {
                MessageBox(hWnd, L"Clear the application window.", L"Error", MB_OK);
                return NULL;
            }
        else if (lParam == LPARAM(BtnMove))
        {
            if (flag2)
            {
                flag5 = true;
            }
            else
            {
                MessageBox(hWnd, L"Paint the picture.", L"Error", MB_OK);
                return NULL;
            }
        }
        else if (lParam == LPARAM(BtnClear))
        {
            if (flag3)
            {
                InvalidateRect(hWnd, NULL, TRUE);
                UpdateWindow(hWnd);
                flag1 = true;
                flag2 = flag3 = flag4 = flag5 = false;
            }
            else
            {
                MessageBox(hWnd, L"Paint the picture.", L"Error", MB_OK);
                return NULL;
            }
    }
        // Разобрать выбор в меню: 
        switch (LOWORD(wParam))
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_LBUTTONDOWN:
    {
        if (flag4)
        {
            x = LOWORD(lParam) - width / 2, y = HIWORD(lParam) - height / 2;
            drawHouse(hdc, width, height);
            flag1 = flag4 = false;
            flag2 = flag3 = true;
        }
    }
    break;
    case WM_DESTROY:
        KillTimer(hWnd, FIRST_TIMER);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
return 0;
}
// Обработчик сообщений для окна "О программе". 
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void drawHouse(HDC hdc, int width, int height)
{
    width = width / 2;
    height = height * 1.5;
    HPEN hpen = CreatePen(PS_SOLID, 10, RGB(3, 3, 3));
    HBRUSH hbrush = CreateSolidBrush(RGB(10, 100, 250));
    SelectObject(hdc, hbrush);
    Rectangle(hdc, x, y, x + width, y + height);
    hbrush = CreateSolidBrush(RGB(50, 20, 150));
    SelectObject(hdc, hbrush);
    Rectangle(hdc, x + width / 4, y + height / 2, x + (width / 2), y + (height / 1.25));
    hbrush = CreateSolidBrush(RGB(90, 60, 10));
    SelectObject(hdc, hbrush);	Rectangle(hdc, x + (2 * width / 3), y + (2 * height / 3), x + (7 * width / 8), y + height);
    hbrush = CreateSolidBrush(RGB(150, 30, 40));
    SelectObject(hdc, hbrush);
    POINT point[3]{};

    point[0].x = x - 20;

    point[0].y = y + 5;

    point[1].x = x + width / 2;

    point[1].y = y - height / 3 - 10;

    point[2].x = x + width + 20;

    point[2].y = y + 5;

    Polygon(hdc, point, 3);
}

