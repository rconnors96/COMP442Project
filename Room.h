#pragma once
#include <string>
#include <algorithm>
#include <vector>
#include "Item.h"
#include <iostream>
#include "Enemy.h"

class Room
{
public:
	Room(std::string name);
	~Room();

	std::string getName();
	void setName(std::string name);

	std::vector<std::string> getDescription();
	void addToDescription(std::string description);
	void printDescription();

	void setRooms(Room* north, Room* south, Room* east, Room* west);
	Room* getRoom(std::string direction);

	std::vector<Item*> getItems();
	void addToItems(Item* item);
	bool hasItems();
	void removeItem(Item* item);

	bool isLocked();
	void setLockStatus(bool status);

	bool hasEnemy();
	Enemy* getEnemy();
	void removeEnemy();
	void setEnemy(Enemy* enemy);

	bool hasDoor();
	void setDoor(bool door);

private:
	bool enemyBool;
	bool door;
	bool locked;
	std::string name;
	std::vector<std::string> description;
	std::vector<Item*> itemsVector;

	Enemy* enemy;

	Room* north;
	Room* south;
	Room* east;
	Room* west;


};

