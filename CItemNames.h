#ifndef CITEMNAMES_H
#define CITEMNAMES_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class CItemNames;
}

class CItemNames : public QDialog
{
    Q_OBJECT

public:
    explicit CItemNames(QWidget *parent = nullptr);
    ~CItemNames();
    QString getLineEditValue();
    void clearLineEdit();




private:
    Ui::CItemNames *ui;

signals:
    void columnName(QString name);
    void rowName(QString name);
    void canceledAction();

public slots:
    virtual void on_pbAdd_clicked();
    void on_pbCancel_clicked();
};

#endif // CITEMNAMES_H
