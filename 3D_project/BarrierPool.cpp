#include "BarrierPool.h"
#include <iostream>
#include "Game.h"
BarrierPool::BarrierPool(Game* _game, int size):
	m_game(_game)
{
	for (int i = 0; i < size; i++)
	{
		// create a Barrier with default position, rotation, and scale
		Barrier b(m_game, glm::vec3(0, 0, 0), glm::vec3(0, 90, 0), glm::vec3(0.08));
		b.initSize();
		b.setActive(false);
		m_pool.push_back(b);
	}
}


Barrier* BarrierPool::getObject()
{
	std::cout << "\npool size" << m_pool.size() << std::endl;
	for (int i = 0; i < m_pool.size(); i++)
	{
		// check if the barrier is inactive
		if (!m_pool[i].getState())
		{
			return &(m_pool[i]); // return pointer to the available barrier
		}
	}
	return nullptr;
}

void BarrierPool::reset()
{
	for (int i = 0; i < m_pool.size(); i++)
	{
		m_pool[i].setActive(false); // mark all barriers as inactive
	}
}