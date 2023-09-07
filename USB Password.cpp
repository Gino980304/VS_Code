#include <windows.h>
#include <string>

// 函數原型
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 創建主視窗
    HWND hwnd;
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("PasswordApp"), NULL };
    RegisterClassEx(&wc);
    hwnd = CreateWindow(wc.lpszClassName, _T("Password App"), WS_OVERLAPPEDWINDOW, 100, 100, 400, 200, NULL, NULL, wc.hInstance, NULL);
    
    // 創建密碼輸入框
    HWND passwordEdit = CreateWindow(_T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | ES_PASSWORD, 150, 50, 150, 30, hwnd, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // 主訊息迴圈
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static std::wstring password = _T("your_password"); // 設定你的密碼
    static std::wstring drive;

    switch (msg) {
        case WM_CREATE:
            // 自動取得磁碟機代號
            {
                TCHAR buffer[MAX_PATH];
                if (GetModuleFileName(NULL, buffer, MAX_PATH)) {
                    drive = buffer[0];
                }
            }
            break;
        case WM_COMMAND:
            if (HIWORD(wParam) == EN_CHANGE) {
                // 當密碼輸入框內容改變時
                TCHAR inputPassword[100];
                GetWindowText(GetDlgItem(hwnd, 1001), inputPassword, 100);
                if (password == inputPassword) {
                    // 密碼正確，打開隱藏資料夾
                    std::wstring folderPath = drive + _T(":\\隱藏資料夾");
                    ShellExecute(NULL, _T("open"), folderPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                }
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
