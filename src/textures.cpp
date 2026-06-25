#include "textures.hpp"
#include "constants.hpp"
#include <SFML/Graphics.hpp>
#include "permaAssert.hpp"
#include "mob.hpp"
#include <string>

void initTextures(RenderContext &rc)
{
	//player textures
	PERMA_ASSERT_MSG(rc.playerTexture.loadFromFile("resources/Textures/player.png"), "Cant Load Player Textures");

	//font
	PERMA_ASSERT_MSG(rc.font.openFromFile("resources/cursyger.ttf"), "Cant Load Font From A File");

	//background textures
	rc.backgroundTexture.loadFromFile("resources/forestBG.png");
	rc.backgroundTexture.setRepeated(true);

	//block textures
	PERMA_ASSERT_MSG(rc.blocksAtlas.loadFromFile("resources/Textures/blocks.png"), "CANT LOAD BLOCKS ATLAS");

	//mob textures
	rc.mobTextures.resize(Mob::MobId::Count);
	auto loadMobTexture = [&rc](const int mi, const std::string& path)
	{
		PERMA_ASSERT_MSG(rc.mobTextures[mi].loadFromFile(path), "Failed to load texture: " + path);
	};
	loadMobTexture(Mob::MobId::Zombie, "resources/Textures/zombie.png");
}