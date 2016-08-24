#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void setText(QString a);
public slots:
    void b1clicked();
    void b2clicked();
    void text1changed();
private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
