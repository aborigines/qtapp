#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

static QSqlDatabase connectDB(QString fileName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fileName);

    if( ! db.open() )
    {
        MainWindow::QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Could not connect"));
    }
    return db;
}

void MainWindow::on_actionOpen_triggered()
{
    // open file
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(),
                QString());

    if (fileName.isEmpty()) {
        QFile file(fileName);

        if (!fileName.isEmpty() && !file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
    }
    else
    {
        // connnect db
        QSqlDatabase db = connectDB(fileName);

        QSqlTableModel *model = new QSqlTableModel(this,db);
        model->setTable("address");
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        model->select();
        model->setHeaderData(0, Qt::Horizontal, tr("Row1"));
        model->setHeaderData(1, Qt::Horizontal, tr("Row2"));

        ui->tableView->setModel(model);
        ui->tableView->show();

        db.close();
    }
}

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::quit ();
//    this->close();
}
