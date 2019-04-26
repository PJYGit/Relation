#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QPixmap>
#include <QPainter>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);
    void my_paintLine(int x1, int y1, int x2, int y2, bool color);

    void GenerateButton(int *hasse, int dimension);
    QRect GetPos(int f, int hor, int s);

    void DrawLines(int fnode, int snode, bool mode);
    void AddNum(int node);

private slots:
    void on_EquivalenceClosure_clicked();

    void on_EquivalenceClasses_clicked();

    void on_Topology_clicked();

    void on_Hasse_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap Pix;
};

#endif // MAINWINDOW_H
