#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

public slots:
	void update();
    
public slots:
	void cellSizeChanged(int value);
	void gridWidthChanged(int value);
	void gridHeightChanged(int value);
    void simulationSpeedChanged(int value);
    void reset();

private:
	void initGrid();

private:
	Ui::MainWindow* ui;
	QGraphicsScene m_scene;
	Game m_game;
	QTimer m_timer;
	float m_cellSize = 0;
    unsigned long m_step = 0;
    std::chrono::steady_clock::time_point m_lastTime;
};
#endif // MAINWINDOW_H
