#include "AppCommand/SystemMutex.h"

ipc::SystemMutex::SystemMutex(const wchar_t* name, const unsigned long dwMilliseconds):
    _dwMilliseconds(dwMilliseconds)
{
#ifdef Q_OS_WIN
    _mutex = CreateMutex(
        NULL,  // default security attributes
        FALSE, // initially not owned
        name); // named mutex
#elif defined(Q_OS_MAC)
    _mutexName = QString::fromWCharArray(name);
    _mutex = std::make_unique<QSharedMemory>(_mutexName);
#else
#endif
}

ipc::SystemMutex::~SystemMutex()
{
#ifdef Q_OS_WIN
    CloseHandle(_mutex);
#elif defined(Q_OS_MAC)
    _mutex->detach();
#else
#endif
}

void ipc::SystemMutex::lock()
{
#ifdef Q_OS_WIN
    WinLock();
#elif defined(Q_OS_MAC)
    MacLock();
#else
#endif
}

void ipc::SystemMutex::unlock()
{
#ifdef Q_OS_WIN
    WinUnlock();
#elif defined(Q_OS_MAC)
    MacUnlock();
#else
#endif
}

#ifdef Q_OS_WIN
void ipc::SystemMutex::WinLock()
{
    WaitForSingleObject(
        _mutex,    // handle to mutex
        _dwMilliseconds); // wait until locked
}

void ipc::SystemMutex::WinUnlock()
{
    ReleaseMutex(_mutex);
}
#endif

#ifdef Q_OS_MAC
void ipc::SystemMutex::MacLock()
{
    //if ever crash, remove previous mutex
    ClearMutex(_mutexName);
    if(_mutex->create(1)){
        _mutex->lock();
    }else if (_mutex->attach()){
        _mutex->lock();
    }else{
        QSharedMemory::SharedMemoryError error = _mutex->error();
    }
}

void ipc::SystemMutex::MacUnlock()
{
    _mutex->unlock();
}

void ipc::SystemMutex::ClearMutex(QString MutexName)
{
    //Clear Crashed Mutex
    QSharedMemory mutex(MutexName);
    if(mutex.attach()){
       mutex.detach();
    }
}
#endif
