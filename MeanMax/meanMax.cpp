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

const bool OUTPUT_GAME_DATA = 0;
const bool USE_HARDCODED_INPUT = 0;

//#define REDIRECT_CIN_FROM_FILE
const string INPUT_FILE_NAME = "input.txt";

const int INVALID_ID = -1;
const int INVALID_NODE_DEPTH = -1;
const int TREE_ROOT_NODE_DEPTH = 1;
const int ZERO_CHAR = '0';
const int DIRECTIONS_COUNT = 8;

const float INVALID_MASS = -1.f;

const int INVALID_RADIUS = 0;
const int INVALID_WATER_QUANTITY = -1;
const int MAX_THROTTLE = 300;
const int MAP_RADIUS = 6000;

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
	UT_DESTROYER = 1,
	UT_TANKER = 3,
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
	Entity(int id, const Coords& position, int radius);
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
	void setPosition(const Coords& position) { this->position = position; }
	void setRadius(int radius) { this->radius = radius; }

	virtual void simulate() = 0;
	virtual void checkForCollisionWith(Entity* entity) = 0;

private:
	int id;
	Coords position;
	int radius;
};

typedef map<int, Entity*> Entities;

//*************************************************************************************************************
//*************************************************************************************************************

Entity::Entity() :
	position(),
	id(INVALID_ID),
	radius(INVALID_RADIUS)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Entity::Entity(int id, const Coords& position, int radius) :
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

class Vehicle : public Entity {
public:
	Vehicle();

	Vehicle(
		int id,
		const Coords& position,
		int radius,
		int playerId,
		const Coords& velocity,
		float mass
	);

	~Vehicle();

	int getPlayerId() const {
		return playerId;
	}

	Coords getVelocity() const {
		return velocity;
	}

	float getMass() const {
		return mass;
	}

	void setPlayerId(int playerId) { this->playerId = playerId; }
	void setVelocity(const Coords& velocity) { this->velocity = velocity; }
	void setMass(float mass) { this->mass = mass; }

	void update(
		int unitId,
		const Coords& position,
		int radius,
		int playerId,
		const Coords& velocity,
		float mass
	);

private:
	int playerId;
	Coords velocity;
	float mass;
};

//*************************************************************************************************************
//*************************************************************************************************************

Vehicle::Vehicle() :
	Entity(),
	playerId(INVALID_ID),
	velocity(),
	mass(INVALID_MASS)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Vehicle::Vehicle(
	int id,
	const Coords& position,
	int radius,
	int playerId,
	const Coords& velocity,
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

void Vehicle::update(
	int unitId,
	const Coords& position,
	int radius,
	int playerId,
	const Coords& velocity,
	float mass
) {
	setId(unitId);
	setPosition(position);
	setRadius(radius);

	this->playerId = playerId;
	this->velocity = velocity;
	this->mass = mass;
}

//*************************************************************************************************************
//*************************************************************************************************************

Vehicle::~Vehicle() {

}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class Reaper : public Vehicle {
public:
	Reaper();

	Reaper(
		int id,
		const Coords& position,
		int radius,
		int playerId,
		const Coords& velocity,
		float mass
	);

	~Reaper();

	void simulate() override;
	void checkForCollisionWith(Entity* entity) override;

private:
};

//*************************************************************************************************************
//*************************************************************************************************************

Reaper::Reaper() :
	Vehicle()
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Reaper::Reaper(
	int id,
	const Coords& position,
	int radius,
	int playerId,
	const Coords& velocity,
	float mass
) :
	Vehicle(id, position, radius, playerId, velocity, mass)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Reaper::~Reaper() {

}

//*************************************************************************************************************
//*************************************************************************************************************

void Reaper::simulate() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Reaper::checkForCollisionWith(Entity* entity) {
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class WaterContainer {
public:
	WaterContainer();

	WaterContainer(
		int waterQuantity
	);

	~WaterContainer();

	int getWaterQuantity() const {
		return waterQuantity;
	}

	void setWaterQuantity(int waterQuantity) { this->waterQuantity = waterQuantity; }

	bool empthy() const;

private:
	int waterQuantity;
};

//*************************************************************************************************************
//*************************************************************************************************************

WaterContainer::WaterContainer() {
}

//*************************************************************************************************************
//*************************************************************************************************************

WaterContainer::WaterContainer(int waterQuantity) : waterQuantity(waterQuantity)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

WaterContainer::~WaterContainer() {

}

//*************************************************************************************************************
//*************************************************************************************************************

bool WaterContainer::empthy() const {
	return 0 == waterQuantity;
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class Tanker : public Vehicle, public WaterContainer {
public:
	Tanker();

	Tanker(
		int id,
		const Coords& position,
		int radius,
		int playerId,
		const Coords& velocity,
		float mass,
		int waterQuantity,
		int waterCapacity
	);

	~Tanker();

	int getWaterCapacity() const {
		return waterCapacity;
	}

	void setWaterCapacity(int waterCapacity) { this->waterCapacity = waterCapacity; }

	void simulate() override;
	void checkForCollisionWith(Entity* entity) override;

	bool inCircleMap() const;

private:
	int waterCapacity;
};

typedef vector<Tanker*> Tankers;

//*************************************************************************************************************
//*************************************************************************************************************

Tanker::Tanker() :
	Vehicle(),
	WaterContainer()
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Tanker::Tanker(
	int id,
	const Coords& position,
	int radius,
	int playerId,
	const Coords& velocity,
	float mass,
	int waterQuantity,
	int waterCapacity
) :
	Vehicle(id, position, radius, playerId, velocity, mass),
	WaterContainer(waterQuantity),
	waterCapacity(waterCapacity)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Tanker::~Tanker() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Tanker::simulate() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Tanker::checkForCollisionWith(Entity* entity) {
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Tanker::inCircleMap() const {
	const Coords center(0, 0);

	int distanceTocenter = getPosition().distance(center);

	return distanceTocenter <= MAP_RADIUS;
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class Wreck : public Entity, public WaterContainer {
public:
	Wreck();
	Wreck(int id, const Coords& position, int radius, int waterQuantity);
	~Wreck();

	void simulate() override;
	void checkForCollisionWith(Entity* entity) override;

private:
};

typedef vector<Wreck*> Wrecks;

//*************************************************************************************************************
//*************************************************************************************************************

Wreck::Wreck() :
	Entity(),
	WaterContainer()
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Wreck::Wreck(int id, const Coords& position, int radius, int waterQuantity) :
	Entity(id, position, radius),
	WaterContainer(waterQuantity)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Wreck::~Wreck() {

}

//*************************************************************************************************************
//*************************************************************************************************************

void Wreck::simulate() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Wreck::checkForCollisionWith(Entity* entity) {
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class Destroyer : public Vehicle {
public:
	Destroyer();

	Destroyer(
		int id,
		const Coords& position,
		int radius,
		int playerId,
		const Coords& velocity,
		float mass,
		int targetId
	);

	~Destroyer();

	int getTargetId() const {
		return targetId;
	}

	void setTargetId(int targetId) { this->targetId = targetId; }

	void simulate() override;
	void checkForCollisionWith(Entity* entity) override;

private:
	int targetId;
};

//*************************************************************************************************************
//*************************************************************************************************************

Destroyer::Destroyer() :
	Vehicle(),
	targetId(INVALID_ID)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Destroyer::Destroyer(
	int id,
	const Coords& position,
	int radius,
	int playerId,
	const Coords& velocity,
	float mass,
	int targetId
) :
	Vehicle(id, position, radius, playerId, velocity, mass),
	targetId(targetId)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Destroyer::~Destroyer() {

}

//*************************************************************************************************************
//*************************************************************************************************************

void Destroyer::simulate() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Destroyer::checkForCollisionWith(Entity* entity) {
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class Team {
public:
	Team();
	Team(Reaper* reaper, Destroyer* destroyer);
	~Team();

	Reaper* getReaper() const {
		return reaper;
	}

	Destroyer* getDestroyer() const {
		return destroyer;
	}

	void setReaper(Reaper* reaper) { this->reaper = reaper; }
	void setDestroyer(Destroyer* destroyer) { this->destroyer = destroyer; }

	void init();

private:
	Reaper* reaper;
	Destroyer* destroyer;
};

//*************************************************************************************************************
//*************************************************************************************************************

Team::Team() :
	reaper(nullptr),
	destroyer(nullptr)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Team::Team(Reaper* reaper, Destroyer* destroyer) :
	reaper(reaper),
	destroyer(destroyer)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Team::~Team() {
	if (reaper) {
		delete reaper;
		reaper = nullptr;
	}

	if (destroyer) {
		delete destroyer;
		destroyer = nullptr;
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Team::init() {
	reaper = new Reaper();
	destroyer = new Destroyer();
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
	Coords findNearestTanker() const;

	Entity* updateVehicle(
		int playerId,
		int unitType,
		int unitId,
		const Coords& position,
		int radius,
		const Coords& velocity,
		float mass
	);

	void fillEntities();
	void clearTankersWrecksMemory();
	Tanker* getTanker(int tankerId) const;

private:
	int turnsCount;

	Team* myTeam;
	Team* enemyTeam1;
	Team* enemyTeam2;

	Wrecks* wrecks;
	Tankers* tankers;

	Entities entities;
};

//*************************************************************************************************************
//*************************************************************************************************************

Game::Game() :
	turnsCount(0),
	myTeam(nullptr),
	enemyTeam1(nullptr),
	enemyTeam2(nullptr),
	wrecks(nullptr),
	tankers(nullptr)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Game::~Game() {
	if (myTeam) {
		delete myTeam;
		myTeam = nullptr;
	}

	if (enemyTeam1) {
		delete enemyTeam1;
		enemyTeam1 = nullptr;
	}

	if (enemyTeam2) {
		delete enemyTeam2;
		enemyTeam2 = nullptr;
	}

	if (wrecks) {
		delete wrecks;
		wrecks = nullptr;
	}

	if (tankers) {
		delete tankers;
		tankers = nullptr;
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::initGame() {
	myTeam = new Team();
	myTeam->init();

	enemyTeam1 = new Team();
	enemyTeam1->init();

	enemyTeam2 = new Team();
	enemyTeam2->init();

	wrecks = new Wrecks;
	tankers = new Tankers;
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
		cerr << myScore << " " << enemyScore1 << " " << enemyScore2 << " " << myRage << " ";
		cerr << enemyRage1 << " " << enemyRage2 << " " << unitCount << " ";
	}

	for (int i = 0; i < unitCount; i++) {
		Entity* entity = nullptr;

		int unitId, unitType, player, radius, x, y, vx, vy, extra, extra2;
		float mass;
		cin >> unitId >> unitType >> player >> mass >> radius >> x >> y >> vx >> vy >> extra >> extra2; cin.ignore();

		if (OUTPUT_GAME_DATA) {
			cerr << unitId << " " << unitType << " " << player << " " << mass << " ";
			cerr << radius << " " << x << " " << y << " " << vx << " " << vy << " ";
			cerr << extra << " " << extra2 << endl;
		}

		Coords position(x, y);
		Coords velocity(vx, vy);

		if (UT_REAPER == unitType || UT_DESTROYER == unitType) {
			entity = updateVehicle(player, unitType, unitId, position, radius, velocity, mass);
		}
		else if (UT_TANKER == unitType) {
			Tanker* tanker = new Tanker(unitId, position, radius, player, velocity, mass, extra, extra2);
			tankers->push_back(tanker);
			entity = tanker;
		}
		else if (UT_WRECK == unitType) {
			Wreck* wreck = new Wreck (unitId, position, radius, extra);
			wrecks->push_back(wreck);
			entity = wreck;
		}

		entities[unitId] = entity;
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::turnBegin() {
	clearTankersWrecksMemory();
	entities.clear();
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::makeTurn() {
	Coords nearestWreck = findNearestWreck();
	Coords nearestTanker = findNearestTanker();

	nearestWreck.print();
	cout << " " << MAX_THROTTLE / 2 << endl;

	nearestTanker.print();
	cout << " " << MAX_THROTTLE << endl;

	cout << WAIT << endl;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::turnEnd() {
	++turnsCount;

	wrecks->clear();
	tankers->clear();
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
		if ((*it)->empthy()) {
			continue;
		}

		const Coords wreckPostion = (*it)->getPosition();
		const int distToMyReaper = wreckPostion.distance(myTeam->getReaper()->getPosition());

		if (distToMyReaper < minDist) {
			res = wreckPostion;
			minDist = distToMyReaper;
		}
	}

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords Game::findNearestTanker() const {
	Coords res;
	int minDist = INT_MAX;

	Destroyer* myDestroyer = myTeam->getDestroyer();
	int targetId = myDestroyer->getTargetId();

	if (INVALID_ID != targetId) {
		Tanker* target = getTanker(targetId);
		
		if (target) {
			res = target->getPosition();
		}
	}

	if (!res.isValid()) {
		for (Tankers::const_iterator it = tankers->begin(); it != tankers->end(); ++it) {
			if ((*it)->empthy() || !(*it)->inCircleMap()) {
				continue;
			}

			const Coords tankerPostion = (*it)->getPosition();
			const int distToMyDestroyer = tankerPostion.distance(myDestroyer->getPosition());

			if (distToMyDestroyer < minDist) {
				res = tankerPostion;
				minDist = distToMyDestroyer;
				myDestroyer->setTargetId((*it)->getId());
			}
		}
	}

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************

Entity* Game::updateVehicle(
	int playerId,
	int unitType,
	int unitId,
	const Coords& position,
	int radius,
	const Coords& velocity,
	float mass
) {
	Team* team = nullptr;
	Vehicle* vehicle = nullptr;

	switch (playerId) {
		case (PI_MY_PALYER): {
			team = myTeam;
			break;
		}
		case (PI_ENEMY_PLAYER_1): {
			team = enemyTeam1;
			break;
		}
		case (PI_ENEMY_PLAYER_2): {
			team = enemyTeam2;
			break;
		}
		default: {
			break;
		}
	}

	if (UT_REAPER == unitType) {
		vehicle = team->getReaper();
	}
	else if (UT_DESTROYER == unitType) {
		vehicle = team->getDestroyer();
	}

	vehicle->update(unitId, position, radius, playerId, velocity, mass);

	return vehicle;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::fillEntities() {

}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::clearTankersWrecksMemory() {
	for (size_t wreckIdx = 0; wreckIdx < tankers->size(); ++wreckIdx) {
		if (tankers->at(wreckIdx)) {
			delete tankers->at(wreckIdx);
			tankers->at(wreckIdx) = nullptr;
		}
	}

	for (size_t tankerIdx = 0; tankerIdx < tankers->size(); ++tankerIdx) {
		if (tankers->at(tankerIdx)) {
			delete tankers->at(tankerIdx);
			tankers->at(tankerIdx) = nullptr;
		}
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

Tanker* Game::getTanker(int tankerId) const {
	Tanker* tanker = nullptr;

	for (Tankers::iterator it = tankers->begin(); it != tankers->end(); ++it) {
		if ((*it)->getId() == tankerId) {
			tanker = *it;
			break;
		}
	}

	return tanker;
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

// seed=744360238