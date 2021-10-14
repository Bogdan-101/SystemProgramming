// Lab2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab2.h"

#define MAX_LOADSTRING 100

#define LEFT_LIST 1
#define RIGHT_LIST 2
#define ADD_BUTTON 3
#define CLEAR_BUTTON 4
#define SEND_TO_RIGHT 5
#define DELETE_BUTTON 6
#define EDIT_BUTTON 7

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB2));

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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB2));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB2);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 960, 540, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rc;

    static HWND hLeftListBox;
    static HWND hRightListBox;
    static HWND hAddButton;
    static HWND hClearButton;
    static HWND hToRightButton;
    static HWND hDeleteButton;
    static HWND hEdit;

    switch (message)
    {
    case WM_CREATE:
    {
        GetClientRect(hWnd, &rc);
        hLeftListBox = CreateWindow(L"listbox", NULL,
            WS_CHILD | WS_VISIBLE | LBS_STANDARD |
            LBS_WANTKEYBOARDINPUT,
            rc.left + 30, 30, 200, 100,
            hWnd, (HMENU)LEFT_LIST, hInst, NULL);

        hRightListBox = CreateWindow(L"listbox", NULL,
            WS_CHILD | WS_VISIBLE | LBS_STANDARD |
            LBS_WANTKEYBOARDINPUT,
            rc.left + 280, 30, 200, 100,
            hWnd, (HMENU)RIGHT_LIST, hInst, NULL);

        hAddButton = CreateWindow(L"button", L"Добавить",
            WS_CHILD | WS_VISIBLE |
            BS_PUSHBUTTON,
            700, 80, 150, 25,
            hWnd, (HMENU)ADD_BUTTON, hInst, NULL);

        hClearButton = CreateWindow(L"button", L"Очистить",
            WS_CHILD | WS_VISIBLE |
            BS_PUSHBUTTON,
            530, 80, 150, 25,
            hWnd, (HMENU)CLEAR_BUTTON, hInst, NULL);

        hToRightButton = CreateWindow(L"button", L"Перенести вправо",
            WS_CHILD | WS_VISIBLE |
            BS_PUSHBUTTON,
            700, 30, 150, 25,
            hWnd, (HMENU)SEND_TO_RIGHT, hInst, NULL);

        hDeleteButton = CreateWindow(L"button", L"Удалить",
            WS_CHILD | WS_VISIBLE |
            BS_PUSHBUTTON,
            530, 30, 150, 25,
            hWnd, (HMENU)DELETE_BUTTON, hInst, NULL);

        hEdit = CreateWindow(L"edit", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER |
            ES_LEFT,
            30, 230, 450, 20,
            hWnd, (HMENU)EDIT_BUTTON, hInst, NULL);
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        switch (wmId)
        {
        case ADD_BUTTON:
            TCHAR chBuff[80];
            WORD cbText;

            cbText = SendMessage(hEdit, EM_GETLINE, 0,
                (LPARAM)(LPSTR)chBuff);

            chBuff[cbText] = '\0';
            if (SendMessage(hLeftListBox, LB_FINDSTRINGEXACT, 0, (LPARAM)(LPSTR)chBuff) == LB_ERR)
                SendMessage(hLeftListBox, LB_ADDSTRING, 0, (LPARAM)(LPSTR)chBuff);
            break;
        case CLEAR_BUTTON:
            SendMessage(hLeftListBox, LB_RESETCONTENT, 0, 0L);
            SendMessage(hRightListBox, LB_RESETCONTENT, 0, 0L);
            break;
        case SEND_TO_RIGHT:
            INT pos;

            pos = SendMessage(hLeftListBox, LB_GETCURSEL, 0, 0L);
            if (pos == LB_ERR)
                break;

            *(WORD*)chBuff = sizeof(chBuff) - 1;

            SendMessage(hLeftListBox, LB_GETTEXT, (WPARAM)pos, (LPARAM)(LPSTR)chBuff);
            if (SendMessage(hRightListBox, LB_FINDSTRINGEXACT, 0, (LPARAM)(LPSTR)chBuff) == LB_ERR)
                SendMessage(hRightListBox, LB_ADDSTRING, 0, (LPARAM)(LPSTR)chBuff);
            break;
        case DELETE_BUTTON:
            pos = SendMessage(hLeftListBox, LB_GETCURSEL, 0, 0L);
            if (pos != LB_ERR) {
                SendMessage(hLeftListBox, LB_DELETESTRING, (WPARAM)pos, 0L);
            }
            pos = SendMessage(hRightListBox, LB_GETCURSEL, 0, 0L);
            if (pos != LB_ERR) {
                SendMessage(hRightListBox, LB_DELETESTRING, (WPARAM)pos, 0L);
            }
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
