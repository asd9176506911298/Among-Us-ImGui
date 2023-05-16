// Generated C++ file by Il2CppInspector - http://www.djkaty.com - https://github.com/djkaty
// Custom injected code entry point

#include "pch-il2cpp.h"
#include "helpers.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include "il2cpp-appdata.h"

#include "GameUtility.h"

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

#define PI 3.14159265359

using namespace app;

String* CreateNETStringFromANSI(const char* string)
{
    return Marshal_PtrToStringAnsi((void*)string, NULL);
}

// Set the name of your log file here
extern const LPCWSTR LOG_FILE = L"il2cpp-log.txt";

static int randomSkinId{};
static int randomHatId{};
static int randomPetId{};
static int randomColorId{};

static int randomAngle = 0;

int NumImpostorsS = 1;

float circleRadius = 1;

bool showMenu = true;
bool noclip = false;
bool teleport = false;
bool circleRing = false;
bool random4 = false;
bool ModifyLight = false;
bool ModifySpeed = false;
bool ventMove = false;
bool EndGame = false;
bool impostorsAmount = false;
bool soloGame = false;
bool birthday = false;

float LightModifier = 1;
float SpeedModifier = 1;

void dHatManager_GetUnlockedPets(HatManager* __this, MethodInfo* method)
{
    std::cout << "HatManager_GetUnlockedPets" << std::endl;
    std::cout << HatManager_GetUnlockedPets(__this, method) << std::endl;

    //return HatManager_GetUnlockedPets(__this, method);
}

int dStatsManager_get_BanMinutesLeft(StatsManager* __this, MethodInfo* method)
{
    return 0;
}

int dGameOptionsData_GetAdjustedNumImpostors(GameOptionsData* __this, int32_t playerCount, MethodInfo* method)
{
    if(impostorsAmount)
        return NumImpostorsS;

    GameOptionsData_GetAdjustedNumImpostors(__this, playerCount, method);
}

void dInnerNetServer_EndGame(InnerNetServer* __this, MessageReader* message, InnerNetServer_Player* source, MethodInfo* method)
{
    if (!EndGame)
        return InnerNetServer_EndGame(__this, message, source, method);
}

void dGameOptionsData_Serialize(GameOptionsData* __this, BinaryWriter* writer, uint8_t version, MethodInfo* method)
{
    __this->fields.KillCooldown = 0;
    __this->fields.NumImpostors = 4;

    GameOptionsData_Serialize(__this, writer, version, method);
}

void dGameStartManager_Update(GameStartManager* __this, MethodInfo* method)
{
    if(soloGame)
        __this->fields.MinPlayers = 1;

    GameStartManager_Update(__this, method);
}

IEnumerator* dPlayerPhysics_WalkPlayerTo(PlayerPhysics* __this, Vector2 worldPos, float tolerance, MethodInfo* method)
{
    tolerance = 9999;
    return PlayerPhysics_WalkPlayerTo(__this, worldPos, tolerance, method);
}

void dKeyboardJoystick_HandleHud(MethodInfo* method)
{
    auto LocalPlayer = (*PlayerControl__TypeInfo)->static_fields->LocalPlayer;
    auto LocalPlayerPos = LocalPlayer->fields.NetTransform->fields.prevPosSent;
    auto GameOptionsData = (*PlayerControl__TypeInfo)->static_fields->GameOptions;
    //std::cout << GameOptionsData << std::endl;

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

    if (teleport && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        Vector3 mousePos = Input_get_mousePosition(NULL);
        auto mainCamera = Camera_get_main(NULL);
        Vector3 mouseVec3 = Camera_ScreenToWorldPoint_1((app::Camera*)mainCamera, mousePos, NULL);
        CustomNetworkTransform_SnapTo(LocalPlayer->fields.NetTransform, { mouseVec3.x, mouseVec3.y }, NULL);
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

    for (auto player : GetAllPlayers())
    {
        if (player != LocalPlayer)
        {
            if (circleRing)
            {
                randomAngle = rand() % 360;
                float circle_y = sin(randomAngle * PI / 180) * circleRadius;
                float circle_x = cos(randomAngle * PI / 180) * circleRadius;
                CustomNetworkTransform_SnapTo(player->fields.NetTransform, { LocalPlayerPos.x + circle_x, LocalPlayerPos.y + circle_y }, NULL);
            }

            if (random4)
            {
                randomSkinId = rand() % 15;
                randomHatId = rand() % 93;
                randomPetId = rand() % 10;
                randomColorId = rand() % 12;

                PlayerControl_RpcSetSkin(player, randomSkinId, method);
                PlayerControl_RpcSetHat(player, randomHatId, method);
                PlayerControl_RpcSetPet(player, randomPetId, method);
                PlayerControl_RpcSetColor(player, randomColorId, method);
            }
        }
    }

    if (ventMove)
        (*PlayerControl__TypeInfo)->static_fields->LocalPlayer->fields.moveable = 1;


    KeyboardJoystick_HandleHud(method);
}

void dPlayerControl_FixedUpdate(PlayerControl* __this, MethodInfo* method)
{
    if (birthday)
    {
        for(int i = 1; i <= 75; i++)
        {
            std::string birthdayStr = "This is buy from 75#8418 birthday Hack ";
            birthdayStr.append(std::to_string(i));
            PlayerControl_RpcSendChat(__this, convert_to_string(birthdayStr), NULL);
        }
        birthday = false;
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

    return CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);
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

    ImGui::SetNextWindowSize(ImVec2(620, 360));

    if(showMenu)
    {
        ImGui::Begin("75 Hack", nullptr, ImGuiWindowFlags_NoSavedSettings);
        ImGui::Checkbox("Noclip", &noclip);

        ImGui::Checkbox("RightClick Teleport", &teleport);

        ImGui::Checkbox("Light", &ModifyLight);
        ImGui::SameLine();
        ImGui::SliderFloat("Light Radius", &LightModifier, 0, 5);

        ImGui::Checkbox("Speed", &ModifySpeed);
        ImGui::SameLine();
        ImGui::SliderFloat("Speed Value", &SpeedModifier, 0, 5);

        ImGui::Checkbox("CircleRing", &circleRing);
        ImGui::SameLine();
        ImGui::SliderFloat("Circle Radius", &circleRadius, 0.1, 3.5);

        ImGui::Checkbox("Random4", &random4);

        ImGui::Checkbox("ventMove", &ventMove);

        if (ImGui::Button("EnterVent"))
        {
            PlayerPhysics_RpcEnterVent((*PlayerControl__TypeInfo)->static_fields->LocalPlayer->fields.MyPhysics, 0, NULL);
        }

        if (ImGui::Button("ExitVent"))
        {
            PlayerPhysics_RpcExitVent((*PlayerControl__TypeInfo)->static_fields->LocalPlayer->fields.MyPhysics, 0, NULL);
        }
        ImGui::Checkbox("soloGame", &soloGame);
        ImGui::Checkbox("Adjust Impostors amount", &impostorsAmount);
        ImGui::SameLine();
        ImGui::SliderInt("##NumImpostorsS", &NumImpostorsS, 1, 10);

        ImGui::Checkbox("Prevent EndGame", &EndGame);

        ImGui::Checkbox("Happy birthday", &birthday);

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
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    // Place your custom code here

    CreateD3D11Device();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourAttach((LPVOID*)&oPresent, dPresent);
    DetourAttach((LPVOID*)&KeyboardJoystick_HandleHud, dKeyboardJoystick_HandleHud);
    DetourAttach((LPVOID*)&PlayerPhysics_WalkPlayerTo, dPlayerPhysics_WalkPlayerTo);
    DetourAttach((LPVOID*)&GameStartManager_Update, dGameStartManager_Update);
    DetourAttach((LPVOID*)&GameOptionsData_Serialize, dGameOptionsData_Serialize);
    DetourAttach((LPVOID*)&InnerNetServer_EndGame, dInnerNetServer_EndGame);
    DetourAttach((LPVOID*)&GameOptionsData_GetAdjustedNumImpostors, dGameOptionsData_GetAdjustedNumImpostors);
    DetourAttach((LPVOID*)&StatsManager_get_BanMinutesLeft, dStatsManager_get_BanMinutesLeft);
    DetourAttach((LPVOID*)&HatManager_GetUnlockedPets, dHatManager_GetUnlockedPets);
    DetourAttach((LPVOID*)&PlayerControl_FixedUpdate, dPlayerControl_FixedUpdate);
    
    

    DetourTransactionCommit();
}