#include "SystemMutex.h"

ipc::SystemMutex::SystemMutex(const wchar_t* name, const unsigned long dwMilliseconds):
    _dwMilliseconds(dwMilliseconds)
{
    _mutex = CreateMutex(
        NULL,  // default security attributes
        FALSE, // initially not owned
        name); // named mutex
}

ipc::SystemMutex::~SystemMutex()
{
    CloseHandle(_mutex);
}

void ipc::SystemMutex::lock()
{
    WaitForSingleObject(
        _mutex,
        _dwMilliseconds);
}

void ipc::SystemMutex::unlock()
{
    ReleaseMutex(_mutex);
}
