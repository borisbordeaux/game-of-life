#include "game.h"

#include <QDebug>
#include <qrandom.h>
#include <thread>

Game::Game()
{

}

void Game::init(qsizetype w, qsizetype h)
{
	m_grid1.resize(w);
	m_grid2.resize(w);

	for (qsizetype i = 0; i < w ; i++)
	{
		//init all cells to false
		m_grid1[i].resize(h, false);
		m_grid2[i].resize(h, false);
	}

	m_currentGrid1 = true;
}

void Game::setRandom()
{
	m_currentGrid1 = true;

	for (qsizetype i = 0; i < m_grid1.size(); i++)
	{
		for (qsizetype j = 0; j < m_grid1[i].size(); j++)
			m_grid1[i][j] = QRandomGenerator::global()->generateDouble() > 0.5;
	}
}

void Game::update()
{
	// read current grid, write next grid
	auto& nextGrid = m_currentGrid1 ? m_grid2 : m_grid1;
	auto& currentGrid = m_currentGrid1 ? m_grid1 : m_grid2;

#if 1
	// use multithreading for this computation since it can take time
	// and can be done independently for each pixel
	unsigned int nb_thread = std::thread::hardware_concurrency();

	std::function<void(unsigned int)> f = [&](unsigned int idxThread)
	{
		// each thread handles the grid cut in vertical parts
		// hence the width is determined by the number of thread
		unsigned int width = currentGrid.size() / nb_thread;

		// the last thread can handle a larger width than the others
		// but always strictly shorter than twice the width handled by one thread
		unsigned int realWidth = idxThread < nb_thread - 1 ? width : currentGrid.size() - idxThread * width;

		// the beginning depends on the thread ID
		int begin = width * static_cast<int>(idxThread);
		int end = begin + realWidth;

		// update each cell
		for (int i = begin; i < end; i++)   // columns
		{
			for (int j = 0; j < currentGrid[0].size(); j++)   // lines
			{
				int n = countNeighbors(i, j);
				bool s = currentGrid[i][j];

				nextGrid[i][j] = (n == 3) || (s && n == 2);
			}
		}
	};

	std::vector<std::thread> ThreadVector;

	// for each thread, emplace in the vector their job
	for (unsigned int i = 0; i < nb_thread; i++)
		ThreadVector.emplace_back(f, i);

	// then wait them to finish
	for (auto& t : ThreadVector)
		t.join();

#else

	// update each cell
	for (qsizetype i = 0; i < m_grid1.size(); i++)
	{
		for (qsizetype j = 0; j < m_grid1[i].size(); j++)
		{
			int n = countNeighbors(i, j);
			bool s = currentGrid[i][j];

			nextGrid[i][j] = (n == 3) || (s && n == 2);
		}
	}

#endif
	m_currentGrid1 = !m_currentGrid1;
}

QVector<QVector<bool>> const& Game::currentGrid() const
{
	return m_currentGrid1 ? m_grid1 : m_grid2;
}

int Game::countNeighbors(qsizetype w, qsizetype h) const
{
	int res = 0;
	auto& currentGrid = m_currentGrid1 ? m_grid1 : m_grid2;

	for (qsizetype i = 0; i < 3; i++)
	{
		for (qsizetype j = 0; j < 3; j++)
		{
			if (i != 1 || j != 1)
			{
				qsizetype actualW = w + i > 0 ? (w - 1 + i < currentGrid.size() ? w - 1 + i : 0) : currentGrid.size() - 1;
				qsizetype actualH = h + j > 0 ? (h - 1 + j < currentGrid[0].size() ? h - 1 + j : 0) : currentGrid[0].size() - 1;

				res += currentGrid[actualW][actualH] ? 1 : 0;
			}
		}
	}

	return res;
}
