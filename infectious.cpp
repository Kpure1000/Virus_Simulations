#include"total.h"
#include<cmath>
#include<random>
#include<algorithm>

const unsigned long long Populations = 3000;   //  populations

const float MapRange = 280.0f;

const int OriginNum = 40;      //  original number
// const float MoveRate = 0.0f;   //  people move rate
const float HosRefTime = 3.5f; //  Hospital Reflect time
const unsigned long long HosBedNum = 100;  //  Hospital Bed Numbers

const float IncubationTime = 10.0f; //  Average Incubation, 5 days
const float SpreadRate = 0.8f; //  Spread Rate, 80%

const float DeathRate = 0.03f;  //  Death Rate, 3.0%
const float DeathTime = 7.0f;  //  if not be isolated, dead after 7 Day

const float LeastCureTime = 16.0f; //  Cured least need 16 Day
const float CuredRate = 0.6f;   //  Cured Rate

unsigned long long InfectCount = 0, HealthCount = 0, IncubeCount = 0;
unsigned long long DeathCount = 0;
unsigned long long UsingBed = 0, CuredCount = 0;
std::random_device rd;

//Death Rate = 

enum class STATE { Healthy, Incubat, Infected, Death, Isolated, Cured };

/////////////////
//  thread func
float MoveSpeed = 0;
void KeyCtrlMoveSpeed(sf::RenderWindow&App) {
	while (App.isOpen()) {
		if (KeyPressing and KeyEvent(Up) and MoveSpeed < 4.0) {
			MoveSpeed += 0.1f; 
		}
		if (KeyPressing and KeyEvent(Down) and MoveSpeed > 0) {
			MoveSpeed -= 0.1f; 
		}
		this_thread::sleep_for(chrono::milliseconds(100));
	}
	
}

class Person :public sf::Drawable
{
public:
	Person(sf::Vector2f position, STATE state)
		:State(state), InfectTime(0), FoundTime(0), CureTime(0), IsDraw(true) {
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

	bool CollisionCheck(Person in) {
		auto inpos = in.GetPosition();
		auto thispos = this->GetPosition();
		float distance = sqrt(pow(inpos.x - thispos.x, 2) + pow(inpos.y - thispos.y, 2));
		if (distance < this->GetRadius() + in.GetRadius()) {
			return true;
		}
		return false;
	}

	void Update(vector<Person>&dots, float DayTime) {
		if (this->State == STATE::Infected or this->State == STATE::Incubat)
		{
			for (size_t i = InfectCount+IncubeCount; i < dots.size(); i++)
			{
				if (
					(fabs(this->GetPosition().x - dots[i].GetPosition().x) > this->GetRadius() * 2)
					or
					(fabs(this->GetPosition().y - dots[i].GetPosition().y) > this->GetRadius() * 2)
					) 
				{
					continue;
				}
				else if (CollisionCheck(dots[i]))
				{
					float SpR_K;
					if (this->State == STATE::Incubat)SpR_K = 0.15f;
					if (this->State == STATE::Infected)SpR_K = 0.3f;

					default_random_engine rng{ rd() };
					uniform_real_distribution<float> Rate(0, 1);

					if (dots[i].State==STATE::Healthy and Rate(rng) < SpR_K) 
					{ 
						dots[i].BeIncubated(); //  被感染但处于潜伏期
						dots[i].InfectTime = DayTime;
						swap(dots[i], dots[InfectCount + IncubeCount]);
						IncubeCount++;
					}
				}
			}
		}

		if (this->State==STATE::Incubat and DayTime - InfectTime > IncubationTime) 
		{
			this->BeInfected(); //  症状被发现
			this->FoundTime = DayTime;
			InfectCount++;
			IncubeCount--;
		}
		if (this->State == STATE::Infected and DayTime - FoundTime > HosRefTime)
		{
			if (UsingBed < HosBedNum)
			{
				this->BeIsolated(); //  被隔离在医院
				UsingBed++;
				InfectCount--;
				this->CureTime = DayTime;
				this->IsDraw = false;
			}
		}
		if (this->State == STATE::Infected and DayTime - FoundTime > DeathTime)
		{
			this->BeDead(); //  死亡
			this->IsDraw = false;
			InfectCount--;
		}
		if (this->State == STATE::Isolated and DayTime - CureTime > LeastCureTime)
		{
			default_random_engine rng{ rd() };
			uniform_real_distribution<float> Rate(0, 1);
			if (Rate(rng) < DeathRate)
			{
				this->BeDead(); //  死亡
				this->IsDraw = false;
				if (UsingBed > 0)
				{
					UsingBed--;
				}
			}
			if (Rate(rng) < CuredRate)
			{
				this->BeCuited(); //  治愈
				this->IsDraw = true;
				if (UsingBed > 0)
				{
					UsingBed--;
				}
			}
		}
		if (IsDraw)
		{
			this->SetColor();
			this->randomove(MoveSpeed);
		}
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates state)const {
		target.draw(Dot, state);
	}

	void SetPosition(sf::Vector2f position) { Dot.setPosition(position); }
	
	sf::Vector2f GetPosition() { return Dot.getPosition(); }

	float GetRadius() { return this->Dot.getRadius(); }

	void BeIncubated() { this->State = STATE::Incubat; HealthCount--; }

	void BeInfected() { this->State = STATE::Infected;   }

	void BeIsolated() { this->State = STATE::Isolated; }

	void BeDead() { this->State = STATE::Death; DeathCount++; }

	void BeCuited() { this->State = STATE::Cured; CuredCount++; }

	bool IsDraw;

private:
	void randomove(float moverate) {
		std::default_random_engine rng{ rd() };
		std::normal_distribution<> norm{ 0, 1 };
		sf::Vector2f mover(moverate * norm(rng), moverate * norm(rng));
		Dot.move(mover);
	}

private:
	sf::CircleShape Dot;

	STATE State;

	float InfectTime;

	float FoundTime;

	float CureTime;

};

bool Comp_person(Person A, Person B) {
	return A.GetPosition().x < B.GetPosition().x;
}

int infectious_main()
{
	const float AppW = 2000, AppH = 1500;
	sf::RenderWindow App(sf::VideoMode((unsigned int)AppW, (unsigned int)AppH), "Infectious", CannotResize);
	App.setFramerateLimit(120);
	float TotalDay = 0.0;

	AssetManager Ma;
	sf::Font TDFont = AssetManager::GetFont(font_3D);
	sf::Text text_Red;
	text_Red.setFont(TDFont);
	text_Red.setCharacterSize(20);
	text_Red.setFillColor(sf::Color::Red);
	text_Red.setPosition(50, 50);
	sf::Text text_Green;
	text_Green.setFont(TDFont);
	text_Green.setCharacterSize(20);
	text_Green.setFillColor(sf::Color::Green);
	text_Green.setPosition(50, 50);


	vector<Person>persons;

	std::default_random_engine rng{ rd() };
	uniform_real_distribution<float> Rate(0, 1);

	std::normal_distribution<> norm{ 0, MapRange };
	/////////
	//  Initialization:
	for (int i = 0; i < Populations; i++) 
	{
		sf::Vector2f axisoripos(AppW / 2, AppH / 2);
		sf::Vector2f originpos;
		originpos.x = norm(rng), originpos.y = norm(rng);
		if (Rate(rng) <= 0.015) 
		{
			persons.push_back(Person(originpos + axisoripos, STATE::Incubat));
			swap(persons.back(), persons[InfectCount + IncubeCount]);
			IncubeCount++;
		}
		else 
		{
			persons.push_back(Person(originpos + axisoripos, STATE::Healthy));
			HealthCount++;
		}
		
	}

	string CountStr = "";

	thread KeyCrtlMove_Thread(KeyCtrlMoveSpeed,ref(App));

	while (App.isOpen()) {
		sf::Event ev;
		while (App.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				App.close();
			}
			else if (KeyPressing and KeyEvent(Escape)) {
				App.close();
			}
			else if (TotalDay >= 1500) {
				App.close();
			}
		}

		CountStr = ("\tDAY: " + ToString((int)TotalDay) + "\n\n");
		CountStr += ("Healthy: " + ToString(HealthCount) + "\n\n");
		CountStr += ("Incubated:" + ToString(IncubeCount) + "\n");
		CountStr += ("Infected: " + ToString(InfectCount) + "\n");
		CountStr += ("Dead: " + ToString(DeathCount) + "\n");
		text_Red.setString(CountStr);
		CountStr = ("\n\n\n\n\n\n\n\n");
		CountStr += ("Isolated: " + ToString(UsingBed) + "\n");
		CountStr += ("Cured: " + ToString(CuredCount) + "\n\n");
		string float_tmpstr = (ToString((int)MoveSpeed) + "." + ToString(100 * MoveSpeed) + "\n");
		CountStr += ("MoveRate: " + float_tmpstr);
		text_Green.setString(CountStr);

		App.clear(sf::Color(0, 0, 0, 0));

		size_t count = 0;
		for (auto it = persons.begin(); it != persons.end(); it++) {
			it->Update(persons, TotalDay);
			if (it->GetPosition().x < AppW and it->GetPosition().x>0
				and it->GetPosition().y < AppH and it->GetPosition().y>0
				and it->IsDraw)
			App.draw(*it);
		}

		App.draw(text_Red);
		App.draw(text_Green);

		App.display();

		TotalDay += 0.01;

	}

	KeyCrtlMove_Thread.join();

	return 0;
}