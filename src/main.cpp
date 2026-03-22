#include <windows.h>
#include <d3d11.h>
#include <tchar.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "dll_injector.hpp"
#include <tlhelp32.h>
#include <vector>
#include <string>
#include <algorithm>

typedef struct s_process
{
    std::string name;
    DWORD pid;
} t_process;

std::vector<t_process> get_process_list()
{
    std::vector<t_process> list;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        return list;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &pe))
    {
        do
        {
            t_process p;
            p.name = pe.szExeFile;
            p.pid = pe.th32ProcessID;
            list.push_back(p);
        }
        while (Process32Next(snapshot, &pe));
        std::sort(list.begin(), list.end(),
        [](const t_process &a, const t_process &b)
        {
            return a.name < b.name;
        });
    }

    CloseHandle(snapshot);
    return list;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

static void cleanup_render_target()
{
    if (g_mainRenderTargetView)
    {
        g_mainRenderTargetView->Release();
        g_mainRenderTargetView = NULL;
    }
}

static void create_render_target()
{
    ID3D11Texture2D* pBackBuffer = NULL;
    if (g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)) != S_OK)
        return;
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

static bool create_device_d3d(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 2;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;

    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
        NULL, 0, NULL, 0, D3D11_SDK_VERSION,
        &sd, &g_pSwapChain, &g_pd3dDevice,
        NULL, &g_pd3dDeviceContext) != S_OK)
        return false;

    create_render_target();
    return true;
}

static void cleanup_device_d3d()
{
    cleanup_render_target();
    if (g_pSwapChain) g_pSwapChain->Release();
    if (g_pd3dDeviceContext) g_pd3dDeviceContext->Release();
    if (g_pd3dDevice) g_pd3dDevice->Release();
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;
    if (msg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int main()
{
    WNDCLASSEX wc = { sizeof(wc), CS_CLASSDC, WndProc, 0, 0,
        GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
        _T("Injector"), NULL };
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, _T("DLL Injector"),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        100, 100, 600, 600,
        NULL, NULL, wc.hInstance, NULL);

    if (!create_device_d3d(hwnd))
        return 1;

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 12.0f;
    style.FrameRounding = 10.0f;
    style.GrabRounding = 8.0f;
    style.ScrollbarRounding = 10.0f;
    style.FramePadding = ImVec2(10, 6);

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);

    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);

    style.Colors[ImGuiCol_Button]        = ImVec4(0.60f, 0.00f, 1.00f, 0.70f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.75f, 0.20f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]  = ImVec4(0.50f, 0.00f, 0.80f, 1.00f);

    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.60f, 0.00f, 1.00f, 0.70f);

    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 0.40f, 0.3f);

    ImGuiIO& io = ImGui::GetIO();

    ImFont* font_bold = io.Fonts->AddFontFromFileTTF(
        "C:/Windows/Fonts/segoeuib.ttf", 18.0f);

    if (!font_bold)
        font_bold = io.Fonts->AddFontDefault();

    io.FontGlobalScale = 1.3f;

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    char dll_path[260] = "";
    DWORD pid = 0;

    MSG msg;
    std::vector<t_process> process_list = get_process_list();
    while (true)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                goto end;
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

        ImGui::Begin("Dll_injector",
            NULL,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove);

        float width = ImGui::GetContentRegionAvail().x;
        ImGui::Text("Selected PID: %ld", pid);

        ImGui::BeginChild("process_list", ImVec2(0, 300), true);
        for (size_t i = 0; i < process_list.size(); i++)
        {
            std::string label = process_list[i].name + " (" + std::to_string(process_list[i].pid) + ")";

            if (ImGui::Selectable(label.c_str(), pid == process_list[i].pid))
                pid = process_list[i].pid;
        }
        ImGui::EndChild();

        if (ImGui::Button("Refresh"))
        {
            process_list = get_process_list();
        }

        ImGui::PushItemWidth(width);
        ImGui::Text("Enter the path of the DLL to inject");
        ImGui::InputText("##dll", dll_path, 260);
        ImGui::PopItemWidth();

        ImGui::PushFont(font_bold);
        if (ImGui::Button("Inject", ImVec2(width, 45)))
        {
            if (pid > 0 && dll_path[0] != '\0')
                inject_dll(pid, dll_path);
        }
        ImGui::PopFont();

        ImGui::End();

        ImGui::Render();
        float clear_color[4] = {0.85f, 0.92f, 1.0f, 1.0f};
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }

end:
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    cleanup_device_d3d();
    DestroyWindow(hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);
    return 0;
}