#include "Room.h"

Room::Room(std::string name)
{
	this->name = name;
	enemyBool = false;
	enemy = nullptr;
	door = false;
}

Room::~Room()
{
}

/*
RETURNS THE NAME OF THE ROOM
*/
std::string Room::getName()
{
	return name;
}

/*
SETS THE NAME OF THE ROOM
*/
void Room::setName(std::string name)
{
	this->name = name;
}

/*
RETURNS THE ROOM DESCRIPTION VECTOR
*/
std::vector<std::string> Room::getDescription()
{
	return description;
}

/*
ADDS A STRING TO THE ROOM DESCRIPTION VECTOR
*/
void Room::addToDescription(std::string description)
{
	this->description.push_back(description);
}

/*
PRINTS ROOM DESCRIPTION VECTOR
*/
void Room::printDescription()
{
	for (std::string desc : this->description) {
		std::cout << desc << std::endl;
	}
}

/*
SETS DIRECTIONS OF OTHER ROOMS
*/
void Room::setRooms(Room * north, Room * south, Room * east, Room * west)
{
	this->north = north;
	this->south = south;
	this->east = east;
	this->west = west;
}

Room * Room::getRoom(std::string direction)
{
	std::transform(direction.begin(), direction.end(), direction.begin(), ::tolower); //changes direction string to lowercase, hopefully

	if (direction == "north" && this->north != nullptr) {
		return this->north;
	}
	else if (direction == "south" && this->south != nullptr) {
		return this->south;
	}
	else if (direction == "east" && this->east != nullptr) {
		return this->east;
	}
	else if (direction == "west" && this->west != nullptr) {
		return this->west;
	}
	else {
		return this;
	}
}

void Room::addToItems(Item* item)
{
	itemsVector.push_back(item);
}

std::vector<Item*> Room::getItems()
{
	return itemsVector;
}

bool Room::hasItems()
{
	if (itemsVector.size() == 0) {
		return false;
	}
	else {
		return true;
	}
}

void Room::removeItem(Item* item)
{
	std::vector<Item*> temp;
	for (Item* it : itemsVector) {
		if (it != item) {
			temp.push_back(it);
		}
	}
	itemsVector = temp;
}

bool Room::isLocked()
{
	return locked;
}

void Room::setLockStatus(bool status)
{
	this->locked = status;
}

bool Room::hasEnemy()
{
	return this->enemyBool;
}

Enemy* Room::getEnemy()
{
	return this->enemy;
}

void Room::removeEnemy()
{
	this->enemy = nullptr;
	this->enemyBool = false;
}

void Room::setEnemy(Enemy* enemy)
{
	this->enemy = enemy;
	this->enemyBool = true;
}

bool Room::hasDoor()
{
	return door;
}

void Room::setDoor(bool door)
{
	this->door = door;
}
