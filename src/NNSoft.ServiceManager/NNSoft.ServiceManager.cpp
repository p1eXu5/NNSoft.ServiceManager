// MathLibrary.cpp : Defines the exported functions for the DLL.
#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include "NNSoft.ServiceManager.h"
#include <objbase.h>
#include <strsafe.h>



// DLL internal state variables:
static unsigned long long previous_;  // Previous value, if any
static unsigned long long current_;   // Current sequence value
static unsigned index_;               // Current seq. position

// Initialize a Fibonacci relation sequence
// such that F(0) = a, F(1) = b.
// This function must be called before any other function.
void fibonacci_init(
    const unsigned long long a,
    const unsigned long long b)
{
    index_ = 0;
    current_ = a;
    previous_ = b; // see special case when initialized
}

// Produce the next value in the sequence.
// Returns true on success, false on overflow.
bool fibonacci_next()
{
    // check to see if we'd overflow result or position
    if ((ULLONG_MAX - previous_ < current_) ||
        (UINT_MAX == index_))
    {
        return false;
    }

    // Special case when index == 0, just return b value
    if (index_ > 0)
    {
        // otherwise, calculate next sequence value
        previous_ += current_;
    }
    std::swap(current_, previous_);
    ++index_;
    return true;
}

// Get the current value in the sequence.
unsigned long long fibonacci_current()
{
    return current_;
}

// Get the current index position in the sequence.
unsigned fibonacci_index()
{
    return index_;
}

SC_HANDLE openSCManager() 
{
    SC_HANDLE sc = OpenSCManagerW(nullptr, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
    return sc;
}

bool closeServiceHandle( SC_HANDLE sc )
{
    return CloseServiceHandle( sc );
}

bool EnumServices(func callback)
{
    auto services = ServiceEnumerator::EnumerateServices();
    for (auto const& s : services)
    {
        callback(s);
    }

    return false;
}

bool EnumServices2(func2 callback)
{
    std::wstring name(L"Steve Nash");

    const size_t alloc_size = 64;
    STRSAFE_LPSTR result = (STRSAFE_LPSTR)CoTaskMemAlloc(alloc_size);
    STRSAFE_LPCSTR teststr = "This is return value";
    StringCchCopyA(result, alloc_size, teststr);

    MyStruct* s = (MyStruct*)CoTaskMemAlloc(alloc_size + sizeof(MyStruct));
    s->a = (char*)result;

    callback(s);
    return false;
}
