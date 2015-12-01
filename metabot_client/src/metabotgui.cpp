#include "metabotgui.h"
#include "ui_metabotgui.h"

MetaBotGui::MetaBotGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MetaBotGui)
{
    ui->setupUi(this);
}

MetaBotGui::~MetaBotGui()
{
    delete ui;
}
