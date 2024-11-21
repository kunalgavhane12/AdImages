#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel *displayLabel;
    QTimer *imageTimer;
    QStringList imagePaths;
    int currentImageIndex;

    void loadImageFiles();
    void showNextImage();

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // MAINWINDOW_H
