#ifndef GAME_H
#define GAME_H

#include <QImage>

class Game
{
public:
	Game();
    void init(int w, int h);
	void setRandom();
	void update();
    const QImage &currentGrid() const;

private:
    int countNeighbors(int w, int h) const;

private:
    QImage m_img1;
    QImage m_img2;
    bool m_currentGrid1 = true;
    QColor ALIVE = QColor(Qt::black);
    QColor DEAD = QColor(Qt::white);
};

#endif // GAME_H
