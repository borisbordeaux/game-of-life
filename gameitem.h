#ifndef GAMEITEM_H
#define GAMEITEM_H

#include <QGraphicsItem>
#include <QPen>

class Game;
class GameItem : public QGraphicsItem
{
public:
	GameItem(Game const& game, qreal cellSize);

    void setCellSize(qreal cellSize);

	QRectF boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

	inline qreal cellSize() const { return m_cellSize; };

private:
	const Game& m_game;
	qreal m_cellSize;
	QPen m_pen;
};

#endif // GAMEITEM_H
