// ServicesConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <tchar.h>
#include "ServiceEnumerator.h"

#ifdef UNICODE
#define ServiceString   std::wstring
#else
#define ServiceString   std::string
#endif

ServiceString ServiceStatusToString(ServiceStatus status)
{
    ServiceString s;

    switch (status)
    {
    case ServiceStatus::Unknown:
        return _T("Unknown");
    case ServiceStatus::Stopped:
        return _T("Stopped");
    case ServiceStatus::Starting:
        return _T("Starting");
    case ServiceStatus::Stopping:
        return _T("Stopping");
    case ServiceStatus::Running:
        return _T("Running");
    case ServiceStatus::Continuing:
        return _T("Continuing");
    case ServiceStatus::Pausing:
        return _T("Pausing");
    case ServiceStatus::Paused:
        return _T("Paused");
    default:
        return _T("Unknown");
    }
}

int main()
{
    auto services = ServiceEnumerator::EnumerateServices();
    for (auto const& s : services)
    {
        std::wcout << "Name:    " << s.ServiceName << std::endl
            << "Display: " << s.DisplayName << std::endl
            << "Status:  " << ServiceStatusToString(s.Status) << std::endl
            << "--------------------------" << std::endl;
    }

    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
