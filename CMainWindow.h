#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include <CItemNames.h>
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class CMainWindow; }
QT_END_NAMESPACE

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = nullptr);
    ~CMainWindow();

    inline CItemNames *getItemToAdd() const { return itemToAdd; }
    inline void setItemToAdd(CItemNames *value){ itemToAdd = value; }

    inline QStringList &getSpecialistNames(){ return specialistNames; }
    inline void setSpecialistNames(const QStringList &value){ specialistNames = value; }

    inline QStringList &getFactorNames(){ return factorNames; }
    inline void setFactorNames(const QStringList &value){ factorNames = value; }

    inline QStringList &getSatisfyIndex(){ return satisfyIndex; }
    inline void setSatisfyIndex(const QStringList &value){ satisfyIndex = value; }

    inline bool &getCanceledAction(){ return canceledAction; }
    inline void setCanceledAction(bool value){ canceledAction = value; }

    inline int getRowsDemand() const{ return rowsDemand; }
    inline void setRowsDemand(int value){ rowsDemand = value; }

    inline int getColumnsDemand() const{ return columnsDemand; }
    inline void setColumnsDemand(int value){ columnsDemand = value; }

    inline int getColumnsOffer() const { return columnsOffer; }
    inline void setColumnsOffer(int value) {columnsOffer = value;}

    inline bool getDemandApplyed() const { return demandApplyed;}
    inline void setDemandApplyed(bool value) {demandApplyed = value;}

    inline bool getOfferApplyed() const { return offerApplyed;}
    inline void setOfferApplyed(bool value) {offerApplyed = value;}

private slots:

    void on_addFactor_clicked();

    void on_removeFactor_clicked();

    void on_pbApplyDemand_clicked();

    void on_addLinguistic_clicked();

    void on_removeLinguistic_clicked();

    void on_tbAddOferta_clicked();

    void on_tbRemoveOferta_clicked();

    void on_pbApplyOferta_clicked();

    void itemChanged(QTableWidgetItem *item);

    void on_actionSave_triggered();

    void on_actionClose_triggered();

    void on_actionOpen_File_triggered();

    void on_pbRun_clicked();

    void on_actionAbout_the_software_triggered();

    void on_PBSave_clicked();

    void on_pbAddDemmandOffer_clicked();

    void on_pbRemoveDemandOffer_clicked();

private:
    Ui::CMainWindow *ui;

    CItemNames *itemToAdd;    
    QStringList specialistNames;
    QStringList factorNames;
    QStringList satisfyIndex;

    bool canceledAction;
    int rowsDemand;
    int columnsDemand;
    int columnsOffer;

    QVector<QVector<double> > demandMatrix;
    QVector<QVector<double> > demandPertinence;
    QVector<QVector<double> > demandCardinality;

    QVector<QVector<double> > offerMatrix;
    QVector<QVector<double> > offerPertinence;
    QVector<QVector<double> > offerCardinality;

    QVector<QVector<double> > demandOffer;
    QVector<double> fuzzyIndexes;


    void testFunction();


    void configureTables();

    void fillEmptyValuesOnDemand();
    void createTableDemand();
    void createTablePertinenceDemand();
    void createTableCardinalityDemand();
    bool demandApplyed;

    void createTableOffer();
    void fillEmptyValuesOnOffer();
    void createTablePertinenceOffer();
    void createTableCardinalityOffer();
    bool offerApplyed;

    void createTableFormula();
    void createTableDemandOffer();
    void createTableFuzzyIndex();

    void createDemmandPlot();
    void createOfferPlot();



};
#endif // CMAINWINDOW_H
