#pragma once
#include"Simulation_total.h"
class Parameter
{
public:
	Parameter()
	{
		assert(IsInstance == nullptr);
		IsInstance = this;
	}

public:
	/////////////////////------------CONST---------------////////////////////////
	static unsigned long long Populations;	//  populations

	static float MapRange;					//  People Active Range, sigma^2

	static int OriginNum;					//  original number
	static float HosRefTime;				//  Hospital Reflect time
	static unsigned long long HosBedNum;	//  Hospital Bed Numbers

	static float IncubationTime;			//  Average Incubation, 5 days
	static float SpreadRate;				//  Spread Rate, 80%

	static float DeathRate;					//  Death Rate, 3.0%
	static float DeathTime;					//  if not be isolated, dead after 7 Day

	static float LeastCureTime;				//  Cured least need 16 Day
	static float CuredRate;					//  Cured Rate

	static float MoveSpeed_MAX;				//  Max movespeed
	static float MoveSpeed_MIN;				//  Min movespeed

	/////////////////////-------------STATIC-------------////////////////////////
	static float MoveSpeed;
	static unsigned long long DeathCount;
	static unsigned long long UsingBed, CuredCount;
	static unsigned long long InfectCount, HealthCount, IncubeCount;
	static std::random_device rd;

private:
	static Parameter* IsInstance; //  确保只有一个实例
};

/////////////////////------------INIT----------------////////////////////////
Parameter* Parameter::IsInstance = nullptr;
////
unsigned long long Parameter::Populations = 4000;
float Parameter::MapRange = 280;
int Parameter::OriginNum = 40;
float Parameter::HosRefTime = 3.5f;
unsigned long long Parameter::HosBedNum = 200;
float Parameter::IncubationTime = 10.0f;
float Parameter::SpreadRate = 0.8f;
float Parameter::DeathRate = 0.02f;
float Parameter::DeathTime = 7.0f;
float Parameter::LeastCureTime = 16.0f;
float Parameter::CuredRate = 0.6f;
float Parameter::MoveSpeed_MAX = 5.0f;
float Parameter::MoveSpeed_MIN = 0.0f;
////
float Parameter::MoveSpeed = 0;
unsigned long long Parameter::InfectCount = 0,
Parameter::HealthCount = 0, 
Parameter::IncubeCount = 0,
Parameter::DeathCount = 0, 
Parameter::UsingBed = 0, 
Parameter::CuredCount = 0;
////
std::random_device Parameter::rd;