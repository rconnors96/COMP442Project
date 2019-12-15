#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::Enemy(std::string name, int health)
{
	this->name = name;
	this->health = health;
	alive = true;
	blockValue = 0;
	item = nullptr;
}

Enemy::~Enemy()
{
}

void Enemy::addToAttacks(std::string name, std::string type, int value)
{
	struct Attack tempAttack;
	tempAttack.name = name;
	tempAttack.type = type;
	tempAttack.value = value;
	attacksVector.push_back(&tempAttack);
}

std::vector<struct Attack*> Enemy::getAttacks()
{
	return attacksVector;
}

void Enemy::printWarning()
{
	for (std::string warn : warning) {
		std::cout << warn << std::endl;
	}
}

void Enemy::setWarning(std::string warning)
{
	this->warning.push_back(warning);
}

Item * Enemy::getItem()
{
	return this->item;
}

void Enemy::setItem(Item * item)
{
	this->item = item;
}

bool Enemy::hasItem()
{
	if (item == nullptr) {
		return false;
	}
	else {
		return true;
	}
}

void Enemy::attacked(struct Attack attack)
{
	subHealth(attack.value);
}


