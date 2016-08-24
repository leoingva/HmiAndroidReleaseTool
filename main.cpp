#include "dialog.h"
#include <QApplication>
#include<QString>
Dialog *dlg;

void customMessageHandler(QtMsgType type ,const QMessageLogContext &context, const QString & msg)
{
    dlg->setText(msg);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    dlg=&w;
    w.show();
    qInstallMessageHandler(customMessageHandler);
    return a.exec();
}


