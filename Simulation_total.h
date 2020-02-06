#pragma once
#include<SFML/Graphics.hpp>
#include"AssetManager.h"
#include<algorithm>
#include<iostream>
#include<cassert>
#include<vector>
#include<thread>
#include<random>
#include<cmath>
using namespace std;

/////////////////////------------DEFINE--------------////////////////////////
//// Define some Setting and Functino
#define CannotResize  (sf::Style::Titlebar |  sf::Style::Close)
#define KeyPressing sf::Event::KeyPressed
#define KeyReleasing sf::Event::KeyReleased
#define KeyEvent(EV) (sf::Keyboard::isKeyPressed(sf::Keyboard::EV))

////  Define Source File Path String
#define font_3D "Source\\3D Hand Drawns.ttf"

/////////////////////-------------ENUM---------------////////////////////////
//// Define State of Person
enum class STATE { Healthy, Incubat, Infected, Death, Isolated, Cured };

/////////////////////--------------------------------////////////////////////
////  Looper Function
int infectious_main();

////  Change int to string
string ToString(int num);

////  Thread Recall function
void KeyCtrlMoveSpeed(sf::RenderWindow& App);
