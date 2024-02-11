#include "gameitem.h"
#include "game.h"

#include <QPainter>

GameItem::GameItem(const Game& game, qreal cellSize): m_game(game), m_cellSize(cellSize), m_pen(Qt::gray)
{

}

QRectF GameItem::boundingRect() const
{
	return QRectF(-10, -10, m_game.currentGrid().size() * m_cellSize + 12, m_game.currentGrid()[0].size() * m_cellSize + 12);
}

void GameItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	auto& grid = m_game.currentGrid();
	painter->setPen(m_pen);

	for (qsizetype i = 0; i < grid.size() ; i++)
	{
		//add all items to scene
		for (qsizetype j = 0; j < grid[i].size(); j++)
		{
			QRectF rect(static_cast<qreal>(i) * m_cellSize - m_cellSize / 2.0,
			            static_cast<qreal>(j) * m_cellSize - m_cellSize / 2.0,
			            m_cellSize,
			            m_cellSize);
			painter->fillRect(rect, grid[i][j] ? Qt::black : Qt::white);
			painter->drawRect(rect);
		}
	}
}
