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
#include <cstring>

using namespace std;

const bool OUTPUT_GAME_DATA = 0;
const bool USE_HARDCODED_INPUT = 0;

#define REDIRECT_CIN_FROM_FILE
const string INPUT_FILE_NAME = "input.txt";

const int INVALID_ID = -1;
const int INVALID_NODE_DEPTH = -1;
const int TREE_ROOT_NODE_DEPTH = 1;
const int ZERO_CHAR = '0';
const int DIRECTIONS_COUNT = 8;
const int TEAMS_COUNT = 3;
const int VEHICLES_PER_TEAM = 2;
const int VEHICLES_COUNT = TEAMS_COUNT * VEHICLES_PER_TEAM;

const float INVALID_MASS = -1.f;
const float INVALID_FRICTION = -1.f;
const float REAPER_FRICTION = .2f;
const float DESTROYER_FRICTION = .3f;
const float TANKER_FRICTION = .4f;

const int INVALID_RADIUS = 0;
const int INVALID_WATER_QUANTITY = -1;
const int INVALID_THROTTLE = -1;
const int MIN_THROTTLE = 0;
const int MAX_THROTTLE = 300;
const int MAP_RADIUS = 6000;

const string WAIT = "WAIT";

enum UnitId {
	UI_INVALID = -1,
	UI_MY_REAPER = 0,
	UI_MY_DESTROYER,
	UI_ENEMY_1_REAPER,
	UI_ENEMY_1_DESTROYER,
	UI_ENEMY_2_REAPER,
	UI_ENEMY_2_DESTROYER,
};

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

class Action {
public:
	Action();
	Action(Coords accelerationDir, int throttle);
	~Action();

	Coords getAccelerationDir() const {
		return accelerationDir;
	}

	int getThrottle() const {
		return throttle;
	}

	void setAccelerationDir(Coords accelerationDir) { this->accelerationDir = accelerationDir; }
	void setThrottle(int throttle) { this->throttle = throttle; }

private:
	Coords accelerationDir;
	int throttle;
};

//*************************************************************************************************************
//*************************************************************************************************************

Action::Action() :
	accelerationDir(),
	throttle(INVALID_THROTTLE)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Action::Action(Coords accelerationDir, int throttle) :
	accelerationDir(accelerationDir),
	throttle(throttle)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Action::~Action() {

}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class Entity {
public:
	Entity();
	Entity(int id, int unitType, const Coords& position, int radius);
	virtual ~Entity();

	int getId() const {
		return id;
	}

	int getUnitType() const {
		return unitType;
	}

	Coords getPosition() const {
		return position;
	}

	int getRadius() const {
		return radius;
	}

	void setId(int id) { this->id = id; }
	void setUnitType(int unitType) { this->unitType = unitType; }
	void setPosition(const Coords& position) { this->position = position; }
	void setRadius(int radius) { this->radius = radius; }

	virtual void simulate() = 0;
	virtual void checkForCollisionWith(Entity* entity) = 0;

private:
	int id;
	int unitType;
	Coords position;
	int radius;
};

typedef map<int, Entity*> Entities;

//*************************************************************************************************************
//*************************************************************************************************************

Entity::Entity() :
	id(UI_INVALID),
	unitType(UT_INVALID),
	position(),
	radius(INVALID_RADIUS)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Entity::Entity(int id, int unitType, const Coords& position, int radius) :
	id(id),
	unitType(unitType),
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
		int unitType,
		const Coords& position,
		int radius,
		int playerId,
		const Coords& velocity,
		float mass,
		float friction
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

	float getFriction() const {
		return friction;
	}

	void setPlayerId(int playerId) { this->playerId = playerId; }
	void setVelocity(const Coords& velocity) { this->velocity = velocity; }
	void setMass(float mass) { this->mass = mass; }
	void setFriction(float friction) { this->friction = friction; }
	
	void move(const Action& action);

private:
	int playerId;
	Coords velocity;
	float mass;
	float friction;
};

//*************************************************************************************************************
//*************************************************************************************************************

Vehicle::Vehicle() :
	Entity(),
	playerId(INVALID_ID),
	velocity(),
	mass(INVALID_MASS),
	friction(INVALID_FRICTION)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Vehicle::Vehicle(
	int id,
	int unitType,
	const Coords& position,
	int radius,
	int playerId,
	const Coords& velocity,
	float mass,
	float friction
) :
	Entity(id, unitType, position, radius),
	playerId(playerId),
	velocity(velocity),
	mass(mass),
	friction(friction)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

void Vehicle::move(const Action& action) {
	Coords accDir = action.getAccelerationDir();
	int xAccDir = accDir.getXCoord();
	int yAccDir = accDir.getYCoord();
	int throttle = action.getThrottle();

	float acceleration = float(throttle) / mass;

	int x = getPosition().getXCoord();
	int vXInitial = getVelocity().getXCoord();

	int y = getPosition().getYCoord();
	int vYInitial = getVelocity().getYCoord();

	float horizontalKatet = float(xAccDir - x);
	float verticalKatet = float(yAccDir - y);

	float hypotenose = sqrtf((horizontalKatet * horizontalKatet) + (verticalKatet * verticalKatet));

	float cosAlpha = horizontalKatet / hypotenose;
	float sinAlpha = verticalKatet / hypotenose;

	float xAcc = cosAlpha * acceleration;
	float yAcc = sinAlpha * acceleration;

	float vXFinal = float(vXInitial) + xAcc;
	float vYFinal = float(vYInitial) + yAcc;

	float vXFriction = vXFinal * (1.f - friction);
	float vYFriction = vYFinal * (1.f - friction);

	float roundVXFriction = round(vXFriction);
	float roundVYFriction = round(vYFriction);

	setVelocity(Coords(int(roundVXFriction), int(roundVYFriction)));

	float xFinal = x + vXFinal;
	float yFinal = y + vYFinal;

	float roundXFinal = round(xFinal);
	float roundYFinal = round(yFinal);

	setPosition(Coords(int(roundXFinal), int(roundYFinal)));
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
		int unitType,
		const Coords& position,
		int radius,
		int playerId,
		const Coords& velocity,
		float mass,
		float friction
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
	int unitType,
	const Coords& position,
	int radius,
	int playerId,
	const Coords& velocity,
	float mass,
	float friction
) :
	Vehicle(id, unitType, position, radius, playerId, velocity, mass, friction)
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
		int unitType,
		const Coords& position,
		int radius,
		int playerId,
		const Coords& velocity,
		float mass,
		float friction,
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
	int unitType,
	const Coords& position,
	int radius,
	int playerId,
	const Coords& velocity,
	float mass,
	float friction,
	int waterQuantity,
	int waterCapacity
) :
	Vehicle(id, unitType, position, radius, playerId, velocity, mass, friction),
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

	Wreck(
		int id,
		int unitType,
		const Coords& position,
		int radius,
		int waterQuantity
	);

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

Wreck::Wreck(
	int id,
	int unitType,
	const Coords& position,
	int radius,
	int waterQuantity
) :
	Entity(id, unitType, position, radius),
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
		int unitType,
		const Coords& position,
		int radius,
		int playerId,
		const Coords& velocity,
		float mass,
		float friction,
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
	int unitType,
	const Coords& position,
	int radius,
	int playerId,
	const Coords& velocity,
	float mass,
	float friction,
	int targetId
) :
	Vehicle(id, unitType, position, radius, playerId, velocity, mass, friction),
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

class State {
public:
	State();
	State(Entities entities, Action actions[VEHICLES_COUNT]);
	~State();

	void simulate();

private:
	Entities entities;
	Action actions[VEHICLES_COUNT];
};

//*************************************************************************************************************
//*************************************************************************************************************

State::State() :
	entities(),
	actions()
{

}

//*************************************************************************************************************
//*************************************************************************************************************

State::State(Entities entities, Action actions[VEHICLES_COUNT]) :
	entities(entities)
{
	memcpy(this->actions, actions, VEHICLES_COUNT * sizeof(Action));
}

//*************************************************************************************************************
//*************************************************************************************************************

State::~State() {

}

//*************************************************************************************************************
//*************************************************************************************************************

void State::simulate() {
	for (int vehicleIdx = 0; vehicleIdx < VEHICLES_COUNT; ++vehicleIdx) {
		Vehicle* vehicle = dynamic_cast<Vehicle*>(entities[vehicleIdx]);
		vehicle->move(actions[vehicleIdx]);
	}

	// Move Tankers
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
	void gameDataClear();

	void simulateTurn();

private:
	int turnsCount;

	Entities entities;
};

//*************************************************************************************************************
//*************************************************************************************************************

Game::Game() :
	turnsCount(0),
	entities()
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Game::~Game() {
	gameDataClear();
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::initGame() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::gameBegin() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::gameLoop() {
	while (true) {
		turnBegin();
		getTurnInput();
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
		cerr << enemyRage1 << " " << enemyRage2 << " " << unitCount << " " << endl;;
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

		if (UT_REAPER == unitType) {
			entity = new Reaper(unitId, unitType, position, radius, player, velocity, mass, REAPER_FRICTION);
		}
		else if (UT_DESTROYER == unitType) {
			entity = new Destroyer(unitId, unitType, position, radius, player, velocity, mass, DESTROYER_FRICTION, INVALID_ID);
		}
		else if (UT_TANKER == unitType) {
			entity = new Tanker(unitId, unitType, position, radius, player, velocity, mass, TANKER_FRICTION, extra, extra2);
		}
		else if (UT_WRECK == unitType) {
			entity = new Wreck(unitId, unitType, position, radius, extra);
		}

		entities[unitId] = entity;
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::turnBegin() {
	gameDataClear();
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::makeTurn() {
	simulateTurn(); // Test simulation

	if (0 == turnsCount) {
		Coords nearestWreck = findNearestWreck();
		Coords nearestTanker = findNearestTanker();

		nearestWreck.print();
		cout << " " << MAX_THROTTLE / 2 << endl;

		nearestTanker.print();
		cout << " " << MAX_THROTTLE << endl;
	}
	else if (1 == turnsCount){
		cout << "5000 5000 150" << endl;
		cout << "5000 5000 300" << endl;
	}
	else {
		cout << "-3897 -10 78" << endl;
		cout << "90 -6776 265" << endl;
	}

	cout << WAIT << endl;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::turnEnd() {
	++turnsCount;
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

	for (Entities::const_iterator it = entities.begin(); it != entities.end(); ++it) {
		Entity* entity = it->second;

		if (UT_WRECK == entity->getUnitType()) {
			Wreck* wreck = dynamic_cast<Wreck*>(entity);

			if (wreck->empthy()) {
				continue;
			}

			const Coords wreckPostion = wreck->getPosition();

			Reaper* myReaper = dynamic_cast<Reaper*>(entities.at(UI_MY_REAPER));
			const int distToMyReaper = wreckPostion.distance(myReaper->getPosition());

			if (distToMyReaper < minDist) {
				res = wreckPostion;
				minDist = distToMyReaper;
			}
		}
	}

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords Game::findNearestTanker() const {
	Coords res;

	Destroyer* myDestroyer = dynamic_cast<Destroyer*>(entities.at(UI_MY_DESTROYER));
	int targetId = myDestroyer->getTargetId();

	if (INVALID_ID != targetId) {
		Tanker* target = dynamic_cast<Tanker*>(entities.at(targetId));
		
		if (target) {
			res = target->getPosition();
		}
	}

	if (!res.isValid()) {
		int minDist = INT_MAX;

		for (Entities::const_iterator it = entities.begin(); it != entities.end(); ++it) {
			Entity* entity = it->second;

			if (UT_TANKER == entity->getUnitType()) {
				Tanker* tanker = dynamic_cast<Tanker*>(entity);

				if (tanker->empthy() || !tanker->inCircleMap()) {
					continue;
				}

				const Coords tankerPostion = tanker->getPosition();
				const int distToMyDestroyer = tankerPostion.distance(myDestroyer->getPosition());

				if (distToMyDestroyer < minDist) {
					res = tankerPostion;
					minDist = distToMyDestroyer;
					myDestroyer->setTargetId(tanker->getId());
				}
			}
		}
	}

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::gameDataClear() {
	for (Entities::iterator it = entities.begin(); it != entities.end();) {
		if (it->second) {
			delete it->second;
			it->second = nullptr;

			it = entities.erase(it);
		}
	}

	entities.clear();
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::simulateTurn() {
	Action actions[VEHICLES_COUNT];

	if (0 == turnsCount) {
		actions[0] = Action(Coords(-1, -1), 150);
		actions[1] = Action(Coords(-1, -1), 300);
		actions[2] = Action(Coords(-7558, -4208), 250);
		actions[3] = Action(Coords(-7558, -4208), 300);
		actions[4] = Action(Coords(7423, -4441), 250);
		actions[5] = Action(Coords(7423, -4441), 300);
	}
	else if (1 == turnsCount) {
		actions[0] = Action(Coords(5000, 5000), 150);
		actions[1] = Action(Coords(5000, 5000), 300);
		actions[2] = Action(Coords(-7063, -3932), 250);
		actions[3] = Action(Coords(-7063, -3932), 300);
		actions[4] = Action(Coords(6937, -4150), 250);
		actions[5] = Action(Coords(6937, -4150), 300);
	}
	else {
		actions[0] = Action(Coords(-3897, -10), 78);
		actions[1] = Action(Coords(90, -6776), 265);
		actions[2] = Action(Coords(-6620, -3685), 250);
		actions[3] = Action(Coords(-6620, -3685), 300);
		actions[4] = Action(Coords(6502, -3890), 250);
		actions[5] = Action(Coords(6502, -3890), 300);
	}

	State state(entities, actions);
	state.simulate();
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
// Di_Masta, it's
// velocity += acceleration;
// position += velocity;
// velocity *= friction;