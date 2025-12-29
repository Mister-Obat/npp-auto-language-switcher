# AutoLangSwitcher

**AutoLangSwitcher** is a Notepad++ plugin designed to automatically enforce a specific programming language or User Defined Language (UDL) when opening **plain text files** (like `.txt`) or creating **new documents**. It ensures a consistent environment for your notes while **preserving** the default syntax highlighting for files with specific extensions (e.g. `.cpp`, `.xml`, `.json` will remain untouched).

## English

### Features
*   **Automatic Language Switching**: Applies a preferred language to new and opened text files immediately.
*   **Safe for Code**: Wisely ignores files with known extensions (like `.cpp`, `.py`) to respect their default highlighting.
*   **UDL Support**: Fully compatible with User Defined Languages.
*   **Smart State Tracking**: Respects manual language changes per tab.
*   **Simple Configuration**: Set your default language directly from the editor menu.

### Installation
1.  **[Download AutoLangSwitcher.dll](https://github.com/Mister-Obat/npp-auto-language-switcher/raw/main/release/AutoLangSwitcher.dll)** (Direct Link).
2.  Navigate to your Notepad++ plugins directory (usually `C:\Program Files\Notepad++\plugins`).
3.  Create a new folder named `AutoLangSwitcher`.
4.  Copy `AutoLangSwitcher.dll` into this folder.
5.  Restart Notepad++.

### Usage
Access the controls via the **Plugins > AutoLangSwitcher** menu:
*   **Set Current as Default**: Saves the active document's language as the global default.
*   **Disable / Reset Config**: Disables the automatic switching mechanism.
*   **About**: Displays version information.

### Build from Source
1.  Open an **x64 Native Tools Command Prompt for VS**.
2.  Navigate to the `AutoLangSwitcher` directory.
3.  Run:
    ```cmd
    build.bat
    ```
4.  Output in `release/`.

---

*Created by Mister Obat with the assistance of an Artificial Intelligence.*  

---

## Français

**AutoLangSwitcher** est un plugin pour Notepad++ conçu pour imposer automatiquement un langage ou un UDL spécifique lors de l'ouverture de **fichiers texte** (comme `.txt`) ou de **nouveaux documents**. Il garantit un environnement cohérent pour vos notes tout en **préservant** la coloration syntaxique des fichiers ayant une extension connue (ex: `.cpp`, `.xml`, `.json` restent inchangés).

### Fonctionnalités
*   **Bascule Automatique** : Applique instantanément le langage défini aux nouveaux fichiers et fichiers texte.
*   **Sécurité** : Ignore intelligemment les fichiers avec des extensions connues (comme `.cpp`, `.py`) pour respecter leur coloration par défaut.
*   **Support UDL** : Compatible avec les langages définis par l'utilisateur.
*   **Suivi Intelligent** : Respecte les changements manuels de langage par onglet.
*   **Configuration Rapide** : Définissez le langage par défaut depuis le menu.

### Installation
1.  **[Télécharger AutoLangSwitcher.dll](https://github.com/Mister-Obat/npp-auto-language-switcher/raw/main/release/AutoLangSwitcher.dll)** (Lien Direct).
2.  Accédez au dossier plugins de Notepad++ (généralement `C:\Program Files\Notepad++\plugins`).
3.  Créez un dossier nommé `AutoLangSwitcher`.
4.  Copiez `AutoLangSwitcher.dll` dedans.
5.  Redémarrez Notepad++.

### Utilisation
Menu **Compléments > AutoLangSwitcher** :
*   **Set Current as Default** : Définit le langage actuel comme défaut.
*   **Disable / Reset Config** : Désactive le plugin.
*   **About** : Infos version.

### Compilation
1.  Ouvrez **x64 Native Tools Command Prompt for VS**.
2.  Allez dans le dossier `AutoLangSwitcher`.
3.  Lancez :
    ```cmd
    build.bat
    ```
4.  Résultat dans `release/`.

---

*Créé par Mister Obat avec l'assistance d'une Intelligence Artificielle.*
