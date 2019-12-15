#include "Player.h"
#include "Room.h"
#include "Enemy.h"
#include "Item.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <string>
#include <map>
#include <exception>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

std::pair<Player*, Enemy*> doCombat(Player* player, Enemy* enemy) {
	system("CLS");

	std::vector<struct Attack*> enemyAttacks = enemy->getAttacks();
	struct Attack* enemyAttack;
	struct Attack userAttack;
	std::vector<Item*> playerWeapons = player->getWeapons();
	std::vector<Item*> playerItems = player->getItems();
	Item* userWeapon = nullptr;
	std::string userIn;


	/*
	These are used for loop conditionals/input/iteration
	*/
	bool run = false;
	bool moveSelect = true;

	player->resetStatus();
	enemy->resetStatus();


	while (player->isAlive() && enemy->isAlive()) {

		bool goBack = false;//used to restart turn in a fight


		/*
		Shows choices to the player
		*/
		std::cout << player->getName() << "'s Health: " << player->getHealth() << std::endl;
		std::cout << enemy->getName() << "'s Health: " << enemy->getHealth() << std::endl;
		std::cout << "Fight!" << std::endl;
		std::cout << "1) Attacks" << std::endl;
		std::cout << "2) Items" << std::endl;
		std::cout << "3) Run" << std::endl;
		std::getline(std::cin, userIn);
		system("CLS");

		std::transform(userIn.begin(), userIn.end(), userIn.begin(), ::tolower); //sets input to lowercase, wont matter for numbers. EX: User can enter "1" OR "ATTACKS" for the first option

		/*
		ATTACK OPTION - SHOWS ALL OPTIONS FOR ATTACKS
		*/
		if (userIn == "1" || userIn == "attacks") {
			if (playerWeapons.size() > 0) {
				run = false;

				/*
				Selects an Attack for the enemy to use
				*/
				if (moveSelect) {
					enemyAttack = enemyAttacks[0];
					moveSelect = !moveSelect;
				}
				else {
					enemyAttack = enemyAttacks[1];
					moveSelect = !moveSelect;
				}

				/*
				This ensures the user is entering a number and that the number is in range
				*/
				while (!run) {
					system("CLS");
					/*
					Prints all of the player's "weapons" (includes melee, ranged, and magic)
					*/
					player->printWeapons();

					std::cout << std::endl;
					std::cout << "Select a weapon! You must enter a number." << std::endl;
					std::cout << "All weapons have multiple attack types." << std::endl;
					std::cout << "Enter 0 or any letter to go back!" << std::endl;
					int userIn;
					std::cin >> userIn;
					while (!std::cin) {
						std::cin.clear();
						std::cin.ignore();
						continue;
					}
					if (userIn == 0) {
						system("CLS");
						goBack = true;
						break;
					}
					if ((unsigned int)userIn <= playerWeapons.size() && userIn > 0) {
						run = true;
						/*
						creates an Item* for the user's choice of weapon
						The -1 is so player can see list of weapons, when they pick first(1) weapon,
						they are actually picking the first(0) index of the weapons vector
						*/
						userWeapon = playerWeapons[userIn - 1];
						system("CLS");
					}
				}//End of while loop for weapon input

				/*
				This ensures the user is entering a number and that the number is in range
				*/
				run = false;
				while (!run && !goBack) {
					system("CLS");
					/*
					Displays the types of moves available for that weapon ("Stab", "Block", etc)
					*/
					userWeapon->printAttackTypes();

					std::cout << std::endl;
					std::cout << "Choose what to do with this attack! You must enter a number: " << std::endl;
					std::cout << "Enter 0 or any letter to go back!" << std::endl;
					int userIn;
					std::cin >> userIn;
					while (!std::cin) {
						std::cin.clear();
						std::cin.ignore();
						continue;
					}
					if (userIn == 0) {
						system("CLS");
						goBack = true;
						break;
					}
					else if ((unsigned int)userIn <= userWeapon->getAttacks().size() && userIn > 0) {
						run = true;
						userAttack = userWeapon->getAttacks()[userIn - 1];
						system("CLS");
					}
				}//end of while loop for attack input

				if (!goBack) {
					player->subHealth(enemyAttack->value);
					enemy->attacked(userAttack);
				}

				/*
				Checks if the player or enemy have any status damages that need to be subtracted
				*/
				player->checkStatus();
				enemy->checkStatus();
			} //End of IF statement for (player has more than 0 weapons)

			else { //IF YOU HAVE NO ATTACKS, IT GOES TO THIS ELSE STATEMENT
				std::cout << "You have no attacks." << std::endl;
			}
		} //end of IF statement for (user entered 1 OR attack)

		/*
		ITEMS OPTIONS - SHOWS ALL OPTIONS FOR ATTACKS
		*/
		else if (userIn == "2" || userIn == "items") {
			if (player->getItems().size() <= 0) {
				std::cout << "You don't have any items." << std::endl;
			}
			else {
				player->printItems();
				std::cout << std::endl;
				std::cout << "Choose an item!" << std::endl;
				std::cout << "Enter 0 or any letter to go back!" << std::endl;
				int userIn;
				std::cin >> userIn;
				while (!std::cin) {
					std::cin.clear();
					std::cin.ignore();
					continue;
				}
				if (userIn == 0) {
					system("CLS");
					goBack = true;
					break;
				}
				else if ((unsigned int)userIn <= player->getItems().size() && userIn > 0) {
					Item* playerItem = player->getItems()[userIn - 1];
					if (playerItem->getType() == "health") {
						player->addHealth(playerItem->getValue());
					}
					else {
						std::cout << "THIS ITEM DOES NOT HAVE A USE RIGHT NOW." << std::endl;
					}
				}
			}


		}
		else if (userIn == "3" || userIn == "run") {
			system("CLS");
			break;
		}

	}
	return std::make_pair(player, enemy);
}
/*
Creates and populates the rooms that will be traveled through.
*/
std::vector<Room*> createRooms() {
	std::vector<Room*> room;
	for (int i = 0; i < 21; i++) {
		room.push_back(new Room("Room " + std::to_string(i)));
	}

	/*
	Room[0] Starting Room | Outside starting Town
	forest path is the only option for the next room
	Items:
	3x hp pots heal for 150
	1x armor adds 250 // the only thing the guild could spare due to the waves spread across the land and supplies for battaling them are so low
	2x Fireball  dmg 75-100
	3x throwing knife 25-50 dmg
	3x water
	2x rations
	No enemies in this room
	*/
	room[0]->setRooms(room[1], nullptr, nullptr, nullptr);
	room[0]->addToDescription("For now you may only go north towards the Forest...");
	room[0]->addToDescription("In the distance you see an ominous black miasma overcast in the direction you need to go...");
	/* Items*/
	Item*  sMageStaff = new Item("MageStaff", "attack");
	sMageStaff->addToDescription("Your trusty magic staff you have fought may enemies with while adventuring");
	sMageStaff->addToAttacks("Fireball", "magic", 45);
	sMageStaff->addToAttacks("Lightning", "magic", 20);
	sMageStaff->addToAttacks("Purification", "magic", 55);
	Item* sHolySword = new Item("HolySword", "attack");
	sHolySword->addToDescription("Your trusty sword  you have had since you first became an adventure, it has been passed down in your family for generations");
	sHolySword->addToAttacks("Slash", "melee", 40);
	sHolySword->addToAttacks("cleave", "melee", 50);
	sHolySword->addToAttacks("Impale", "melee", 55);
	Item* sBow = new Item("Bow&Arrow", "attack");
	sBow->addToDescription("eww it is just a normal bow you just kinda found");
	sBow->addToAttacks("VortexArrow", "ranged", 35);
	sBow->addToAttacks("FireArrow", "ranged", 45);
	sBow->addToAttacks("Snipe", "ranged", 30);
	room[0]->addToItems(sBow);
	room[0]->addToItems(sHolySword);
	room[0]->addToItems(sMageStaff);
	Item* WaterRation1 = new Item("WaterRation", "health");
	WaterRation1->setUsable(true);
	WaterRation1->setValue(30);
	WaterRation1->addToDescription("Gain some health back for hydrating");
	room[0]->addToItems(WaterRation1);
	Item* FoodRation1 = new Item("FoodRation", "health");
	FoodRation1->setUsable(true);
	FoodRation1->setValue(20);
	FoodRation1->addToDescription("You can't fight on an empty stomach");
	room[0]->addToItems(FoodRation1);
	Item* HealthPotion1 = new Item("HealthPotion", "health");
	HealthPotion1->setUsable(true);
	HealthPotion1->setValue(50);
	HealthPotion1->addToDescription("An avarge quality Health Potion");
	room[0]->addToItems(HealthPotion1);
	Item* HealthPotion2 = new Item("HealthPotion", "health");
	HealthPotion2->setUsable(true);
	HealthPotion2->setValue(50);
	HealthPotion2->addToDescription("An avarge quality Health Potion");
	room[0]->addToItems(HealthPotion2);
	Item* HealthPotion3 = new Item("HealthPotion", "health");
	HealthPotion3->setUsable(true);
	HealthPotion3->setValue(50);
	HealthPotion3->addToDescription("An avarge quality Health Potion");
	room[0]->addToItems(HealthPotion3);
	Item* LeatherArmor = new Item("LeatherArmor", "health");
	LeatherArmor->setUsable(true);
	LeatherArmor->setValue(250);
	LeatherArmor->addToDescription("Well crafted armor which helps with taking some hit from enemies");
	room[0]->addToItems(LeatherArmor);
	/*
	ROOM 1 Starting path in the naba forest
	shield adds 32 hp // its a wood plank
	*/
	room[1]->setRooms(nullptr, room[0], room[2], room[3]);
	room[1]->addToDescription("You enter the path into the Nabaran Forest...");
	room[1]->addToDescription("The path is over cast by trees and darkness.....");
	room[1]->addToDescription("This however is the only way to the necropolis of the old kingdom....");
	room[1]->addToDescription("Infomation from the guild tells you ...");
	room[1]->addToDescription("In the east is the direct route while in the west in a village you may stop and get supplies... ");
	/* Items */
	Item* HealthPotion4 = new Item("HealthPotion", "health");
	HealthPotion4->setUsable(true);
	HealthPotion4->setValue(50);
	HealthPotion4->addToDescription("An avarge quality Health Potion");
	room[1]->addToItems(HealthPotion4);
	Item* BookofLostWill = new Item("BookofLostWill", "attack");
	BookofLostWill->addToDescription("Ancient magic text of the Underworld. ");
	BookofLostWill->setUsable(true);
	BookofLostWill->setValue(50);
	BookofLostWill->addToAttacks("MindShock", "magic", 55);
	BookofLostWill->addToAttacks("Demoralized", "magic", 45);
	BookofLostWill->addToAttacks("SpiritBreak", "magic", 35);
	room[1]->addToItems(BookofLostWill);
	/* Enemeis*/
	Enemy* CHumman1 = new Enemy("Alex", 60);
	CHumman1->addToAttacks("Inpact", "melee", 10);
	CHumman1->addToAttacks("AxeSlash", "melee", 40);
	CHumman1->setWarning("During the first waves when the chaos lord returned he had corrupted the spirts of  humans  make them mindless servents to chaos. . ..");
	CHumman1->setWarning("As you enter the forest you come upon Alex who you have known since childhood, as you get closser you see his eyes are glossed over. . .");
	CHumman1->setWarning("It is like they can't even see the world around them any more. . .");
	room[1]->setEnemy(CHumman1);

	/*
	ROOM 2  Western path of the Nahouran forest
	*/
	room[2]->setRooms(room[5], nullptr, nullptr, room[1]);
	room[2]->addToDescription("You have decided to go to the next village to find some supplies to fight the lord. . . .");
	room[2]->addToDescription("You still have a ways to go before you get there, you still can turn back. . .");
	room[2]->addToDescription("You have no idea what this path has in store for you . . .");
	room[2]->addToDescription("As you enter deeper into the forest the pressure of the miasma grows. . .");
	room[2]->addToDescription("you see several abandoned carts as you journey along the path may of them have been destroyed. . . but by what? . . .");
	/* Items */
	Item* HealthPotion5 = new Item("HealthPotion", "health");
	HealthPotion5->setUsable(true);
	HealthPotion5->setValue(50);
	HealthPotion5->addToDescription("An avarge quality Health Potion");
	room[2]->addToItems(HealthPotion3);
	Item* HealthPotion6 = new Item("HealthPotion", "health");
	HealthPotion6->setUsable(true);
	HealthPotion6->setValue(50);
	HealthPotion6->addToDescription("An avarge quality Health Potion");
	room[2]->addToItems(HealthPotion6);
	/*Enemies */
	Enemy* Bear = new Enemy("Bear", 100);
	Bear->addToAttacks("Charge", "melee", 35);
	Bear->addToAttacks("Bite", "melee", 50);
	Bear->addToAttacks("Claw", "melee", 30);
	room[2]->setEnemy(Bear);
	/*
	ROOM 3 Eastern path of the Nahouran forest
	*/
	room[3]->addToDescription("You have decided to take the shorter path . . .");
	room[3]->addToDescription("As you reach the end it seems like the bridge is out and a dark  miasma  covers the other side. . .");
	room[3]->addToDescription("you find a path to the east which will take you around to the exit of the forest....");
	room[3]->setRooms(room[4], nullptr, room[1], nullptr);
	/* Items */
	Item* HealthPotion7 = new Item("HealthPotion", "health");
	HealthPotion7->setUsable(true);
	HealthPotion7->setValue(50);
	HealthPotion7->addToDescription("An avarge quality Health Potion");
	room[3]->addToItems(HealthPotion7);
	Item* WaterRation2 = new Item("WaterRation", "health");
	WaterRation2->setUsable(true);
	WaterRation2->setValue(20);
	WaterRation2->addToDescription("Gain some health back for hydrating");
	room[2]->addToItems(WaterRation2);
	Item* WoodSheild = new Item("WoodPlank", "health");
	WoodSheild->setValue(70);
	WoodSheild->addToDescription("Gain some more protection from damage");
	room[2]->addToItems(WoodSheild);
	/*Enemies */
	Enemy* Wolf1 = new Enemy("Wolf", 30);
	Wolf1->addToAttacks("Charge", "melee", 15);
	Wolf1->addToAttacks("Bite", "melee", 30);
	Wolf1->addToAttacks("Claw", "melee", 20);
	room[3]->setEnemy(Wolf1);
	/*
	ROOM 4 Off the eastern trail exit to the outskirts of the onld kingdom the plains
	*/
	room[4]->addToDescription("You have ended up closer to  the old  kingdom . . .");
	room[4]->addToDescription("As you reach the end of the forest you can see a field with a path . .");
	room[4]->addToDescription("you are still a good distance away from the old kingdom, but you see it from the forest the skies pitch black due to the evil  miasma coming from the necropolis. . .");
	room[4]->addToDescription("you need to go East to arrive at the old kingdom to fight the chaos lord. . .");
	room[4]->setRooms(nullptr, room[3], room[6], nullptr);
	/* Items */
	Item* sFireStorm = new Item("Firestorm", "attack");
	sFireStorm->addToDescription("A ripped page from a spellbook. ");
	sFireStorm->setUsable(true);
	sFireStorm->setValue(50);
	sFireStorm->addToAttacks("MeteorShower", "magic", 400);
	sFireStorm->addToAttacks("Firewall", "magic", 125);
	sFireStorm->addToAttacks("Ignite", "magic", 100);
	room[4]->addToItems(sFireStorm);
	/*Enemies */
	Enemy* Wolf5 = new Enemy("Wolf", 60);
	Wolf5->addToAttacks("Charge", "melee", 25);
	Wolf5->addToAttacks("Bite", "melee", 35);
	Wolf5->addToAttacks("Claw", "melee", 40);
	room[4]->setEnemy(Wolf5);


	/*
	ROOM 5 the exit of the forest, next to the destroyed town of tohya
	*/
	room[5]->setRooms(room[6], room[2], nullptr, nullptr);
	room[5]->addToDescription("You are almost to the village of Toyha however you find the path has been blocked. . .");
	room[5]->addToDescription("There is a hole in the blockade and you  decide to get up on the blockade. . .");
	room[5]->addToDescription("When you are on the blockade you see the village of Toyha  burned to the ground, miasma cover the path between houses . . .");
	room[5]->addToDescription("It is infested with zombies and what you belive crazzed humans killing each other within the  miasma. . .");
	room[5]->addToDescription("You have two choices either move north to the old kingdom which you see in the distance  and exit the forest or go back and try to go east to look for more supplies . . .");
	Item* FoodRation2 = new Item("Food Ration", "health");
	FoodRation2->setUsable(true);
	FoodRation2->setValue(25);
	FoodRation2->addToDescription("Gain some health back for Eating");
	room[5]->addToItems(FoodRation2);
	Item* FoodRation3 = new Item("Food Ration", "health");
	FoodRation3->setUsable(true);
	FoodRation3->setValue(20);
	FoodRation3->addToDescription("Gain some health back for Eating");
	room[5]->addToItems(FoodRation3);
	Enemy* ZHumman1 = new Enemy("Zombie", 60);
	ZHumman1->addToAttacks("Grab", "melee", 5);
	ZHumman1->addToAttacks("Bite", "melee", 20);
	ZHumman1->setWarning("When the chaos lord returned he had corrupted the spirts of and trapped them inside dead humans bringing their bodies back to life. . .");
	ZHumman1->setWarning("As are about to leave blockade you find two zombies have wandered over to you. . .");
	room[5]->setEnemy(ZHumman1);

	/*
	ROOM 6  Old KIngdom City outskirt ruins 2 sekeltons
	*/
	room[6]->setRooms(room[7], room[5], nullptr, room[4]);
	room[6]->addToDescription("You arrive at the walls of the old kingdom. . .");
	room[6]->addToDescription("There is a giant hole piercing the wall at the bottom of one of the watch towers. . .");
	room[6]->addToDescription("It appears to be the only way in to the old kingdom. . .");
	room[6]->addToDescription("The Front Gates are closed and appear not to have moved in centuries. . .");
	Enemy* Skeleton1 = new Enemy("Skeleton", 50);
	Skeleton1->addToAttacks("Arrow Shot", "melee", 5);
	Skeleton1->addToAttacks("BoneThrow", "melee", 20);
	Skeleton1->setWarning("When the chaos lord returned he returned some of the bones of the crypt located in the necropolis to motion traping slayed humans soul within their marrow. . .");
	Skeleton1->setWarning("You see two skeletons with bows standing perched on platforms higher up on the inside of the tower. . .");
	Skeleton1->setWarning("You must defeat them to make it into the city. . .");
	room[6]->setEnemy(Skeleton1);


	/*
	ROOM 7  Old kingdom main square
	*/
	room[7]->setRooms(nullptr, room[6], nullptr, room[8]);
	room[7]->addToDescription("After making it through the tower you have now arrived at the main plaza of the kingdom. . .");
	room[7]->addToDescription("You notice there are many bodies on the ground they look like the have been dropped from a significant high. . .");
	room[7]->addToDescription("You also notice some of them look to have been preyed on by vultures and other animals. . .");
	room[7]->addToDescription("ou see intestines strewn from these bodies as if what ever did this savagely ripped them apart after they fell from a high. . .");
	Item* DragonFury = new Item("DragonFury", "attack");
	DragonFury->addToDescription("A dragon talon of impenetrable wrath. ");
	DragonFury->setUsable(true);
	DragonFury->setValue(60);
	DragonFury->addToAttacks("DiveBomb", "melee", 125);
	DragonFury->addToAttacks("Swift", "melee", 70);
	DragonFury->addToAttacks("Slash", "melee", 55);
	room[7]->addToItems(DragonFury);
	Enemy* Wayvern1 = new Enemy("Wayvern", 150);
	Wayvern1->addToAttacks("Bite", "melee", 50);
	Wayvern1->addToAttacks("TailWhip", "melee", 120);
	Wayvern1->setWarning("you hear a loud screeching noise from within the cloud of  miasma, then out  comes an undead wayvern. . .");
	Wayvern1->setWarning("you have heard legends that the kingdoms of old use to have the power to train and control wayverns and dragons. . .");
	Wayvern1->setWarning("You never believed that they existed, but now you know they must have! ! ! . . .");
	room[7]->setEnemy(Wayvern1);

	/*
	ROOM 8 Estize Kingdom Necropolis
	*/
	room[8]->setRooms(nullptr, room[9], room[7], nullptr);
	room[8]->addToDescription("You gotten to what seems  like the old kingdom's necropolis. . .");
	room[8]->addToDescription("There is a hole in the door that  miasma  is coming out from and you slip your way in. . .");
	room[8]->addToDescription("You light a torch and see a map on the ground you pick it up and you notice it has the name Estize. . .");
	room[8]->addToDescription("The map looks similar to maps you have seen of the area now, the only diffrence was that insdead of the old kingdom it has the name Estize. . .");
	room[8]->addToDescription("You must decided to continue further south into the necropolis or to fail the world and it's people. . .");
	Enemy* ZHumman6 = new Enemy("Zombie", 60);
	ZHumman6->addToAttacks("Grab", "melee", 5);
	ZHumman6->addToAttacks("Bite", "melee", 20);
	ZHumman6->setWarning("As you descend deeper into the crypts  you find . . .");
	ZHumman6->setWarning("There is a zombie wandering around the hall that has doors to the south and east . . .");
	room[8]->setEnemy(ZHumman6);

	/*
	ROOM 9 Ritual Chamber
	*/
	room[9]->setRooms(room[8], nullptr, room[10], room[11]);
	room[9]->addToDescription("You arrive  in a cold room, surrounded on all four sides by walls with an alter in the middle with doors on the east and west sides.");
	room[9]->addToDescription("It seems like the temple where you see off the dead in the crypt of Estnophor");
	room[9]->addToDescription("If you remeber correctly the layout of the Estnophor crypt had a priests quarters to the  west and a path to the crypts in the east");
	Enemy* ChaosKnight1 = new Enemy("ChaosKnight", 255);
	ChaosKnight1->addToAttacks("Inpact", "melee", 25);
	ChaosKnight1->addToAttacks("Punch", "melee", 20);
	room[9]->setEnemy(ChaosKnight1);

	/*
	ROOM 10 corridor to the crypts
	*/
	room[10]->setRooms(room[13], room[12], room[11], room[9]);
	room[10]->addToDescription("You descend deeper into the crypt to find the lord of chaos.");
	room[10]->addToDescription("There is many bones lined up on the walls of the crypt as you descent level after level");
	Enemy* ZHumman3 = new Enemy("Zombie", 60);
	ZHumman3->addToAttacks("Grab", "melee", 5);
	ZHumman3->addToAttacks("Bite", "melee", 20);
	ZHumman3->setWarning("As you descend deeper into the crypts  you find  a large a hall that branches off into two diffrent directions. . .");
	ZHumman3->setWarning("You find a zombie wandering around the hall that has doors to the south and the north . .");
	room[10]->setEnemy(ZHumman3);
	/*
ROOM 11  priests quarters
*/
	room[11]->setRooms(nullptr, nullptr, room[9], room[10]);
	room[11]->addToDescription("You are in the priests quarters, it is bleak and empty just as you would excpect a man of the cloths room to be. . .");
	room[11]->addToDescription("As when they join the brother hood of the cloth they give up worldly possesions. . .");

	Enemy* Skeleton4 = new Enemy("Skeleton", 40);
	Skeleton4->addToAttacks("BowBash", "melee", 5);
	Skeleton4->addToAttacks("BoneThrow", "melee", 20);
	Skeleton4->setWarning("You Find a former man of the cloth now brough back as a minion of the lord of chaos. . .");
	room[11]->setEnemy(Skeleton4);

	/*
ROOM 12 southern wing of the crypt
*/
	room[12]->setRooms(room[10], room[20], room[14], nullptr);
	room[12]->addToDescription("You travel a while down the southern wing of the crypt and come to two doors one giant and one normal sized. . .");
	room[12]->addToDescription("The giant door is locked and the smaller one seems open");
	room[12]->addToDescription("You can go back north and find a key for the southern door possibly deeper in the crypt from the ritual chamber or go east into what seems like a crypt for royals of the kingdom. . .");
	Enemy* UndeadKnight3 = new Enemy("UndeadKnight", 115);
	UndeadKnight3->addToAttacks("MaceBludgeon", "melee", 35);
	UndeadKnight3->addToAttacks("cleave", "melee", 40);
	room[12]->setEnemy(UndeadKnight3);

	/*
		/*
	ROOM 13 norther wing of the crypt
	*/
	room[13]->setRooms(room[15], room[10], nullptr, nullptr);
	room[13]->addToDescription("You venture deeper into the crypt then anyone has since the old kingdom's fall. . .");
	room[13]->addToDescription("There is cobwebs and dust filling the air to make it hard to see even with light. . .");
	room[13]->addToDescription("you explore the new area of crypts long forgotten. . .");
	room[13]->addToDescription("Upon apon climbing down countless amounts of stairs you come to a northern room. . .");
	Item* SkullScepter = new Item("SkullScepter", "attack");
	SkullScepter->addToDescription("For the forgotten king ready to ravage the land.");
	SkullScepter->setUsable(true);
	SkullScepter->setValue(60);
	SkullScepter->addToAttacks("HallowEve", "ranged", 200);
	SkullScepter->addToAttacks("Dismember", "ranged", 85);
	SkullScepter->addToAttacks("SkullInvitation", "ranged", 40);
	room[13]->addToItems(SkullScepter);
	Enemy* Skeleton6 = new Enemy("Skeleton", 40);
	Skeleton6->addToAttacks("BowBash", "melee", 5);
	Skeleton6->addToAttacks("BoneThrow", "melee", 20);
	Skeleton6->setWarning("You Find a skeleton brough back as a minion of the lord of chaos. . .");
	room[13]->setEnemy(Skeleton6);


	/*
ROOM 14 1st generation tomb of royalty
*/
	room[14]->setRooms(nullptr, nullptr, nullptr, room[12]);
	room[14]->addToDescription("You are in a Tomb for the first generation of royals of the Estize kingdom. . .");
	Enemy* UndeadKnight5 = new Enemy("UndeadKnight", 155);
	UndeadKnight5->addToAttacks("MaceBludgeon", "melee", 35);
	UndeadKnight5->addToAttacks("RainOfMaces", "melee", 80);
	room[14]->setEnemy(UndeadKnight5);

	/*
	ROOM 15 corrador of the dammed
	*/
	room[15]->setRooms(room[16], room[13], room[17], nullptr);
	room[15]->addToDescription("You one you come out of crypt you arrive what essentialy looks like a gate to hell on the west wall that is painted in extreame detail  by someone of old as if to liken the crypts to never ending suffering for those who are not invited.");
	room[15]->addToDescription("You have the option of two more crypts either to the north or to the east.");
	Enemy* ZHumman9 = new Enemy("Zombie", 60);
	ZHumman9->addToAttacks("Grab", "melee", 5);
	ZHumman9->addToAttacks("Bite", "melee", 20);
	ZHumman9->setWarning("You find a zombie wandering around the hall that has doors to the east and the north . .");
	room[15]->setEnemy(ZHumman9);


	/*
	ROOM 16  north wing of the second lvl of the crypt
	*/
	room[16]->setRooms(nullptr, room[15], room[19], nullptr);
	room[16]->addToDescription("You have entered another crypt with only one exit to the east. . . .");
	room[16]->addToDescription("The eastern door is lkarge enoug for several of the wayverns you fought back in the town square to fit through, what does that mean for wha't whating for you ? ? ?.");
	Enemy* UndeadKnight10 = new Enemy("UndeadKnight", 125);
	UndeadKnight10->addToAttacks("Inpact", "melee", 25);
	UndeadKnight10->addToAttacks("Punch", "melee", 20);
	room[16]->setEnemy(UndeadKnight10);

	/*
	ROOM 17 Eastern wing of the second lvl of the crypt
	*/
	room[17]->setRooms(room[18], nullptr, nullptr, room[15]);
	room[17]->addToDescription("This crypt was alot shorter then the others and comes to a western door which is made out of a material you haven't seen before. . .");
	room[17]->addToDescription("You have a really bad feeling about it and peek in.");
	room[17]->addToDescription("When you look in all you see is a great hall filled with 2 chaos knights and 3 undead.");
	room[17]->addToDescription("It does look to be where the chaos lord is nor is there any other doors entries to the room.");
	room[17]->addToDescription("You notice a msg in blood on the wall after you close the door saying you should not enter and turn back. . .");
	Enemy* UndeadKnight12 = new Enemy("UndeadKnight", 125);
	UndeadKnight12->addToAttacks("Inpact", "melee", 25);
	UndeadKnight12->addToAttacks("Punch", "melee", 20);
	room[17]->setEnemy(UndeadKnight12);

	/*
	ROOM 18  2nd generation tomb of royalty 3 undead knight  2 chaos knights Dead End room dont enter
	*/
	room[18]->setRooms(nullptr, room[17], nullptr, nullptr);
	room[18]->addToDescription("You what looks like a royal tomb of a diffrent era long before the times of now. . .");
	room[18]->addToDescription("It Also Seems to have an aura of war and blood to it. . ..");
	room[18]->addToDescription("It must have been a generation of war. . .");
	Item* Loyality = new Item("Loyality", "attack");
	Loyality->addToDescription("A dusty legendary sword kept in the kings tomb locked away in hopes of a worthy man.");
	Loyality->setUsable(true);
	Loyality->setValue(80);
	Loyality->addToAttacks("Judgement", "melee", 500);
	Loyality->addToAttacks("Gulliotine", "melee", 225);
	Loyality->addToAttacks("ThousandCuts", "melee", 100);
	room[18]->addToItems(Loyality);

	Enemy* ChaosKnight2 = new Enemy("ChaosKnight", 225);
	ChaosKnight2->addToAttacks("Inpact", "melee", 25);
	ChaosKnight2->addToAttacks("Punch", "melee", 20);
	room[18]->setEnemy(ChaosKnight1);
	/*
ROOM 19 Undead Dragon  tomb of royal the gardian 1 undead dragon key room
*/
	room[19]->setRooms(nullptr, nullptr, nullptr, room[19]);
	room[19]->addToDescription("Room is Filled with gold and jews . . .");
	room[19]->addToDescription("There is a hole in the ceiling above them . . .");
	room[19]->addToDescription("All of a sudden a LARGE DRAGON COMES CRASHING DOWN INTO THE PILE OF GOLD. . ..");
	room[19]->addToDescription("Upon looking around you see a key made of gold on the floor next to a corpse of whaat looks like a freshly killed adventurer");
	Item* key = new Item("key", "item");
	key->addToDescription("A key in the hand of a dead adventure, it must open something important. . .");
	room[19]->addToItems(key);
	Enemy* UndeadDragon = new Enemy("UndeadDragon", 325);
	UndeadDragon->addToAttacks("FireBreath", "melee", 100);
	UndeadDragon->addToAttacks("WingSlam", "melee", 50);
	UndeadDragon->addToAttacks("TailWhip", "melee", 90);
	room[19]->setEnemy(UndeadDragon);
	/*
	ROOM 20 Boss Room [Chaos Lord] Crypt of the elders 1 lord of destruction and chaos
	*/
	room[20]->setRooms(room[12], nullptr, nullptr, nullptr);
	room[20]->addToDescription("You are in a large Hall with a throne at the end of it. . .");
	room[20]->addToDescription("On the throne You see a mass of black miasma covering the throne. . .");
	room[20]->addToDescription("You hear a loud clap, anmd the black miasma dissapates. . .");
	room[20]->addToDescription("You see on the throne a man the size of a giant . . . ");
	room[20]->addToDescription("The giant before you is the lord of chaos & Destruction! ! !");
	room[20]->setLockStatus(true);
	room[20]->setDoor(true);
	Enemy* ChaosLord = new Enemy("ChaosLord", 525);
	ChaosLord->addToAttacks("Inpact", "melee", 25);
	ChaosLord->addToAttacks("Punch", "melee", 20);
	room[20]->setEnemy(ChaosLord);
	return room;
}
/*
Returns a vector of user input tokens
*/
std::vector<std::string> tokenize(std::string userIn) {
	char seperator = ' ';
	std::vector<std::string> tokens;
	std::stringstream ss(userIn);
	std::string tok;
	bool stop = false;

	while (getline(ss, tok, seperator)) {
		tokens.push_back(tok);
	}
	return tokens;
}
/*
prints all options for help
*/
void printHelp() {
	/*
	creates 'help' list of available commands
	*/
	std::vector<std::string> help;
	help.push_back("Interaction Commands:");
	help.push_back("[ {north} | {south} | {east} | {west} ] Will move you into the next room");
	help.push_back("{Inventory} Displays Your Item Inventory");
	help.push_back("{Weapons} Displays Your Weapons");
	help.push_back("{look} tells the player infomation about the room  & what items it has");
	help.push_back("[ {take} | {grab} ] Will allow you to take an item");
	help.push_back("[ {search} ]tells what items a room has");
	help.push_back("");
	help.push_back("Encounter Commands: ");
	help.push_back("{run} Allows you to return to the previous room before combat");
	help.push_back("{fight} Starts a combat encounter with an enemy");
	for (std::string msg : help) {
		std::cout << msg << std::endl;
	}
}
int main() {
	/*
	Creates rooms, run conditional
	*/
	std::vector<Room*> room = createRooms();
	Room* currentRoom = room[0];
	bool runGame = true;
	std::string userIn;

	/*
	Get character name here:
	*/
	std::cout << "The lord of destruction & chaos has awakened. . . ." << std::endl;
	Sleep(10);
	std::cout << "The land and its people are in dire peril! . . ." << std::endl;
	Sleep(10);
	std::cout << "You must become the hero that save the kingdom & the world from ruin. . ." << std::endl;
	Sleep(10);
	std::cout << "Will you stand against the chaos and save the world . . ." << std::endl;
	Sleep(10);
	std::cout << ". . ." << std::endl;
	Sleep(25);
	std::cout << std::endl << std::endl;
	std::string name;
	std::cout << "What is Your Name Hero. . . " << std::endl;
	std::getline(std::cin, name);
	while (name.length() <= 0) {
		system("CLS");
		std::cout << "What is Your Name Hero. . . " << std::endl;
		std::getline(std::cin, name);
	}
	Player* player = new Player(name);
	system("CLS");
	bool printRoom = true;

	while (runGame) {
		Room* previousRoom = currentRoom; //This is used if the user wants to run from a fight, returns them to "previous room"


		/*
		Prevents room information from being printed an unnecessary amount of times
		*/
		if (printRoom) {
			std::cout << player->getName() << "'s health is: " << player->getHealth() << std::endl << std::endl;
			currentRoom->printDescription();
		}
		printRoom = true;


		/*
		takes user input and tokenizes it
		*/
		std::cout << "What do you want to do?" << std::endl;
		std::getline(std::cin, userIn); //gets input
		std::transform(userIn.begin(), userIn.end(), userIn.begin(), ::tolower); //lowercases input
		std::vector<std::string> tokens = tokenize(userIn); //seperates input into tokens
		system("CLS");

		if (tokens.size() == 0) {
			tokens.push_back("look");
		}
		/*
		checks first token against possible commands
		*/
		if (tokens[0] == "quit") {
			runGame = false;
			break;
		}//"QUIT" IF STATEMENT
		else if (tokens[0] == "help") {
			printRoom = false;
			printHelp();
			std::cout << std::endl;
		}//"HELP IF STATEMENT
		else if (tokens[0] == "north" || tokens[0] == "south" || tokens[0] == "east" || tokens[0] == "west") {
			system("CLS");
			currentRoom = currentRoom->getRoom(tokens[0]); //SETS CURRENT ROOM TO NEW ROOM, OR SAME ROOM IF DIRECTION WAS NULLPTR
			std::cout << player->getName() << "'s health is: " << player->getHealth() << std::endl << std::endl;
			currentRoom->printDescription();
			printRoom = false;
			if (currentRoom->hasEnemy()) { //CHECKS IF THERE IS AN ENEMY IN THE ROOM
				currentRoom->getEnemy()->printWarning();
				std::cout << std::endl << "Do you want to fight or run?" << std::endl;
				std::getline(std::cin, userIn);
				std::transform(userIn.begin(), userIn.end(), userIn.begin(), ::tolower);
				while (userIn != "fight" && userIn != "run") {
					system("CLS");
					std::cout << "Incorrect choice! Do you want to fight or run?" << std::endl;
					std::cin >> userIn;
				}
				if (userIn == "run") {
					currentRoom = previousRoom;
					std::cout << player->getName() << "'s health is: " << player->getHealth() << std::endl << std::endl;
					currentRoom->printDescription();
					system("CLS");
					printRoom = true;
				}
				else {
					std::pair<Player*, Enemy*> combatants = doCombat(player, currentRoom->getEnemy());
					player = combatants.first;
					currentRoom->setEnemy(combatants.second);
					if (!currentRoom->getEnemy()->isAlive()) { //IF THE ENEMY IS DEAD
						std::cout << "You killed the " << currentRoom->getEnemy()->getName() << "!" << std::endl;
						if (currentRoom->getEnemy()->hasItem()) { //IF THE ENEMY HAD AN ITEM, IT GETS ADDED TO YOUR INVENTORY
							player->addToInv(currentRoom->getEnemy()->getItem());
							std::cout << "The " << currentRoom->getEnemy()->getName() << " dropped a " << currentRoom->getEnemy()->getItem()->getName() << "!" << std::endl;
							std::cout << "It has been added to your inventory!" << std::endl;
							int pause;
							std::cout << "Enter any number to continue.";
							std::cin >> pause;
							while (!std::cin) {
								std::cin.clear();
								std::cin.ignore();
								std::cout << "You must enter a number.";
								std::cin >> pause;
							}
						}
						currentRoom->removeEnemy();
					}
					else if (!player->isAlive()) { //IF THE PLAYER IS DEAD
						std::cout << "You were killed by the " << currentRoom->getEnemy()->getName() << "!" << std::endl;
						std::cout << "The World and kingdom has fallen into ruins. . ." << std::endl;
						Sleep(10);
						std::cout << "You have failed however you do have another chance. . ." << std::endl;
						Sleep(10);
						runGame = false;
						int pause;
						std::cout << "Enter any number to continue.";
						std::cin >> pause;
						while (!std::cin) {
							std::cin.clear();
							std::cin.ignore();
							std::cout << "You must enter a number.";
							std::cin >> pause;
						}
					}
				}
			}//if currentRoom.hasEnemy()
		}//ROOM TRAVEL DIRECTION IF STATEMENT
		else if (tokens[0] == "inventory") {
			if (player->getItems().size() != 0) {
				std::cout << "Items: " << std::endl;
				player->printItems();
			}
			else {
				std::cout << "You have no items." << std::endl;
			}
		}//END OF INVENTORY IF STATMENT
		else if (tokens[0] == "use") {
			for (Item* it : player->getItems()) {
				if (it->getName() == tokens[1]) {
					if (it->isUsable()) {
						player->useItem(it);
					}
					else {
						std::cout << "That item is not usable.";
					}
					break;
				}
			}
		}//END OF USE IF STATMENT
		else if (tokens[0] == "weapons") {
			if (player->getWeapons().size() != 0) {
				std::cout << "Weapons: " << std::endl;
				player->printWeapons();
			}
			else {
				std::cout << "You have no weapons." << std::endl;
			}
		}//END OF WEAPONS IF STATEMENT
		else if (tokens[0] == "search") {
			if (!currentRoom->hasItems()) {
				std::cout << "This room has no usable items." << std::endl;
			}
			else {
				std::vector<Item*> tempItems;
				tempItems = currentRoom->getItems();
				std::cout << "This room contains: " << std::endl;
				for (Item* it : tempItems) {
					std::cout << it->getName() << std::endl;
				}
			}
		}//END OF SEARCH IF STATEMENT
		else if (tokens[0] == "open") {
			if (currentRoom->hasDoor()) {
				if (player->numOfKeys() > 0) {
					currentRoom->setLockStatus(false);
					currentRoom = room[9]; //CHANGE THIS TO BE THE BOSS ROOM[?]
				}
				else {
					std::cout << "That is locked, find the key." << std::endl;
				}
			}
			else {
				std::cout << "There is no door to open in here." << std::endl;
			}

		}//END OF DOOR IF STATEMENT
		else if (tokens[0] == "take" || tokens[0] == "grab") {
			if (currentRoom->hasItems()) {
				if (tokens[1] == "all") {
					for (Item* it : currentRoom->getItems()) {
						player->addToInv(it);
						currentRoom->removeItem(it);
					}
				}
				else {
					for (Item* it : currentRoom->getItems()) {
						std::string tempName = it->getName();
						std::transform(tempName.begin(), tempName.end(), tempName.begin(), ::tolower);
						if (tempName == tokens[1]) {
							player->addToInv(it);
							currentRoom->removeItem(it);
						}
					}
				}
			}
			else {
				std::cout << "There are no items you can take." << std::endl;
			}
		}//CHEST IF STATEMENT
		else if (tokens[0] == "look") {
			system("CLS");
			std::cout << player->getName() << "'s health is: " << player->getHealth() << std::endl << std::endl;
			currentRoom->printDescription();
			printRoom = false;
			if (currentRoom->hasEnemy()) { //CHECKS IF THERE IS AN ENEMY IN THE ROOM
				currentRoom->getEnemy()->printWarning();
				std::cout << "Do you want to fight or run?";
				std::getline(std::cin, userIn);
				std::transform(userIn.begin(), userIn.end(), userIn.begin(), ::tolower);
				while (userIn != "fight" && userIn != "run") {
					system("CLS");
					std::cout << "Incorrect choice! Do you want to fight or run?";
					std::cin >> userIn;
				}
				if (userIn == "run") {
					printRoom = true;
					currentRoom = previousRoom;
					system("CLS");
				}
				else {
					std::pair<Player*, Enemy*> combatants = doCombat(player, currentRoom->getEnemy());
					player = combatants.first;
					currentRoom->setEnemy(combatants.second);
					if (!currentRoom->getEnemy()->isAlive()) { //IF THE ENEMY IS DEAD
						std::cout << "You killed the " << currentRoom->getEnemy()->getName() << "!" << std::endl;
						currentRoom->removeEnemy();
					}
					else if (!player->isAlive()) { //IF THE PLAYER IS DEAD
						std::cout << "You were killed by the " << currentRoom->getEnemy()->getName() << "!" << std::endl;
						runGame = false;
					}
				}
			}//IF CURRENT ROOM HAS ENEMY
		}//LOOK ELSE IF STATEMENT
		else {
			std::cout << "INCORRECT INPUT." << std::endl;
		}//INCORRECT INPUT ELSE STATEMENT
	} //WHILE (RUN)
} //BOTTOM OF MAIN
