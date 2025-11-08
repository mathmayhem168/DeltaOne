#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJSEngine>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Function to append text to display
    auto append = [=](QString s) {
        ui->display->setText(ui->display->text() + s);
    };

    // Digits
    connect(ui->btn0, &QPushButton::clicked, this, [=](){ append("0"); });
    connect(ui->btn1, &QPushButton::clicked, this, [=](){ append("1"); });
    connect(ui->btn2, &QPushButton::clicked, this, [=](){ append("2"); });
    connect(ui->btn3, &QPushButton::clicked, this, [=](){ append("3"); });
    connect(ui->btn4, &QPushButton::clicked, this, [=](){ append("4"); });
    connect(ui->btn5, &QPushButton::clicked, this, [=](){ append("5"); });
    connect(ui->btn6, &QPushButton::clicked, this, [=](){ append("6"); });
    connect(ui->btn7, &QPushButton::clicked, this, [=](){ append("7"); });
    connect(ui->btn8, &QPushButton::clicked, this, [=](){ append("8"); });
    connect(ui->btn9, &QPushButton::clicked, this, [=](){ append("9"); });
    connect(ui->btnDot, &QPushButton::clicked, this, [=](){ append("."); });

    // Operators
    connect(ui->btnAdd, &QPushButton::clicked, this, [=](){ append("+"); });
    connect(ui->btnSub, &QPushButton::clicked, this, [=](){ append("-"); });
    connect(ui->btnMul, &QPushButton::clicked, this, [=](){ append("*"); });
    connect(ui->btnDiv, &QPushButton::clicked, this, [=](){ append("/"); });

    // Clear
    connect(ui->btnClear, &QPushButton::clicked, this, [=](){
        ui->display->clear();
    });

    // +/- Toggle
    connect(ui->btnSign, &QPushButton::clicked, this, [=](){
        QString txt = ui->display->text();
        if (txt.startsWith("-"))
            ui->display->setText(txt.mid(1));
        else
            ui->display->setText("-" + txt);
    });

    // Evaluate Expression
    connect(ui->btnEq, &QPushButton::clicked, this, [=](){
        QString expr = ui->display->text();
        QJSEngine engine;
        double result = engine.evaluate(expr).toNumber();
        ui->display->setText(QString::number(result));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
