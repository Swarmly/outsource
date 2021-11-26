!include "MUI2.nsh"

!define MUI_ICON "install icon.ico"
!define MUI_UNICON "uninstall icon.ico"

!define APP_NAME "Hermes"
!define APP_TARGET "gcs"
!define HMTOOL_APP_NAME "HMTool"
!define HMTOOL_TARGET "hmtool"

!define MUI_FINISHPAGE_RUN "$INSTDIR\${APP_TARGET}.exe"

SetCompressor lzma

Name ${APP_NAME}

ManifestDPIAware true
Unicode true

!ifdef X64
InstallDir "$PROGRAMFILES64\${APP_NAME}"
!else
InstallDir "$PROGRAMFILES32\${APP_NAME}"
!endif

!insertmacro MUI_PAGE_WELCOME
#!insertmacro MUI_PAGE_LICENSE LICENSE
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_FINISHPAGE_RUN_TEXT "Run ${APP_NAME}"
!define MUI_FINISHPAGE_RUN_FUNCTION "LaunchHermes"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

!include "extensions.nsh"

Section "Hermes"
    SectionIn RO
    SetOutPath $INSTDIR
    File /r hermes\*
    WriteUninstaller "$INSTDIR\uninstall.exe"

    # Add possibility to Uninstall from "Add or remove programs"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" \
                     "DisplayName" "Hermes"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" \
                     "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
    # use the same icon as for the app
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" \
                     "DisplayIcon" "$\"$INSTDIR\${APP_TARGET}.exe$\",0"

    # Install Visual C++ 2010 Redistributable
    File "vcredist_x64.exe"
    ExecWait '"$INSTDIR\vcredist_x64.exe" /quiet'
    Delete "$INSTDIR\vcredist_x64.exe"

    # Register file types supported by our apps
    !insertmacro MacroAllExtensions RegisterExtensionMacro
SectionEnd

Section "Create Desktop shortcut"
    CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${APP_TARGET}.exe"
SectionEnd

Section "Create Start Menu shortcut"
    CreateDirectory "$SMPROGRAMS\${APP_NAME}"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk" "$INSTDIR\${APP_TARGET}.exe"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"

    # HMTool
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\${HMTOOL_APP_NAME}.lnk" "$INSTDIR\${HMTOOL_TARGET}.exe"
SectionEnd

SectionGroup /e "!Associate files with ${APP_NAME}"
    SectionGroup "Video"
        !insertmacro MacroVideoExtensions AssociateExtensionSection
    SectionGroupEnd

    SectionGroup "Config"
        !insertmacro MacroConfigExtensions AssociateExtensionSection
    SectionGroupEnd

    Section # hidden section to apply changes for explorer
        System::Call 'shell32.dll::SHChangeNotify(i, i, i, i) v (0x08000000, 0, 0, 0)'
    SectionEnd
SectionGroupEnd

Section "Context Menus" SectionContextMenus
    WriteRegStr HKCR "*\shell\OpenWith${APP_NAME}" "" "Open with ${APP_NAME}"
    WriteRegStr HKCR "*\shell\OpenWith${APP_NAME}" "Icon" "$\"$INSTDIR\${APP_TARGET}.exe$\",0"
    WriteRegStr HKCR "*\shell\OpenWith${APP_NAME}\command" "" "$\"$INSTDIR\${APP_TARGET}.exe$\" $\"%1$\""

    WriteRegStr HKCR "*\shell\OpenWith${HMTOOL_APP_NAME}" "" "Open with ${HMTOOL_APP_NAME}"
    WriteRegStr HKCR "*\shell\OpenWith${HMTOOL_APP_NAME}" "Icon" "$\"$INSTDIR\${HMTOOL_TARGET}.exe$\",0"
    WriteRegStr HKCR "*\shell\OpenWith${HMTOOL_APP_NAME}\command" "" "$\"$INSTDIR\${HMTOOL_TARGET}.exe$\" $\"%1$\""

    # Refresh context menu items
    System::Call 'shell32.dll::SHChangeNotify(i, i, i, i) v (0x08000000, 0, 0, 0)'
SectionEnd

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${SectionContextMenus} "Add context menu items ('Open with ${APP_NAME}' and 'Open with ${HMTOOL_APP_NAME}')"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

UninstPage uninstConfirm
UninstPage instfiles

Section "uninstall"

    rmdir /r "$INSTDIR"

    # Remove "Add or remove programs" item
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

    Delete "$DESKTOP\${APP_NAME}.lnk"
    rmdir /r "$SMPROGRAMS\${APP_NAME}"

    !insertmacro MacroAllExtensions UnRegisterExtensionSection

    # Remove context menus
    DeleteRegKey HKCR "*\shell\OpenWith${APP_NAME}"
    DeleteRegKey HKCR "*\shell\OpenWith${HMTOOL_APP_NAME}"

    # Refresh shell icons
    System::Call 'shell32.dll::SHChangeNotify(i, i, i, i) v (0x08000000, 0, 0, 0)'
SectionEnd

Function LaunchHermes
    ShellExecAsUser::ShellExecAsUser "" "$INSTDIR\${APP_TARGET}.exe"
FunctionEnd