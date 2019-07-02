#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <windows.h>
#include <mutex>
#include <cserver.h>
#include <SystemMutex.h>
typedef HANDLE mutex_t;


void Mutex(const std::string& strMutex)
{
    std::wstring wstrMutex(strMutex.begin(), strMutex.end());
    LPCWSTR lpcwName = wstrMutex.c_str();
    mutex_t _mutex = CreateMutex(
        NULL,  // default security attributes
        FALSE, // initially not owned
        lpcwName); // unnamed mutex
        
    WaitForSingleObject(
        _mutex,    // handle to mutex
        INFINITE); // wait until locked
        
    while(1);
    
    ReleaseMutex(_mutex);
    /*
    int len = ((strMutex.c_str()) ? (strlen(strMutex.c_str()) + 1) : 1);
    if (len) {
        m_pstrMutex = new char[len];
        strcpy(m_pstrMutex, strMutex.c_str() ? strMutex.c_str() : "");
    }
    */
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    /*
    CServer test;
    if(!test.mpTryToConnectToServer()){
        test.RunServer();
    }*/

    //Mutex("clickshareMutex");
 
    {
    ipc::SystemMutex mutex(L"testmutex", INFINITE);
    std::lock_guard<ipc::SystemMutex> lock(mutex);
    }
    
    return app.exec();
}
