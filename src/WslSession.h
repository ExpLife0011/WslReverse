#ifndef WSLSESSION_H
#define WSLSESSION_H

#include <Windows.h>

static const GUID CLSID_LxssUserSession = {
    0x4F476546,
    0xB412,
    0x4579,
    { 0xB6, 0x4C, 0x12, 0x3D, 0xF3, 0x31, 0xE3, 0xD6 } };

static const GUID IID_ILxssUserSession = {
    0x536A6BCF,
    0xFE04,
    0x41D9,
    { 0xB9, 0x78, 0xDC, 0xAC, 0xA9, 0xA9, 0xB5, 0xB9 } };

typedef enum _WslDefaultUID {
    RootUser = 0,
    NormalUser = 1000
} WslDefaultUID;

typedef enum _LxssDistributionState {
    Installed = 1,
    ToBeInstall = 2,
    Installing = 3,
    Uninstalling = 4,
    Upgrading = 5
} LxssDistributionState;

typedef enum _WSL_DISTRIBUTION_FLAGS {
    WSL_DISTRIBUTION_FLAGS_NONE = 0,
    WSL_DISTRIBUTION_FLAGS_ENABLE_INTEROP = 1,
    WSL_DISTRIBUTION_FLAGS_APPEND_NT_PATH = 2,
    WSL_DISTRIBUTION_FLAGS_ENABLE_DRIVE_MOUNTING = 4,
    WSL_DISTRIBUTION_FLAGS_DEFAULT = 7
} WSL_DISTRIBUTION_FLAGS;

typedef struct _LXSS_STD_HANDLE {
    ULONG Handle;
    ULONG Pipe;
} LXSS_STD_HANDLE, *PLXSS_STD_HANDLE;

typedef struct _LXSS_STD_HANDLES {
    LXSS_STD_HANDLE StdIn;
    LXSS_STD_HANDLE StdOut;
    LXSS_STD_HANDLE StdErr;
} LXSS_STD_HANDLES, *PLXSS_STD_HANDLES;

typedef struct _WslSession WslSession, *PWslSession;

struct _WslSession {
    PVOID QueryInterface;
    PVOID AddRef;
    PVOID Release;

    /**
    * PVOID ObjectStublessClient3;
    * Create new LxssInstance or get the running one
    * If no DistroId is provided get the default one
    **/
    HRESULT(STDMETHODCALLTYPE *CreateInstance)(
        _In_ PWslSession* wslSession,
        _In_opt_ GUID* DistroId,
        _In_opt_ ULONG InstanceMode
        );

    /**
    * PVOID ObjectStublessClient4;
    * Write State to THREE means installing
    * Run as administrator otherwise E_ACCESSDENIED error
    * State Must be TWO for newer installation
    **/
    HRESULT(STDMETHODCALLTYPE *RegisterDistributionV1)(
        _In_ PWslSession* wslSession,
        _In_ PWSTR DistributionName,
        _In_ ULONG State,
        _In_ HANDLE TarGzFileHandle,
        _In_ PWSTR BasePath,
        _In_ GUID* DistroId
        );

    /**
    * PVOID ObjectStublessClient5;
    * RPC_S_CANNOT_SUPPORT The requested operation is not supported.
    **/
    HRESULT(STDMETHODCALLTYPE *RegisterDistributionV2)(
        _In_ PWslSession* wslSession,
        _In_ PWSTR DistributionName,
        _In_ ULONG State,
        _In_ HANDLE TarGzFileHandle,
        _In_ PWSTR BasePath,
        _In_ GUID* DistroId
        );

    /**
    * PVOID ObjectStublessClient6;
    * Query State registry value in Lxss registry key
    * Allows only if that value is ONE means installed
    **/
    HRESULT(STDMETHODCALLTYPE *GetDistributionId)(
        _In_ PWslSession* wslSession,
        _In_ PWSTR DistroName,
        _In_ ULONG State,
        _Out_ GUID* DistroId
        );

    /**
    * PVOID ObjectStublessClient7;
    * If DistroId is NULL it will be default distribution
    * Use IServerSecurity interface and terminates any process
    **/
    HRESULT(STDMETHODCALLTYPE *TerminateDistribution)(
        _In_ PWslSession* wslSession,
        _In_opt_ GUID* DistroId
        );

    /**
    * PVOID ObjectStublessClient8;
    * Query GUID with already registered in Lxss key
    * Write State registry value as FOUR means uninstalling
    **/
    HRESULT(STDMETHODCALLTYPE *UnregisterDistribution)(
        _In_ PWslSession* wslSession,
        _In_ GUID* DistroId
        );

    /**
    * PVOID ObjectStublessClient9;
    * Flags should be less than or equal to SEVEN
    * Writes Environment variables iff EnvironmentCount present
    **/
    HRESULT(STDMETHODCALLTYPE *ConfigureDistribution)(
        _In_ PWslSession* wslSession,
        _In_ GUID* DistroId,
        _In_opt_ PCSTR KernelCommandLine,
        _In_opt_ ULONG DefaultUid,
        _In_opt_ ULONG EnvironmentCount,
        _In_opt_ PCSTR* DefaultEnvironment,
        _In_ ULONG Flags
        );

    /**
    * PVOID ObjectStublessClient10;
    * Enumerate all value in Lxss\DistroId registry key
    * Query CurrentControlSet\services\LxssManager\DistributionFlags
    **/
    HRESULT(STDMETHODCALLTYPE *GetDistributionConfiguration)(
        _In_ PWslSession* wslSession,
        _In_ GUID* DistroId,
        _Out_ PWSTR* DistributionName,
        _Out_ PULONG Version,
        _Out_ PWSTR* BasePath,
        _Out_ PSTR* KernelCommandLine,
        _Out_ PULONG DefaultUid,
        _Out_ PULONG EnvironmentCount,
        _Out_ PSTR** DefaultEnvironment,
        _Out_ PULONG Flags
        );

    /**
    * PVOID ObjectStublessClient11;
    * Query DefaultDistribution registry string
    * Delete that if BasePath doesn't exist
    **/
    HRESULT(STDMETHODCALLTYPE *GetDefaultDistribution)(
        _In_ PWslSession* wslSession,
        _Out_ GUID* DistroId
        );

    /**
    * PVOID ObjectStublessClient12;
    * Query State registry value should be ONE
    * Write DefaultDistribution registry with provided GUID
    **/
    HRESULT(STDMETHODCALLTYPE *SetDefaultDistribution)(
        _In_ PWslSession* wslSession,
        _In_ GUID* DistroId
        );

    /**
    * PVOID ObjectStublessClient13;
    * Query State registry value should be ONE
    * Returns GUID list in 16 bytes offsets
    **/
    HRESULT(STDMETHODCALLTYPE *EnumerateDistributions)(
        _In_ PWslSession* wslSession,
        _In_ BOOLEAN WithThrow,
        _Out_ PULONG DistroCount,
        _Out_ GUID** DistroIdList
        );

    /**
    * PVOID ObjectStublessClient14;
    * Related with hidden WSL_VM_Mode feature
    **/
    HRESULT(STDMETHODCALLTYPE *CreateLxProcess)(
        _In_ PWslSession* wslSession,
        _In_opt_ GUID* DistroId,
        _In_opt_ PSTR CommandLine,
        _In_opt_ ULONG ArgumentCount,
        _In_opt_ PSTR* Arguments,
        _In_opt_ PWSTR CurrentDirectory,
        _In_opt_ PWSTR SharedEnvironment,
        _In_opt_ PWSTR ProcessEnvironment,
        _In_opt_ SIZE_T EnvironmentLength,
        _In_opt_ PWSTR LinuxUserName,
        _In_ USHORT WindowWidthX,
        _In_ USHORT WindowHeightY,
        _In_ ULONG ConsoleHandle,
        _In_ PLXSS_STD_HANDLES StdHandles,
        _Out_ GUID* InitiatedDistroId,
        _Out_ GUID* LxInstanceId,
        _Out_ PHANDLE ProcessHandle,
        _Out_ PHANDLE ServerHandle,
        _In_ PVOID VmModeSocketA,
        _In_ PVOID VmModeSocketB,
        _In_ PVOID VmModeSocketC,
        _In_ PVOID VmModeSocketD
        );

    /**
    * PVOID ObjectStublessClient15;
    * Query Distribution Configuration and Version is TWO for newer installation
    * Write State registry to FIVE means upgrading
    **/
    HRESULT(STDMETHODCALLTYPE *BeginUpgradeDistribution)(
        _In_ PWslSession* wslSession,
        _In_ GUID* DistroId,
        _Out_ PULONG Version,
        _Out_ PWSTR* BasePath
        );

    /**
    * PVOID ObjectStublessClient16;
    * Query Distribution Configuration and Write State registry to ONE means installed
    * Write Version registry to TWO for newer installation
    **/
    HRESULT(STDMETHODCALLTYPE *FinishUpgradeDistribution)(
        _In_ PWslSession* wslSession,
        _In_ GUID* DistroId,
        _In_ ULONG Version
        );

    /**
    * PVOID ObjectStublessClient17;
    * E_NOINTERFACE No such interface supported
    **/
    HRESULT(STDMETHODCALLTYPE *DisableVmMode)(
        _In_ PWslSession* wslSession,
        _In_ GUID* DistroId
        );

    /**
    * PVOID ObjectStublessClient18;
    * E_NOINTERFACE No such interface supported
    **/
    HRESULT(STDMETHODCALLTYPE *EnableVmMode)(
        _In_ PWslSession* wslSession,
        _In_ GUID* DistroId
        );

    /**
    * PVOID ObjectStublessClient19;
    * Element not found
    **/
    HRESULT(STDMETHODCALLTYPE *RegisterLxBusServer)(
        _In_ PWslSession* wslSession,
        _In_ GUID* DistroId,
        _In_ PSTR LxBusServerName,
        _Out_ PHANDLE ServerHandle
        );

};

#endif //WSLSESSION_H
