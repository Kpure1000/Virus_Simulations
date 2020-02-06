#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include<map>
#include<cassert>
using namespace sf;
using namespace std;
class AssetManager
{	
private:
	std::map<std::string, sf::Texture> m_Textures;
	std::map<string, Font>m_Fonts;
	static AssetManager* sInstance;
public:
	AssetManager();
	~AssetManager() {}
	static Font& GetFont(string const& filename);
	static Texture& GetTexture(std::string const& filename);
};

//texture filenames
#define HYAKIMARU_FRONT "source\\Hyakimaru_Front.png"
#define FIRE_animate "source\\Fire_newstd.png"
#define Dragon_Test "source\\ZZdragon_Test.png"
#define ChikenWalk_left "source\\Chiken\\Chiken_LeftWalking.png"
#define Enemy "source\\Plane.png"
#define BlackWing "source\\BLACKWINGS.png"
#define CrossHair "source\\CrossHair.png"
#define Bullet "source\\Bullet.png"
#define Missile "source\\missile01.png"

//font filenames
#define BRUSH_FONT "source\\font\\download\\againts.ttf"
