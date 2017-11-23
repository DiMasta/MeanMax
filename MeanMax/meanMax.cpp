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

const bool OUTPUT_GAME_DATA = 1;
const bool USE_HARDCODED_INPUT = 0;

//#define REDIRECT_CIN_FROM_FILE
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

typedef float Coord;
const Coord INVALID_COORD = -1.f;

class Coords {
public:
	Coords();
	Coords(Coord xCoord, Coord yCoord);
	Coords(const Coords& coords);

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
	Coords operator-(const Coords& other);
	Coords& operator-=(const Coords& other);

	bool isValid() const;
	void debug() const;
	void print() const;

	Coord distance(const Coords& coords) const;

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

Coords::Coords(const Coords& coords) :
	xCoord(coords.xCoord),
	yCoord(coords.yCoord)
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

Coords Coords::operator-(const Coords& other) {
	return Coords(xCoord - other.xCoord, yCoord - other.yCoord);
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords& Coords::operator-=(const Coords& other) {
	xCoord -= other.xCoord;
	yCoord -= other.yCoord;

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

Coord Coords::distance(const Coords& coords) const {
	Coord kat0 = coords.xCoord - xCoord;
	Coord kat1 = coords.yCoord - yCoord;

	Coord hip = (Coord)sqrt((kat0 * kat0) + (kat1 * kat1));
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

	Entity(
		int id,
		int unitType,
		const Coords& position,
		int radius,
		const Coords& velocity
	);

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

	Coords getVelocity() const {
		return velocity;
	}

	void setId(int id) { this->id = id; }
	void setUnitType(int unitType) { this->unitType = unitType; }
	void setPosition(const Coords& position) { this->position = position; }
	void setRadius(int radius) { this->radius = radius; }
	void setVelocity(const Coords& velocity) { this->velocity = velocity; }

private:
	int id;
	int unitType;
	Coords position;
	int radius;
	Coords velocity;
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

Entity::Entity(
	int id,
	int unitType,
	const Coords& position,
	int radius,
	const Coords& velocity
) :
	id(id),
	unitType(unitType),
	position(position),
	radius(radius),
	velocity(velocity)
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

	float getMass() const {
		return mass;
	}

	float getFriction() const {
		return friction;
	}

	void setPlayerId(int playerId) { this->playerId = playerId; }
	void setMass(float mass) { this->mass = mass; }
	void setFriction(float friction) { this->friction = friction; }
	
	void move(const Action& action);

private:
	int playerId;
	float mass;
	float friction;
};

//*************************************************************************************************************
//*************************************************************************************************************

Vehicle::Vehicle() :
	Entity(),
	playerId(INVALID_ID),
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
	Entity(id, unitType, position, radius, velocity),
	playerId(playerId),
	mass(mass),
	friction(friction)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

void Vehicle::move(const Action& action) {
	Coords accDir = action.getAccelerationDir();
	Coord xAccDir = accDir.getXCoord();
	Coord yAccDir = accDir.getYCoord();
	int throttle = action.getThrottle();

	float acceleration = float(throttle) / mass;

	Coord x = getPosition().getXCoord();
	Coord vXInitial = getVelocity().getXCoord();

	Coord y = getPosition().getYCoord();
	Coord vYInitial = getVelocity().getYCoord();

	float horizontalKatet = xAccDir - x;
	float verticalKatet = yAccDir - y;

	float hypotenose = sqrtf((horizontalKatet * horizontalKatet) + (verticalKatet * verticalKatet));

	float cosAlpha = horizontalKatet / hypotenose;
	float sinAlpha = verticalKatet / hypotenose;

	float xAcc = cosAlpha * acceleration;
	float yAcc = sinAlpha * acceleration;

	float vXFinal = vXInitial + xAcc;
	float vYFinal = vYInitial + yAcc;

	float vXFriction = vXFinal * (1.f - friction);
	float vYFriction = vYFinal * (1.f - friction);

	float roundVXFriction = round(vXFriction);
	float roundVYFriction = round(vYFriction);

	setVelocity(Coords(roundVXFriction, roundVYFriction));

	float xFinal = x + vXFinal;
	float yFinal = y + vYFinal;

	float roundXFinal = round(xFinal);
	float roundYFinal = round(yFinal);

	setPosition(Coords(roundXFinal, roundYFinal));
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

bool Tanker::inCircleMap() const {
	const Coords center(0, 0);

	Coord distanceTocenter = getPosition().distance(center);

	return distanceTocenter <= float(MAP_RADIUS);
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
		int waterQuantity,
		const Coords& velocity
	);

	~Wreck();

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
	int waterQuantity,
	const Coords& velocity
) :
	Entity(id, unitType, position, radius, velocity),
	WaterContainer(waterQuantity)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Wreck::~Wreck() {

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

	Coords closestPointToLine(
		const Coords& point,
		const Coords& linePoint1,
		const Coords& linePoint2
	);

	void checkForCollision(Entity* entityA, Entity* entityB);

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
	//Reaper* reaperA = new Reaper();
	//reaperA->setPosition(Coords(2.f, 4.f));
	//reaperA->setVelocity(Coords(5.f, 4.f));
	//reaperA->setRadius(1);
	//
	//Reaper* reaperB = new Reaper();
	//reaperB->setPosition(Coords(4.f, 2.f));
	//reaperB->setVelocity(Coords(4.f, 5.f));
	//reaperB->setRadius(1);
	//
	//checkForCollision(reaperA, reaperB);

	for (size_t entityAIdx = 0; entityAIdx < entities.size(); ++entityAIdx) {
		for (size_t entityBIdx = entityAIdx + 1; entityBIdx < entities.size(); ++entityBIdx) {
			checkForCollision(entities[entityAIdx], entities[entityBIdx]);
		}
	}

	for (int vehicleIdx = 0; vehicleIdx < VEHICLES_COUNT; ++vehicleIdx) {
		Vehicle* vehicle = dynamic_cast<Vehicle*>(entities[vehicleIdx]);
		vehicle->move(actions[vehicleIdx]);
	}

	// Move Tankers
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords State::closestPointToLine(
	const Coords& point,
	const Coords& linePoint1,
	const Coords& linePoint2
) {
	float da = linePoint2.getYCoord() - linePoint1.getYCoord();
	float db = linePoint1.getXCoord() - linePoint2.getXCoord();
	float c1 = da * linePoint1.getXCoord() + db * linePoint1.getYCoord();
	float c2 = -db * point.getXCoord() + da * point.getYCoord();
	float det = da * da + db * db;

	Coords clossestPoint;

	if (det != 0) {
		clossestPoint.setXCoord((da * c1 - db * c2) / det);
		clossestPoint.setYCoord((da * c2 + db * c1) / det);
	}
	else {
		// The point is already on the line
		clossestPoint.setXCoord(point.getXCoord());
		clossestPoint.setYCoord(point.getYCoord());
	}

	return clossestPoint;
}

//*************************************************************************************************************
//*************************************************************************************************************

void State::checkForCollision(Entity* entityA, Entity* entityB) {
	float radiusSum = float(entityA->getRadius() + entityB->getRadius());

	// B will be used as center of the coordinate system
	Coords newAPosition = entityA->getPosition() - entityB->getPosition();
	Coords newAVelocity = entityA->getVelocity() - entityB->getVelocity();

	Coords positionAfterVelocity = newAPosition + newAVelocity;

	Coords origin(0.f, 0.f);
	Coords pointFromOriginToLine = closestPointToLine(origin, newAPosition, positionAfterVelocity);

	float distFromOriginToLinePoint = origin.distance(pointFromOriginToLine);
	float distFromAToLinePoint = newAPosition.distance(pointFromOriginToLine);

	if (distFromOriginToLinePoint < radiusSum) {
		float vX = newAVelocity.getXCoord();
		float vY = newAVelocity.getYCoord();
		float lenTraveledOnLinefromA = sqrt((vX * vX) + (vY * vY));

		float radiusSumMinusDistToLine = radiusSum - distFromOriginToLinePoint;
		Coord moveAlongTheLineX = radiusSumMinusDistToLine * vX / lenTraveledOnLinefromA;
		Coord moveAlongTheLineY = radiusSumMinusDistToLine * vY / lenTraveledOnLinefromA;
		Coords moveAlongTheLine(moveAlongTheLineX, moveAlongTheLineY);

		Coords moved = pointFromOriginToLine - moveAlongTheLine;
		float distFromAToMoved = newAPosition.distance(moved);

		if (distFromAToMoved > distFromAToLinePoint) {
			// If the point is now further away it means we are not going the right way, therefore the collision won't happen
			int noCollision = 0;
			++noCollision;
		}
		else if (distFromAToMoved > lenTraveledOnLinefromA) {
			// The point of impact is further than what we can travel in one turn
			int noCollision = 0;
			++noCollision;
		}
		else {
			// Time needed to reach the impact point
			float collisionTime = distFromAToMoved / lenTraveledOnLinefromA;

			int collision = 0;
			++collision;
		}
	}
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

		Coords position = Coords(Coord(x), Coord(y));
		Coords velocity = Coords(Coord(vx), Coord(vy));

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
			entity = new Wreck(unitId, unitType, position, radius, extra, Coords(0.f, 0.f));
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

	//if (0 == turnsCount) {
		Coords nearestWreck = findNearestWreck();
		Coords nearestTanker = findNearestTanker();

		nearestWreck.print();
		cout << " " << MAX_THROTTLE / 2 << endl;

		nearestTanker.print();
		cout << " " << MAX_THROTTLE << endl;
	//}
	//else if (1 == turnsCount){
	//	cout << "5000 5000 150" << endl;
	//	cout << "5000 5000 300" << endl;
	//}
	//else {
	//	cout << "-3897 -10 78" << endl;
	//	cout << "90 -6776 265" << endl;
	//}

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
			const int distToMyReaper = int(wreckPostion.distance(myReaper->getPosition()));

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
				const int distToMyDestroyer = int(tankerPostion.distance(myDestroyer->getPosition()));

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
		actions[2] = Action(Coords(0, 0), 200);
		actions[3] = Action(Coords(-7558, -4208), 200);
		actions[4] = Action(Coords(0, 0), 200);
		actions[5] = Action(Coords(7423, -4441), 200);
	}
	else if (2 == turnsCount) {
		actions[0] = Action(Coords(-1, -1), 150);
		actions[1] = Action(Coords(-1, -1), 300);
		actions[2] = Action(Coords(0, 0), 200);
		actions[3] = Action(Coords(-7063, -3932), 200);
		actions[4] = Action(Coords(0, 0), 200);
		actions[5] = Action(Coords(6937, -4150), 200);
	}
	else if (3 == turnsCount) {
		actions[0] = Action(Coords(-1, -1), 150);
		actions[1] = Action(Coords(-1, -1), 300);
		actions[2] = Action(Coords(0, 0), 200);
		actions[3] = Action(Coords(-6620, -3685), 200);
		actions[4] = Action(Coords(0, 0), 200);
		actions[5] = Action(Coords(6502, -3890), 200);
	}
	else if (4 == turnsCount) {
		actions[0] = Action(Coords(-1, - 1), 150);
		actions[1] = Action(Coords(-1, - 1), 300);
		actions[2] = Action(Coords(0, 0), 200);
		actions[3] = Action(Coords(-6208, - 3456), 200);
		actions[4] = Action(Coords(0, 0), 200);
		actions[5] = Action(Coords(6098, - 3648), 200);
	}
	else if (5 == turnsCount) {
		actions[0] = Action(Coords(-1, - 1), 150);
		actions[1] = Action(Coords(-1, - 1), 300);
		actions[2] = Action(Coords(0, 0), 200);
		actions[3] = Action(Coords(-5815, - 3238), 200);
		actions[4] = Action(Coords(0, 0), 200);
		actions[5] = Action(Coords(5713, - 3417), 200);
	}
	else if (6 == turnsCount) {
		actions[0] = Action(Coords(-1, -1), 150);
		actions[1] = Action(Coords(-1, -1), 300);
		actions[2] = Action(Coords(0, 0), 200);
		actions[3] = Action(Coords(-5433, -3026), 200);
		actions[4] = Action(Coords(0, 0), 200);
		actions[5] = Action(Coords(5339, -3193), 200);
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