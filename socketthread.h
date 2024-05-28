#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QRunnable>
#include <QLoggingCategory>
#include <QTcpSocket>
#include <QDateTime>
#include <QThread>
#undef NOW
#define NOW QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
extern QLoggingCategory category;
class SocketThread :public QObject, public QRunnable
{
    Q_OBJECT
public:
    SocketThread(QTcpSocket* socket);
    void run();
    ~SocketThread();
signals:
    void finished();
private slots:
    void do_socket_readyRead();
    void do_socket_disconnected();
private:
    QTcpSocket* socket;
    QString ipStr;
    quint16 port;
};

#endif // SOCKETTHREAD_H
