#include "CRowNames.h"

CRowNames::CRowNames()
{

}

void CRowNames::on_pbAdd_clicked()
{
    if(!getLineEditValue().isEmpty()){
        emit rowName(getLineEditValue());
        clearLineEdit();
        close();
    }else{
        QMessageBox msgWarning;
        msgWarning.setText("Please, enter a correct value or cancel operation.");
        msgWarning.setIcon(QMessageBox::Warning);
        msgWarning.setWindowTitle("Caution");
        msgWarning.exec();
    }

}
