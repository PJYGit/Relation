#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "relation.h"
#include <QStack>
#include <QPushButton>
#include <QFileDialog>

int count_level = 1;
int hori_count = 1;
QStack<int> stack;
int added[20];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Pix = QPixmap(width(), height());
    Pix.fill(Qt::gray);

    for (int i=0;i<20;i++) added[i] = -1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_EquivalenceClosure_clicked()
{
    ui->Outputs->setPlainText("");

    QString filename = QFileDialog::getOpenFileName(this, "Matrix choosing", QDir::currentPath());

    Relation r;
    Relation rout;
    r=Relation(filename.toLatin1().data());
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
    QString filename = QFileDialog::getOpenFileName(this, "Matrix choosing", QDir::currentPath());

    Relation r;
    Relation rr;
    r=Relation(filename.toLatin1().data());

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

    rr = r.EquivalenceClosure();
    if (rr.GetatPosition(8,12) == 1){
        ui->Outputs->append("No 9&13 in one module!");
    }

}

void MainWindow::on_Topology_clicked()
{
    ui->Outputs->setPlainText("");
    QString filename = QFileDialog::getOpenFileName(this, "Matrix choosing", QDir::currentPath());

    Relation r(filename.toLatin1().data());
    if (r.IsCycle()){
        ui->Outputs->setPlainText("There is a cycle! No Topology Order!");
    }
    else {
        int *topo = new int[r.GetDimension()];
        r.Topology(topo);
        ui->Outputs->setPlainText("One topology order is:\n");
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
    QString filename = QFileDialog::getOpenFileName(this, "Matrix choosing", QDir::currentPath());
    Relation r(filename.toLatin1().data());
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
        GenerateButton(temp,dimension);

        for (int t=0;t<dimension;t++){
            AddNum(t);
        }

        int *emm = new int[dimension * dimension];
        rout.GetMatrix(emm);
        for (int m=0;m<dimension;m++){
            for (int n=0;n<dimension;n++){
                if (emm[m*dimension+n] == 1){
                    DrawLines(m, n, true);
                    this->repaint();
                }
            }
        }

    }
    else {
        ui->Outputs->setPlainText("NO HASSE!");
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter Painter(this);
    Painter.drawPixmap(0, 0, width(), height(), Pix);
}

void MainWindow::my_paintLine(int x1, int y1, int x2, int y2, bool color){

    QPainter Painter(&Pix);
    if(color){
        Painter.setPen(QPen(Qt::black, 2));
    }
    else {
        Painter.setPen(QPen(Qt::red, 2));
    }

    Painter.drawLine(x1, y1, x2, y2);
}

void MainWindow::GenerateButton(int *hasse, int dimension){
    bool check;
    int number;

    while (count_level<=dimension) {  
        for (int column = 0; column < dimension; column++)
        {
            check = true;
            for (int row = 0; row < dimension; row++)
            {
                if (hasse[row*dimension+column] == 1) {
                    check = false;
                }
            }

            if (check){
                check = false;
                if (added[column] != 1)
                    stack.push(column);
                added[column] = 1;
            }
        }


        number = stack.size();
        while (!stack.isEmpty()){
            int temp = stack.pop();
            for (int p=0;p<dimension;p++){
                hasse[temp*dimension+p] = 0;
            }

            QPushButton *but = new QPushButton(ui->centralWidget);
            but->setFixedSize(12,12);
            but->setObjectName(QString::number(temp));
            //but->setText(QString::number(temp+1));
            but->setStyleSheet("background-color:rgb(0,255,0);border:1px solid green;border-radius:5px");
            but->setGeometry(GetPos(number,hori_count, count_level));
            but->show();
            hori_count++;
        }
        hori_count = 1;
        count_level++;
    }

}

QRect MainWindow::GetPos(int f, int hor, int s){
    int part = 520 / (f+1);

    int x = part * hor - 15;

    int y = 504 - 60 * (s-1);
    return QRect(x, y, 30, 30);
}

void MainWindow::DrawLines(int fnode, int snode, bool mode){
    QString temp1 = QString::number(fnode);
    QString temp2 = QString::number(snode);
    QPushButton *tempBut1 = ui->centralWidget->findChild<QPushButton*>(temp1);
    QPushButton *tempBut2 = ui->centralWidget->findChild<QPushButton*>(temp2);
    int a = tempBut1->x()+3;
    int b = tempBut1->y()+17;
    int c = tempBut2->x()+3;
    int d = tempBut2->y()+17;

    my_paintLine(a,b,c,d,mode);
}

void MainWindow::AddNum(int node){
    QString temp = QString::number(node);
    QString num_name = QString::number(node+1);
    QPushButton *tempBut = ui->centralWidget->findChild<QPushButton*>(temp);
    int a = tempBut->x()-10;
    int b = tempBut->y();
    QLabel *node_num = new QLabel(ui->centralWidget);
    node_num->setText(num_name);
    node_num->setObjectName(temp);
    node_num->setGeometry(a, b, 15, 15);
    node_num->show();
}










