#pragma once
#include<SFML/Graphics.hpp>
#include<vector>
#include<iostream>
#include<thread>
#include<vector>
#include"Animator.h"
using namespace std;

#define CannotResize  (sf::Style::Titlebar |  sf::Style::Close)

#define KeyPressing sf::Event::KeyPressed
#define KeyReleasing sf::Event::KeyReleased
#define KeyEvent(EV) (sf::Keyboard::isKeyPressed(sf::Keyboard::EV))

//Source File Path:
#define Player_f "Source\\player_first.png"
#define PlatForm_f "Source\\Platform.png"
#define font_3D "Source\\3D Hand Drawns.ttf"
#define BackGround_f "Source\\BackGround_p64.png"
#define Player_virus_f "Source\\enemy_p54_anemi.png"

string ToString(int num);

int cube();

int painting_main();

int Star_main();

int ch01_main();

int infectious_main();