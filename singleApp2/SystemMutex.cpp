#include "SystemMutex.h"

ipc::SystemMutex::SystemMutex(const wchar_t* name, const unsigned long dwMilliseconds):
    _dwMilliseconds(dwMilliseconds)
{
    _mutexName = QString::fromWCharArray(name);
    _mutex = std::make_unique<QSharedMemory>(_mutexName);
}

ipc::SystemMutex::~SystemMutex()
{
    _mutex->detach();
}

void ipc::SystemMutex::lock()
{
    MacLock();
}

void ipc::SystemMutex::unlock()
{
    MacUnlock();
}

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
    bool battach = mutex.isAttached();
    if(mutex.attach()){
       mutex.detach();
    }
}
