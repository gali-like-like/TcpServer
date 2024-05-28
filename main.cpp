#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QStandardPaths>
#include <QLoggingCategory>
#include "tcpservice.h"
QLoggingCategory category("service");
#define NOW QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QLoggingCategory::setFilterRules("service.debug=true;service.info=true;service.critical=true");
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Service_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    qCInfo(category)<<NOW<<QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
    TcpService t;
    return a.exec();
}
