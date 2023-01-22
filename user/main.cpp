// Generated C++ file by Il2CppInspector - http://www.djkaty.com - https://github.com/djkaty
// Custom injected code entry point

#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include "il2cpp-appdata.h"


#include "imgui\imgui.h"
#include "imgui\imgui_impl_dx11.h"
#include "imgui\imgui_impl_win32.h"

#include <d3d11.h>
#include <dxgi.h>
#pragma comment(lib, "d3d11.lib")

#include "detours\detours.h"

static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;


using namespace app;

String* CreateNETStringFromANSI(const char* string)
{
    return Marshal_PtrToStringAnsi((void*)string, NULL);
}

// Set the name of your log file here
extern const LPCWSTR LOG_FILE = L"il2cpp-log.txt";

bool showMenu = true;
bool noclip = false;
bool ModifyLight = false;
bool ModifySpeed = false;
float LightModifier = 1;
float SpeedModifier = 1;
void dPlayerControl_FixedUpdate(PlayerControl* __this, MethodInfo* method)
{
    if (noclip)
    {
        auto comp = Component_get_gameObject((Component*)(*PlayerControl__TypeInfo)->static_fields->LocalPlayer, NULL);
        GameObject_set_layer(comp, LayerMask_NameToLayer(CreateNETStringFromANSI("Ghost"), NULL), NULL);
    }
    else
    {
        auto comp = Component_get_gameObject((Component*)(*PlayerControl__TypeInfo)->static_fields->LocalPlayer, NULL);
        GameObject_set_layer(comp, LayerMask_NameToLayer(CreateNETStringFromANSI("Players"), NULL), NULL);
    }

    if (ModifyLight)
    {
        (*PlayerControl__TypeInfo)->static_fields->GameOptions->fields.ImpostorLightMod = LightModifier;
        (*PlayerControl__TypeInfo)->static_fields->GameOptions->fields.CrewLightMod = LightModifier;
    }
    else
    {
        (*PlayerControl__TypeInfo)->static_fields->GameOptions->fields.ImpostorLightMod = 1.5;
        (*PlayerControl__TypeInfo)->static_fields->GameOptions->fields.CrewLightMod = 1.0;
    }

    if (ModifySpeed)
    {
        (*PlayerControl__TypeInfo)->static_fields->GameOptions->fields.PlayerSpeedMod = SpeedModifier;
    }
    else
    {
        (*PlayerControl__TypeInfo)->static_fields->GameOptions->fields.PlayerSpeedMod = 1;
    }


    PlayerControl_FixedUpdate(__this, method);
}

typedef HRESULT(__stdcall* Present)(IDXGISwapChain* This,UINT SyncInterval,UINT Flags);
Present oPresent;
HWND window;
WNDPROC oWndProc;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    if (GetAsyncKeyState(VK_INSERT) & 1)
        showMenu = !showMenu;

    CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall dPresent(IDXGISwapChain* This, UINT SyncInterval, UINT Flags)
{
    if (!init)
    {
        if(SUCCEEDED(This->GetDevice(__uuidof(ID3D11Device), (void**)&g_pd3dDevice)))
        {
            g_pd3dDevice->GetImmediateContext(&g_pd3dDeviceContext);
            DXGI_SWAP_CHAIN_DESC sd;
            This->GetDesc(&sd);
            window = sd.OutputWindow;
            ID3D11Texture2D* pBackBuffer;
            This->GetBuffer(0, __uuidof(ID3D11Texture2D),(void**)(&pBackBuffer));
            pBackBuffer->Release();
            g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
            ImGui::CreateContext();
            oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            ImGui::StyleColorsDark();
            ImGui_ImplWin32_Init(window);
            ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
            init = true;
        }
        else
            return oPresent(This, SyncInterval, Flags);
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::SetWindowSize(ImVec2(520, 100));

    if(showMenu)
    {
        ImGui::Begin("Test");
        ImGui::Checkbox("Noclip", &noclip);

        ImGui::Checkbox("Light", &ModifyLight);
        ImGui::SameLine();
        ImGui::SliderFloat("Light Radius", &LightModifier, 0, 5);

        ImGui::Checkbox("Speed", &ModifySpeed);
        ImGui::SameLine();
        ImGui::SliderFloat("Speed Value", &SpeedModifier, 0, 5);
        ImGui::End();
    }

    ImGui::Render();
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return oPresent(This, SyncInterval, Flags);
}

bool CreateD3D11Device()
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = GetForegroundWindow();
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL featureLevel;
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, nullptr, 0, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, nullptr) != S_OK)
        return false;

    void** vTable = *(void***)g_pSwapChain;
    oPresent = (Present)vTable[8];

    g_pSwapChain->Release();
    g_pd3dDevice->Release();

    return true;
}

// Custom injected code entry point
void Run()
{
    // Initialize thread data - DO NOT REMOVE
    //il2cpp_thread_attach(il2cpp_domain_get());

    // If you would like to write to a log file, specify the name above and use il2cppi_log_write()
    // il2cppi_log_write("Startup");

    // If you would like to output to a new console window, use il2cppi_new_console() to open one and redirect stdout
    // il2cppi_new_console();

    // Place your custom code here

    CreateD3D11Device();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourAttach((LPVOID*)&oPresent, dPresent);
    DetourAttach((LPVOID*)&PlayerControl_FixedUpdate, dPlayerControl_FixedUpdate);

    DetourTransactionCommit();
}