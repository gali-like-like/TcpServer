#include "socketthread.h"
SocketThread::SocketThread(QTcpSocket *socket)
{
    this->socket = socket;
    ipStr = this->socket->peerAddress().toString();
    port = this->socket->peerPort();
    this->setAutoDelete(true);
}

void SocketThread::run()
{
    qCInfo(category)<<NOW<<" "<<QThread::currentThreadId()<<"开始负责"<<ipStr<<":"<<port;
    QAbstractSocket::connect(socket,&QTcpSocket::readyRead,this,&SocketThread::do_socket_readyRead);
    QAbstractSocket::connect(socket,&QTcpSocket::disconnected,this,&SocketThread::do_socket_disconnected);
    while(this->socket->state() !=QAbstractSocket::SocketState::ClosingState)
    {

    }
    emit this->finished();
}

void SocketThread::do_socket_readyRead()
{
    QString info = QString("%1 [%2:%3]:%4").arg(NOW).arg(ipStr).arg(port).arg(socket->readAll());
    qCInfo(category)<<info;
}

void SocketThread::do_socket_disconnected()
{
    if (socket->state() != QAbstractSocket::UnconnectedState) {
        socket->waitForDisconnected();
    }
    QString info = QString("%1 [%2:%3]离线了").arg(NOW).arg(ipStr).arg(port);
    qCInfo(category)<<info;
    // this->socket->deleteLater();
}

SocketThread::~SocketThread()
{
    if(this->socket != nullptr)
    {
        qCInfo(category)<<NOW<<"socket没有被删除";
        if(this != nullptr)
        {
            qCInfo(category)<<NOW<<"runnable没有被删除";
        }
    }
}
