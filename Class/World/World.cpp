#include "World.h"

World::World()
{

	json_.Init("World.json");

	json_.BeginGroup("World")
		.AddValue<float>("WorldLimitRadius", &worldLimitRadius_)
		.EndGroup();

}

void World::DebugGUI() {

	json_.DebugGUI();

}
