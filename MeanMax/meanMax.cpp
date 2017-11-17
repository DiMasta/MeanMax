#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <stack>
#include <set>
#include <queue>
#include <algorithm>
#include <ctime>
#include <deque>
#include <math.h>
#include <fstream>
#include <climits>

using namespace std;

const bool OUTPUT_GAME_DATA = 1;
const bool USE_HARDCODED_INPUT = 0;

//#define REDIRECT_CIN_FROM_FILE
const string INPUT_FILE_NAME = "input.txt";

const int INVALID_ID = -1;
const int INVALID_NODE_DEPTH = -1;
const int TREE_ROOT_NODE_DEPTH = 1;
const int ZERO_CHAR = '0';
const int DIRECTIONS_COUNT = 8;

const int INVALID_WATER_QUANTITY = -1;
const int MAX_THROTTLE = 300;
const string WAIT = "WAIT";


enum PlayerId {
	PI_INVALID = -1,
	PI_MY_PALYER = 0,
	PI_ENEMY_PLAYER_1,
	PI_ENEMY_PLAYER_2,
};

enum UnitType {
	UT_INVALID = -1,
	UT_REAPER = 0,
	UT_WRECK = 4,
};

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

typedef int Coord;
const Coord INVALID_COORD = -1;

class Coords {
public:
	Coords();
	Coords(Coord xCoord, Coord yCoord);

	Coord getXCoord() const {
		return xCoord;
	}

	Coord getYCoord() const {
		return yCoord;
	}

	void setXCoord(Coord xCoord) { this->xCoord = xCoord; }
	void setYCoord(Coord yCoord) { this->yCoord = yCoord; }

	Coords& operator=(const Coords& other);
	bool operator==(const Coords& other);
	Coords operator+(const Coords& other);
	Coords& operator+=(const Coords& other);

	bool isValid() const;
	void debug() const;
	void print() const;

	int distance(const Coords& coords) const;

private:
	Coord xCoord;
	Coord yCoord;
};

//*************************************************************************************************************
//*************************************************************************************************************

Coords::Coords() :
	xCoord(INVALID_COORD),
	yCoord(INVALID_COORD)
{
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords::Coords(
	Coord xCoord,
	Coord yCoord
) :
	xCoord(xCoord),
	yCoord(yCoord)
{
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords& Coords::operator=(const Coords& other) {
	if (this != &other) {
		xCoord = other.xCoord;
		yCoord = other.yCoord;
	}

	return *this;
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Coords::operator==(const Coords& other) {
	return (xCoord == other.xCoord) && (yCoord == other.yCoord);
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords Coords::operator+(const Coords& other) {
	return Coords(xCoord + other.xCoord, yCoord + other.yCoord);
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords& Coords::operator+=(const Coords& other) {
	xCoord += other.xCoord;
	yCoord += other.yCoord;

	return *this;
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Coords::isValid() const {
	return INVALID_COORD != xCoord && INVALID_COORD != yCoord;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Coords::debug() const {
	cerr << "Position: X=" << xCoord << ", Y=" << yCoord << endl;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Coords::print() const {
	cout << xCoord << " " << yCoord;
}

//*************************************************************************************************************
//*************************************************************************************************************

int Coords::distance(const Coords& coords) const {
	int kat0 = coords.xCoord - xCoord;
	int kat1 = coords.yCoord - yCoord;

	int hip = (int)sqrt((kat0 * kat0) + (kat1 * kat1));
	return hip;
}

//*************************************************************************************************************
//*************************************************************************************************************

enum Direction {
	DIR_INVALID = -1,
	DIR_N = 0,
	DIR_NE,
	DIR_E,
	DIR_SE,
	DIR_S,
	DIR_SW,
	DIR_W,
	DIR_NW,
};

//*************************************************************************************************************
//*************************************************************************************************************

Coords DIRECTIONS[DIRECTIONS_COUNT] = {
	Coords(0, -1), // N
	Coords(1, -1), // NE
	Coords(1,  0), // E
	Coords(1,  1), // SE
	Coords(0,  1), // S
	Coords(-1,  1), // SW
	Coords(-1,  0), // W
	Coords(-1, -1)  // NW
};

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class Entity {
public:
	Entity();
	Entity(int id, Coords position, int radius);
	virtual ~Entity();

	int getId() const {
		return id;
	}

	Coords getPosition() const {
		return position;
	}

	int getRadius() const {
		return radius;
	}

	void setId(int id) { this->id = id; }
	void setPosition(Coords position) { this->position = position; }
	void setRadius(int radius) { this->radius = radius; }

private:
	int id;
	Coords position;
	int radius;
};

//*************************************************************************************************************
//*************************************************************************************************************

Entity::Entity() :
	position(),
	id(INVALID_ID)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Entity::Entity(int id, Coords position, int radius) :
	id(id),
	position(position),
	radius(radius)
{
}

//*************************************************************************************************************
//*************************************************************************************************************

Entity::~Entity() {

}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class Reaper : public Entity {
public:
	Reaper();
	Reaper(
		int id,
		Coords position,
		int radius,
		int playerId,
		Coords velocity,
		float mass
	);
	~Reaper();

	int getPlayerId() const {
		return playerId;
	}

	Coords getVelocity() const {
		return velocity;
	}

	float getMass() const {
		return mass;
	}

	void setvelocity(Coords velocity) { this->velocity = velocity; }
	void setMass(float mass) { this->mass = mass; }
	void setPlayerId(int playerId) { this->playerId = playerId; }

private:
	int playerId;
	Coords velocity;
	float mass;
};

//*************************************************************************************************************
//*************************************************************************************************************

Reaper::Reaper() :
	Entity()
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Reaper::Reaper(
	int id,
	Coords position,
	int radius,
	int playerId,
	Coords velocity,
	float mass
) :
	Entity(id, position, radius),
	playerId(playerId),
	velocity(velocity),
	mass(mass)
{
}

//*************************************************************************************************************
//*************************************************************************************************************

Reaper::~Reaper() {
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class Wreck : public Entity {
public:
	Wreck();
	Wreck(int id, Coords position, int radius, int waterQuantity);
	~Wreck();

	int getWaterQuantity() const {
		return waterQuantity;
	}

	void setWaterQuantity(int waterQuantity) { this->waterQuantity = waterQuantity; }

	bool empthy() const;

private:
	int waterQuantity;
};

typedef vector<Wreck> Wrecks;

//*************************************************************************************************************
//*************************************************************************************************************

Wreck::Wreck() :
	waterQuantity(INVALID_WATER_QUANTITY)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Wreck::Wreck(int id, Coords position, int radius, int waterQuantity) :
	Entity(id, position, radius),
	waterQuantity(waterQuantity)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Wreck::~Wreck() {

}

//*************************************************************************************************************
//*************************************************************************************************************

bool Wreck::empthy() const {
	return 0 == waterQuantity;
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class Game {
public:
	Game();
	~Game();

	void initGame();
	void gameBegin();
	void gameLoop();
	void getGameInput();
	void getTurnInput();
	void turnBegin();
	void makeTurn();
	void turnEnd();
	void play();

	void debug() const;

	Coords findNearestWreck() const;

private:
	int turnsCount;

	Reaper* myReaper;
	Reaper* enemyReaper1;
	Reaper* enemyReaper2;

	Wrecks* wrecks;
};

//*************************************************************************************************************
//*************************************************************************************************************

Game::Game() :
	turnsCount(0),
	myReaper(nullptr),
	enemyReaper1(nullptr),
	enemyReaper2(nullptr),
	wrecks(nullptr)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Game::~Game() {
	if (myReaper) {
		delete myReaper;
		myReaper = nullptr;
	}

	if (enemyReaper1) {
		delete enemyReaper1;
		enemyReaper1 = nullptr;
	}

	if (enemyReaper2) {
		delete enemyReaper2;
		enemyReaper2 = nullptr;
	}

	if (wrecks) {
		delete wrecks;
		wrecks = nullptr;
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::initGame() {
	wrecks = new Wrecks;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::gameBegin() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::gameLoop() {
	while (true) {
		getTurnInput();
		turnBegin();
		makeTurn();
		turnEnd();
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::getGameInput() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::getTurnInput() {
	int myScore, enemyScore1, enemyScore2, myRage, enemyRage1, enemyRage2, unitCount;
	cin >> myScore >> enemyScore1 >> enemyScore2 >> myRage >> enemyRage1>> enemyRage2 >> unitCount; cin.ignore();

	if (OUTPUT_GAME_DATA) {
		cerr << myScore << " ";
		cerr << enemyScore1 << " ";
		cerr << enemyScore2 << " ";
		cerr << myRage << " ";
		cerr << enemyRage1 << " ";
		cerr << enemyRage2 << " ";
		cerr << unitCount << " ";
	}

	for (int i = 0; i < unitCount; i++) {
		int unitId, unitType, player, radius, x, y, vx, vy, extra, extra2;
		float mass;
		cin >> unitId >> unitType >> player >> mass >> radius >> x >> y >> vx >> vy >> extra >> extra2; cin.ignore();

		if (OUTPUT_GAME_DATA) {
			cerr << unitId << " ";
			cerr << unitType << " ";
			cerr << player << " ";
			cerr << mass << " ";
			cerr << radius << " ";
			cerr << x << " ";
			cerr << y << " ";
			cerr << vx << " ";
			cerr << vy << " ";
			cerr << extra << " ";
			cerr << extra2 << endl;
		}

		Coords position(x, y);
		Coords velocity(vx, vy);

		if (UT_REAPER == unitType) {
			switch (player) {
				case (PI_MY_PALYER): {
					myReaper = new Reaper(unitId, position, radius, player, velocity, mass);
					break;
				}
				case (PI_ENEMY_PLAYER_1): {
					enemyReaper1 = new Reaper(unitId, position, radius, player, velocity, mass);
					break;
				}
				case (PI_ENEMY_PLAYER_2): {
					enemyReaper2 = new Reaper(unitId, position, radius, player, velocity, mass);
					break;
				}
				default: {
					break;
				}
			}
		}
		else if (UT_WRECK == unitType) {
			Wreck wreck(unitId, position, radius, extra);
			wrecks->push_back(wreck);
		}
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::turnBegin() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::makeTurn() {
	Coords nearestWreck = findNearestWreck();
	nearestWreck.print();

	cout << " " << MAX_THROTTLE / 2 << endl;
	cout << WAIT << endl;
	cout << WAIT << endl;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::turnEnd() {
	++turnsCount;

	wrecks->clear();
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::play() {
	initGame();
	getGameInput();
	gameBegin();
	gameLoop();
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::debug() const {
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords Game::findNearestWreck() const {
	Coords res;
	int minDist = INT_MAX;

	for (Wrecks::const_iterator it = wrecks->begin(); it != wrecks->end(); ++it) {
		if (it->empthy()) {
			continue;
		}

		const Coords wreckPostion = it->getPosition();
		const int distToMyReaper = wreckPostion.distance(myReaper->getPosition());

		if (distToMyReaper < minDist) {
			res = wreckPostion;
			minDist = distToMyReaper;
		}
	}

	return res;
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

#ifdef TESTS
#include "debug.h"
#endif // TESTS

int main(int argc, char** argv) {
#ifdef TESTS
	doctest::Context context;
	int res = context.run();
#else

#ifdef REDIRECT_CIN_FROM_FILE
	ifstream in(INPUT_FILE_NAME);
	streambuf *cinbuf = cin.rdbuf();
	cin.rdbuf(in.rdbuf());
#endif

	Game game;
	game.play();
#endif // TESTS

	return 0;
}

// seed=208265368