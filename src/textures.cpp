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
	rc.playerTexture.setSmooth(false);

	//hearts
	PERMA_ASSERT_MSG(rc.hearts.loadFromFile("resources/Textures/hearts.png"), "Cant Load Hearts Texture");

	//font
	PERMA_ASSERT_MSG(rc.font.openFromFile("resources/cursyger.ttf"), "Cant Load Font From A File");

	//background textures
	PERMA_ASSERT_MSG(rc.backgroundTexture.loadFromFile("resources/forestBG.png"), "Cant Load Forest BG Texture");
	rc.backgroundTexture.setRepeated(true);

	//block textures
	PERMA_ASSERT_MSG(rc.blocksAtlas.loadFromFile("resources/Textures/blocks.png"), "CANT LOAD BLOCKS ATLAS");
	rc.blocksAtlas.setSmooth(false);

	//mob textures
	rc.mobTextures.resize(Mob::MobId::Count);
	auto loadMobTexture = [&rc](const int mi, const std::string& path)
	{
		PERMA_ASSERT_MSG(rc.mobTextures[mi].loadFromFile(path), "Failed to load texture: " + path);
		rc.mobTextures[mi].setSmooth(false);
	};
	loadMobTexture(Mob::MobId::Zombie, "resources/Textures/zombie.png");
}