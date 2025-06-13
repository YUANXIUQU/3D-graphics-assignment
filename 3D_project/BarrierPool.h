#pragma once
#include "barrier.h"

class Game;
class BarrierPool
{
private:
	std::vector<Barrier> m_pool;
	Game* m_game;
public:
	BarrierPool(Game* _game, int size);
	void reset();
	Barrier* getObject();
};
