#pragma once
#include"Parameter.h"
class UI : public sf::Drawable
{
public:
	UI(sf::RenderWindow& app) :App(app), SquareNum(0)
	{
		/////////////////////--------------------------------////////////////////////
		//// Text Init
		TDFont = AssetManager::GetFont(font_3D);
		text_Red.setFont(TDFont);
		text_Red.setCharacterSize(18);
		text_Red.setFillColor(sf::Color::Red);
		text_Red.setPosition(25, 25);

		text_Green.setFont(TDFont);
		text_Green.setCharacterSize(18);
		text_Green.setFillColor(sf::Color::Green);
		text_Green.setPosition(25, 25);

		/////////////////////--------------------------------////////////////////////
		////  ISO UI Graphic Init
		////  Contour Init
		sf::Color BgColor{ 50, 50, 50, 255 };
		sf::Color LEDColor{ 0,255,0,255 };
		Contour.setSize(sf::Vector2f{ 60, 510 });
		Contour.setFillColor(BgColor);
		Contour.setOutlineThickness(5);
		Contour.setOutlineColor(LEDColor);
		Contour.setPosition(40, App.getSize().y - Contour.getSize().y - 25);
		OriginPosOfContour = Contour.getPosition() + sf::Vector2f{ 0 + 5,510 - 5 };
		////  Square Init
		Square.setSize(sf::Vector2f{ 50, 50 });
		Square.setOutlineThickness(3);
		Square.setOutlineColor(BgColor);
		Square.setOrigin(0, Square.getSize().y);

		Square.setPosition(OriginPosOfContour);
		Square.setFillColor(LEDColor);
		for (int i = 0; i < 10; i++) {
			Squares.push_back(Square);
			Square.move(sf::Vector2f{ 0,-50 });
			LEDColor.r += 255 / 10, LEDColor.g -= 255 / 10;
			Square.setFillColor(LEDColor);
		}
		SquareNum = 6;
	}

	void Update(float const& TotalDay)
	{
		/////////////////////--------------------------------////////////////////////
		//  Text Update
		string CountStr = "";
		CountStr = ("DAY(s): " + ToString((int)TotalDay) + "\n\n");
		CountStr += ("Healthy: " + ToString((int)Parameter::HealthCount) + "/" + ToString((int)Parameter::Populations) + "\n\n");
		CountStr += ("Incubated:" + ToString((int)Parameter::IncubeCount) + "\n");
		CountStr += ("Infected: " + ToString((int)Parameter::InfectCount) + "\n");
		CountStr += ("Dead: " + ToString((int)Parameter::DeathCount) + "\n");
		text_Red.setString(CountStr);
		CountStr = ("\n\n\n\n\n\n\n\n");
		CountStr += ("Curing: " + ToString((int)Parameter::UsingBed) + "/" + ToString((int)Parameter::HosBedNum) + "\n");
		CountStr += ("Cured: " + ToString((int)Parameter::CuredCount) + "\n\n");
		string float_tmpstr = (ToString((int)Parameter::MoveSpeed) + "." +
			ToString((int)1000 * (Parameter::MoveSpeed - (int)Parameter::MoveSpeed)) + "\n");
		CountStr += ("MoveRate: " + float_tmpstr);
		CountStr += ("\n\n\n\n\n\n\n\n\n\n\nISO:");
		text_Green.setString(CountStr);

		/////////////////////--------------------------------////////////////////////
		//  ISO UI Update
		SquareNum = static_cast<int>((float)Parameter::UsingBed / (float)Parameter::HosBedNum * 10);
		if (SquareNum < 10) {
			Contour.setOutlineColor(Squares[SquareNum].getFillColor());
		}
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates state)const {
		target.draw(text_Red, state);
		target.draw(text_Green, state);
		target.draw(Contour, state);
		
		int i = 0;
		for (auto it = Squares.begin(); it != Squares.end(); it++) {
			if (i == SquareNum)break;
			target.draw(*it, state);
			i++;
		}
	}
private:
	sf::RenderWindow& App;

	sf::Font TDFont;

	sf::Text text_Red, text_Green;

	sf::RectangleShape Contour;

	sf::Vector2f OriginPosOfContour;

	sf::RectangleShape Square;

	vector<sf::RectangleShape>Squares;

	int SquareNum;

};