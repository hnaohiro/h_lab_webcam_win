#ifndef DIALOGCONFIG_H
#define DIALOGCONFIG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class DialogConfig;
}

class DialogConfig : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogConfig(QWidget *parent = 0);
    ~DialogConfig();
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogConfig *ui;
    QSettings *settings;
};

#endif // DIALOGCONFIG_H
