# Builds a .qmod file for loading with QuestPatcher
$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt

$buildScript = "$NDKPath/build/ndk-build"
if (-not ($PSVersionTable.PSEdition -eq "Core")) {
    $buildScript += ".cmd"
}

$ArchiveName = "screwgravity.qmod"
$TempArchiveName = "screwgravity.qmod.zip"

& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk
Compress-Archive -Path "./libs/arm64-v8a/libscrewgravity.so", "./libs/arm64-v8a/libbeatsaber-hook_2_3_0.so", "./mod.json", "cover.png" -DestinationPath $TempArchiveName -Force
Move-Item $TempArchiveName $ArchiveName -Force