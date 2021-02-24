#include "CAboutScreen.h"
#include "ui_CAboutScreen.h"

CAboutScreen::CAboutScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAboutScreen)
{
    ui->setupUi(this);
}

CAboutScreen::~CAboutScreen()
{
    delete ui;
}

void CAboutScreen::on_pushButton_clicked()
{
    close();
}
