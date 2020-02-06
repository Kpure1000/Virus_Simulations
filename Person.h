#pragma once
#include"Parameter.h"

class Person :public sf::Drawable
{
public:
	Person(sf::Vector2f position, STATE state)
		:State(state), InfectTime(0), FoundTime(0), CureTime(0), IsDraw(true)
	{
		Dot.setRadius(2);
		Dot.setPosition(position);
	}

	void SetColor()
	{
		switch (State)
		{
		case STATE::Healthy: Dot.setFillColor(sf::Color(180, 180, 180, 230));
			break;
		case STATE::Incubat: Dot.setFillColor(sf::Color::Yellow);
			break;
		case STATE::Infected: Dot.setFillColor(sf::Color::Red);
			break;
		case STATE::Cured: Dot.setFillColor(sf::Color::Green);
			break;
		default:
			break;
		}
	}

	bool CollisionCheck(Person in)
	{
		auto inpos = in.GetPosition();
		auto thispos = this->GetPosition();
		float distance = sqrt(pow(inpos.x - thispos.x, 2) + pow(inpos.y - thispos.y, 2));
		if (distance < this->GetRadius() + in.GetRadius()) {
			return true;
		}
		return false;
	}

	void Update(vector<Person>& dots, float DayTime)
	{
		if (this->State == STATE::Infected or this->State == STATE::Incubat) //  具备传染性
		{
			for (size_t i = Parameter::InfectCount + Parameter::IncubeCount; i < dots.size(); i++) {
				if (
					(fabs(this->GetPosition().x - dots[i].GetPosition().x) > this->GetRadius() * 2)
					or
					(fabs(this->GetPosition().y - dots[i].GetPosition().y) > this->GetRadius() * 2)
					) {
					continue;
				}
				else if (CollisionCheck(dots[i])) {  //  碰撞检测
					float SpR_K;				//  感染率系数
					if (this->State == STATE::Incubat)SpR_K = 0.1f;
					if (this->State == STATE::Infected)SpR_K = 0.3f;

					default_random_engine rng{ Parameter::rd() };
					uniform_real_distribution<float> Rate(0, 1);

					if (dots[i].State == STATE::Healthy and Rate(rng) < SpR_K) {
						dots[i].BeIncubated(); //  被感染但处于潜伏期
						dots[i].InfectTime = DayTime;
						swap(dots[i], dots[Parameter::InfectCount + Parameter::IncubeCount]);
						Parameter::IncubeCount++;
					}
				}
			}
		}
		/////////////////////--------------------------------////////////////////////
		if (this->State == STATE::Incubat and DayTime - InfectTime > Parameter::IncubationTime)  //  潜伏期
		{
			this->BeInfected();				//  症状被发现
			this->FoundTime = DayTime;
			Parameter::InfectCount++;
			Parameter::IncubeCount--;
		}
		/////////////////////--------------------------------////////////////////////
		if (this->State == STATE::Infected and DayTime - FoundTime > Parameter::HosRefTime)   //  被感染
		{
			if (Parameter::UsingBed < Parameter::HosBedNum) {		//  医院有空床位
				this->BeIsolated();			//  被隔离在医院
				Parameter::UsingBed++;
				Parameter::InfectCount--;
				this->CureTime = DayTime;
				this->IsDraw = false;
			}
		}
		/////////////////////--------------------------------////////////////////////
		if (this->State == STATE::Infected and DayTime - FoundTime > Parameter::DeathTime)   //   被感染
		{
			default_random_engine rng{ Parameter::rd() };
			uniform_real_distribution<float> Rate(0, 1);
			if (Rate(rng) < Parameter::DeathRate) {
				this->BeDead(); //  死亡
				this->IsDraw = false;
				Parameter::InfectCount--;
			}
		}
		/////////////////////--------------------------------////////////////////////
		if (this->State == STATE::Isolated and DayTime - CureTime > Parameter::LeastCureTime) //  被隔离
		{
			default_random_engine rng{ Parameter::rd() };
			uniform_real_distribution<float> Rate(0, 1);

			if (Rate(rng) < Parameter::DeathRate / 2) {
				this->BeDead();			//  死亡
				this->IsDraw = false;
				if (Parameter::UsingBed > 0)
					Parameter::UsingBed--;			//  出院，空出床位
			}
			else if (Rate(rng) < Parameter::CuredRate) {
				this->BeCuited();		//  治愈
				this->IsDraw = true;	//  变绿且可以回归社会，不会再被感染 (未设置免疫期限)
				if (Parameter::UsingBed > 0)
					Parameter::UsingBed--;			//  出院，空出床位
			}
		}
		/////////////////////--------------------------------////////////////////////
		if (IsDraw)
		{
			this->SetColor();
			this->randomove(Parameter::MoveSpeed);
		}
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates state)const {
		if (this->GetPosition().x < target.getSize().x and this->GetPosition().x>0
			and this->GetPosition().y < target.getSize().y and this->GetPosition().y>0
			and this->IsDraw)
			target.draw(Dot, state);
		else return;
	}
	/////////////////////--------------------------------////////////////////////
	void SetPosition(sf::Vector2f position) { Dot.setPosition(position); }

	sf::Vector2f GetPosition()const { return Dot.getPosition(); }

	float GetRadius() { return this->Dot.getRadius(); }
	/////////////////////--------------------------------////////////////////////
	void BeIncubated() { this->State = STATE::Incubat; Parameter::HealthCount--; }

	void BeInfected() { this->State = STATE::Infected; }

	void BeIsolated() { this->State = STATE::Isolated; }

	void BeDead() { this->State = STATE::Death; Parameter::DeathCount++; }

	void BeCuited() { this->State = STATE::Cured; Parameter::CuredCount++; }
	/////////////////////--------------------------------////////////////////////


private:
	void randomove(float moverate) {
		std::default_random_engine rng{ Parameter::rd() };
		std::normal_distribution<> norm{ 0, 1 };
		sf::Vector2f mover((float)moverate * norm(rng), (float)moverate * norm(rng));
		Dot.move(mover);
	}

private:
	sf::CircleShape Dot;

	STATE State;

	bool IsDraw;

	float InfectTime;

	float FoundTime;

	float CureTime;

};
