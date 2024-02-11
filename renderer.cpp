#include "renderer.h"

#include "game.h"
#include "gameitem.h"
#include <QGraphicsScene>

QPen Renderer::s_pen(Qt::gray);
GameItem* Renderer::s_item = nullptr;

void Renderer::render(QGraphicsScene& scene, const Game& game, float cellSize)
{
	if (s_item == nullptr || s_item->cellSize() != cellSize)
	{
        scene.clear();
		s_item = new GameItem(game, cellSize);
		scene.addItem(s_item);
	}
	else
		scene.update();
}
