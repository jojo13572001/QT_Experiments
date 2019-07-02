#ifndef WINDOWS_SYSTEMMUTEX_H
#define WINDOWS_SYSTEMMUTEX_H
#include <string>

#include <memory>
#include <Windows.h>

#include <QtCore/QSharedMemory>



namespace ipc
{
    /**
     * SystemMutex is a system level mutex that implements lock(), unlock() interface for lock_guard usage
     */
class SystemMutex
{
public:
    
    /**
     * Connect to a base unit by specifying the base unit name.
     * @param System mutex name, which could be used for IPC communication
     * @param dwMilliseconds is the max lock waiting time, default 0xFFFFFFFF - waiting forever.
     *        In Mac, the parameter is useless. Mac's lock always waits forever
     */
    SystemMutex(const wchar_t* name, const unsigned long dwMilliseconds = 0xFFFFFFFF);
    virtual ~SystemMutex();
    
    /**
     * The interface is uesed to lock mutex in Windows and MAC
     */
    virtual void lock();
    
    /**
     * The interface is uesed to lock mutex in Windows and MAC
     */
    virtual void unlock();

private:
    HANDLE _mutex {};
    QString _mutexName {};
    unsigned long _dwMilliseconds;
};
} // namespace ipc
#endif
