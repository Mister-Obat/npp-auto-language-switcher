#include "PluginDefinition.h"
#include <tchar.h>

NppData nppData;

HANDLE g_hModule = NULL;

extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{
    nppData = notpadPlusData;
    pluginInit(g_hModule);
}

extern "C" __declspec(dllexport) const TCHAR * getName()
{
    return TEXT("AutoLangSwitcher");
}

FuncItem funcItems[3];

void commandMenuInit()
{
// 1. Set Current as Default
    lstrcpy(funcItems[0]._itemName, TEXT("Set Current as Default"));
    funcItems[0]._pFunc = (void *)SetCurrentAsDefault;
    funcItems[0]._cmdID = 0;
    funcItems[0]._init2Check = false;
    funcItems[0]._pShKey = NULL;

    // 2. Disable / Reset Config
    lstrcpy(funcItems[1]._itemName, TEXT("Disable / Reset Config"));
    funcItems[1]._pFunc = (void *)ResetConfig;
    funcItems[1]._cmdID = 1;
    funcItems[1]._init2Check = false;
    funcItems[1]._pShKey = NULL;

    // 3. About
    lstrcpy(funcItems[2]._itemName, TEXT("About"));
    funcItems[2]._pFunc = (void *)pluginAbout;
    funcItems[2]._cmdID = 2;
    funcItems[2]._init2Check = false;
    funcItems[2]._pShKey = NULL;
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF)
{
    *nbF = 3;
    return funcItems;
}

extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
    pluginBeNotified(notifyCode);
}
extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam)
{
    return FALSE;
}

extern "C" __declspec(dllexport) BOOL isUnicode()
{
    return TRUE;
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  reasonForCall, 
                       LPVOID lpReserved
					 )
{
    switch (reasonForCall)
    {
      case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        break;
      case DLL_PROCESS_DETACH:
        pluginCleanUp();
        break;
      case DLL_THREAD_ATTACH:
      case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}
