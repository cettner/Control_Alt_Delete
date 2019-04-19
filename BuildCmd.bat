::BatchFile Directory
set PATH=C:\Program Files\Epic Games\UE_4.19\Engine\Build\BatchFiles;%PATH%

call Build.bat RTS_ProjectEditor Win64 Development "C:\Users\cettn\Code\Control_Alt_Delete\RTS_Project.uproject"


::\UnrealEngine\Engine\Build\BatchFiles\Build.bat ProjectNameEditor Win64 Debug "X:\Path\To\The\Project\ProjectName.uproject" -waitmutex

::\UnrealEngine\Engine\Build\BatchFiles\Rebuild.bat ProjectNameEditor Win64 Debug "X:\Path\To\The\Project\ProjectName.uproject" -waitmutex

::\UnrealEngine\Engine\Build\BatchFiles\Clean.bat ProjectNameEditor Win64 Debug "X:\Path\To\The\Project\ProjectName.uproject"