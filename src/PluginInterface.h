#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <windows.h>

const int NPPMSG = WM_USER + 1000;

#define NPPM_GETCURRENTSCINTILLA  (NPPMSG + 4)
#define NPPM_DOCOPEN  (NPPMSG + 41)
#define NPPM_GETPLUGINSCONFIGDIR (NPPMSG + 46)
#define NPPM_GETCURRENTLANGUAGETYPE (NPPMSG + 5)
#define NPPM_SETCURRENTLANGUAGETYPE (NPPMSG + 6)
#define NPPM_GETCURRENTLANGUAGENAME  (NPPMSG + 83)
#define NPPM_SETCURRENTLANGUAGENAME  (NPPMSG + 80)

#define NPPM_GETNBUSERLANG (NPPMSG + 22)
#define NPPM_GETCURRENTBUFFERID (NPPMSG + 23)
#define NPPM_GETMENUHANDLE (NPPMSG + 25)
#define NPPMAINMENU 1
#define NPPM_MENUCOMMAND (NPPMSG + 48)

#define NPPN_FIRST 1000
#define NPPN_READY (NPPN_FIRST + 1)
#define NPPN_TBMODIFICATION (NPPN_FIRST + 2)
#define NPPN_FILEBEFORECLOSE (NPPN_FIRST + 3)
#define NPPN_FILEOPENED (NPPN_FIRST + 4)
#define NPPN_FILECLOSED (NPPN_FIRST + 5)
#define NPPN_FILEBEFOREOPEN (NPPN_FIRST + 6)
#define NPPN_FILEBEFORESAVE (NPPN_FIRST + 7)
#define NPPN_FILESAVED (NPPN_FIRST + 8)
#define NPPN_SHUTDOWN (NPPN_FIRST + 9)
#define NPPN_BUFFERACTIVATED (NPPN_FIRST + 10)
#define NPPN_LANGCHANGED (NPPN_FIRST + 11)
#define NPPN_WORDSTYLESUPDATED (NPPN_FIRST + 12)

struct NppData {
    HWND _nppHandle;
    HWND _scintillaMainHandle;
    HWND _scintillaSecondHandle;
};

struct SCNotification {
    struct NotifyHeader {
        void *hwndFrom;
        UINT_PTR idFrom;
        UINT code;
    } nmhdr;
    int position;
    int ch;
    int modifiers;
    int modificationType;
    const char *text;
    int length;
    int linesAdded;
    int message;
    UINT_PTR wParam;
    LONG_PTR lParam;
    int line;
    int foldLevelNow;
    int foldLevelPrev;
    int margin;
    int listType;
    int x;
    int y;
    int token;
    int annotationLinesAdded;
    int updated;
    int listCompletionMethod;
};

struct ShortcutKey {
    bool _isCtrl;
    bool _isAlt;
    bool _isShift;
    UCHAR _key;
};

struct FuncItem {
    TCHAR _itemName[64];
    PVOID _pFunc;
    int _cmdID;
    bool _init2Check;
    ShortcutKey *_pShKey;
};

#endif // PLUGININTERFACE_H
