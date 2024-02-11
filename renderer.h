#ifndef RENDERER_H
#define RENDERER_H

#include <QPen>
#include <QVector>

class GameItem;
class QGraphicsScene;
class Game;

class Renderer
{
public:
	static void render(QGraphicsScene& scene, Game const& game, float cellSize);

private:
    static QPen s_pen;
    static GameItem* s_item;
};

#endif // RENDERER_H
