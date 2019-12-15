#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>

struct Attack {
	std::string name;
	std::string type;
	int value;
};

class Item
{
public:
	Item(std::string name, std::string type);
	~Item();

	/*
	returns the name of the item EX: "Health Kit", "Rusty Sword"
	*/
	std::string getName();
	void setName(std::string name);

	/*
	returns the type EX: "health", "attack", "key"
	*/
	std::string getType();
	void setType(std::string type);

	/*
	returns item's int value EX: health kit value
	*/
	int getValue();
	void setValue(int value);

	/*
	Adds attacks to the attack vector
	*/
	void addToAttacks(std::string name, std::string type, int value);
	std::vector<struct Attack> getAttacks();
	void printAttackTypes();

	/*
	Item's description, can be multiple lines long.
	*/
	void addToDescription(std::string desc);
	std::vector<std::string> getDescription();
	void printDescription();

	void setUsable(bool usable);
	bool isUsable();

private:
	std::vector<std::string> description;
	std::string name;
	std::string type; //KEY, item, weapon...
	int value; //for health items

	std::vector<struct Attack> attacksVector;

	bool usable;
};

