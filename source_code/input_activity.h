#ifndef INPUT_ACTIVITY_H
#define INPUT_ACTIVITY_H

#include <QWidget>
#include"activity.h"
namespace Ui {
class input_activity;
}

class input_activity : public QWidget
{
    Q_OBJECT

public:
    explicit input_activity(QWidget *parent = nullptr);
    ~input_activity();
    activity get_activity(){return item;};
signals:
    void is_closed();
private slots:
    void on_btn_cancel_clicked();

    void on_btn_yes_clicked();

private:
    Ui::input_activity *ui;
    activity item;
};

#endif // INPUT_ACTIVITY_H
