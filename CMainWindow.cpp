#include "CMainWindow.h"
#include "ui_CMainWindow.h"
#include "CColumnNames.h"
#include "CRowNames.h"
#include <QDebug>
#include <QString>
#include <algorithm>
#include <QFileDialog>
#include <QTextStream>
#include "CAboutScreen.h"

CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMainWindow)
{
    ui->setupUi(this);

    configureTables();
    ui->lbValueOfN->setVisible(false);
    ui->LBTotal->setVisible(false);
    ui->LBMedia->setVisible(false);

    ui->rbDefine->setEnabled(false);

    setDemandApplyed(false);
    setOfferApplyed(false);

    //testFunction();
}

CMainWindow::~CMainWindow()
{
    delete ui;
}

void CMainWindow::on_pbApplyDemand_clicked()
{
    setRowsDemand(ui->tbDemandMatrix->rowCount());
    setColumnsDemand(ui->tbDemandMatrix->columnCount());

    fillEmptyValuesOnDemand();    
    createTableDemand();    
    createTablePertinenceDemand();    
    createTableCardinalityDemand();

    setDemandApplyed(true);
}

void CMainWindow::on_pbApplyOferta_clicked()
{
    setRowsDemand(ui->tbOfferMatrix->rowCount());
    setColumnsOffer(ui->tbOfferMatrix->columnCount());

    fillEmptyValuesOnOffer();
    createTableOffer();
    createTablePertinenceOffer();
    createTableCardinalityOffer();

    setOfferApplyed(true);
}

void CMainWindow::configureTables()
{
    connect(ui->tbDemandMatrix, &QTableWidget::itemChanged, this, &CMainWindow::itemChanged);
    connect(ui->tbOfferMatrix, &QTableWidget::itemChanged, this, &CMainWindow::itemChanged);

    QHeaderView* demandMatrix = ui->tbDemandMatrix->horizontalHeader();
    demandMatrix->setSectionResizeMode(QHeaderView::Stretch);

    QHeaderView* demandPertinence = ui->tbPertinenceDemand->horizontalHeader();
    demandPertinence->setSectionResizeMode(QHeaderView::Stretch);

    QHeaderView* tbCardinalidades = ui->tbCardinalityDemand->horizontalHeader();
    tbCardinalidades->setSectionResizeMode(QHeaderView::Stretch);

    QHeaderView* tbMatrixOffer = ui->tbOfferMatrix->horizontalHeader();
    tbMatrixOffer->setSectionResizeMode(QHeaderView::Stretch);    

    QHeaderView* offerPertinence = ui->tbPertinenceOffer->horizontalHeader();
    offerPertinence->setSectionResizeMode(QHeaderView::Stretch);

    QHeaderView* tbCardinalidadesOffer = ui->tbCardinalityOffer->horizontalHeader();
    tbCardinalidadesOffer->setSectionResizeMode(QHeaderView::Stretch);

    QHeaderView* tbFormula = ui->tbFormula->horizontalHeader();
    tbFormula->setSectionResizeMode(QHeaderView::Stretch);

    QHeaderView* tbDemandOffer = ui->tbDemandOffer->horizontalHeader();
    tbDemandOffer->setSectionResizeMode(QHeaderView::Stretch);

    QHeaderView* tbFuzzyIndex = ui->tbFuzzyIndex->horizontalHeader();
    tbFuzzyIndex->setSectionResizeMode(QHeaderView::Stretch);
}

void CMainWindow::itemChanged(QTableWidgetItem *item)
{
    QObject* obj = sender();
    if( obj == ui->tbDemandMatrix ){
        ui->tbDemandMatrix->item(item->row(), item->column())->setTextAlignment(Qt::AlignCenter);
    }else{
        ui->tbOfferMatrix->item(item->row(), item->column())->setTextAlignment(Qt::AlignCenter);
    }
}

void CMainWindow::fillEmptyValuesOnDemand()
{
    disconnect(ui->tbDemandMatrix, &QTableWidget::itemChanged, this, &CMainWindow::itemChanged);
    for(int i = 0; i < getRowsDemand(); i++){
        for(int j = 0; j < getColumnsDemand(); j++){
            if(ui->tbDemandMatrix->item(i,j) == NULL){
                ui->tbDemandMatrix->setItem(i,j,new QTableWidgetItem("0"));
                ui->tbDemandMatrix->item(i,j)->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
    connect(ui->tbDemandMatrix, &QTableWidget::itemChanged, this, &CMainWindow::itemChanged);
}

void CMainWindow::createTableDemand()
{
    demandMatrix.resize(getRowsDemand());
    for(int i = 0; i < getRowsDemand(); i++){
        demandMatrix[i].resize(getColumnsDemand());
    }

    for(int i = 0; i < getRowsDemand(); i++){
        for(int j = 0; j < getColumnsDemand(); j++){
            demandMatrix[i][j] = ui->tbDemandMatrix->item(i,j)->text().toDouble();
        }
    }
}

void CMainWindow::createTablePertinenceDemand()
{
    for(int i = 0; i < getRowsDemand(); i++){
        for(int j = 0; j < getColumnsDemand(); j++){
            ui->tbPertinenceDemand->setItem(i,j,
                                              new QTableWidgetItem(QString::number(demandMatrix[i][j] / *std::max_element(demandMatrix[i].constBegin(), demandMatrix[i].constEnd()),
                                                                                   'f', 2)));
            ui->tbPertinenceDemand->item(i,j)->setTextAlignment(Qt::AlignCenter);
        }
    }

    demandPertinence.resize(getRowsDemand());
    for(int i = 0; i < getRowsDemand(); i++){
        demandPertinence[i].resize(getColumnsDemand());
    }

    for(int i = 0; i < getRowsDemand(); i++){
        for(int j = 0; j < getColumnsDemand(); j++){
            if(ui->tbPertinenceDemand->item(i,j)->text().isEmpty()){
                demandPertinence[i][j] = 0.0;
            }else{
                demandPertinence[i][j] = ui->tbPertinenceDemand->item(i,j)->text().toDouble();
            }
        }
    }
}

void CMainWindow::createTableCardinalityDemand()
{
    int SIZE_OF_CARDINALITY = 3;

    demandCardinality.resize(getRowsDemand());
    for(int i = 0; i < getRowsDemand(); i++){
         demandCardinality[i].resize(SIZE_OF_CARDINALITY);
    }

    for(int i = 0; i < getRowsDemand(); i++){
        for(int j = 0; j < SIZE_OF_CARDINALITY; j++){

            if(j == 0){
                double sumOfPertinences = 0.0;

                for(int w = 0; w < getColumnsDemand(); w++){
                    sumOfPertinences = sumOfPertinences + demandPertinence[i][w];
                }
                ui->tbCardinalityDemand->setItem(i, 0, new QTableWidgetItem(QString::number(sumOfPertinences, 'f', 2)));
                ui->tbCardinalityDemand->item(i,0)->setTextAlignment(Qt::AlignCenter);
                demandCardinality[i][0] = sumOfPertinences;
            }
            if(j == 1){
                  double aux = 0.0;
                  double sum = 0.0;
                  int column = getColumnsDemand();
                  for(int w = 0; w < getColumnsDemand(); w++){
                    aux = demandMatrix[i][w];
                    aux = aux * column;
                    sum = sum + aux;
                    column--;
                  }
                  demandCardinality[i][1] = (double)sum/ui->LENumberOfSpecialist->text().toInt();
                  ui->tbCardinalityDemand->setItem(i, 1, new QTableWidgetItem(QString::number( demandCardinality[i][1], 'f', 2)));
                  ui->tbCardinalityDemand->item(i,1)->setTextAlignment(Qt::AlignCenter);
            }
        }
    }

    QVector<double> aux;
    for(int i = 0; i < getRowsDemand(); i++){
        aux.push_back(demandCardinality[i][1]);
    }
    double maxValue = *std::max_element(aux.constBegin(), aux.constEnd());
    for(int i = 0; i < getRowsDemand(); i++){
        for(int j = 0; j < SIZE_OF_CARDINALITY; j++){
            if(j == 2){

                demandCardinality[i][2] = demandCardinality[i][1]/maxValue;
                ui->tbCardinalityDemand->setItem(i, 2, new QTableWidgetItem(QString::number( demandCardinality[i][2], 'f', 2)));
                ui->tbCardinalityDemand->item(i,2)->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
}

void CMainWindow::fillEmptyValuesOnOffer()
{
    disconnect(ui->tbOfferMatrix, &QTableWidget::itemChanged, this, &CMainWindow::itemChanged);

    for(int i = 0; i < getRowsDemand(); i++){
        for(int j = 0; j < getColumnsOffer(); j++){
            if(ui->tbOfferMatrix->item(i,j) == NULL){
                ui->tbOfferMatrix->setItem(i,j,new QTableWidgetItem("0"));
                ui->tbOfferMatrix->item(i,j)->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
    connect(ui->tbOfferMatrix, &QTableWidget::itemChanged, this, &CMainWindow::itemChanged);
}

void CMainWindow::createTableOffer()
{
    offerMatrix.resize(getRowsDemand());
    for(int i = 0; i < getRowsDemand(); i++){
        offerMatrix[i].resize(getColumnsOffer());
    }

    for(int i = 0; i < getRowsDemand(); i++){
        for(int j = 0; j < getColumnsOffer(); j++){
            offerMatrix[i][j] = ui->tbOfferMatrix->item(i,j)->text().toDouble();
        }
    }
}

void CMainWindow::createTablePertinenceOffer()
{
    for(int i = 0; i < getRowsDemand(); i++){
        for(int j = 0; j < getColumnsOffer(); j++){
            ui->tbPertinenceOffer->setItem(i,j,
                                              new QTableWidgetItem(QString::number(offerMatrix[i][j] / *std::max_element(offerMatrix[i].constBegin(), offerMatrix[i].constEnd()),
                                                                                   'f', 2)));
            ui->tbPertinenceOffer->item(i,j)->setTextAlignment(Qt::AlignCenter);
        }
    }

    offerPertinence.resize(getRowsDemand());
    for(int i = 0; i < getRowsDemand(); i++){
        offerPertinence[i].resize(getColumnsOffer());
    }

    for(int i = 0; i < getRowsDemand(); i++){
        for(int j = 0; j < getColumnsOffer(); j++){
            if(ui->tbPertinenceOffer->item(i,j)->text().isEmpty()){
                offerPertinence[i][j] = 0.0;
            }else{
                offerPertinence[i][j] = ui->tbPertinenceOffer->item(i,j)->text().toDouble();
            }
        }
    }
}

void CMainWindow::createTableCardinalityOffer()
{
    int SIZE_OF_CARDINALITY = 3;

    offerCardinality.resize(getRowsDemand());
    for(int i = 0; i < getRowsDemand(); i++){
        offerCardinality[i].resize(SIZE_OF_CARDINALITY);
    }

    for(int i = 0; i < getRowsDemand(); i++){
        for(int j = 0; j < SIZE_OF_CARDINALITY; j++){

            if(j == 0){
                double sumOfPertinences = 0.0;

                for(int w = 0; w < getColumnsOffer(); w++){
                    sumOfPertinences = sumOfPertinences + offerPertinence[i][w];
                }
                ui->tbCardinalityOffer->setItem(i, 0, new QTableWidgetItem(QString::number(sumOfPertinences, 'f', 2)));
                ui->tbCardinalityOffer->item(i,0)->setTextAlignment(Qt::AlignCenter);
                offerCardinality[i][0] = sumOfPertinences;
            }


            if(j == 1){
                double aux = 0.0;
                double sum = 0.0;
                int column = getColumnsOffer();
                for(int w = 0; w < getColumnsOffer(); w++){
                  aux = offerMatrix[i][w];
                  aux = aux * column;
                  sum = sum + aux;
                  column--;
                }
                offerCardinality[i][1] = (double)sum/ui->LENumberOfSpecialist->text().toInt();
                ui->tbCardinalityOffer->setItem(i, 1, new QTableWidgetItem(QString::number( offerCardinality[i][1], 'f', 2)));
                ui->tbCardinalityOffer->item(i,1)->setTextAlignment(Qt::AlignCenter);
            }
        }
    }

    QVector<double> aux;
    for(int i = 0; i < getRowsDemand(); i++){
        aux.push_back(offerCardinality[i][1]);
    }
    double maxValue = *std::max_element(aux.constBegin(), aux.constEnd());

    for(int i = 0; i < getRowsDemand(); i++){
        for(int j = 0; j < SIZE_OF_CARDINALITY; j++){
            if(j == 2){

                offerCardinality[i][2] = offerCardinality[i][1]/maxValue;
                ui->tbCardinalityOffer->setItem(i,2, new QTableWidgetItem(QString::number( offerCardinality[i][2], 'f', 2)));
                ui->tbCardinalityOffer->item(i,2)->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
}

void CMainWindow::on_addLinguistic_clicked()
{    
    setItemToAdd(new CColumnNames());

    connect(getItemToAdd(), &CColumnNames::columnName, [this](QString columnName){
        setCanceledAction(false);
        columnName.replace(" ", "_");
        getSpecialistNames().append(columnName);
    });

    connect(getItemToAdd(), &CColumnNames::canceledAction, [this](){
        setCanceledAction(true);
    });
    getItemToAdd()->exec();

    if(!getCanceledAction()){
        ui->tbDemandMatrix->insertColumn(ui->tbDemandMatrix->columnCount());
        ui->tbDemandMatrix->setHorizontalHeaderLabels(getSpecialistNames());
        ui->tbPertinenceDemand->insertColumn(ui->tbPertinenceDemand->columnCount());
        ui->tbPertinenceDemand->setHorizontalHeaderLabels(getSpecialistNames());
        ui->tbFormula->insertRow(ui->tbFormula->columnCount());
        ui->tbFormula->setVerticalHeaderLabels(getSpecialistNames());
        ui->tbDemandOffer->insertRow(ui->tbDemandOffer->columnCount());
        ui->tbDemandOffer->setVerticalHeaderLabels(getSpecialistNames());
    }
}

void CMainWindow::on_removeLinguistic_clicked()
{
    if(ui->tbDemandMatrix->columnCount() != 0){
        ui->tbDemandMatrix->removeColumn(ui->tbDemandMatrix->columnCount()-1);
        ui->tbPertinenceDemand->removeColumn(ui->tbPertinenceDemand->columnCount()-1);
        ui->tbFormula->removeRow(ui->tbFormula->rowCount()-1);
        ui->tbDemandOffer->removeRow(ui->tbDemandOffer->rowCount()-1);
        getSpecialistNames().removeLast();
    }else{
        QMessageBox msgWarning;
        msgWarning.setText("You don't have more linguistics to remove.");
        msgWarning.setIcon(QMessageBox::Warning);
        msgWarning.setWindowTitle("Caution");
        msgWarning.exec();
    }
}

void CMainWindow::on_addFactor_clicked()
{
    setItemToAdd(new CRowNames());
    connect(getItemToAdd(), &CRowNames::rowName, [this](QString rowName){
        setCanceledAction(false);
        rowName.replace(" ", "_");
        getFactorNames().append(rowName);
    });

    connect(getItemToAdd(), &CRowNames::canceledAction, [this](){
       setCanceledAction(true);
    });
    getItemToAdd()->exec();

    if(!getCanceledAction()){
        ui->tbDemandMatrix->insertRow(ui->tbDemandMatrix->rowCount());
        ui->tbDemandMatrix->setVerticalHeaderLabels(getFactorNames());
        ui->tbPertinenceDemand->insertRow(ui->tbPertinenceDemand->rowCount());
        ui->tbPertinenceDemand->setVerticalHeaderLabels(getFactorNames());
        ui->tbCardinalityDemand->insertRow(ui->tbCardinalityDemand->rowCount());
        ui->tbCardinalityDemand->setVerticalHeaderLabels(getFactorNames());
        ui->tbOfferMatrix->insertRow(ui->tbOfferMatrix->rowCount());
        ui->tbOfferMatrix->setVerticalHeaderLabels(getFactorNames());
        ui->tbCardinalityOffer->insertRow(ui->tbCardinalityOffer->rowCount());
        ui->tbCardinalityOffer->setVerticalHeaderLabels(getFactorNames());
        ui->tbPertinenceOffer->insertRow(ui->tbPertinenceOffer->rowCount());
        ui->tbPertinenceOffer->setVerticalHeaderLabels(getFactorNames());        
        ui->tbFuzzyIndex->insertRow(ui->tbFuzzyIndex->rowCount());
        ui->tbFuzzyIndex->setVerticalHeaderLabels(getFactorNames());
    }
}

void CMainWindow::on_removeFactor_clicked()
{
    if(ui->tbDemandMatrix->rowCount() != 0){
        ui->tbDemandMatrix->removeRow(ui->tbDemandMatrix->rowCount()-1);
        ui->tbPertinenceDemand->removeRow(ui->tbPertinenceDemand->rowCount()-1);
        ui->tbCardinalityDemand->removeRow(ui->tbCardinalityDemand->rowCount()-1);
        ui->tbOfferMatrix->removeRow(ui->tbCardinalityDemand->rowCount()-1);
        ui->tbCardinalityOffer->removeRow(ui->tbCardinalityDemand->rowCount()-1);
        ui->tbPertinenceOffer->removeRow(ui->tbCardinalityDemand->rowCount()-1);
        ui->tbFuzzyIndex->removeRow(ui->tbFuzzyIndex->rowCount()-1);
        getFactorNames().removeLast();
    }else{
        QMessageBox msgWarning;
        msgWarning.setText("You don't have more factors to remove.");
        msgWarning.setIcon(QMessageBox::Warning);
        msgWarning.setWindowTitle("Caution");
        msgWarning.exec();
    }
}

void CMainWindow::on_tbAddOferta_clicked()
{
    setItemToAdd(new CColumnNames());

    connect(getItemToAdd(), &CColumnNames::columnName, [this](QString satisfyName){
        setCanceledAction(false);
        satisfyName.replace(" ", "_");
        getSatisfyIndex().append(satisfyName);
    });

    connect(getItemToAdd(), &CColumnNames::canceledAction, [this](){
        setCanceledAction(true);
    });
    getItemToAdd()->exec();

    if(!getCanceledAction()){
        ui->tbOfferMatrix->insertColumn(ui->tbOfferMatrix->columnCount());
        ui->tbOfferMatrix->setHorizontalHeaderLabels(getSatisfyIndex());

        ui->tbPertinenceOffer->insertColumn(ui->tbPertinenceOffer->columnCount());
        ui->tbPertinenceOffer->setHorizontalHeaderLabels(getSatisfyIndex());

        ui->tbFormula->insertColumn(ui->tbFormula->columnCount());
        ui->tbFormula->setHorizontalHeaderLabels(getSatisfyIndex());

        ui->tbDemandOffer->insertColumn(ui->tbDemandOffer->columnCount());
        ui->tbDemandOffer->setHorizontalHeaderLabels(getSatisfyIndex());
    }
}

void CMainWindow::on_tbRemoveOferta_clicked()
{
    if(ui->tbOfferMatrix->columnCount() != 0){
        ui->tbOfferMatrix->removeColumn(ui->tbOfferMatrix->columnCount()-1);
        ui->tbPertinenceOffer->removeColumn(ui->tbPertinenceOffer->columnCount()-1);
        ui->tbFormula->removeColumn(ui->tbFormula->columnCount()-1);
        ui->tbDemandOffer->removeColumn(ui->tbDemandOffer->columnCount()-1);
        getSatisfyIndex().removeLast();

    }else{
        QMessageBox msgWarning;
        msgWarning.setText("You don't have more indexes to remove.");
        msgWarning.setIcon(QMessageBox::Warning);
        msgWarning.setWindowTitle("Caution");
        msgWarning.exec();
    }
}

void CMainWindow::on_actionSave_triggered()
{
    QString filePath;
    QFile file;
    QString auxiliar;
    filePath = QFileDialog::getSaveFileName(this, "Save file...",".", "qFuzzApp file(*.fuz)");
    file.setFileName(filePath);

    on_pbApplyDemand_clicked();
    on_pbApplyOferta_clicked();

    QTextStream out(&file);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        out << "/************************************************************************** \n";
        out << "* ======================================================================= * \n";
        out << "*                              qFuzzyApp                                  * \n";
        out << "*                               v. 0.1                                    * \n";
        out << "* ======================================================================= * \n";
        out << "***************************************************************************/\n";
        out << "\n";
        out << "------------------------------- Settings -----------------------------------\n";
        out << "*TITLE \t" << ui->LETitle->text() << "\n";
        out << "*MAX_VALUES \t" << ui->LEMaximunValuesOnOffers->text() << "\n";
        out << "*NUM_SPECILISTS \t" << ui->LENumberOfSpecialist->text() << "\n";               
        if(ui->rbCoppeCosenza->isChecked()){
            out << "*MODEL \t" << "COPPE-COSENZA" << "\n";
        }else if(ui->rbDefine->isChecked()){
            out << "*MODEL \t" << "DEFINE_MODEL" << "\n";
        }
        out << "\n";
        out << "-------------------------------- Demand -----------------------------------\n";
        out << "*LINGUISTICS \n";
        for(int i = 0; i < specialistNames.size(); i++){
            out << specialistNames.at(i) << "\t";
        }
        out << "\n";
        out << "\n";
        out << "*FACTORS \n";
        for(int i = 0; i < factorNames.size(); i++){
            out << factorNames.at(i) << "\t";
        }
        out << "\n";
        out << "\n";
        out << "*VALUES_DEMAND \n";
        for(int i = 0; i < getRowsDemand();i++){
            for (int j = 0; j < getColumnsDemand();j++){
                out << demandMatrix[i][j] << "\t";
            }
            out << "\n";
        }
        out << "\n";
        out << "-------------------------------- Offer ------------------------------------\n";
        out << "*INDEXES \n";
        for(int i = 0; i < satisfyIndex.size(); i++){
            out << satisfyIndex.at(i) << "\t";
        }
        out << "\n";
        out << "\n";
        out << "*VALUES_OFFER \n";
        for(int i = 0; i < getRowsDemand();i++){
            for (int j = 0; j < getColumnsOffer();j++){
                out << offerMatrix[i][j] << "\t";
            }
            out << "\n";
        }
        out << "\n";
        out << "----------------------------- End of File ----------------------------------\n";
    }
    file.close();
    QMessageBox::information(this, "qFuzzyApp", "File saved successfully.", QMessageBox::Ok);
}

void CMainWindow::on_actionOpen_File_triggered()
{
    QString filePath;
    QFile file;
    QString auxiliar;

    filePath = QFileDialog::getOpenFileName(this, "Open file...", ".", "qFuzzApp file(*.fuz)");
    file.setFileName(filePath);

    QTextStream in(&file);
    if(file.open(QIODevice::ReadOnly)){
        while(!in.atEnd()){
            QString line = in.readLine();
            QStringList list;
            list = line.split(QRegExp("\\s+"));

            if(list.at(0) == "*TITLE"){
                QString aux = "";
                for(int i = 1; i < list.size(); i++){
                    aux.append(list.at(i) + " ");
                }
                ui->LETitle->setText(aux);
            }
            if(list.at(0) == "MAX_VALUES"){
                ui->LEMaximunValuesOnOffers->setText(list.at(1));
            }
            if(list.at(0) == "*NUM_SPECILISTS"){
                ui->LENumberOfSpecialist->setText(list.at(1));
            }

            if(list.at(0) == "*MODEL"){
                if(list.at(1) == "COPPE-COSENZA"){
                    ui->rbCoppeCosenza->setChecked(true);
                }else if (list.at(1) == "DEFINE"){
                    ui->rbDefine->setChecked(true);
                }
            }
            if(list.at(0) == "*LINGUISTICS"){
                line = in.readLine();
                list = line.split(QRegExp("\\s+"));

                for(int i = 0; i < list.size(); i++){
                    if(list.at(i) != ""){
                        specialistNames.append(list.at(i));
                        ui->tbDemandMatrix->insertColumn(ui->tbDemandMatrix->columnCount());
                        ui->tbPertinenceDemand->insertColumn(ui->tbPertinenceDemand->columnCount());
                        ui->tbFormula->insertRow(ui->tbFormula->columnCount());
                        ui->tbDemandOffer->insertRow(ui->tbDemandOffer->columnCount());
                    }
                }
                ui->tbDemandMatrix->setHorizontalHeaderLabels(getSpecialistNames());
                ui->tbPertinenceDemand->setHorizontalHeaderLabels(getSpecialistNames());                
                ui->tbFormula->setVerticalHeaderLabels(getSpecialistNames());
                ui->tbDemandOffer->setVerticalHeaderLabels(getSpecialistNames());
            }
            if(list.at(0) == "*FACTORS"){
                line = in.readLine();
                list = line.split(QRegExp("\\s+"));

                for(int i = 0; i < list.size(); i++){
                    if(list.at(i) != ""){
                        factorNames.append(list.at(i));
                        ui->tbDemandMatrix->insertRow(ui->tbDemandMatrix->rowCount());
                        ui->tbPertinenceDemand->insertRow(ui->tbPertinenceDemand->rowCount());
                        ui->tbCardinalityDemand->insertRow(ui->tbCardinalityDemand->rowCount());
                        ui->tbOfferMatrix->insertRow(ui->tbOfferMatrix->rowCount());
                        ui->tbCardinalityOffer->insertRow(ui->tbCardinalityOffer->rowCount());
                        ui->tbPertinenceOffer->insertRow(ui->tbPertinenceOffer->rowCount());                        
                        ui->tbFuzzyIndex->insertRow(ui->tbFuzzyIndex->rowCount());
                    }
                }
                ui->tbDemandMatrix->setVerticalHeaderLabels(getFactorNames());
                ui->tbPertinenceDemand->setVerticalHeaderLabels(getFactorNames());
                ui->tbCardinalityDemand->setVerticalHeaderLabels(getFactorNames());
                ui->tbOfferMatrix->setVerticalHeaderLabels(getFactorNames());
                ui->tbCardinalityOffer->setVerticalHeaderLabels(getFactorNames());
                ui->tbPertinenceOffer->setVerticalHeaderLabels(getFactorNames());
                ui->tbFuzzyIndex->setVerticalHeaderLabels(getFactorNames());
            }
            if(list.at(0) == "*VALUES_DEMAND"){
                line = in.readLine();
                list = line.split(QRegExp("\\s+"));

                disconnect(ui->tbDemandMatrix, &QTableWidget::itemChanged, this, &CMainWindow::itemChanged);

                for(int row = 0; row < factorNames.size();row++){
                    for(int column = 0; column < specialistNames.size();column++){
                        ui->tbDemandMatrix->setItem(row,column, new QTableWidgetItem(list.at(column)));
                        ui->tbDemandMatrix->item(row,column)->setTextAlignment(Qt::AlignCenter);
                    }
                    line = in.readLine();
                    list = line.split(QRegExp("\\s+"));
                }
                connect(ui->tbDemandMatrix, &QTableWidget::itemChanged, this, &CMainWindow::itemChanged);
            }
            if(list.at(0) == "*INDEXES"){
                line = in.readLine();
                list = line.split(QRegExp("\\s+"));

                for(int i = 0; i < list.size();i++){
                    if(list.at(i) != ""){
                        satisfyIndex.append(list.at(i));
                        ui->tbOfferMatrix->insertColumn(ui->tbOfferMatrix->columnCount());
                        ui->tbPertinenceOffer->insertColumn(ui->tbPertinenceOffer->columnCount());                        
                        ui->tbFormula->insertColumn(ui->tbFormula->columnCount());
                        ui->tbDemandOffer->insertColumn(ui->tbDemandOffer->columnCount());
                    }
                }
                ui->tbOfferMatrix->setHorizontalHeaderLabels(getSatisfyIndex());
                ui->tbPertinenceOffer->setHorizontalHeaderLabels(getSatisfyIndex());
                ui->tbFormula->setHorizontalHeaderLabels(getSatisfyIndex());
                ui->tbDemandOffer->setHorizontalHeaderLabels(getSatisfyIndex());
            }
            if(list.at(0) == "*VALUES_OFFER"){
                line = in.readLine();
                list = line.split(QRegExp("\\s+"));
                disconnect(ui->tbOfferMatrix, &QTableWidget::itemChanged, this, &CMainWindow::itemChanged);

                for(int row = 0; row < factorNames.size();row++){
                    for(int column = 0; column < satisfyIndex.size();column++){
                        ui->tbOfferMatrix->setItem(row,column, new QTableWidgetItem(list.at(column)));
                        ui->tbOfferMatrix->item(row,column)->setTextAlignment(Qt::AlignCenter);
                    }
                    line = in.readLine();
                    list = line.split(QRegExp("\\s+"));
                }
                connect(ui->tbOfferMatrix, &QTableWidget::itemChanged, this, &CMainWindow::itemChanged);
            }
        }
    }
    file.close();
    QMessageBox::information(this, "qFuzzyApp", "File loaded successfully.", QMessageBox::Ok);
}

void CMainWindow::createTableFormula()
{
    QString aux = "";
    for (int row = 0; row < getColumnsDemand(); row++){
        for (int column = 0; column < getColumnsOffer(); column++){

            if(row == column){
                ui->tbFormula->setItem(row,column, new QTableWidgetItem("1"));
                ui->tbFormula->item(row,column)->setTextAlignment(Qt::AlignCenter);
            }else{
                aux = "";
                aux.append("1+");
                aux.append(QString::number(row - column));
                aux.append("/n");
                ui->tbFormula->setItem(row,column, new QTableWidgetItem(aux));
                ui->tbFormula->item(row,column)->setTextAlignment(Qt::AlignCenter);

                aux = "";
                aux.append("1-");
                aux.append(QString::number(abs(column - row)));
                aux.append("/n");
                ui->tbFormula->setItem(column, row, new QTableWidgetItem(aux));
                ui->tbFormula->item(column, row)->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
    ui->lbValueOfN->setVisible(true);
    ui->lbValueOfN->setText("n = " + QString::number(getRowsDemand()));
}

void CMainWindow::createTableDemandOffer()
{
    double n = getRowsDemand();
    double value = 0.0;
    QString aux = "";    

    demandOffer.resize(getColumnsDemand());
    for(int i = 0; i < getColumnsDemand(); i++){
        demandOffer[i].resize(getColumnsOffer());
    }

    for (int row = 0; row < getColumnsDemand(); row++){
         for (int column = 0; column < getColumnsOffer(); column++){

             if(row == column){
                 ui->tbDemandOffer->setItem(row,column, new QTableWidgetItem("1.0"));
                 ui->tbDemandOffer->item(row,column)->setTextAlignment(Qt::AlignCenter);
                 demandOffer[row][column] = 1.0;
             }else{
                 value = 1 + (row - column)/n;
                 ui->tbDemandOffer->setItem(row,column, new QTableWidgetItem(QString::number(value, 'f', 2)));
                 ui->tbDemandOffer->item(row,column)->setTextAlignment(Qt::AlignCenter);
                 demandOffer[row][column] = value;

                 value = 1 - (abs(column - row))/n;
                 ui->tbDemandOffer->setItem(column, row, new QTableWidgetItem(QString::number(value, 'f',2)));
                 ui->tbDemandOffer->item(column, row)->setTextAlignment(Qt::AlignCenter);
                 demandOffer[column][row] = value;
             }
         }
    }
}

void CMainWindow::createTableFuzzyIndex()
{
    fuzzyIndexes.resize(getRowsDemand());

    for(int row = 0; row < getRowsDemand(); row++){
        QVector<int> auxDemand;
        for(int i = 0; i < getColumnsDemand(); i++){
            auxDemand.push_back(demandMatrix[row][i]);
        }
        auto positionDemand = std::distance(auxDemand.begin(), std::max_element(auxDemand.begin(), auxDemand.end()));

        QVector<int> auxOffer;
        for(int i = 0; i < getColumnsOffer(); i++){
            auxOffer.push_back(offerMatrix[row][i]);
        }
        auto positionOffer = std::distance(auxOffer.begin(), std::max_element(auxOffer.begin(), auxOffer.end()));

        ui->tbFuzzyIndex->setItem(row,0, new QTableWidgetItem(QString::number(demandOffer[positionDemand][positionOffer], 'f', 3)));
        ui->tbFuzzyIndex->item(row,0)->setTextAlignment(Qt::AlignCenter);
        fuzzyIndexes[row] = demandOffer[positionDemand][positionOffer];
    }

    double total = 0.0;
    for(int i = 0; i < fuzzyIndexes.size();i++){
        total = total + fuzzyIndexes[i];
    }
    ui->LBTotal->setText("Total:      " + QString::number(total, 'f', 3));
    ui->LBMedia->setText("Média:      " + QString::number(total/fuzzyIndexes.size(), 'f',3));
    ui->LBTotal->setVisible(true);
    ui->LBMedia->setVisible(true);
}

void CMainWindow::createDemmandPlot()
{
    QVector<double> xData;
    for(int i = 0; i < 6; i++){
        xData.push_back(i+1);
    }

    QVector<QVector<double> > yData;
    for(int i = columnsDemand-1; i >= 0; i--){
        QVector<double> yValues;
        yValues.resize(columnsDemand+1);
        yValues.fill(0.0);
        yValues[i] = 1.0;

        qDebug() << yValues;
        yData.push_back(yValues);
    }

    QVector<QColor> colors;
    colors.push_back(Qt::red);
    colors.push_back(Qt::blue);
    colors.push_back(Qt::darkCyan);
    colors.push_back(Qt::darkMagenta);
    colors.push_back(Qt::darkGray);
    colors.push_back(Qt::darkGreen);
    colors.push_back(Qt::yellow);

    QVector<QPen> currentPen;
    for(int i = 0; i < colors.size();i++){
        QPen pen;
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(3);
        pen.setColor(colors[i]);
        currentPen.push_back(pen);
    }

    QPen pen;
    pen.setStyle(Qt::NoPen);
    pen.setWidth(0);

    ui->plotDemmand->legend->setVisible(true);
    ui->plotDemmand->legend->setBorderPen(pen);

    QCPLayoutGrid *subLayout = new QCPLayoutGrid;
    ui->plotDemmand->plotLayout()->addElement(0, 1, subLayout);
    subLayout->addElement(0, 0, new QCPLayoutElement);
    subLayout->addElement(1, 0, ui->plotDemmand->legend);
    subLayout->addElement(2, 0, new QCPLayoutElement);
    ui->plotDemmand->plotLayout()->setColumnStretchFactor(1, 0.001);

    for(int i = 0; i < columnsDemand; i++){
        ui->plotDemmand->addGraph();
        ui->plotDemmand->graph(i)->setData(xData, yData[i]);
        ui->plotDemmand->graph(i)->setPen(currentPen[i]);
        ui->plotDemmand->graph(i)->setName(specialistNames.at(i));

        ui->plotDemmand->xAxis->setRange(1, 5);
        ui->plotDemmand->yAxis->setRange(0, 1);
        ui->plotDemmand->replot();
    }
}

void CMainWindow::createOfferPlot()
{
    QVector<double> xData;
    for(int i = 0; i < 6; i++){
        xData.push_back(i+1);

    }

    QVector<QVector<double> > yData;
    for(int i = columnsOffer-1; i >= 0; i--){
        QVector<double> yValues;
        yValues.resize(columnsOffer+1);
        yValues.fill(0.0);
        yValues[i] = 1.0;

        qDebug() << yValues;
        yData.push_back(yValues);
    }

    QVector<QColor> colors;
    colors.push_back(Qt::red);
    colors.push_back(Qt::blue);
    colors.push_back(Qt::darkCyan);
    colors.push_back(Qt::darkMagenta);
    colors.push_back(Qt::darkGray);
    colors.push_back(Qt::darkGreen);
    colors.push_back(Qt::yellow);

    QVector<QPen> currentPen;
    for(int i = 0; i < colors.size();i++){
        QPen pen;
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(3);
        pen.setColor(colors[i]);
        currentPen.push_back(pen);
    }

    QPen pen;
    pen.setStyle(Qt::NoPen);
    pen.setWidth(0);

    ui->plotOffer->legend->setVisible(true);
    ui->plotOffer->legend->setBorderPen(pen);
    QCPLayoutGrid *subLayout = new QCPLayoutGrid;
    ui->plotOffer->plotLayout()->addElement(0, 1, subLayout);
    subLayout->addElement(0, 0, new QCPLayoutElement);
    subLayout->addElement(1, 0, ui->plotOffer->legend);
    subLayout->addElement(2, 0, new QCPLayoutElement);
    ui->plotOffer->plotLayout()->setColumnStretchFactor(1, 0.001);

    for(int i = 0; i < columnsOffer; i++){
        ui->plotOffer->addGraph();
        ui->plotOffer->graph(i)->setData(xData, yData[i]);
        ui->plotOffer->graph(i)->setPen(currentPen[i]);
        ui->plotOffer->graph(i)->setName(satisfyIndex.at(i));

        ui->plotOffer->xAxis->setRange(1, 5);
        ui->plotOffer->yAxis->setRange(0, 1);
        ui->plotOffer->replot();
    }
}

void CMainWindow::on_pbRun_clicked()
{
    //on_pbApplyDemand_clicked();
    //on_pbApplyOferta_clicked();

    if(demandApplyed & offerApplyed){
        createTableFormula();
        createTableDemandOffer();
        createTableFuzzyIndex();
        createDemmandPlot();
        createOfferPlot();


        ui->PBSave->setEnabled(true);
    }else if(demandApplyed == false){
        QMessageBox::critical(this, "qFuzzyApp", "Please, apply values on demand table and retry operation.", QMessageBox::Ok);
    }else if(offerApplyed == false){
        QMessageBox::critical(this, "qFuzzyApp", "Please, apply values on offer table and retry operation.", QMessageBox::Ok);
    }
}

void CMainWindow::on_PBSave_clicked()
{
    QString filePath;
    QFile file;
    QString auxiliar;
    filePath = QFileDialog::getSaveFileName(this, "Save results file...",".", "qFuzzApp file(*.rfuz)");
    file.setFileName(filePath);

    QTextStream out(&file);
    out.setFieldAlignment(QTextStream::AlignLeft);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {        
        out << "/************************************************************************************************************************* \n";
        out << "* ====================================================================================================================== * \n";
        out << "*                                                        qFuzzyApp                                                       * \n";
        out << "*                                                         v. 0.3                                                         * \n";
        out << "* ====================================================================================================================== * \n";
        out << "/************************************************************************************************************************/\n";
        out << "\n";
        out << "Project name: " << ui->LETitle->text() << "\n";
        out << "\n";
        out << "--------------------------------------------------------- Formula ---------------------------------------------------------\n";
        out << "\n";
        out << "\t\t\t\t\t\t\t  ";
        for(int i = 0; i < getSatisfyIndex().size(); i++){
            out << qSetFieldWidth(20) <<getSatisfyIndex()[i];
        }
        out << qSetFieldWidth(0) << "\n";
        for(int i = 0; i < getSatisfyIndex().size(); i++){
            out << qSetFieldWidth(30) << getSpecialistNames()[i];
            for(int j = 0; j < getSpecialistNames().size(); j++){
                out << qSetFieldWidth(20) << ui->tbFormula->item(i, j)->text();
            }
            out << qSetFieldWidth(0) << "\n";
        }

        out << "\n";
        out << "------------------------------------------------------ Demand x Offer ------------------------------------------------------\n";
        out << "\n";
        out << "\t\t\t\t\t\t\t  ";
        for(int i = 0; i < getSatisfyIndex().size(); i++){
            out << qSetFieldWidth(20) << getSatisfyIndex()[i];
        }
        out << qSetFieldWidth(0) << "\n";

        for(int i = 0; i < getSatisfyIndex().size(); i++){
            out << qSetFieldWidth(30) << getSpecialistNames()[i];
            for(int j = 0; j < getSpecialistNames().size(); j++){
                out << qSetFieldWidth(20) << ui->tbDemandOffer->item(i, j)->text();
            }
            out << qSetFieldWidth(0) << "\n";
        }
        out << "\n";
        out << "------------------------------------------------------- Fuzzy Indexes -------------------------------------------------------\n";
        out << "\n";
        for(int i = 0; i < fuzzyIndexes.size();i++){
            out << qSetFieldWidth(30) << getFactorNames()[i] << fuzzyIndexes[i] << qSetFieldWidth(0) << "\n";
        }
        out << qSetFieldWidth(0);
        out << "------------------------------------ \n";

        double total = 0.0;
        for(int i = 0; i < fuzzyIndexes.size();i++){
            total = total + fuzzyIndexes[i];
        }
        out << qSetFieldWidth(30) << "Total:" << total << qSetFieldWidth(0) << "\n";
        out << qSetFieldWidth(30) << "Media:" << total/fuzzyIndexes.size() << qSetFieldWidth(0) << "\n";
        out << "\n";        
    }
    file.close();
    QMessageBox::information(this, "qFuzzyApp", "File saved successfully.", QMessageBox::Ok);
}


void CMainWindow::on_actionClose_triggered()
{
    close();
}

void CMainWindow::on_actionAbout_the_software_triggered()
{
    CAboutScreen *about = new CAboutScreen();
    about->exec();
}

void CMainWindow::testFunction()
{
    disconnect(ui->tbOfferMatrix, &QTableWidget::itemChanged, this, &CMainWindow::itemChanged);
    disconnect(ui->tbDemandMatrix, &QTableWidget::itemChanged, this, &CMainWindow::itemChanged);

    specialistNames.append("Muito_Importante");
    specialistNames.append("Importante");
    specialistNames.append("Relativamente_Importante");
    specialistNames.append("Pouco_Importante");
    specialistNames.append("Nenhuma_Importancia");

    for(int i = 0; i < specialistNames.size(); i++){
        ui->tbDemandMatrix->insertColumn(ui->tbDemandMatrix->columnCount());
        ui->tbPertinenceDemand->insertColumn(ui->tbPertinenceDemand->columnCount());
        ui->tbFormula->insertRow(ui->tbFormula->rowCount());
        ui->tbDemandOffer->insertRow(ui->tbDemandOffer->rowCount());
    }

    ui->tbDemandMatrix->setHorizontalHeaderLabels(getSpecialistNames());
    ui->tbPertinenceDemand->setHorizontalHeaderLabels(getSpecialistNames());
    ui->tbFormula->setVerticalHeaderLabels(getSpecialistNames());
    ui->tbDemandOffer->setVerticalHeaderLabels(getSpecialistNames());

    factorNames.append("Mobilidade");
    factorNames.append("Higiene");
    factorNames.append("Seguranca");
    factorNames.append("Saude");
    factorNames.append("Emprego");
    factorNames.append("Ed._Ambiental");
    factorNames.append("Infraestrutura");
    factorNames.append("Sinalizacao");

    for(int i = 0; i < factorNames.size(); i++){
        ui->tbDemandMatrix->insertRow(ui->tbDemandMatrix->rowCount());
        ui->tbPertinenceDemand->insertRow(ui->tbPertinenceDemand->rowCount());
        ui->tbCardinalityDemand->insertRow(ui->tbCardinalityDemand->rowCount());
        ui->tbOfferMatrix->insertRow(ui->tbOfferMatrix->rowCount());
        ui->tbCardinalityOffer->insertRow(ui->tbCardinalityOffer->rowCount());
        ui->tbPertinenceOffer->insertRow(ui->tbPertinenceOffer->rowCount());
        ui->tbFuzzyIndex->insertRow(ui->tbFuzzyIndex->rowCount());
    }
    ui->tbDemandMatrix->setVerticalHeaderLabels(getFactorNames());
    ui->tbPertinenceDemand->setVerticalHeaderLabels(getFactorNames());
    ui->tbCardinalityDemand->setVerticalHeaderLabels(getFactorNames());
    ui->tbOfferMatrix->setVerticalHeaderLabels(getFactorNames());
    ui->tbCardinalityOffer->setVerticalHeaderLabels(getFactorNames());
    ui->tbPertinenceOffer->setVerticalHeaderLabels(getFactorNames());
    ui->tbFuzzyIndex->setVerticalHeaderLabels(getFactorNames());

    ui->tbDemandMatrix->setItem(0,0,new QTableWidgetItem("0"));
    ui->tbDemandMatrix->item(0,0)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(0,1,new QTableWidgetItem("4"));
    ui->tbDemandMatrix->item(0,1)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(0,2,new QTableWidgetItem("1"));
    ui->tbDemandMatrix->item(0,2)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(0,3,new QTableWidgetItem("1"));
    ui->tbDemandMatrix->item(0,3)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(0,4,new QTableWidgetItem("0"));
    ui->tbDemandMatrix->item(0,4)->setTextAlignment(Qt::AlignCenter);

    ui->tbDemandMatrix->setItem(1,0,new QTableWidgetItem("1"));
    ui->tbDemandMatrix->item(1,0)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(1,1,new QTableWidgetItem("2"));
    ui->tbDemandMatrix->item(1,1)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(1,2,new QTableWidgetItem("3"));
    ui->tbDemandMatrix->item(1,2)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(1,3,new QTableWidgetItem("0"));
    ui->tbDemandMatrix->item(1,3)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(1,4,new QTableWidgetItem("0"));
    ui->tbDemandMatrix->item(1,4)->setTextAlignment(Qt::AlignCenter);

    ui->tbDemandMatrix->setItem(2,0,new QTableWidgetItem("6"));
    ui->tbDemandMatrix->item(2,0)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(2,1,new QTableWidgetItem("0"));
    ui->tbDemandMatrix->item(2,1)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(2,2,new QTableWidgetItem("0"));
    ui->tbDemandMatrix->item(2,2)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(2,3,new QTableWidgetItem("0"));
    ui->tbDemandMatrix->item(2,3)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(2,4,new QTableWidgetItem("0"));
    ui->tbDemandMatrix->item(2,4)->setTextAlignment(Qt::AlignCenter);

    ui->tbDemandMatrix->setItem(3,0,new QTableWidgetItem("5"));
    ui->tbDemandMatrix->item(3,0)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(3,1,new QTableWidgetItem("1"));
    ui->tbDemandMatrix->item(3,1)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(3,2,new QTableWidgetItem("0"));
    ui->tbDemandMatrix->item(3,2)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(3,3,new QTableWidgetItem("0"));
    ui->tbDemandMatrix->item(3,3)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(3,4,new QTableWidgetItem("0"));
    ui->tbDemandMatrix->item(3,4)->setTextAlignment(Qt::AlignCenter);

    ui->tbDemandMatrix->setItem(4,0,new QTableWidgetItem("0"));
    ui->tbDemandMatrix->item(4,0)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(4,1,new QTableWidgetItem("4"));
    ui->tbDemandMatrix->item(4,1)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(4,2,new QTableWidgetItem("1"));
    ui->tbDemandMatrix->item(4,2)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(4,3,new QTableWidgetItem("1"));
    ui->tbDemandMatrix->item(4,3)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(4,4,new QTableWidgetItem("0"));
    ui->tbDemandMatrix->item(4,4)->setTextAlignment(Qt::AlignCenter);

    ui->tbDemandMatrix->setItem(5,0,new QTableWidgetItem("1"));
    ui->tbDemandMatrix->item(5,0)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(5,1,new QTableWidgetItem("1"));
    ui->tbDemandMatrix->item(5,1)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(5,2,new QTableWidgetItem("4"));
    ui->tbDemandMatrix->item(5,2)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(5,3,new QTableWidgetItem("0"));
    ui->tbDemandMatrix->item(5,3)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(5,4,new QTableWidgetItem("0"));
    ui->tbDemandMatrix->item(5,4)->setTextAlignment(Qt::AlignCenter);

    ui->tbDemandMatrix->setItem(6,0,new QTableWidgetItem("2"));
    ui->tbDemandMatrix->item(6,0)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(6,1,new QTableWidgetItem("2"));
    ui->tbDemandMatrix->item(6,1)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(6,2,new QTableWidgetItem("1"));
    ui->tbDemandMatrix->item(6,2)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(6,3,new QTableWidgetItem("1"));
    ui->tbDemandMatrix->item(6,3)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(6,4,new QTableWidgetItem("0"));
    ui->tbDemandMatrix->item(6,4)->setTextAlignment(Qt::AlignCenter);

    ui->tbDemandMatrix->setItem(7,0,new QTableWidgetItem("1"));
    ui->tbDemandMatrix->item(7,0)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(7,1,new QTableWidgetItem("3"));
    ui->tbDemandMatrix->item(7,1)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(7,2,new QTableWidgetItem("1"));
    ui->tbDemandMatrix->item(7,2)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(7,3,new QTableWidgetItem("1"));
    ui->tbDemandMatrix->item(7,3)->setTextAlignment(Qt::AlignCenter);
    ui->tbDemandMatrix->setItem(7,4,new QTableWidgetItem("0"));
    ui->tbDemandMatrix->item(7,4)->setTextAlignment(Qt::AlignCenter);

    satisfyIndex.append("Muito_Bom");
    satisfyIndex.append("Bom");
    satisfyIndex.append("Regular");
    satisfyIndex.append("Fraco");
    satisfyIndex.append("Inexistente");

    for(int i = 0; i < satisfyIndex.size(); i++){
        ui->tbOfferMatrix->insertColumn(ui->tbOfferMatrix->columnCount());
        ui->tbPertinenceOffer->insertColumn(ui->tbPertinenceOffer->columnCount());
        ui->tbFormula->insertColumn(ui->tbFormula->columnCount());
        ui->tbDemandOffer->insertColumn(ui->tbDemandOffer->columnCount());
    }
    ui->tbOfferMatrix->setHorizontalHeaderLabels(getSatisfyIndex());
    ui->tbPertinenceOffer->setHorizontalHeaderLabels(getSatisfyIndex());
    ui->tbFormula->setHorizontalHeaderLabels(getSatisfyIndex());
    ui->tbDemandOffer->setHorizontalHeaderLabels(getSatisfyIndex());

    ui->tbOfferMatrix->setItem(0,0,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(0,0)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(0,1,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(0,1)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(0,2,new QTableWidgetItem("2"));
    ui->tbOfferMatrix->item(0,2)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(0,3,new QTableWidgetItem("4"));
    ui->tbOfferMatrix->item(0,3)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(0,4,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(0,4)->setTextAlignment(Qt::AlignCenter);

    ui->tbOfferMatrix->setItem(1,0,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(1,0)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(1,1,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(1,1)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(1,2,new QTableWidgetItem("2"));
    ui->tbOfferMatrix->item(1,2)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(1,3,new QTableWidgetItem("4"));
    ui->tbOfferMatrix->item(1,3)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(1,4,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(1,4)->setTextAlignment(Qt::AlignCenter);

    ui->tbOfferMatrix->setItem(2,0,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(2,0)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(2,1,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(2,1)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(2,2,new QTableWidgetItem("1"));
    ui->tbOfferMatrix->item(2,2)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(2,3,new QTableWidgetItem("4"));
    ui->tbOfferMatrix->item(2,3)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(2,4,new QTableWidgetItem("1"));
    ui->tbOfferMatrix->item(2,4)->setTextAlignment(Qt::AlignCenter);

    ui->tbOfferMatrix->setItem(3,0,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(3,0)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(3,1,new QTableWidgetItem("1"));
    ui->tbOfferMatrix->item(3,1)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(3,2,new QTableWidgetItem("2"));
    ui->tbOfferMatrix->item(3,2)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(3,3,new QTableWidgetItem("3"));
    ui->tbOfferMatrix->item(3,3)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(3,4,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(3,4)->setTextAlignment(Qt::AlignCenter);

    ui->tbOfferMatrix->setItem(4,0,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(4,0)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(4,1,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(4,1)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(4,2,new QTableWidgetItem("4"));
    ui->tbOfferMatrix->item(4,2)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(4,3,new QTableWidgetItem("2"));
    ui->tbOfferMatrix->item(4,3)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(4,4,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(4,4)->setTextAlignment(Qt::AlignCenter);

    ui->tbOfferMatrix->setItem(5,0,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(5,0)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(5,1,new QTableWidgetItem("1"));
    ui->tbOfferMatrix->item(5,1)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(5,2,new QTableWidgetItem("2"));
    ui->tbOfferMatrix->item(5,2)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(5,3,new QTableWidgetItem("3"));
    ui->tbOfferMatrix->item(5,3)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(5,4,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(5,4)->setTextAlignment(Qt::AlignCenter);

    ui->tbOfferMatrix->setItem(6,0,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(6,0)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(6,1,new QTableWidgetItem("1"));
    ui->tbOfferMatrix->item(6,1)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(6,2,new QTableWidgetItem("4"));
    ui->tbOfferMatrix->item(6,2)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(6,3,new QTableWidgetItem("1"));
    ui->tbOfferMatrix->item(6,3)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(6,4,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(6,4)->setTextAlignment(Qt::AlignCenter);

    ui->tbOfferMatrix->setItem(7,0,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(7,0)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(7,1,new QTableWidgetItem("1"));
    ui->tbOfferMatrix->item(7,1)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(7,2,new QTableWidgetItem("3"));
    ui->tbOfferMatrix->item(7,2)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(7,3,new QTableWidgetItem("2"));
    ui->tbOfferMatrix->item(7,3)->setTextAlignment(Qt::AlignCenter);
    ui->tbOfferMatrix->setItem(7,4,new QTableWidgetItem("0"));
    ui->tbOfferMatrix->item(7,4)->setTextAlignment(Qt::AlignCenter);

    connect(ui->tbOfferMatrix, &QTableWidget::itemChanged, this, &CMainWindow::itemChanged);
    connect(ui->tbDemandMatrix, &QTableWidget::itemChanged, this, &CMainWindow::itemChanged);
}

void CMainWindow::on_pbAddDemmandOffer_clicked()
{


}

void CMainWindow::on_pbRemoveDemandOffer_clicked()
{


}

//void CMainWindow::on_addFactor_clicked()
//{
//    setItemToAdd(new CRowNames());
//    connect(getItemToAdd(), &CRowNames::rowName, [this](QString rowName){
//        setCanceledAction(false);
//        rowName.replace(" ", "_");
//        getFactorNames().append(rowName);
//    });

//    connect(getItemToAdd(), &CRowNames::canceledAction, [this](){
//       setCanceledAction(true);
//    });
//    getItemToAdd()->exec();

//    if(!getCanceledAction()){
//        ui->tbDemandMatrix->insertRow(ui->tbDemandMatrix->rowCount());
//        ui->tbDemandMatrix->setVerticalHeaderLabels(getFactorNames());
//        ui->tbPertinenceDemand->insertRow(ui->tbPertinenceDemand->rowCount());
//        ui->tbPertinenceDemand->setVerticalHeaderLabels(getFactorNames());
//        ui->tbCardinalityDemand->insertRow(ui->tbCardinalityDemand->rowCount());
//        ui->tbCardinalityDemand->setVerticalHeaderLabels(getFactorNames());
//        ui->tbOfferMatrix->insertRow(ui->tbOfferMatrix->rowCount());
//        ui->tbOfferMatrix->setVerticalHeaderLabels(getFactorNames());
//        ui->tbCardinalityOffer->insertRow(ui->tbCardinalityOffer->rowCount());
//        ui->tbCardinalityOffer->setVerticalHeaderLabels(getFactorNames());
//        ui->tbPertinenceOffer->insertRow(ui->tbPertinenceOffer->rowCount());
//        ui->tbPertinenceOffer->setVerticalHeaderLabels(getFactorNames());
//        ui->tbFuzzyIndex->insertRow(ui->tbFuzzyIndex->rowCount());
//        ui->tbFuzzyIndex->setVerticalHeaderLabels(getFactorNames());
//    }
//}

//void CMainWindow::on_removeFactor_clicked()
//{
//    if(ui->tbDemandMatrix->rowCount() != 0){
//        ui->tbDemandMatrix->removeRow(ui->tbDemandMatrix->rowCount()-1);
//        ui->tbPertinenceDemand->removeRow(ui->tbPertinenceDemand->rowCount()-1);
//        ui->tbCardinalityDemand->removeRow(ui->tbCardinalityDemand->rowCount()-1);
//        ui->tbOfferMatrix->removeRow(ui->tbCardinalityDemand->rowCount()-1);
//        ui->tbCardinalityOffer->removeRow(ui->tbCardinalityDemand->rowCount()-1);
//        ui->tbPertinenceOffer->removeRow(ui->tbCardinalityDemand->rowCount()-1);
//        ui->tbFuzzyIndex->removeRow(ui->tbFuzzyIndex->rowCount()-1);
//        getFactorNames().removeLast();
//    }else{
//        QMessageBox msgWarning;
//        msgWarning.setText("You don't have more factors to remove.");
//        msgWarning.setIcon(QMessageBox::Warning);
//        msgWarning.setWindowTitle("Caution");
//        msgWarning.exec();
//    }
//}
