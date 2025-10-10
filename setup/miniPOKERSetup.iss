; -----------------------------------------------------------
;  MINI POKER INSTALLER (by BinaryOxide)
; -----------------------------------------------------------
;  Generated and polished for full distribution
; -----------------------------------------------------------

[Setup]
AppId={{D16D13F8-F0B4-4512-B280-FDE325C86796}
AppName=miniPOKER
AppVersion=1.0
AppPublisher=BinaryOxide
AppPublisherURL=https://github.com/BinaryOxide
AppSupportURL=https://github.com/BinaryOxide
AppUpdatesURL=https://github.com/BinaryOxide
DefaultDirName={autopf}\miniPOKER
UninstallDisplayIcon={app}\miniPOKER.exe
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
DisableProgramGroupPage=yes
LicenseFile=C:\Users\OBITO\Downloads\MiniPokerInstaller\LICENSE\LICENSE.txt
OutputDir=C:\Users\OBITO\Downloads
OutputBaseFilename=miniPokerSetup
SetupIconFile=C:\Users\OBITO\Downloads\MiniPokerInstaller\energy.ico
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "turkish"; MessagesFile: "compiler:Languages\Turkish.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "H:\programming\projects\miniPOKER\x64\Release\miniPOKER.exe"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{autoprograms}\miniPOKER"; Filename: "{app}\miniPOKER.exe"
Name: "{autodesktop}\miniPOKER"; Filename: "{app}\miniPOKER.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\miniPOKER.exe"; Description: "{cm:LaunchProgram,miniPOKER}"; Flags: nowait postinstall skipifsilent

; -----------------------------------------------------------
; ADD MINI POKER TO PATH (AUTO COMMAND-LINE SUPPORT)
; -----------------------------------------------------------
[Code]
procedure AddToPath();
var
  PathVar: string;
  AppPath: string;
begin
  AppPath := ExpandConstant('{app}');
  if RegQueryStringValue(HKEY_LOCAL_MACHINE,
     'SYSTEM\CurrentControlSet\Control\Session Manager\Environment',
     'Path', PathVar) then
  begin
    if Pos(AppPath, PathVar) = 0 then
    begin
      if Copy(PathVar, Length(PathVar), 1) <> ';' then
        PathVar := PathVar + ';';
      RegWriteStringValue(HKEY_LOCAL_MACHINE,
        'SYSTEM\CurrentControlSet\Control\Session Manager\Environment',
        'Path', PathVar + AppPath);
    end;
  end;
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    AddToPath();
    MsgBox('✅ MiniPOKER installed successfully!'#13#13 +
           'You can now open any terminal and type "minipoker" to launch it.',
           mbInformation, MB_OK);
  end;
end;
