#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <set>
#include "PluginInterface.h"
#include "PluginDefinition.h"

#pragma comment(lib, "comctl32.lib")

// Tracking buffers to avoid re-forcing language on tab switch
std::set<void*> processedBuffers;

/*
    AutoLangSwitcher - Development & Debugging History
    ==================================================

    Challenges Encountered & Solutions:
    -----------------------------------
    1. Architecture Mismatch:
       - Issue: Linker errors when building 32-bit DLL for 64-bit Notepad++.
       - Fix: User must use "x64 Native Tools Command Prompt".

    2. Startup Crashes (Access Violation):
       - Issue: Notepad++ crashed immediately on load.
       - Cause: `FuncItem` struct alignment mismatch. `ShortcutKey` pointer was defined as `TCHAR` in initial code.
       - Fix: Corrected `FuncItem` definition to match Notepad++'s expected memory layout.

    3. Permission Issues (Settings File):
       - Issue: "Settings" menu did nothing; INI file not created.
       - Cause: Plugin tried to write to `Program Files` (DLL directory).
       - Fix: Switched to `NPPM_GETPLUGINSCONFIGDIR` (%AppData%) to ensure write permissions.

    4. Character Encoding (Chinese Characters):
       - Issue: `GetCurrentLanguageName` returned garbage/Chinese characters.
       - Cause: Notepad++ API returns ANSI strings for language names, even in Unicode builds.
       - Fix: Implemented `MultiByteToWideChar` conversion.
       - Final Fix: Switched to `NPPM_SETCURRENTLANGUAGETYPE` (Integer IDs) to avoid string fragility entirely.

    5. Input Glitches (Mouse/Right-Click Spam):
       - Issue 1: `messageProc` returned `FALSE`, swallowing Windows messages. -> Fix: Return `TRUE`.
       - Issue 2: Infinite Loop. `SetLanguage` triggered `NPPN_BUFFERACTIVATED`, which triggered `SetLanguage` again...
       - Fix: 
         a) Added check `if (currentLang != targetLang)` to prevent redundant updates.
         b) Disabled `NPPN_BUFFERACTIVATED` trigger (too volatile).
         c) Disabled `NPPN_READY` trigger (caused startup right-click glitch).

    Valid Triggers Used:
    --------------------
    - NPPN_FILEOPENED: Best time to apply language (file loaded).
    - NPPN_DOCCREATED: When creating a new empty file.
    - NPPN_FILESAVED: To reload settings dynamically.
*/

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

extern NppData nppData;
TCHAR iniFilePath[MAX_PATH];
int targetLanguageID = 0;
TCHAR targetLanguageName[MAX_PATH] = {0};
bool isNppReady = false;
bool pendingNewDoc = false;

void pluginInit(HANDLE hModule)
{
    // Use Notepad++ Config Dir (AppData) - SAFEST for permissions
    TCHAR configDir[MAX_PATH];
    ::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)configDir);
    
    // Create directory (standard CreateDirectory is enough if parent exists, skipping SHCreateDirectoryEx to avoid linking headache)
    ::CreateDirectory(configDir, NULL);

    wsprintf(iniFilePath, TEXT("%s\\AutoLangSwitcher_Config.ini"), configDir);
    
    // Default to Normal Text (0) or C++ (1) if missing
    if (!PathFileExists(iniFilePath))
    {
        WritePrivateProfileString(TEXT("Settings"), TEXT("TargetLanguageID"), TEXT("1"), iniFilePath);
    }
    
    LoadSettings();
    commandMenuInit();
}

void pluginCleanUp()
{
    // Cleanup if needed
}

void commandMenuCleanUp()
{
    // Cleanup menu
}

void LoadSettings()
{
    targetLanguageID = GetPrivateProfileInt(TEXT("Settings"), TEXT("TargetLanguageID"), 1, iniFilePath);
    GetPrivateProfileString(TEXT("Settings"), TEXT("TargetLanguageName"), TEXT(""), targetLanguageName, MAX_PATH, iniFilePath);
}

void OpenSettings()
{
    // Ensure it exists
    if (!PathFileExists(iniFilePath))
    {
        WritePrivateProfileString(TEXT("Settings"), TEXT("TargetLanguageID"), TEXT("1"), iniFilePath);
    }
    ShellExecute(nppData._nppHandle, TEXT("open"), iniFilePath, NULL, NULL, SW_SHOW);
}


void SetUDLByName(const TCHAR* name)
{
    int startID = 0;
    int count = (int)::SendMessage(nppData._nppHandle, NPPM_GETNBUSERLANG, 0, (LPARAM)&startID);
    
    HMENU hMainMenu = (HMENU)::SendMessage(nppData._nppHandle, NPPM_GETMENUHANDLE, NPPMAINMENU, 0);
    if (!hMainMenu) return;

    // Handle "udf - " prefix which Notepad++ adds to the internal name
    const TCHAR* searchName = name;
    if (lstrlen(name) > 6 && _tcsncmp(name, TEXT("udf - "), 6) == 0)
    {
        searchName = name + 6;
    }

    TCHAR buffer[MAX_PATH];
    for (int i = 0; i < count; ++i)
    {
        int cmdID = startID + i;
        if (GetMenuString(hMainMenu, cmdID, buffer, MAX_PATH, MF_BYCOMMAND))
        {
            // STRICT MATCH with the stripped name
            if (lstrlen(buffer) > 0 && lstrcmp(buffer, searchName) == 0)
            {
                ::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, cmdID);
                return;
            }
        }
    }
}

void checkAndForceLanguage()
{
    if (!isNppReady || !nppData._nppHandle) return;
    
    // -1 means Disabled. Do nothing.
    if (targetLanguageID == -1) return;

    // Prevent infinite loop: Only set if different
    int currentLang = 0;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTLANGUAGETYPE, 0, (LPARAM)&currentLang);
    
    // DEBUG: Popup to see if we are trying to switch
    // TCHAR debugMsg[256];
    // wsprintf(debugMsg, TEXT("checking... Current: %d vs Target: %d"), currentLang, targetLanguageID);
    // ::MessageBox(nppData._nppHandle, debugMsg, TEXT("Switch Debug"), MB_OK);

    // If target is UDL (15), check name match
    if (targetLanguageID == 15 && lstrlen(targetLanguageName) > 0)
    {
        TCHAR currentName[MAX_PATH] = {0};
        ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTLANGUAGENAME, (WPARAM)currentLang, (LPARAM)currentName);
        
        // Handle prefix for comparison
        const TCHAR* cmpCurrent = currentName;
        if (lstrlen(currentName) > 6 && _tcsncmp(currentName, TEXT("udf - "), 6) == 0) cmpCurrent += 6;

        if (lstrcmp(cmpCurrent, targetLanguageName) != 0)
        {
            // ::MessageBox(nppData._nppHandle, TEXT("Switching UDL!"), TEXT("Debug"), MB_OK);
            SetUDLByName(targetLanguageName);
        }
    }
    else if (currentLang != targetLanguageID)
    {
        // ::MessageBox(nppData._nppHandle, TEXT("Switching Standard IDs!"), TEXT("Debug"), MB_OK);
        ::SendMessage(nppData._nppHandle, NPPM_SETCURRENTLANGUAGETYPE, 0, (LPARAM)targetLanguageID);
    }
}

void ManualApply()
{
    LoadSettings();
    checkAndForceLanguage();
}

void SetCurrentAsDefault()
{
    if (!nppData._nppHandle) return;

    int langType = 0;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTLANGUAGETYPE, 0, (LPARAM)&langType);
    
    // Buffer for language name
    TCHAR langName[MAX_PATH] = {0};
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTLANGUAGENAME, (WPARAM)langType, (LPARAM)langName);

    // Save to INI
    TCHAR idStr[16];
    wsprintf(idStr, TEXT("%d"), langType);
    WritePrivateProfileString(TEXT("Settings"), TEXT("TargetLanguageID"), idStr, iniFilePath);
    WritePrivateProfileString(TEXT("Settings"), TEXT("TargetLanguageName"), langName, iniFilePath);

    // Reload settings immediately
    LoadSettings();
    
    TCHAR msg[512];
    wsprintf(msg, TEXT("Langage par défaut défini sur :\n\n%s (ID: %d)\n\nSauvegardé dans : %s"), langName, langType, iniFilePath);
    ::MessageBox(nppData._nppHandle, msg, TEXT("Configuration Sauvegardée !"), MB_OK);
}

void ResetConfig()
{
    // Write -1 to Disable
    WritePrivateProfileString(TEXT("Settings"), TEXT("TargetLanguageID"), TEXT("-1"), iniFilePath);
    WritePrivateProfileString(TEXT("Settings"), TEXT("TargetLanguageName"), TEXT(""), iniFilePath);
    
    LoadSettings();
    
    ::MessageBox(nppData._nppHandle, TEXT("Le plugin est maintenant DÉSACTIVÉ.\n(Aucun langage ne sera forcé automatiquement)"), TEXT("AutoLangSwitcher"), MB_OK);
}

// Callback to handle the link click in TaskDialog
HRESULT CALLBACK TaskDialogCallbackProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LONG_PTR lpRefData)
{
    if (msg == TDN_HYPERLINK_CLICKED)
    {
        LPCWSTR url = (LPCWSTR)lParam;
        ::ShellExecute(NULL, TEXT("open"), url, NULL, NULL, SW_SHOWNORMAL);
    }
    return S_OK;
}

void pluginAbout()
{
    TASKDIALOGCONFIG config = {0};
    config.cbSize = sizeof(config);
    config.hwndParent = nppData._nppHandle;
    config.hInstance = NULL;
    config.dwFlags = TDF_ENABLE_HYPERLINKS | TDF_ALLOW_DIALOG_CANCELLATION;
    config.pszWindowTitle = TEXT("About AutoLangSwitcher");
    config.pszMainIcon = TD_INFORMATION_ICON;
    
    // Using simple string concatenation for the dynamic content
    TCHAR text[256];
    wsprintf(text, TEXT("AutoLangSwitcher v1.3\nCreated by Obat\n\nCurrent Target ID: %d"), targetLanguageID);
    config.pszContent = text;
    
    config.pszFooter = TEXT("<A HREF=\"https://www.paypal.com/paypalme/creaprisme\">Make a donation</A>");
    config.pszFooterIcon = TD_INFORMATION_ICON; // Optional icon for footer
    config.pfCallback = TaskDialogCallbackProc;
    config.dwCommonButtons = TDCBF_CLOSE_BUTTON;

    TaskDialogIndirect(&config, NULL, NULL, NULL);
}

void pluginBeNotified(SCNotification *notifyCode)
{
    if (!isNppReady && notifyCode->nmhdr.code != NPPN_READY) return;

    void* bufID = (void*)notifyCode->nmhdr.idFrom;

    switch (notifyCode->nmhdr.code)
    {
        case NPPN_READY:
            isNppReady = true;
            // Record the initial buffer as processed to allow checkAndForce
            processedBuffers.clear(); 
            checkAndForceLanguage();
            if (nppData._nppHandle) {
                // Get current buffer ID to mark it
                // We don't have a direct NPPM_GETCURRENTBUFFERID, but BufferID is often passed in notifications.
                // For READY, we just force it once.
            }
            break;

        case NPPN_FILEOPENED:
        case NPPN_BUFFERACTIVATED:
            if (isNppReady && bufID) 
            {
                if (processedBuffers.find(bufID) == processedBuffers.end()) 
                {
                    checkAndForceLanguage();
                    processedBuffers.insert(bufID);
                }
            }
            break;

        case NPPN_FILECLOSED:
            if (bufID) processedBuffers.erase(bufID);
            break;

        case NPPN_FILESAVED: // Kept for dynamic settings reload
            LoadSettings();
            break;

        default:
            return;
    }
}
