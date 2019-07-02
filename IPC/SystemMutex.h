#ifndef WINDOWS_SYSTEMMUTEX_H
#define WINDOWS_SYSTEMMUTEX_H
#include <QtCore/QtGlobal>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#ifdef Q_OS_MAC
#include <QtCore/QSharedMemory>
#endif

#include <string>

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
#ifdef Q_OS_WIN

    /**
     * The function is uesed to lock mutex in Windows
     */
    void WinLock();
    
    /**
     * The function is uesed to unlock mutex in Windows
     */
    void WinUnlock();
#elif defined(Q_OS_MAC)
    
    /**
     * The function is uesed to lock mutex in MAC
     */
    void MacLock();
    
    /**
     * The function is uesed to unlock mutex in MAC
     */
    void MacUnlock();
    
    /**
     * The function is uesed to clear mutex in MAC if crash ever happens
     * @param MutexName mutex name, it's used to detach shared memory
     */
    void ClearMutex(QString MutexName);
#else
#endif

private:
#ifdef Q_OS_WIN
    HANDLE _mutex {};
#elif defined(Q_OS_MAC)
    std::unique_ptr<QSharedMemory> _mutex {};
    QString _mutexName {};
#else
#endif
    unsigned long _dwMilliseconds;
};
} // namespace ipc
#endif // SYTEMMUTEX_H
