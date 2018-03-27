#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <curl/curl.h>
#include <iostream>
#include <ctime>
#include <string.h>
#include "mailnotification.h"
//sudo apt install curl libcurl4-openssl-dev

using namespace std;

MailNotification mail;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mail.setUser(string("something@gmail.com"), string("lgstdkoytvdhkgnb")); //username, password (can be an app password)
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_send_clicked()
{
    from = ui->lineEdit_from->text().toStdString();
    to = ui->lineEdit_to->text().toStdString();
    subject = ui->lineEdit_subject->text().toStdString();
    message = ui->textEdit_message->toPlainText().toStdString();

    mail.changeFrom(from);
    mail.changeTo(to);
    mail.changeSubject(subject);
    mail.changeMessage(message);

    mail.sendEmail();

}
