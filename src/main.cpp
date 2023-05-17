#include "mainwindow.h"
#include <QStyleFactory>
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //若是系统自带的QStyle风格，则需要先创建为QStyleFactory::create(""，然后设置qApp->setStyle()
    QStringList listStyle = QStyleFactory::keys();
    foreach(QString val, listStyle)     //打印当前系统支持的系统风格,,且打印出来
        qDebug()<<val<<"  ";
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ToupCameraDemo_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
