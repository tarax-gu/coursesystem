#ifndef ADMINISTORWINDOW_H
#define ADMINISTORWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "page_login.h"
#include "course.h"
#include "input_course.h"

namespace Ui {
class administerWindow;
}

class administerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit administerWindow(QWidget *parent = nullptr);
    ~administerWindow();
    void initcoursetable();
    void inithomeworktable();
    void read_course_information();
    void clear_homework_table();
    QString get_str(){return str_log;};
    Ui::administerWindow* get_ui(){return ui;};
    course item_c[20];
    int total_course = 0;
    int total_homework = 0;
    int a[20],choose;
    int num=0,cur_course=0;
    QString str_log;
signals:
    void is_closed();
    void operate_over();
    void issue_over();
    void is_clear();
    void send_log();

private slots:

    void on_btn_return_clicked();
    void on_btn_add_clicked();
    void update_table(); 
    void on_btn_delete_clicked();
    void on_btn_search_clicked();
    void on_btn_modify_clicked();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_btn_search_2_clicked();
    void on_btn_return_2_clicked();
    void on_btn_issue_clicked();

    void on_btn_clear_clicked();

private:
    Ui::administerWindow *ui;
    input_course input_c_add,input_c_modify;
};
#endif // ADMINISTORWINDOW_H
