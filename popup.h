#ifndef POPUP_H
#define POPUP_H

#include <QWidget>

namespace Ui {
class Popup;
}

class Popup : public QWidget
{
    Q_OBJECT
    
public:
    explicit Popup(QWidget *parent = 0);
    ~Popup();
    
private:
    Ui::Popup *ui;
};

#endif // POPUP_H
