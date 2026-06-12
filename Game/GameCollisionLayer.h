#pragma once

namespace dae
{
	// GameCollisionLayers namespace defines unique identifiers for different collision layers in the game, such as tanks, bullets, and walls.
	namespace GameCollisionLayers
	{
		constexpr unsigned int Tank = 1;
		constexpr unsigned int Bullet = 2;
		constexpr unsigned int Wall = 3;
	}
}