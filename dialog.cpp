#include "dialog.h"
#include "ui_dialog.h"
#include<QDebug>
#include <QDomDocument>
#include<QFile>
#include<QTextStream>
#include<QCoreApplication>
#include<QDebug>
#include<QCommandLineOption>
#include<QTextCodec>
#include<QDir>
#include<QFileInfo>
#include<QDateTime>
#include<QMetaObject>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->listWidget_2->setCurrentRow(0);
    QMetaObject::invokeMethod(this,"text1changed",Qt::QueuedConnection);
}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::b1clicked()
{
    qDebug()<<"";
    QTextCodec *code = QTextCodec::codecForLocale();
    QTextCodec::setCodecForLocale(code);

    //qDebug()<<"b1clicked";
    QString filename = ui->lineEdit->text()+"\\AndroidManifest.xml";
    QString appname = ui->listWidget_2->currentItem()->text();
    QString versionname = ui->lineEdit_3->text();
    qDebug()<<"File path name = "<<filename;
    QDomDocument doc("AndroidManifest");
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"XML file not found";
        return ;
    }
    if (!doc.setContent(&file)) {
        file.close();
        qDebug()<<"XML file can not be parsed";
        return ;
    }
    file.close();

    QDomElement elem = doc.elementsByTagName("manifest").at(0).toElement();
    elem.setAttribute("android:versionName",versionname);
    elem = doc.elementsByTagName("application").at(0).toElement();
    elem.setAttribute("android:label",appname);
    //qDebug()<<"manifest->android:versionName set to "<<versionname;
    elem = doc.elementsByTagName("activity").at(0).toElement();
    elem.setAttribute("android:label",appname);
    //qDebug()<<"activity->android:label set to "<<appname;

    QFile outfile(filename);
    if (!outfile.open(QIODevice::WriteOnly))
        return ;
    QTextStream out(&outfile);
    doc.save(out,4);
    outfile.close();
    qDebug()<<"XML file modified";
    QMetaObject::invokeMethod(this,"text1changed",Qt::QueuedConnection);
}

void Dialog::b2clicked()
{
    qDebug()<<"";
    QString srcDir(ui->lineEdit_2->text());
    QString srcFile(srcDir + "\\android-build\\bin\\QtApp-release-signed.apk");
    if (!QFile::exists(srcFile)){
        qDebug("Source file not exists");
        return ;
    }
    //////////////////////////////////////////////////////
    QString xmlfile = srcDir + "\\android-build\\bin\\AndroidManifest.xml";
    QDomDocument doc("AndroidManifest");
    QFile file(xmlfile);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"XML file not found, check name failed";
        return ;
    }
    if (!doc.setContent(&file)) {
        file.close();
        qDebug()<<"XML file can not be parsed, check name failed";
        return ;
    }
    file.close();

    QDomElement elem = doc.elementsByTagName("activity").at(0).toElement();
    QString getname =     elem.attribute("android:label");
    if(getname!=ui->listWidget_2->currentItem()->text())
    {
        qDebug()<<"This exist apk's name is " << getname<<" , different from the name you choose: " << ui->listWidget_2->currentItem()->text()<<" please build it first";
        qDebug()<<"Copy process terminated";
        return;
    }

    //////////////////////////////////////////////////////
    QFileInfo fi(srcFile);
    qDebug()<<"File Build time = "<<fi.created().toString("yyyy/m/d  h:mm:ss");
    QDir dir;
    dir.mkpath("D:\\androidRelease");

    QString dstDir("D:\\androidRelease");
    QString platform;
    if(srcDir.contains("armeabi",Qt::CaseInsensitive)) platform = "ARM";
    if(srcDir.contains("x86",Qt::CaseInsensitive)) platform = "x86";
    qDebug()<<"platform = "<<platform;
    QString dstFile = dstDir + "\\" + ui->listWidget_2->currentItem()->text() + "_" + platform + ".apk";
    if (QFile::exists(dstFile)){
        QFile::remove(dstFile);
        qDebug("Destination file is already exists, removed");
    }

    if(!QFile::copy(srcFile,dstFile))
    {
        qDebug("Copy failed");
        return ;
    }
    qDebug()<<dstFile << " seccessfully copied";
}

void Dialog::setText(QString a)
{
    ui->plainTextEdit->appendPlainText(a);
}
void Dialog::text1changed()
{
    qDebug()<<"";
    QString filename = ui->lineEdit->text()+"\\AndroidManifest.xml";
    QDomDocument doc("AndroidManifest");
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"XML file not found";
        return ;
    }
    if (!doc.setContent(&file)) {
        file.close();
        qDebug()<<"XML file can not be parsed";
        return ;
    }
    file.close();

    QDomElement elem = doc.elementsByTagName("manifest").at(0).toElement();
    QString versionname = elem.attribute("android:versionName");
    elem = doc.elementsByTagName("activity").at(0).toElement();
    QString appname = elem.attribute("android:label");
    qDebug()<<"Current XML file setting is Version Name = "<<versionname<<", App Name = "<<appname;
}
