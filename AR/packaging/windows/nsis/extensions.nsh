!include "StrFunc.nsh"

;; "Initialize" string functions
${StrRep}
${StrCase}

# TODO: create a function if we need to rewrite user's choice
# Rewrite previously selected user's choice example:
# WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.json\UserChoice" "ProgId" "${APP_NAME}.Config"

;; Function that associates one extension with Hermes
Function AssociateExtension
  ; back up old value for extension $R0 (eg. ".json")
  ReadRegStr $1 HKCR "$R0" ""
  StrCmp $1 "" NoBackup
    StrCmp $1 "${APP_NAME}$R0" "NoBackup"
    WriteRegStr HKCR "$R0" "${APP_NAME}.backup" $1
NoBackup:
  WriteRegStr HKCR "$R0" "" "${APP_NAME}$R0"
FunctionEnd

!macro AssociateExtensionSection TYPE EXT
  Section ${EXT}
    SectionIn 1 3
    Push $R0
    StrCpy $R0 ${EXT}
    Call AssociateExtension
    Pop $R0
  SectionEnd
!macroend

;; Function that registers one extension for Hermes
Function RegisterExtension
  ; R0 contains the extension, R1 contains the type (Config/Video)
  ; Remove the leading dot from the filetype string
  ${StrRep} $R2 $R0 "." ""
  ; And capitalize the extension
  ${StrCase} $R2 $R2 "U"
  ; for instance: TS Video File (Hermes)

  # WriteRegStr HKCR ".json" "" "${APP_NAME}.Config"
  WriteRegStr HKCR "${APP_NAME}$R0" "" "$R2 $R1 File (${APP_NAME})"
  WriteRegStr HKCR "${APP_NAME}$R0\shell" "" "open" # set open as default command
  WriteRegStr HKCR "${APP_NAME}$R0\shell\open\command" "" "$\"$INSTDIR\${APP_TARGET}.exe$\" $\"%1$\""
  WriteRegStr HKCR "${APP_NAME}$R0\DefaultIcon" "" "$\"$INSTDIR\${APP_TARGET}.exe$\",1"
FunctionEnd

;; Function that removes one extension that Hermes owns.
Function un.RegisterExtension
  ;start of restore script
  ReadRegStr $1 HKCR "$R0" ""
  StrCmp $1 "${APP_NAME}$R0" 0 NoOwn ; only do this if we own it
    ; Read the old value from Backup
    ReadRegStr $1 HKCR "$R0" "${APP_NAME}.backup"
    StrCmp $1 "" 0 Restore ; if backup="" then delete the whole key
      DeleteRegKey HKCR "$R0"
    Goto NoOwn
Restore:
      WriteRegStr HKCR "$R0" "" $1
      DeleteRegValue HKCR "$R0" "${APP_NAME}.backup"
NoOwn:
    DeleteRegKey HKCR "${APP_NAME}$R0" ;Delete key with association settings
FunctionEnd

!macro RegisterExtensionMacro TYPE EXT
  Push $R0
  StrCpy $R0 ${EXT}
  Push $R1
  StrCpy $R1 ${TYPE}
  Call RegisterExtension
  Pop $R1
  Pop $R0
!macroend

!macro UnRegisterExtensionSection TYPE EXT
  Push $R0
  StrCpy $R0 ${EXT}
  Call un.RegisterExtension
  Pop $R0
!macroend

!macro MacroVideoExtensions _action
  !insertmacro ${_action} Video ".ts"
  !insertmacro ${_action} Video ".mts"
  !insertmacro ${_action} Video ".m2ts"
!macroend

!macro MacroConfigExtensions _action
  !insertmacro ${_action} Config ".json"
!macroend

; One macro to rule them all
!macro MacroAllExtensions _action
  !insertmacro MacroVideoExtensions ${_action}
  !insertmacro MacroConfigExtensions ${_action}
!macroend