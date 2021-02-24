#ifndef CABOUTSCREEN_H
#define CABOUTSCREEN_H

#include <QDialog>

namespace Ui {
class CAboutScreen;
}

class CAboutScreen : public QDialog
{
    Q_OBJECT

public:
    explicit CAboutScreen(QWidget *parent = nullptr);
    ~CAboutScreen();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CAboutScreen *ui;
};

#endif // CABOUTSCREEN_H
