#include <Windows.h>
#include "resource.h"

LRESULT CALLBACK WindowProc(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
BOOL CALLBACK DialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    UNREFERENCED_PARAMETER(lpCmdLine);

    WNDCLASSEXW wcex;
    wcex.cbSize         = sizeof(WNDCLASSEXW); 
    wcex.style          = (CS_HREDRAW | CS_VREDRAW);                                 // https://docs.microsoft.com/en-us/windows/win32/winmsg/window-class-styles
    wcex.lpfnWndProc    = WindowProc;                                                // DefWindowProcW()
    wcex.cbClsExtra     = NULL;                                                      
    wcex.cbWndExtra     = NULL;                                                    
    wcex.hInstance      = hInstance;                                                 
    wcex.hIcon          = LoadIconW(hInstance, MAKEINTRESOURCE(WIN32_ICON));         
    wcex.hCursor        = LoadCursorW(hInstance, MAKEINTRESOURCE(STEVE_CURSOR));     // Source of awesome Steve cursor http://www.rw-designer.com/cursor-detail/139835
    wcex.hbrBackground  = (HBRUSH)COLOR_WINDOW;                                      
    wcex.lpszMenuName   = MAKEINTRESOURCEW(RES_MENU_ONE);                             
    wcex.lpszClassName  = L"MAINWINDOW";                                             
    wcex.hIconSm        = LoadIconW(hInstance, MAKEINTRESOURCE(WIN32_16X16));         

    RegisterClassExW(&wcex);
    
    DWORD ExWindowStyles = (WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES | WS_EX_TOPMOST);   // https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
    DWORD WindowStyles   = (WS_OVERLAPPEDWINDOW);                                    // https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles
    HWND WndHandle       = CreateWindowExW(ExWindowStyles, L"MAINWINDOW", L"Win32 App Template", WindowStyles, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, hInstance, NULL);
    
    ShowWindow(WndHandle, nShowCmd);
    UpdateWindow(WndHandle);
    
    MSG sMsg;

    while (GetMessageW(&sMsg, WndHandle, NULL, NULL) > 0)
    {
        TranslateMessage(&sMsg);
        DispatchMessageW(&sMsg);
    }
    return sMsg.wParam;
}

LRESULT CALLBACK WindowProc(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        AnimateWindow(hwnd, 1500, AW_ACTIVATE | AW_SLIDE | AW_HOR_NEGATIVE);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_HELP_ABOUT:
            DialogBoxW(NULL, MAKEINTRESOURCE(RES_ABOUT_DIALOG), NULL, (DLGPROC)DialogProc);
            break;
        case ID_NEW_EXIT:
            SendMessageW(hwnd, WM_CLOSE, NULL, NULL);
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    default:
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

BOOL CALLBACK DialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    HBITMAP BMP;
    HWND DlgItem = GetDlgItem(hwnd, DLG_PICTUREBOX);

    switch (Message)
    {
    case WM_CLOSE:
        EndDialog(hwnd, 0);
        return TRUE;

    case WM_INITDIALOG:
        BMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(RES_SMILE_FACE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);        
        SendDlgItemMessageW(hwnd, DLG_PICTUREBOX, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)BMP);
        return TRUE;


    // case WM_PAINT:
    //     return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            EndDialog(hwnd, IDOK);
            MessageBoxW(hwnd, L"Bye bye", L"Good bye!", MB_ICONINFORMATION | MB_OK);
            break;
        }
        break;
    default:
        return FALSE;
    }
    return TRUE;
}
