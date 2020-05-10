#pragma once

#include <winsvc.h>
#include <winnt.h>
#include <string>

enum class ServiceStatus
{
    Unknown = 0,
    Stopped = SERVICE_STOPPED,
    Starting = SERVICE_START_PENDING,
    Stopping = SERVICE_STOP_PENDING,
    Running = SERVICE_RUNNING,
    Continuing = SERVICE_CONTINUE_PENDING,
    Pausing = SERVICE_PAUSE_PENDING,
    Paused = SERVICE_PAUSED
};

enum class ServiceControls
{
    Stop = SERVICE_ACCEPT_STOP,
    PauseAndContinue = SERVICE_ACCEPT_PAUSE_CONTINUE,
    ChangeParams = SERVICE_ACCEPT_PARAMCHANGE,
    ChangeBindings = SERVICE_ACCEPT_NETBINDCHANGE,
    PreShutdown = SERVICE_ACCEPT_PRESHUTDOWN,
    ShutdownNotification = SERVICE_ACCEPT_SHUTDOWN,
    HardwareProfileChangedNotification = SERVICE_ACCEPT_HARDWAREPROFILECHANGE,
    PowerChangedNotification = SERVICE_ACCEPT_POWEREVENT,
    SessionChangedNotification = SERVICE_ACCEPT_SESSIONCHANGE,
    TriggerEventNotification = SERVICE_ACCEPT_TRIGGEREVENT,
    TimeChangeNotification = SERVICE_ACCEPT_TIMECHANGE,
    UserModeNotification = 0x00000800, //SERVICE_ACCEPT_USERMODEREBOOT
};

enum class ServiceType
{
    KernelDriver = SERVICE_KERNEL_DRIVER,
    FileSystemDriver = SERVICE_FILE_SYSTEM_DRIVER,
    Adapter = SERVICE_ADAPTER,
    RecognizerDriver = SERVICE_RECOGNIZER_DRIVER,
    Win32OwnProcess = SERVICE_WIN32_OWN_PROCESS,
    Win32ShareProcess = SERVICE_WIN32_SHARE_PROCESS,
    InteractiveDriver = SERVICE_INTERACTIVE_PROCESS,
    Driver = SERVICE_DRIVER,
    Win32 = SERVICE_WIN32,
    All = SERVICE_TYPE_ALL
};

enum class ServiceStartType
{
    Boot = SERVICE_BOOT_START,
    System = SERVICE_SYSTEM_START,
    Auto = SERVICE_AUTO_START,
    Demand = SERVICE_DEMAND_START,
    Disabled = SERVICE_DISABLED,
};

enum class ServiceErrorControl
{
    Ignore = SERVICE_ERROR_IGNORE,
    Normal = SERVICE_ERROR_NORMAL,
    Severe = SERVICE_ERROR_SEVERE,
    Critical = SERVICE_ERROR_CRITICAL,
};

enum class ServiceState
{
    Active = SERVICE_ACTIVE,
    Inactive = SERVICE_INACTIVE,
    All = SERVICE_STATE_ALL
};

#ifdef UNICODE
#define ServiceString   std::wstring
#else
#define ServiceString   std::string
#endif

