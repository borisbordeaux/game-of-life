#include "game.h"

#include <QDebug>
#include <qrandom.h>
#include <thread>

Game::Game()
{

}

void Game::init(int w, int h)
{
    m_img1 = QImage(w, h, QImage::Format_RGB888);
    m_img1.fill(255);
    m_img2 = QImage(w, h, QImage::Format_RGB888);
    m_currentGrid1 = true;
}

void Game::setRandom()
{
    for (int i = 0; i < m_img1.width(); i++)
	{
        for (int j = 0; j < m_img1.height(); j++)
            m_img1.setPixelColor(i, j, QRandomGenerator::global()->generateDouble() > 0.5 ? ALIVE : DEAD);
	}

    m_currentGrid1 = true;
}

void Game::update()
{
	// read current grid, write next grid
    auto &nextGrid = m_currentGrid1 ? m_img2 : m_img1;
    auto &currentGrid = m_currentGrid1 ? m_img1 : m_img2;

#if 1
	// use multithreading for this computation since it can take time
	// and can be done independently for each pixel
	unsigned int nb_thread = std::thread::hardware_concurrency();

    std::function<void(int)> f = [&](int idxThread)
	{
		// each thread handles the grid cut in vertical parts
		// hence the width is determined by the number of thread
        unsigned int width = m_img1.width() / nb_thread;

		// the last thread can handle a larger width than the others
		// but always strictly shorter than twice the width handled by one thread
        int realWidth = idxThread < nb_thread - 1 ? width : m_img1.width() - idxThread * width;

		// the beginning depends on the thread ID
        int begin = width * idxThread;
		int end = begin + realWidth;

		// update each cell
		for (int i = begin; i < end; i++)   // columns
		{
            for (int j = 0; j < currentGrid.height(); j++)   // lines
			{
				int n = countNeighbors(i, j);
                bool s = currentGrid.pixelColor(i, j) == ALIVE;

                nextGrid.setPixelColor(i, j, ((n == 3) || (s && n == 2)) ? ALIVE : DEAD);
			}
		}
	};

	std::vector<std::thread> ThreadVector;

	// for each thread, emplace in the vector their job
    for (int i = 0; i < nb_thread; i++)
		ThreadVector.emplace_back(f, i);

	// then wait them to finish
    for (auto &t : ThreadVector)
		t.join();

#else

    // update each cell
    for (int i = 0; i < currentGrid.width(); i++)   // columns
    {
        for (int j = 0; j < currentGrid.height(); j++)   // lines
        {
            int n = countNeighbors(i, j);
            bool s = currentGrid.pixelColor(i, j) == ALIVE;

            nextGrid.setPixelColor(i, j, ((n == 3) || (s && n == 2)) ? ALIVE : DEAD);
        }
    }

#endif
	m_currentGrid1 = !m_currentGrid1;
}

QImage const &Game::currentGrid() const
{
    return m_currentGrid1 ? m_img1 : m_img2;
}

int Game::countNeighbors(int w, int h) const
{
	int res = 0;
    auto &currentGrid = m_currentGrid1 ? m_img1 : m_img2;

	for (qsizetype i = 0; i < 3; i++)
	{
		for (qsizetype j = 0; j < 3; j++)
		{
			if (i != 1 || j != 1)
			{
                qsizetype actualW = w + i > 0 ? (w - 1 + i < currentGrid.width() ? w - 1 + i : 0) : currentGrid.width() - 1;
                qsizetype actualH = h + j > 0 ? (h - 1 + j < currentGrid.height() ? h - 1 + j : 0) : currentGrid.height() - 1;

                res += currentGrid.pixelColor(actualW, actualH) == ALIVE ? 1 : 0;
			}
		}
	}

	return res;
}
