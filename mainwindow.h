#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_EquivalenceClosure_clicked();

    void on_EquivalenceClasses_clicked();

    void on_Topology_clicked();

    void on_Hasse_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
