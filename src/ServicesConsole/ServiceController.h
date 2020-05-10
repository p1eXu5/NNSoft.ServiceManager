#pragma once

#include <utility>
#include <map>
#include <vector>

#include <chrono>
using namespace std::literals::chrono_literals;

#include <thread>

#include <windows.h>

#include "ServiceConstants.h"
#include "ServiceHandle.h"

#include "ServiceConfig.h"

class ServiceController
{
private:
    ServiceHandle _scHandle;
    ServiceHandle _srvHandle;
    ServiceString _srvName;

    std::map<ServiceControls, bool> _acceptedControls =
    {
       { ServiceControls::Stop, false},
       { ServiceControls::PauseAndContinue, false },
       { ServiceControls::ChangeParams, false },
       { ServiceControls::ChangeBindings, false },
       { ServiceControls::PreShutdown, false },
       { ServiceControls::ShutdownNotification, false },
       { ServiceControls::HardwareProfileChangedNotification, false },
       { ServiceControls::PowerChangedNotification, false },
       { ServiceControls::SessionChangedNotification, false },
       { ServiceControls::TriggerEventNotification, false },
       { ServiceControls::TimeChangeNotification, false },
       { ServiceControls::UserModeNotification, false },
    };


public:
    ServiceController( ServiceString name, DWORD access = SERVICE_ALL_ACCESS)
    {
        _srvName = name;
        _scHandle = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);

        if (_scHandle)
        {
            _srvHandle = ::OpenService(_scHandle, name.c_str(), access);
            if (!_srvHandle)
            {
                _scHandle = nullptr;
            }
            else
            {
                auto bytesNeeded = DWORD{ 0 };
                auto ssp = SERVICE_STATUS_PROCESS{ 0 };

                auto result = ::QueryServiceStatusEx(
                    _srvHandle,
                    SC_STATUS_PROCESS_INFO,
                    reinterpret_cast<LPBYTE>(&ssp),
                    sizeof(ssp),
                    &bytesNeeded);

                if (result != 0)
                {
                    auto setControl = [ssp](std::map<ServiceControls, bool>& controls,
                        ServiceControls const control)
                    {
                        controls[control] =
                            (ssp.dwControlsAccepted & static_cast<int>(control)) != 0;
                    };

                    setControl(_acceptedControls, ServiceControls::Stop);
                    setControl(_acceptedControls, ServiceControls::PauseAndContinue);
                    setControl(_acceptedControls, ServiceControls::ChangeParams);
                    setControl(_acceptedControls, ServiceControls::ChangeBindings);
                    setControl(_acceptedControls, ServiceControls::PreShutdown);
                    setControl(_acceptedControls, ServiceControls::ShutdownNotification);
                    setControl(_acceptedControls,
                        ServiceControls::HardwareProfileChangedNotification);
                    setControl(_acceptedControls, ServiceControls::PowerChangedNotification);
                    setControl(_acceptedControls, ServiceControls::SessionChangedNotification);
                    setControl(_acceptedControls, ServiceControls::TriggerEventNotification);
                    setControl(_acceptedControls, ServiceControls::TimeChangeNotification);
                    setControl(_acceptedControls, ServiceControls::UserModeNotification);
                }
            }
        }
    }


    bool CanAcceptControl(ServiceControls const control) const
    {
        auto it = _acceptedControls.find(control);
        return it != std::end(_acceptedControls) ? it->second : false;
    }

    bool CanPauseContinue() const
    {
        return CanAcceptControl(ServiceControls::PauseAndContinue);
    }
    bool CanShutdown() const
    {
        return CanAcceptControl(ServiceControls::ShutdownNotification);
    }
    bool CanStop() const
    {
        return CanAcceptControl(ServiceControls::Stop);
    }

    bool Start()
    {
        auto success = false;

        if (_srvHandle)
        {
            auto result = ::StartService(_srvHandle, 0, nullptr);
            success = result != 0;
        }

        return success;
    }

    bool Stop()
    {
        auto success = false;

        if (_srvHandle)
        {
            success = StopDependentServices();

            if (success)
            {
                auto ssp = SERVICE_STATUS_PROCESS{ 0 };
                success = ChangeServiceStatus(_srvHandle, SERVICE_CONTROL_STOP, ssp);
            }
        }

        return success;
    }

    bool Pause()
    {
        auto success = false;

        if (_srvHandle)
        {
            auto ssp = SERVICE_STATUS_PROCESS{ 0 };
            success = ChangeServiceStatus(_srvHandle, SERVICE_CONTROL_PAUSE, ssp);
        }

        return success;
    }

    bool Continue()
    {
        auto success = false;

        if (_srvHandle)
        {
            auto ssp = SERVICE_STATUS_PROCESS{ 0 };
            success = ChangeServiceStatus(_srvHandle, SERVICE_CONTROL_CONTINUE, ssp);
        }

        return success;
    }

    bool Delete()
    {
        auto success = false;

        if (_srvHandle)
        {
            success = ::DeleteService(_srvHandle) != 0;

            if (success)
                _srvHandle = nullptr;
        }

        return success;
    }


    ServiceConfig GetServiceConfig()
    {
        return ServiceConfig::Create(_srvHandle);
    }

private:

    static bool ChangeServiceStatus
    (SC_HANDLE const handle, DWORD const controlCode, SERVICE_STATUS_PROCESS& ssp)
    {
        auto success = false;

        if (handle)
        {
            auto result = ::ControlService(
                handle,
                controlCode,
                reinterpret_cast<LPSERVICE_STATUS>(&ssp));

            success = result != 0;
        }

        return success;
    }
    
    
    bool StopDependentServices()
    {
        auto ess = ENUM_SERVICE_STATUS{ 0 };
        auto bytesNeeded = DWORD{ 0 };
        auto count = DWORD{ 0 };

        if (!::EnumDependentServices(
            _srvHandle,
            SERVICE_ACTIVE,
            nullptr, // to determine the required size
            0,
            &bytesNeeded,
            &count))
        {
            if (GetLastError() != ERROR_MORE_DATA)
                return false;

            std::vector<unsigned char> buffer(bytesNeeded, 0);

            if (!::EnumDependentServices(
                _srvHandle,
                SERVICE_ACTIVE,
                reinterpret_cast<LPENUM_SERVICE_STATUS>(buffer.data()),
                bytesNeeded,
                &bytesNeeded,
                &count))
            {
                return false;
            }

            for (auto i = DWORD{ 0 }; i < count; ++i)
            {
                auto ess = static_cast<ENUM_SERVICE_STATUS>
                    (*(reinterpret_cast<LPENUM_SERVICE_STATUS>(buffer.data() + i)));

                ServiceHandle handle = ::OpenService(
                    _scHandle,
                    ess.lpServiceName,
                    SERVICE_STOP | SERVICE_QUERY_STATUS);

                if (!handle)
                    return false;

                auto ssp = SERVICE_STATUS_PROCESS{ 0 };

                if (!ChangeServiceStatus(handle, SERVICE_CONTROL_STOP, ssp))
                    return false;

                if (!WaitForStatus(handle, ssp, ServiceStatus::Stopped))
                    return false;
            }
        }

        return true;
    }



    bool WaitForStatus(ServiceStatus desiredStatus, std::chrono::milliseconds const timeout = 30000ms )
    {
        auto success = false;

        if (_srvHandle)
        {
            auto ssp = SERVICE_STATUS_PROCESS{ 0 };

            auto bytesNeeded = DWORD{ 0 };

            if (::QueryServiceStatusEx(
                _srvHandle,
                SC_STATUS_PROCESS_INFO,
                reinterpret_cast<LPBYTE>(&ssp),
                sizeof(ssp),
                &bytesNeeded))
            {
                success = WaitForStatus(_srvHandle, ssp, desiredStatus, timeout);
            }
        }

        return success;
    }

    static std::chrono::milliseconds GetWaitTime(DWORD const waitHint)
    {
        auto waitTime = waitHint / 10;

        if (waitTime < 1000)
            waitTime = 1000;
        else if (waitTime > 10000)
            waitTime = 10000;

        return std::chrono::milliseconds(waitTime);
    }

    static bool WaitForStatus(SC_HANDLE const handle,
        SERVICE_STATUS_PROCESS& ssp,
        ServiceStatus const desireStatus,
        std::chrono::milliseconds const timeout = 30000ms)
    {
        auto success = ssp.dwCurrentState == static_cast<DWORD>(desireStatus);

        if (!success && handle)
        {
            auto start = std::chrono::high_resolution_clock::now();
            auto waitTime = GetWaitTime(ssp.dwWaitHint);

            while (ssp.dwCurrentState != static_cast<DWORD>(desireStatus))
            {
                std::this_thread::sleep_for(waitTime);

                auto bytesNeeded = DWORD{ 0 };

                if (!::QueryServiceStatusEx(
                    handle,
                    SC_STATUS_PROCESS_INFO,
                    reinterpret_cast<LPBYTE>(&ssp),
                    sizeof(ssp),
                    &bytesNeeded))
                    break;

                if (ssp.dwCurrentState == static_cast<DWORD>(desireStatus))
                {
                    success = true;
                    break;
                }

                if (std::chrono::high_resolution_clock::now() - start > timeout)
                    break;
            }
        }

        return success;
    }
};