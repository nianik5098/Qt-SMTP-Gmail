#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    string from;
    string to;
    string subject;
    string message;

private slots:
    void on_pushButton_send_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
