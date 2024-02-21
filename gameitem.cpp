#include "gameitem.h"
#include "game.h"

#include <QPainter>

GameItem::GameItem(const Game& game, qreal cellSize): m_game(game), m_cellSize(cellSize), m_pen(Qt::gray)
{

}

void GameItem::setCellSize(qreal cellSize)
{
	m_cellSize = cellSize;
}

QRectF GameItem::boundingRect() const
{
	return QRectF(-10, -10, m_game.currentGrid().width() * m_cellSize + 12, m_game.currentGrid().height() * m_cellSize + 12);
}

void GameItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	auto& grid = m_game.currentGrid();
	QRectF rect(0, 0, m_cellSize * m_game.currentGrid().width(), m_cellSize * m_game.currentGrid().height());
	painter->drawImage(rect, m_game.currentGrid());

	//draw grid
	for (int i = 0; i <= m_game.currentGrid().width(); i++)
		painter->drawLine(i * m_cellSize, 0, i * m_cellSize, m_cellSize * m_game.currentGrid().height());

	for (int i = 0; i <= m_game.currentGrid().height(); i++)
		painter->drawLine(0, i * m_cellSize, m_cellSize * m_game.currentGrid().width(), i * m_cellSize);
}
