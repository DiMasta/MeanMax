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
#include <cfloat>

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
const int TEAMS_COUNT = 3;
const int VEHICLES_PER_TEAM = 2;
const int VEHICLES_COUNT = TEAMS_COUNT * VEHICLES_PER_TEAM;

const float INVALID_MASS = -1.f;
const float INVALID_FRICTION = -1.f;
const float REAPER_FRICTION = .2f;
const float DESTROYER_FRICTION = .3f;
const float TANKER_FRICTION = .4f;
const float TURN_INVALID_TIME = FLT_MAX;
const float TURN_START_TIME = 0.f;
const float TURN_END_TIME = 1.f;
const float EPSILON = 0.00001f;
const float MIN_IMPULSE = 30.f;
const float IMPULSE_COEFF = .5f;

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
	UI_MY_DOOF,
	UI_ENEMY_1_REAPER,
	UI_ENEMY_1_DESTROYER,
	UI_ENEMY_1_DOOF,
	UI_ENEMY_2_REAPER,
	UI_ENEMY_2_DESTROYER,
	UI_ENEMY_2_DOOF,
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
	UT_DOOF = 2,
	UT_TANKER = 3,
	UT_WRECK = 4,
	UT_TAR_POOL = 5,
	UT_OIL_POOL = 6
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
	Coords operator*(const Coords& other);
	Coords& operator*=(const Coords& other);
	Coords operator*(Coord numerical);
	Coords& operator*=(Coord numerical);

	bool isValid() const;
	void debug() const;
	void print() const;

	Coord distance(const Coords& coords) const;
	void roundCoords();

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

Coords Coords::operator*(const Coords& other) {
	return Coords(xCoord * other.xCoord, yCoord * other.yCoord);
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords& Coords::operator*=(const Coords& other) {
	xCoord *= other.xCoord;
	yCoord *= other.yCoord;

	return *this;
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords Coords::operator*(Coord numerical) {
	return Coords(xCoord * numerical, yCoord * numerical);
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords & Coords::operator*=(Coord numerical) {
	xCoord *= numerical;
	yCoord *= numerical;

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

void Coords::roundCoords() {
	xCoord = round(xCoord);
	yCoord = round(yCoord);
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

class Skill : public Entity {
public:
	Skill();

	Skill(
		int id,
		int unitType,
		const Coords& position,
		int radius,
		const Coords& velocity,
		int duration
	);

	~Skill();

	int getDuration() const {
		return duration;
	}

	void setDuration(int duration) { this->duration = duration; }

private:
	int duration;
};

//*************************************************************************************************************
//*************************************************************************************************************

Skill::Skill() :
	Entity(),
	duration(0)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Skill::Skill(
	int id,
	int unitType,
	const Coords& position,
	int radius,
	const Coords& velocity,
	int duration
) :
	Entity(id, unitType, position, radius, velocity),
	duration(duration)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Skill::~Skill() {

}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class TarPool : public Skill {
public:
	TarPool();

	TarPool(
		int id,
		int unitType,
		const Coords& position,
		int radius,
		const Coords& velocity,
		int duration
	);

	~TarPool();
private:

};

//*************************************************************************************************************
//*************************************************************************************************************

TarPool::TarPool() {

}

//*************************************************************************************************************
//*************************************************************************************************************

TarPool::TarPool(
	int id,
	int unitType,
	const Coords& position,
	int radius,
	const Coords& velocity,
	int duration
) :
	Skill(id, unitType, position, radius, velocity, duration)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

TarPool::~TarPool() {

}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class OilPool : public Skill {
public:
	OilPool();

	OilPool(
		int id,
		int unitType,
		const Coords& position,
		int radius,
		const Coords& velocity,
		int duration
	);

	~OilPool();
private:

};

//*************************************************************************************************************
//*************************************************************************************************************

OilPool::OilPool() {

}

//*************************************************************************************************************
//*************************************************************************************************************

OilPool::OilPool(
	int id,
	int unitType,
	const Coords& position,
	int radius,
	const Coords& velocity,
	int duration
) :
	Skill(id, unitType, position, radius, velocity, duration)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

OilPool::~OilPool() {

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
	
	void applyAcceleration(const Action& action);
	void move(float time);
	void bounce(Vehicle* vehicle);
	void applyFriction();
	void roundVectors();

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

void Vehicle::applyAcceleration(const Action& action) {
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

	setVelocity(Coords(vXFinal, vYFinal));
}

//*************************************************************************************************************
//*************************************************************************************************************

void Vehicle::move(float time) {
	Coords timeVelocity = getVelocity() * time;
	Coords newPosition = getPosition() + timeVelocity;
	setPosition(newPosition);
}

//*************************************************************************************************************
//*************************************************************************************************************

void Vehicle::bounce(Vehicle* vehicle) {
	float mcoeff = (mass + vehicle->getMass()) / (mass * vehicle->getMass());
	float nx = getPosition().getXCoord() - vehicle->getPosition().getXCoord();
	float ny = getPosition().getYCoord() - vehicle->getPosition().getYCoord();
	float nxnysquare = nx * nx + ny * ny;
	float dvx = getVelocity().getXCoord() - vehicle->getVelocity().getXCoord();
	float dvy = getVelocity().getYCoord() - vehicle->getVelocity().getYCoord();
	float product = (nx * dvx + ny * dvy) / (nxnysquare * mcoeff);
	float fx = nx * product;
	float fy = ny * product;
	float m1c = 1.f / mass;
	float m2c = 1.f / vehicle->getMass();

	Coords thisVelocity = getVelocity();
	Coords thisVelocityChange(fx * m1c, fy * m1c);
	setVelocity(thisVelocity - thisVelocityChange);

	Coords vehicleVelocity = getVelocity();
	Coords vehicleVelocityChange(fx * m1c, fy * m1c);
	setVelocity(vehicleVelocity + vehicleVelocityChange);

	fx = fx * IMPULSE_COEFF;
	fy = fy * IMPULSE_COEFF;

	// Normalize vector at min or max impulse
	float impulse = sqrt(fx * fx + fy * fy);
	float coeff = 1.f;
	if (impulse > EPSILON && impulse < MIN_IMPULSE) {
		coeff = MIN_IMPULSE / impulse;
	}

	fx = fx * coeff;
	fy = fy * coeff;

	thisVelocity = getVelocity();
	Coords thisVelocityChange1(fx * m1c, fy * m1c);
	setVelocity(thisVelocity - thisVelocityChange1);

	vehicleVelocity = getVelocity();
	Coords vehicleVelocityChange1(fx * m1c, fy * m1c);
	setVelocity(vehicleVelocity + vehicleVelocityChange1);
}

//*************************************************************************************************************
//*************************************************************************************************************

void Vehicle::applyFriction() {
	Coords frictionVec(1.f - friction, 1.f - friction);
	Coords newVelocity = getVelocity() * frictionVec;
	setVelocity(newVelocity);
}

//*************************************************************************************************************
//*************************************************************************************************************

void Vehicle::roundVectors() {
	Coords position = getPosition();
	position.roundCoords();
	setPosition(position);

	Coords velocity = getVelocity();
	velocity.roundCoords();
	setVelocity(velocity);
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
		float friction,
		int score
	);

	int getScore() const {
		return score;
	}

	void setScore(int score) { this->score = score; }

	~Reaper();

private:
	int score;
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
	float friction,
	int score
) :
	Vehicle(id, unitType, position, radius, playerId, velocity, mass, friction),
	score(score)
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

class Doof : public Vehicle {
public:
	Doof();

	Doof(
		int id,
		int unitType,
		const Coords& position,
		int radius,
		int playerId,
		const Coords& velocity,
		float mass,
		float friction
	);

	~Doof();
private:

};

//*************************************************************************************************************
//*************************************************************************************************************

Doof::Doof() : Vehicle() {

}

//*************************************************************************************************************
//*************************************************************************************************************

Doof::Doof(
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

Doof::~Doof() {

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

class Collision {
public:
	Collision();

	Collision(
		Entity* entityA,
		Entity* entityB,
		float time
	);

	~Collision();

	Entity* getEntityA() const {
		return entityA;
	}

	Entity* getEntityB() const {
		return entityB;
	}

	float getTime() const {
		return time;
	}

	void setEntityA(Entity* entityA) { this->entityA = entityA; }
	void setEntityB(Entity* entityB) { this->entityB = entityB; }
	void setTime(float time) { this->time = time; }

	bool isValid() const;
	void reset();
	bool compare(const Collision& collision);

private:
	Entity* entityA;
	Entity* entityB;
	float time;
};

//*************************************************************************************************************
//*************************************************************************************************************

Collision::Collision() :
	entityA(nullptr),
	entityB(nullptr),
	time(TURN_INVALID_TIME)
{
}

//*************************************************************************************************************
//*************************************************************************************************************

Collision::Collision(
	Entity* entityA,
	Entity* entityB,
	float time
) :
	entityA(entityA),
	entityB(entityB),
	time(time)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Collision::~Collision() {
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Collision::isValid() const {
	return entityA && entityB && (time != TURN_INVALID_TIME);
}

//*************************************************************************************************************
//*************************************************************************************************************

void Collision::reset() {
	entityA = nullptr;
	entityB = nullptr;
	time = TURN_INVALID_TIME;
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Collision::compare(const Collision& collision) {
	bool sameAEntities = entityA == collision.entityA;
	bool sameBEntities = entityB == collision.entityB;

	return sameAEntities && sameBEntities;
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
	) const;

	void simulateVehicles();
	void moveVehicles(float time);
	Collision checkForCollision(Entity* entityA, Entity* entityB) const;
	void applyVehiclesAccelerations();
	void simulationEnd();
	void getFirstCollision(
		Collision& firstCollision,
		Collision& previousCollision,
		float time
	) const;

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
	applyVehiclesAccelerations();
	simulateVehicles();
	simulationEnd();
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords State::closestPointToLine(
	const Coords& point,
	const Coords& linePoint1,
	const Coords& linePoint2
) const {
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

void State::simulateVehicles() {
	Collision firstCollision;
	Collision previousCollision;

	float time = TURN_START_TIME;
	while (time < TURN_END_TIME) {
		getFirstCollision(firstCollision, previousCollision, time);

		if (!firstCollision.isValid()) {
			moveVehicles(TURN_END_TIME - time);
			time = TURN_END_TIME;
		}
		else {
			float collisionTime = firstCollision.getTime();
			moveVehicles(collisionTime);

			Vehicle* vehicleA = dynamic_cast<Vehicle*>(firstCollision.getEntityA());
			Vehicle* vehicleB = dynamic_cast<Vehicle*>(firstCollision.getEntityB());
			vehicleA->bounce(vehicleB);

			time += collisionTime;
		}

		previousCollision = firstCollision;
		firstCollision.reset();
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void State::moveVehicles(float time) {
	for (size_t vehicleIdx = 0; vehicleIdx < VEHICLES_COUNT; ++vehicleIdx) {
		Vehicle* vehicle = dynamic_cast<Vehicle*>(entities[vehicleIdx]);
		vehicle->move(time);
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

Collision State::checkForCollision(Entity* entityA, Entity* entityB) const {
	Collision collision;

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

			collision.setEntityA(entityA);
			collision.setEntityB(entityB);
			collision.setTime(collisionTime);
		}
	}

	return collision;
}

//*************************************************************************************************************
//*************************************************************************************************************

void State::applyVehiclesAccelerations() {
	for (int entityIdx = 0; entityIdx < VEHICLES_COUNT; ++entityIdx) {
		Vehicle* vehicle = dynamic_cast<Vehicle*>(entities[entityIdx]);

		if (vehicle) {
			Action* action = &actions[entityIdx];
			vehicle->applyAcceleration(*action);
		}
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void State::simulationEnd() {
	for (int entityIdx = 0; entityIdx < VEHICLES_COUNT; ++entityIdx) {
		Vehicle* vehicle = dynamic_cast<Vehicle*>(entities[entityIdx]);

		if (vehicle) {
			vehicle->applyFriction();
			vehicle->roundVectors();
		}
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void State::getFirstCollision(
	Collision& firstCollision,
	Collision& previousCollision,
	float time
) const {
	for (size_t entityAIdx = 0; entityAIdx < entities.size(); ++entityAIdx) {
		for (size_t entityBIdx = entityAIdx + 1; entityBIdx < entities.size(); ++entityBIdx) {
			Collision collision = checkForCollision(entities.at(entityAIdx), entities.at(entityBIdx));

			if (collision.isValid() &&
				//TURN_START_TIME == collision.getTime() &&
				collision.compare(previousCollision)
				) {
				collision = Collision();
			}

			if (collision.isValid()) {
				bool collisionInTurn = (collision.getTime() + time) < TURN_END_TIME;
				bool collisionEarlierThanFirst = collision.getTime() < firstCollision.getTime();
				if (collisionInTurn && collisionEarlierThanFirst) {
					firstCollision = collision;
				}
			}
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

	Wreck* findNearestWreck() const;
	Coords findNearestTanker() const;
	Tanker* findFullestTanker() const;
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

		int score = 0;
		switch (player) {
			case (PI_MY_PALYER): {
				score = myScore;
				break;
			}
			case (PI_ENEMY_PLAYER_1): {
				score = enemyScore1;
				break;
			}
			case (PI_ENEMY_PLAYER_2): {
				score = enemyScore2;
				break;
			}
			default: {
				break;
			}
		}

		Coords position = Coords(Coord(x), Coord(y));
		Coords velocity = Coords(Coord(vx), Coord(vy));

		if (UT_REAPER == unitType) {
			entity = new Reaper(unitId, unitType, position, radius, player, velocity, mass, REAPER_FRICTION, score);
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
		else if (UT_DOOF == unitType) {
			entity = new Doof(unitId, unitType, position, radius, player, velocity, mass, DESTROYER_FRICTION);
		}
		else if (UT_TAR_POOL == unitType) {
			entity = new TarPool(unitId, unitType, position, radius, velocity, extra);
		}
		else if (UT_OIL_POOL == unitType) {
			entity = new OilPool(unitId, unitType, position, radius, velocity, extra);
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
	//simulateTurn(); // Test simulation

	Wreck* nearestWreck = findNearestWreck();
	Entity* myReaper = entities[0];
	Coords nearestWreckPos(-1, -1);
	if (nearestWreck) {
		nearestWreckPos = nearestWreck->getPosition();
		nearestWreckPos -= myReaper->getVelocity();
	}

	int throtlle = 300;
	//Coords myReaperPos = myReaper->getPosition();
	//if (myReaperPos.distance(nearestWreckPos) < 2000.f) {
	//	throtlle = 150;
	//}
	//else if (nearestWreck && (myReaperPos.distance(nearestWreckPos) < nearestWreck->getRadius())) {
	//	throtlle = 0;
	//}

	nearestWreckPos.print();
	cout << " " << throtlle << endl;

	//Coords nearestTanker = findNearestTanker();
	Tanker* tanker = findFullestTanker();

	//nearestTanker.print();
	if (tanker) {
		tanker->getPosition().print();
	}
	else {
		cout << "0 0";
	}
	cout << " " << MAX_THROTTLE << endl;

	Reaper* enemy1Reaper = dynamic_cast<Reaper*>(entities[UI_ENEMY_1_REAPER]);
	Reaper* enemy2Reaper = dynamic_cast<Reaper*>(entities[UI_ENEMY_2_REAPER]);
	Coords doofTarget = enemy1Reaper->getScore() > enemy2Reaper->getScore() ? enemy1Reaper->getPosition() : enemy2Reaper->getPosition();

	doofTarget.print();
	cout << " 300" << endl;
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

Wreck* Game::findNearestWreck() const {
	Wreck* res = nullptr;
	int minDist = INT_MAX;

	for (Entities::const_iterator it = entities.begin(); it != entities.end(); ++it) {
		Entity* entity = it->second;

		if (!entity) {
			continue;
		}

		if (UT_WRECK == entity->getUnitType()) {
			Wreck* wreck = dynamic_cast<Wreck*>(entity);

			if (wreck->empthy()) {
				continue;
			}

			const Coords wreckPostion = wreck->getPosition();

			Reaper* myReaper = dynamic_cast<Reaper*>(entities.at(UI_MY_REAPER));
			const int distToMyReaper = int(wreckPostion.distance(myReaper->getPosition()));

			if (distToMyReaper < minDist) {
				res = wreck;
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

			if (!entity) {
				continue;
			}

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

Tanker* Game::findFullestTanker() const {
	Tanker* res = nullptr;

	Destroyer* myDestroyer = dynamic_cast<Destroyer*>(entities.at(UI_MY_DESTROYER));
	//int targetId = myDestroyer->getTargetId();
	//
	//if (INVALID_ID != targetId) {
	//	Tanker* target = dynamic_cast<Tanker*>(entities.at(targetId));
	//
	//	if (target) {
	//		res = target;
	//	}
	//}

	if (!res) {
		int maxWater = INT_MIN;

		for (Entities::const_iterator it = entities.begin(); it != entities.end(); ++it) {
			Entity* entity = it->second;

			if (!entity) {
				continue;
			}

			if (UT_TANKER == entity->getUnitType()) {
				Tanker* tanker = dynamic_cast<Tanker*>(entity);

				if (tanker->empthy() || !tanker->inCircleMap()) {
					continue;
				}

				const int tankerWater = tanker->getWaterQuantity();

				if (tankerWater > maxWater) {
					res = tanker;
					maxWater = tankerWater;
					//myDestroyer->setTargetId(tanker->getId());
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
	else if (1 == turnsCount) {
		actions[0] = Action(Coords(-1, -1), 150);
		actions[1] = Action(Coords(-1, -1), 300);
		actions[2] = Action(Coords(0, 0), 200);
		actions[3] = Action(Coords(-7063, -3932), 200);
		actions[4] = Action(Coords(0, 0), 200);
		actions[5] = Action(Coords(6937, -4150), 200);
	}
	else if (2 == turnsCount) {
		actions[0] = Action(Coords(-1, -1), 150);
		actions[1] = Action(Coords(-1, -1), 300);
		actions[2] = Action(Coords(0, 0), 200);
		actions[3] = Action(Coords(-6620, -3685), 200);
		actions[4] = Action(Coords(0, 0), 200);
		actions[5] = Action(Coords(6502, -3890), 200);
	}
	else if (3 == turnsCount) {
		actions[0] = Action(Coords(-1, -1), 150);
		actions[1] = Action(Coords(-1, -1), 300);
		actions[2] = Action(Coords(0, 0), 200);
		actions[3] = Action(Coords(-6208, -3456), 200);
		actions[4] = Action(Coords(0, 0), 200);
		actions[5] = Action(Coords(6098, -3648), 200);
	}
	else if (4 == turnsCount) {
		actions[0] = Action(Coords(-1, -1), 150);
		actions[1] = Action(Coords(-1, -1), 300);
		actions[2] = Action(Coords(0, 0), 200);
		actions[3] = Action(Coords(-5815, -3238), 200);
		actions[4] = Action(Coords(0, 0), 200);
		actions[5] = Action(Coords(5713, -3417), 200);
	}
	else if (5 == turnsCount) {
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