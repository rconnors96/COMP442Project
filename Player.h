#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Item.h"
#include <map>
class Player

{
public:
	Player();
	Player(std::string name);
	~Player();

	std::string getName();
	void setName(std::string name);
	void addToInv(Item* item);
	void removeFromInv(Item* item);
	bool searchInv(Item* item);

	std::vector<Item*> getWeapons();
	void printWeapons();
	std::vector<Item*> getItems();
	void printItems();
	bool hasInventory();
	int numOfKeys();
	void useItem(Item* item);

	int getHealth();
	void setHealth(int health);
	void addHealth(int value);
	void subHealth(int value);

	int getBlockValue();
	void setBlockValue(int blockValue);

	bool isAlive();

	void printStatus();
	void setStatus(std::string statusName, int statusValue);
	void checkStatus();
	void resetStatus();

private:
	std::string name;
	std::vector<Item*> weapons;
	std::vector<Item*> items;
	int health;
	int blockValue;

	bool alive;

	struct status {
		std::string statusName;
		int statusValue;
		int statusTurns;
	};
	std::vector<struct status> statusVector;


};

