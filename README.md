# AutoLangSwitcher

**AutoLangSwitcher** is a Notepad++ plugin designed to automatically enforce a specific programming language or User Defined Language (UDL) when opening files or creating new documents. It ensures a consistent environment by prioritizing your preferred language settings over Notepad++ defaults.

## English

### Features
*   **Automatic Language Switching**: Applies a preferred language to new and opened files immediately.
*   **UDL Support**: Fully compatible with User Defined Languages.
*   **Smart State Tracking**: Respects manual language changes per tab; only enforces the default once upon load.
*   **Simple Configuration**: Set your default language directly from the editor menu.

### Installation
1.  Download `AutoLangSwitcher.dll` from the `AutoLangSwitcher/release/` directory in this repository.
2.  Navigate to your Notepad++ plugins directory (usually `C:\Program Files\Notepad++\plugins`).
3.  Create a new folder named `AutoLangSwitcher`.
4.  Copy `AutoLangSwitcher.dll` into this folder.
5.  Restart Notepad++.

### Usage
Access the controls via the **Plugins > AutoLangSwitcher** menu:
*   **Set Current as Default**: Saves the active document's language as the global default to be enforced.
*   **Disable / Reset Config**: Disables the automatic switching mechanism.
*   **About**: Displays version information and current configuration.

### Build from Source
1.  Open an **x64 Native Tools Command Prompt for VS**.
2.  Navigate to the `AutoLangSwitcher` directory within this repository.
3.  Run the build script:
    ```cmd
    build.bat
    ```
4.  The compiled plugin will be available in the `release/` folder.

---

## Français

**AutoLangSwitcher** est un plugin pour Notepad++ conçu pour imposer automatiquement un langage de programmation ou un Langage Utilisateur (UDL) spécifique lors de l'ouverture de fichiers ou de la création de nouveaux documents. Il garantit un environnement de travail cohérent en assurant que votre langage préféré est toujours actif par défaut.

### Fonctionnalités
*   **Bascule Automatique** : Applique instantanément le langage défini aux nouveaux fichiers et aux fichiers ouverts.
*   **Support UDL** : Entièrement compatible avec les langages définis par l'utilisateur.
*   **Suivi Intelligent** : Le plugin ne force le langage qu'une seule fois à l'ouverture. Si vous changez le langage d'un onglet manuellement, votre choix est respecté.
*   **Configuration Rapide** : Définissez le langage par défaut directement depuis le menu du plugin.

### Installation
1.  Téléchargez le fichier `AutoLangSwitcher.dll` situé dans le dossier `AutoLangSwitcher/release/` de ce dépôt.
2.  Accédez au répertoire des plugins de Notepad++ (généralement `C:\Program Files\Notepad++\plugins`).
3.  Créez un nouveau dossier nommé `AutoLangSwitcher`.
4.  Copiez le fichier `AutoLangSwitcher.dll` dans ce dossier.
5.  Redémarrez Notepad++.

### Utilisation
Les commandes sont accessibles via le menu **Compléments > AutoLangSwitcher** :
*   **Set Current as Default** : Enregistre le langage du document actif comme langage par défaut à imposer.
*   **Disable / Reset Config** : Désactive le changement automatique de langage.
*   **About** : Affiche la version et la configuration actuelle.

### Compilation
1.  Ouvrez une invite de commande **x64 Native Tools Command Prompt for VS**.
2.  Naviguez vers le dossier `AutoLangSwitcher` de ce dépôt.
3.  Exécutez le script de compilation :
    ```cmd
    build.bat
    ```
4.  Le plugin compilé sera disponible dans le dossier `release/`.
