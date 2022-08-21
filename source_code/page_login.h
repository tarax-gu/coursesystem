#ifndef PAGE_LOGIN_H
#define PAGE_LOGIN_H

#include <QWidget>

namespace Ui {
class page_login;
}

class page_login : public QWidget
{
    Q_OBJECT

public:
    explicit page_login(QWidget *parent = nullptr);
    ~page_login();

private slots:
    void on_btn_login_clicked();

    void on_pushButton_clicked();
signals:
    void sendStudentLogin();
    void sendAdminLogin();
private:
    Ui::page_login *ui;
};

#endif // PAGE_LOGIN_H
