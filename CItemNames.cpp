#include "CItemNames.h"
#include "ui_CItemNames.h"

CItemNames::CItemNames(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CItemNames)
{
    ui->setupUi(this);
}

CItemNames::~CItemNames()
{
    delete ui;
}

QString CItemNames::getLineEditValue()
{
    return ui->LEPropertieName->text();
}

void CItemNames::clearLineEdit()
{
    ui->LEPropertieName->clear();
}



void CItemNames::on_pbAdd_clicked()
{

}

void CItemNames::on_pbCancel_clicked()
{
    ui->LEPropertieName->text().clear();
    emit canceledAction();
    close();
}
