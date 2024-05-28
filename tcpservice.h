#ifndef TCPSERVICE_H
#define TCPSERVICE_H
#include <QDateTime>
#include <QTcpServer>
#include <QLoggingCategory>
#include <QTcpSocket>
#include <QThreadPool>
#include "socketthread.h"
#undef NOW
#define NOW QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
extern QLoggingCategory category;
class TcpService : public QObject
{
    Q_OBJECT
public:
    explicit TcpService(QObject *parent = nullptr);

signals:

private slots:
    void do_new_connection();
    void do_acceptError(QAbstractSocket::SocketError socketError);
    void do_work_finished();
private:
    QTcpServer* tcpServer;
    SocketThread* thread;
    QMap<SocketThread*,QTcpSocket*> threads;
    QThreadPool* pool = QThreadPool::globalInstance();

};

#endif // TCPSERVICE_H
