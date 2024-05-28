#include "tcpservice.h"

TcpService::TcpService(QObject *parent)
    : QObject{parent}
{
    tcpServer = new QTcpServer(this);
    bool res = tcpServer->listen(QHostAddress::LocalHost,2666);
    QString info = QString("%1 监听127.0.0.1:2666%3").arg(NOW).arg(res?"成功":"失败");
    qCInfo(category)<<info;
    tcpServer->setMaxPendingConnections(10);
    connect(tcpServer,&QTcpServer::newConnection,this,&TcpService::do_new_connection);
    connect(tcpServer,&QTcpServer::acceptError,this,&TcpService::do_acceptError);
    pool->setMaxThreadCount(10);
    qDebug()<<"service debug?"<<category.isDebugEnabled();
    qDebug()<<"service info?"<<category.isInfoEnabled();
    qDebug()<<"service critical?"<<category.isCriticalEnabled();
}

void TcpService::do_new_connection()
{
    QTcpSocket* socket = tcpServer->nextPendingConnection();
    QString info = QString("%1 [%2:%3]上线了").arg(NOW).
                   arg(socket->peerAddress().toString()).arg(socket->peerPort());
    qCInfo(category) << info;
    pool->setExpiryTimeout(1000);
    thread = new SocketThread(socket);
    connect(thread,&SocketThread::finished,this,&TcpService::do_work_finished);
    bool res = pool->tryStart(thread);
    SocketThread* & t = thread;
    QTcpSocket* & s = socket;
    threads.insert(t,s);
    qCInfo(category)<<NOW<<"addr:"<<thread;
    QString threadStartInfo = QString("%1 [%2:%3]%4").arg(NOW).
                              arg(socket->peerAddress().toString()).
                              arg(socket->peerPort()).
                              arg(res?"线程启动成功":"线程启动失败");
    qCInfo(category)<<threadStartInfo;
}

void TcpService::do_acceptError(QAbstractSocket::SocketError socketError)
{
    QString info = QString("%1 %2").arg(NOW).arg(tcpServer->errorString());
    // QLoggingCategory category("driver.usb");
    qCCritical(category) << info;
}

void TcpService::do_work_finished()
{
    int count = pool->activeThreadCount();
    qCInfo(category)<<QString("%1 活跃线程数:%2").arg(NOW).arg(count);
    QTcpSocket* socket = threads.value(thread);
    delete socket;
    threads.remove(thread);
    if(thread != nullptr)
    {
        bool res = pool->tryTake(thread);
        qCInfo(category)<<NOW<<QString("%1").arg(res?"删除成功":"删除失败");
    }
    // QObject* object = this->sender();
    // qCInfo(category)<<NOW<<object<<"开始被删除";
    // if(object != nullptr)
    // {
    //     SocketThread* r = qobject_cast<SocketThread*>(object);
    //     bool res = pool->tryTake(r);
    //     qCInfo(category)<<QString("%1 %2").arg(NOW).arg(res?"删除成功":"删除失败");
    // }
}
