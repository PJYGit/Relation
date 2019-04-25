#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "relation.h"

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

void MainWindow::on_EquivalenceClosure_clicked()
{
    ui->Outputs->setPlainText("");
    Relation r;
    Relation rout;
    r=Relation("F:/Qt codes/Relation/test1.txt");
    rout = r.EquivalenceClosure();

    int dimension = rout.GetDimension();
    int *temp = new int[dimension * dimension];
    rout.GetMatrix(temp);

    int row, column;
    QString text = QString::number(dimension);

    ui->Outputs->setPlainText(text + "\n");

    for (row = 0; row < dimension; row++)
    {
        for (column = 0; column < dimension; column++)
        {
            QString last = ui->Outputs->toPlainText();
            QString te = QString::number(rout.GetatPosition(row, column));
            ui->Outputs->setPlainText(last + te + " ");
        }
        ui->Outputs->append("");
    }
}

void MainWindow::on_EquivalenceClasses_clicked()
{
    ui->Outputs->setPlainText("");
    Relation r;
    r=Relation("F:/Qt codes/Relation/test1.txt");

    int *EClass = new int[r.GetDimension()];
    r.EquivalenceClosure().EquiClasses(EClass);

    ui->Outputs->setPlainText("The Equivalent Classes:");
    int i;
    /*
    for (i=1; i <= r.GetDimension(); i++)
    {
        QString last = ui->Outputs->toPlainText();
        QString te = QString::number(i);
        ui->Outputs->setPlainText(last + te + "  ");
    }
    ui->Outputs->append("");
    for (i=0; i <r.GetDimension(); i++)
    {
        QString st = ui->Outputs->toPlainText();
        QString mp = QString::number(EClass[i]);
        ui->Outputs->setPlainText(st + mp + "  ");
    }
    ui->Outputs->append("");
    */
    QString message1;
    int k = 0;
    for (i = 0; i < r.GetDimension(); i++)
    {
        for (int l = 0; l < r.GetDimension(); l++) {
            if (EClass[l] == i) {
                message1 = message1 +"Mod"+ QString::number(l+1) + " ";
                k++;
            }
        }
        if (k == r.GetDimension()) {
            message1 = message1 ;
        }
        else {
            message1 = message1 + "|| ";
        }

    }
    ui->Outputs->append(message1);

    int n = 0;
    for (i = 0; i < r.GetDimension(); i++)
    {
        n = 0;
        for (int l = 0; l < r.GetDimension(); l++) {
            if (EClass[l] == i) {
                n++;
            }
        }
        if (n > 5) {
            ui->Outputs->append("No >= 5");
            break;
        }
    }
}

void MainWindow::on_Topology_clicked()
{
    ui->Outputs->setPlainText("");
    Relation r("F:/Qt codes/Relation/test5.txt");
    if (r.IsCycle()){
        ui->Outputs->setPlainText("There is a cycle! No Topology Order!");
    }
    else {
        int *topo = new int[r.GetDimension()];
        r.Topology(topo);
        ui->Outputs->setPlainText("The topology order is:\n");
        for (int i = 0; i < r.GetDimension(); i++) {
            QString last = ui->Outputs->toPlainText();
            QString temp = QString::number(topo[i] + 1);
            ui->Outputs->setPlainText(last + temp + "  ");
        }
    }

}

void MainWindow::on_Hasse_clicked()
{
    ui->Outputs->setPlainText("");
    Relation r("F:/Qt codes/Relation/test6.txt");
    Relation rpartial;
    Relation rout;
    rpartial = r.PartialorderClosure();

    if (rpartial.IsPartialOrder()){
        ui->Outputs->setPlainText("The matrix of the Hasse is:\n");

        rout = rpartial.Hasse();

        int dimension = rout.GetDimension();
        int *temp = new int[dimension * dimension];
        rout.GetMatrix(temp);

        int row, column;

        for (row = 0; row < dimension; row++)
        {
            for (column = 0; column < dimension; column++)
            {
                QString last = ui->Outputs->toPlainText();
                QString te = QString::number(rout.GetatPosition(row, column));
                ui->Outputs->setPlainText(last + te + " ");
            }
            ui->Outputs->append("");
        }
    }
    else {
        ui->Outputs->setPlainText("NO HASSE!");
    }
}
















