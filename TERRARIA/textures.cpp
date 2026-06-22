#include "textures.hpp"
#include "Constants.hpp"
#include <SFML/Graphics.hpp>
#include "permaAssert.hpp"
#include "Mob.hpp"
#include <string>

void initTextures(RenderContext &rc)
{
	PERMA_ASSERT_MSG(rc.font.openFromFile("RESOURCES/cursyger.ttf"), "Cant Load Font From A File");

	rc.mobTextures.resize(Mob::MobId::Count);
	
	PERMA_ASSERT_MSG(rc.blocksAtlas.loadFromFile("RESOURCES/Textures/blocks.png"), "CANT LOAD BLOCKS ATLAS");
	
	auto loadText = [&rc](const int mi, const std::string& path)
	{
		PERMA_ASSERT_MSG(rc.mobTextures[mi].loadFromFile(path), "Failed to load texture: " + path);
	};

	loadText(Mob::MobId::Zombie, "RESOURCES/Textures/zombie.png");
}