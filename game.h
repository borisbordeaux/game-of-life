#ifndef GAME_H
#define GAME_H

#include <QVector>

class Game
{
public:
	Game();
	void init(qsizetype w, qsizetype h);
	void setRandom();
	void update();
	QVector<QVector<bool>> const& currentGrid() const;

private:
	int countNeighbors(qsizetype w, qsizetype h) const;

private:
	QVector<QVector<bool>> m_grid1;
	QVector<QVector<bool>> m_grid2;
    bool m_currentGrid1 = true;
};

#endif // GAME_H
