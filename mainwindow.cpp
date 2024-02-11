#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "renderer.h"

#include <QGraphicsLineItem>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	m_cellSize = static_cast<qreal>(ui->horizontalSlider->value());
	this->ui->graphicsView->setScene(&m_scene);
	m_scene.setBackgroundBrush(Qt::white);
	connect(&m_timer, &QTimer::timeout, this, &MainWindow::update);
	this->initGrid();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::update()
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	m_game.update();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    m_step++;
	QString message = "Step : " + QString::number(m_step) + "                ";
	message += "Game update time : " + QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) + "ms";
	message += ", " + QString::number(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) + "µs";
	message += ", " + QString::number(std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count()) + "ns";
    message += "                Frame time : " + QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(begin - m_lastTime).count()) + "ms";
    message += " (" + QString::number(1000000000/std::chrono::duration_cast<std::chrono::nanoseconds>(begin - m_lastTime).count()) + " fps)";
	this->ui->statusBar->showMessage(message);
	Renderer::render(m_scene, m_game, m_cellSize);
    m_lastTime = std::chrono::steady_clock::now();
}

void MainWindow::cellSizeChanged(int value)
{
	m_cellSize = static_cast<qreal>(value);
}

void MainWindow::gridWidthChanged(int)
{
	this->initGrid();
}

void MainWindow::gridHeightChanged(int)
{
	this->initGrid();
}

void MainWindow::simulationSpeedChanged(int value)
{
	m_timer.setInterval(value);
}

void MainWindow::reset()
{
	m_timer.stop();
	m_step = 0;
	m_game.setRandom();
	Renderer::render(m_scene, m_game, m_cellSize);
	m_timer.start(this->ui->horizontalSlider_speed->value());
}

void MainWindow::initGrid()
{
	m_timer.stop();
    m_step = 0;
	m_game.init(this->ui->spinBox_w->value(), this->ui->spinBox_h->value());
	m_game.setRandom();
	Renderer::render(m_scene, m_game, m_cellSize);
	m_timer.start(this->ui->horizontalSlider_speed->value());
}

