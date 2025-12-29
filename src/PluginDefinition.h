#ifndef PLUGINDEFINITION_H
#define PLUGINDEFINITION_H

#include "PluginInterface.h"

void pluginInit(HANDLE hModule);
void pluginCleanUp();
void commandMenuInit();
void commandMenuCleanUp();
void pluginBeNotified(SCNotification *notifyCode);

// Helpers
void checkAndForceLanguage();
void pluginAbout();
void LoadSettings();
void OpenSettings();
void ManualApply();
void SetCurrentAsDefault();
void ResetConfig();

extern int targetLanguageID;
extern bool isNppReady;

#endif // PLUGINDEFINITION_H
