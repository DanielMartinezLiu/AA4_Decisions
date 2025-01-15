#pragma once
#include "Agent.h"

#define PLAYER_MANAGER PlayerManager::Instance()

class PlayerManager 
{
private:

	PlayerManager() = default;

	PlayerManager(const PlayerManager&) = delete;
	PlayerManager& operator =(const PlayerManager&) = delete;

	Agent* player;

public:

	inline static PlayerManager& Instance()
	{
		static PlayerManager playerManager;
		return playerManager;
	}

	inline Agent* GetPlayer() const {  return player; }

	inline void SetPlayer(Agent* _agent) { player = _agent; }
};