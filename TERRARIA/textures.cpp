#include "textures.hpp"
#include "Constants.hpp"
#include <SFML/Graphics.hpp>
#include "permaAssert.hpp"
#include "Mob.hpp"
#include <string>

void initTextures(RenderContext &rc)
{
	//font
	PERMA_ASSERT_MSG(rc.font.openFromFile("RESOURCES/cursyger.ttf"), "Cant Load Font From A File");

	//background textures
	rc.backgroundTexture.loadFromFile("RESOURCES/forestBG.png");
	rc.backgroundTexture.setRepeated(true);

	//block textures
	PERMA_ASSERT_MSG(rc.blocksAtlas.loadFromFile("RESOURCES/Textures/blocks.png"), "CANT LOAD BLOCKS ATLAS");

	//mob textures
	rc.mobTextures.resize(Mob::MobId::Count);
	auto loadMobTexture = [&rc](const int mi, const std::string& path)
	{
		PERMA_ASSERT_MSG(rc.mobTextures[mi].loadFromFile(path), "Failed to load texture: " + path);
	};
	loadMobTexture(Mob::MobId::Zombie, "RESOURCES/Textures/zombie.png");
}