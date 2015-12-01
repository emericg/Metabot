#ifndef METABOTCONTROL_H
#define METABOTCONTROL_H

#include <QMainWindow>

namespace Ui {
class MetaBotGui;
}

class MetaBotGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit MetaBotGui(QWidget *parent = 0);
    ~MetaBotGui();

private:
    Ui::MetaBotGui *ui;
};

#endif // METABOTCONTROL_H
