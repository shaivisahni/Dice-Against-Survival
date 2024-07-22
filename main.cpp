
/*
Header section:
Name of program: Dice against survival
Name: Shaivi
Brief description of functionality: Dice against survival is a single-player
game, where the user is incharge of leading a civilization. They encounter many
events/situation where they must decide how many stats to gain or lose, based
off dice that have been randomly generated. The goal is to manage all your stats
and avoid any of them from reaching 0.
*/

// SFML files
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "ResourcePath.hpp"

// libraries
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <vector>

using namespace std;
// Basically like the using namespace std of sfml
using namespace sf;

// Declaring base stats
int food = 6, military = 6, knowledge = 6, money = 6;
// Adds/subtracts additional stats every turn (status effects)
int food_per_turn, military_per_turn, knowledge_per_turn, money_per_turn;
// Tracks the number of turns
int turns = 0;
// Stores the IDs of each event
int IDS[3];
// Player stats for each box
int stats[3];
// Stores the number of the die
int dice_face[3];

class Events {
public:
    int id;
    string header, info, effect;

    void generateIds(int turns) {
        // generate events based on number of turns taken by user
        // If the first turn, generate the events from 1 - 3
        if (turns == 0) {
            IDS[0] = rand() % 3 + 1;
            do
                IDS[1] = rand() % 3 + 1;
            while (IDS[0] == IDS[1]);
            do
                IDS[2] = rand() % 3 + 1;
            while (IDS[2] == IDS[0] || IDS[2] == IDS[1]);

            // If turn 2, generate events from 1 - 9
        } else if (1 >= turns && turns <= 5) {
            IDS[0] = rand() % 9 + 1;
            do
                IDS[1] = rand() % 9 + 1;
            while (IDS[0] == IDS[1]);
            do
                IDS[2] = rand() % 9 + 1;
            while (IDS[2] == IDS[0] || IDS[2] == IDS[1]);

            // If between 7 and 10 turns, generate events from 1 - 17
        } else if (6 >= turns && turns <= 9) {
            IDS[0] = rand() % 17 + 1;
            do
                IDS[1] = rand() % 17 + 1;
            while (IDS[0] == IDS[1]);
            do
                IDS[2] = rand() % 17 + 1;
            while (IDS[2] == IDS[0] || IDS[2] == IDS[1]);

            // If between 11 and 15 turns, genereate events from 1 - 24
        } else if (10 >= turns && turns <= 14) {
            IDS[0] = rand() % 24 + 1;
            do
                IDS[1] = rand() % 24 + 1;
            while (IDS[0] == IDS[1]);
            do
                IDS[2] = rand() % 24 + 1;
            while (IDS[2] == IDS[0] || IDS[2] == IDS[1]);

            // If more than 15 turns, generate events from 1 - 30
        } else if (turns > 14) {
            IDS[0] = rand() % 30 + 1;
            do
                IDS[1] = rand() % 30 + 1;
            while (IDS[0] == IDS[1]);
            do
                IDS[2] = rand() % 30 + 1;
            while (IDS[2] == IDS[0] || IDS[2] == IDS[1]);
        }
    }

    // Set the header and description of the event
    void setEventDetail(int id) {
        // Each id represents an event (e.g. id:1 = event 1),
        // so it has an associated description on what it looks like
        switch (id) {
        /* POOL 1 OF EVENTS */
        case 1:
            header = "\tHARVEST\n  \tCROPS";
            info = "Our crops are ready to\nharvest! You can use them\n"
                   "to feed people. How much\n  food should you harvest...";
            break;

        case 2:
            header = "\tBARRACKS";
            info = "Our soldiers are starving\nin the barracks! How much\n"
                   "food should you provide\nto sustain our military...";
            break;

        case 3:
            header = "\tLIBRARY";
            info = "The library opens its doors!\n \tYou may find ancient\n"
                   "knowledge there! How much\n \tknowledge can you find...";
            break;

            /* POOL 2 OF EVENTS */
        case 4:
            header = "\t\tTAX \nCOLLECTION";
            info = "Taxes are due! How much\nmoney should you collect\n\tfrom "
                   "our village";
            break;

        case 5:
            header = "  STRUGGLING\n   INDUSTRY";
            info = "The economy is struggling\nwith an absence of "
                   "trading.\n\tHow much "
                   "money and\n \tknowledge will you lose...";
            break;

        case 6:
            header = "BAD ODOUR";
            info =
                "Foul-smelling odours are\ndistracting nearby farmers.\n"
                "3 or more knowledge can\n surely fix it. How much\nknowledge "
                "should you use...";
            break;

        case 7:
            header = "NEARBY CAMP";
            info = "You can steal food from\na nearby camp! If you only\n"
                   "take 3 food or less, no one\nwill notice. How much food\n"
                   "\t\tshould you steal...";
            break;

        case 8:
            header = "\tPURCHASE\n\tARTILLERY";
            info = "You can boost our military\nby purchasing artilery for\n"
                   "defence! How much money\n \t\twill you spend...";
            break;

        case 9:
            header = "\tPOISONED\n \tWATER";
            info = "Contaminated water is killing\n  our crops. A dice of 4 "
                   "or\nhigher can purify the water\n\t\t\tat no cost...";
            break;

        /* POOL 3 OF EVENTS */
        case 10:
            header = "BRUNING\n LIBRARY";
            info = "The library is set ablze!\nYou must send 3 or more "
                   "military\nto prevent any knowledge\n burning to a crisp";
            break;

        case 11:
            header = "GOLD DEPOSIT";
            info = "Miners have struck gold!\nHow much money did\nyou make off "
                   "the gold...";
            break;

        case 12:
            header = "FOOD THIEF";
            info =
                "Someone plan to steal\nour food! With 3 military\nor more, you"
                "surely are able\nto catch them. How much\n military should "
                "you "
                "use...";
            break;

        case 13:
            header = "\tPASSING\n\tSCHOLAR";
            info = "A well known scholar passes\nby the village. They offer\ngreat knowledge daily if you\nfeed them an odd amount of food!\n How much food will you give...";
            break;

        case 14:
            header = "\tBANDITS";
            info = "Bandits have invaded local\ntowns, you"
                   "require 3 or more\nmilitary to fend them off";
            break;

        case 15:
            header = "LABORATORY";
            info = "The labratory requires\nfunding to develop "
                   "new\nrevolutionary technology. You\nneed"
                   "to spend 4 or more money\nto keep a consistent source\n "
                   "of knowledge. How much will\nyou spend?";
            break;

        case 16:
            header = "\tSCHOOL";
            info = "Schools require funding in\norder to educate future\n"
                   "generations! How much\nmoney will you spend...";
            break;

        case 17:
            header = "TRAVELLING\n CONVOY";
            info = "Give 3 or more and 2\nfood to the travelling convoy,\n"
                   "it's  considered good luck...";
            break;

        /* POOL 4 OF EVENTS */
        case 18:
            header = "\tVISITOR";
            info = "An unknown nomad enters your\nvillage. They ask for 4 or\n"
                   "more and will give you 1\nknowledge everyday, but take 1\n"
                   "food everyday as compensation...";
            break;

        case 19:
            header = "\tBANK";
            info = "The bank brings good news.\nInvest 5 or more food "
                   "and\nreceive 1 money per day..";
            break;

        case 20:
            header = "FARMER\n PROTEST";
            info = "The farmers are dissatisfied at\nthe bandits stealing "
                   "their crops.\n"
                   "Send 3 or more military to defend\ntheir farm or lose more "
                   "food...";
            break;

        case 21:
            header = "VILLAGE\n DISPUTES";
            info = "Deploy military to avoid conflict\nand continue food "
                   "production.";
            break;

        case 22:
            header = "CULT UPRISING";
            info = "No matter what number is\nhere, you always lose 2 "
                   "military... ";
            break;

        case 23:
            header = "ARMY\n SIGN UPS";
            info = "The army is looking for new\nrecruits! How many applicants "
                   "will\nyou accept?...";
            break;

        case 24:
            header = "COAL MINE";
            info = "JACKPOT! Send any amount\nof military to get "
                   "the\nequivalent in money...";
            break;

        default:
            header = "Lorem Ipsum";
            info = "Lorem Ipsum";
            id = 0;
            break;
        }
    }
    // Chanaging stats based on users decsions for every case
    void setEventEffect(int id, int dice) {
        string temp1, temp2, temp3;
        string die = to_string(dice);
        // Set the text of what the event will affect after the turn is over
        // Effect changes depending on what the number on the die is placed
        // displaying
        switch (id) {
        /* POOL 1 OF EVENTS */
        case 1:
            effect = "Food + " + die;
            break;

        case 2:
            effect = "Military + " + die + "\nFood - " + die;
            break;

        case 3:
            effect = "Knowledge + " + die;
            break;

            /* POOL 2 OF EVENTS */
        case 4:
            effect = "Money + " + die;
            break;

        case 5:
            effect = "Money - " + die + "\nKnowledge - " + die;
            break;

        case 6:
            if (dice >= 3) {
                effect = "Knowledge - 3";
            } else {
                effect = "Knowledge - " + die + "\nFood - 3";
            }
            break;

        case 7:
            if (dice > 3) {
                effect = "  You were caught!\nNo food was stolen!";
                // consequences (bad)
            } else {
                effect = "Food + " + die;
            }
            break;

        case 8:
            effect = "Military + " + die + "\nMoney - " + die;
            break;

        case 9:
            if (dice <= 3) {
                effect = "Food - 3 \nKnowledge - 3";
            } else {
                effect = "Water Purified";
            }
            break;

        /* POOL 3 OF EVENTS */
        case 10:
            if (dice >= 3) {
                effect = "Military - " + die;
            } else {
                effect = "Military - " + die + "\nKnowledge - " + die;
            }
            break;

        case 11:
            effect = "Money + " + die;
            break;

        case 12:
            if (dice >= 3) {
                effect = "Military - " + die;
            } else {
                effect = "Military - " + die + "\nFood - 3";
            }
            break;

        case 13:
            if (dice % 2 == 1) {
                effect = "Knowledge + 1\nper turn   Food - " + die;
            } else {
                effect = "Decline the\n offer?";
            }
            break;

        case 14:
            if (dice >= 3) {
                effect = "Military - " + die;
            } else {
                effect = "Food - 1/turn\nMilitary - " + die;
            }
            break;

        case 15:
            if (dice >= 4) {
                effect = "Knowledge + 1\nper turn\tMoney - " + die;
            } else {
                effect = "Decline the\n offer?";
                ;
            }
            break;

        case 16:
            effect = "Knowledge + " + die + "\nMoney - " + die;
            break;

        case 17:
            if (dice >= 3) {
                effect = "Knowledge + " + die + "\nFood - 2";
            } else {
                effect = "Knowledge + " + die + "\nThe convoy was displeased";
            }
            break;

        /* POOL 4 OF EVENTS */
        case 18:
            if (dice >= 4) {
                effect = "Knowledge + 1/turn \nFood - 1/turn";
            }
            break;

        case 19:
            if (dice >= 5) {
                effect = "Money + 1/turn \nFood - " + die;
            }
            break;

        case 20:
            temp1 = to_string(dice * 2);
            if (dice >= 3) {
                effect = "Military - " + die + "\nFarmers are happy" + die;
            } else {
                effect = "Military - " + die + "\nFood - " + temp1;
            }
            break;

        case 21:
            effect = "Food + " + die + "\nMilitary - " + die;
            break;

        case 22:
            effect = "Military -= 2";
            break;

        case 23:
            effect = "Military + " + die;
            break;

        case 24:
            effect = "Money + " + die + "Military - " + die;
            break;
        }
    }

    void applyEffect(int id, int dice) {
        int temp1, temp2, temp3;

        // Functioning, and updating the stats for each resource
        // After turn is over, apply the effects of each event based
        // on the number of the dice used to solve the event
        switch (id) {
            /* POOL 1 OF EVENTS */
        case 1:
            food += dice;
            break;

        case 2:
            military += dice;
            food -= dice;
            break;

        case 3:
            knowledge += dice;
            break;

            /* POOL 2 OF EVENTS */
        case 4:
            money += dice;
            break;

        case 5:
            money -= dice;
            knowledge -= dice;
            break;

        case 6:
            if (dice < 3) {
                knowledge -= dice;
                food -= 3;
            } else {
                knowledge -= 3;
            }
            break;

        case 7:
            if (dice <= 3) {
                food += dice;
            }
            break;

        case 8:
            military += dice;
            money -= dice;
            break;

        case 9:
            if (dice <= 3) {
                food -= 3;
                knowledge -= 3;
            }
            break;

        /* POOL 3 OF EVENTS */
        case 10:
            military -= dice;
            if (dice < 3) {
                knowledge -= dice;
            }
            break;

        case 11:
            money += dice;
            break;

        case 12:
            military -= dice;
            if (dice < 3) {
                food -= 3;
            }
            break;

        case 13:
            if (dice % 2 == 1) {
                knowledge_per_turn += 1;
                food -= dice;
            }
            break;

        case 14:
            military -= dice;
            if (dice < 3) {
                food_per_turn -= 1;
            }
            break;

        case 15:
            if (dice >= 4) {
                money -= dice;
                knowledge_per_turn += 1;
            }
            break;

        case 16:
            money -= dice;
            knowledge += dice;
            break;

        case 17:
            knowledge += dice;
            if (dice >= 3) {
                food -= 2;
                // good consequences
            }
            break;

        /* POOL 4 OF EVENTS */
        case 18:
            if (dice >= 4) {
                knowledge_per_turn += 1;
                food_per_turn -= 1;
            }
            break;

        case 19:
            if (dice >= 5) {
                food -= dice;
                money_per_turn += 1;
            }
            break;

        case 20:
            military -= dice;
            if (dice >= 3) {
                // farmers are happy (good consequences)
            } else {
                food -= (dice * 2);
            }
            break;

        case 21:
            food += dice;
            military -= dice;
            break;

        case 22:
            military -= 2;
            break;

        case 23:
            military += dice;
            break;

        case 24:
            money += dice;
            military -= dice;
            break;
        }
    }

    void applyStatusEffects() {
        food += food_per_turn;
        military += military_per_turn;
        knowledge += knowledge_per_turn;
        money += money_per_turn;
    }
};

// Declare 3 instances (objects) of the class Events
Events events[3];

// Set each events id (i.e. what event is is) and their visual details
void setEventDetails() {
    events[0].generateIds(turns);
    for (int i = 0; i < 3; i++) {
        events[i].id = IDS[i];
        events[i].setEventDetail(events[i].id);
    }
}
void setEventEffects(int d1, int d2, int d3) {
    events[0].setEventEffect(events[0].id, d1);
    events[1].setEventEffect(events[1].id, d2);
    events[2].setEventEffect(events[2].id, d3);
}
// When "NEXT TURN" is pressed, apply the actual effects
// of each event to the stat variables
void applyEffects() {
    for (int i = 0; i < 3; i++) {
        events[i].applyEffect(events[i].id, stats[i]);
    }
    events[0].applyStatusEffects();
}

// Algorithm to choose 3 numbers from 1-6 with no duplicates
void diceNum() {
    // Declare dice
    dice_face[0] = rand() % 6 + 1;
    do
        dice_face[1] = rand() % 3 + 1;
    while (dice_face[0] == dice_face[1]);
    do
        dice_face[2] = rand() % 5 + 1;
    while (dice_face[2] == dice_face[0] || dice_face[2] == dice_face[1]);
}

int main() {
    // Window length and width by pixles
    int windowLength = 600;
    int windowWidth = 1000;

    
    RenderWindow window(VideoMode(windowWidth, windowLength), "Dice Of Survival");

  sf::Cursor custom_cursor;

  // Set custom cursor to 'wait' (an hourglass)
  if (custom_cursor.loadFromSystem(sf::Cursor::Wait)){
    window.setMouseCursor(custom_cursor);
  }

  /*     Image not loading properly error
  sf::Image curimg;
  curimg.loadFromFile(resourcePath() + + "finger_cursor.png");

  custom_cursor.loadFromPixels(curimg.getPixelsPtr(), curimg.getSize(),{0, 0});
  window.setMouseCursor(custom_cursor);
  */
    
    // Load a music to play
    sf::Music music;
    music.openFromFile(resourcePath() + "music.ogg");

    // Play the music
    music.play();

  
// start of the turn for the game loop, which loops the game 10 times, for
// 10 turns
start:
    // To make it so that when the dice get randomly generated, the dice won't
    // be randomly generated the same every turn
    srand(time(0));
    bool Npressed = false;
    // chceks if in the current turn if the next button had been presseed
    bool NextPressed[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    // bool to check which dice face (1-6) is placed in which place (3 places to
    // place the dice), for the first dice
    bool EventDice[3][6] = {
        {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};
    // bool to check which dice face (1-6) is placed in which place (3 places to
    // place the dice), for the second dice
    bool EventDice2[3][6] = {
        {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};
    // bool to check which dice face (1-6) is placed in which place (3 places to
    // place the dice), for the third dice
    bool EventDice3[3][6] = {
        {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};
    int EventDiceCounter[3] = {0, 0, 0};

    // checks if next button has been click, but is used differently in the code
    bool nextButtonBool = false;

    // using texture "t" to get the backgraound image of the starting page
    Texture t;
    t.loadFromFile(resourcePath() + "maxresdefault-1.jpg");

    // A sprite called background is created with texture t in it
    Sprite background(t);
    
    // texture diceTexture gets the image of the dice used in the starting page
    Texture diceTexture;
    diceTexture.loadFromFile(resourcePath() + "Dice-Opening.png");

    // A sprite called dice is created with texture diceTexture in it
    Sprite dice(diceTexture);

    // we set sprite dice to the pixle coordinates below (x,y)
    dice.setPosition(375, 280);

    // All fonts used in the program are linked to their respective font files,
    // which are saved into the variables font1, font2... through
    Font font;
    font.loadFromFile(resourcePath() + "Roman SD.ttf");
    Font font2;
    font2.loadFromFile(resourcePath() + "Unbounded/static/Unbounded-Light.ttf");
    Font font3;
    font3.loadFromFile(resourcePath() + "DM_Sans/DMSans-Medium.ttf");

    // the title text is made here using the fonts declared above
    Text titlePage("Dice Of Survival", font);
    Text textNToContinue("Click anywhere to continue", font);

    // size, colour, boldness, and position of the main title are decided here
    titlePage.setCharacterSize(80);
    titlePage.setFillColor(Color::Red);
    titlePage.setStyle(Text::Bold);
    titlePage.setPosition(100, 150);

    // size, colour, and position of the "Clcik anywhere to continue" are
    // decided here
    textNToContinue.setCharacterSize(30);
    textNToContinue.setFillColor(Color::Black);
    textNToContinue.setPosition(250, 450);

    // the white background box's size, colour, opacity and position are decided
    RectangleShape titleBox(Vector2f(920, 450));
    Vector2f titleBoxPosition(1000, 75);
    titleBox.setPosition(titleBoxPosition);
    titleBox.setFillColor(Color(255, 255, 255, 200));

    // var for movement/animation
    float xMovement = 10;

    // texture Background2 gets the image of the second background
    Texture Background2;
    Background2.loadFromFile(resourcePath() + "Background2.jpg");

    // A sprite called background2 is created with texture Background2 in it
    Sprite background2(Background2);

    // text to indicate the number of turns that have passed is created, along
    // with its size, colour, opacity, boldness, and position
    Text Turn("Day " + to_string(turns + 1), font);
    Turn.setCharacterSize(50);
    Turn.setFillColor(Color(247, 189, 3, 255));
    Turn.setStyle(Text::Bold);
    Turn.setPosition(375, 50);

    // the black background box's size, colour, opacity and position are decided
    RectangleShape Box(Vector2f(950, 550));
    Vector2f BoxPosition(1000, 25);
    Box.setPosition(BoxPosition);
    Box.setFillColor(Color(0, 0, 0, 200));

    // Display the stats on the screen
    RectangleShape statsBox(Vector2f(200, 100));
    statsBox.setPosition(730, 50);

    // Convert stats from integers to strings to display in window

    string str_food;
    string str_military;
    string str_knowledge;
    string str_money;
    if (food_per_turn > 0) {
        str_food = to_string(food) + "+ " + to_string(food_per_turn);
    } else if (food_per_turn < 0) {
        str_food = to_string(food) + " " + to_string(food_per_turn);
    } else {
        str_food = to_string(food);
    }

    if (military_per_turn > 0) {
        str_military =
            to_string(military) + "+ " + to_string(military_per_turn);
    } else if (military_per_turn < 0) {
        str_military = to_string(military) + " " + to_string(military_per_turn);
    } else {
        str_military = to_string(military);
    }

    if (knowledge_per_turn > 0) {
        str_knowledge =
            to_string(knowledge) + "+ " + to_string(knowledge_per_turn);
    } else if (knowledge_per_turn < 0) {
        str_knowledge =
            to_string(knowledge) + " " + to_string(knowledge_per_turn);
    } else {
        str_knowledge = to_string(knowledge);
    }

    if (money_per_turn > 0) {
        str_money = to_string(money) + "+ " + to_string(money_per_turn);
    } else if (money_per_turn < 0) {
        str_money = to_string(money) + " " + to_string(money_per_turn);
    } else {
        str_money = to_string(money);
    }

    // Convert food stat (int) to a string to display in text
    Text Food("Food: " + str_food, font2);
    Food.setCharacterSize(18);
    Food.setFillColor(Color(249, 213, 50, 255));
    Food.setStyle(Text::Bold);
    Food.setPosition(740, 55);

    // Convert knowledge stat (int) to a string to display in text
    Text Knowledge("Knowledge: " + str_knowledge, font2);
    Knowledge.setCharacterSize(18);
    Knowledge.setFillColor(Color(249, 213, 50, 255));
    Knowledge.setStyle(Text::Bold);
    Knowledge.setPosition(740, 75);

    // Convert military stat (int) to a string to display in text
    Text Military("Military: " + str_military, font2);
    Military.setCharacterSize(18);
    Military.setFillColor(Color(249, 213, 50, 255));
    Military.setStyle(Text::Bold);
    Military.setPosition(740, 95);

    // Convert money stat (int) to a string to display in text
    Text Money("Money: " + str_money, font2);
    Money.setCharacterSize(18);
    Money.setFillColor(Color(249, 213, 50, 255));
    Money.setStyle(Text::Bold);
    Money.setPosition(740, 115);

    // Event box 1, which hold the event and dice place holder's size, colour,
    // opacity, outline colour, outline thickness, and position are decided
    RectangleShape Event1(Vector2f(225, 300));
    Event1.setFillColor(Color(35, 35, 35, 250));
    Event1.setOutlineColor(Color::White);
    Event1.setOutlineThickness(2);
    Event1.setPosition(100, 250);

    // event box 2, which hold the event and dice place holder's size, colour,
    // opacity, outline colour, outline thickness, and position are decided
    RectangleShape Event2(Vector2f(225, 300));
    Event2.setFillColor(Color(35, 35, 35, 250));
    Event2.setOutlineColor(Color::White);
    Event2.setOutlineThickness(2);
    Event2.setPosition(366, 250);

    // event box 3, which hold the event and dice place holder's size, colour,
    // opacity, outline colour, outline thickness, and position are decided
    RectangleShape Event3(Vector2f(225, 300));
    Event3.setFillColor(Color(35, 35, 35, 250));
    Event3.setOutlineColor(Color::White);
    Event3.setOutlineThickness(2);
    Event3.setPosition(633, 250);

    // box to hold the 3 randomly generated numbers
    RectangleShape diceBox(Vector2f(350, 85));
    diceBox.setFillColor(Color(35, 35, 35, 225));
    diceBox.setPosition(310, 125);

    // box 1 to place the dice inside's size, colour, opacity, outline colour,
    // and position are decided
    RectangleShape event1Box(Vector2f(75, 75));
    event1Box.setFillColor(Color(255, 255, 255, 255));
    event1Box.setPosition(175, 450);

    // box 2 to place the dice inside's size, colour, opacity, outline colour,
    // and position are decided
    RectangleShape event2Box(Vector2f(75, 75));
    event2Box.setFillColor(Color(255, 255, 255, 255));
    event2Box.setPosition(445, 450);

    // box 3 to place the dice inside's size, colour, opacity, outline colour,
    // and position are decided
    RectangleShape event3Box(Vector2f(75, 75));
    event3Box.setFillColor(Color(255, 255, 255, 255));
    event3Box.setPosition(715, 450);

    // Three randomly generated dice are generated here by calling the function
    // and are assigned to different array elements
    diceNum();
    // Store the 3 numbers into the array, to display the appropriate image
    int diceImages[3] = {dice_face[0], dice_face[1], dice_face[2]};

    // texture diceFace1 get the first dice face image in it
    Texture diceFace1;
    diceFace1.loadFromFile(resourcePath() + "dice 1.jpg");

    // sprite face1 saves diceFace1 texture in it and sets its position to be
    // outside of the screen
    Sprite face1(diceFace1);
    face1.setPosition(1100, 700);

    // texture diceFace2 get the second dice face image in it
    Texture diceFace2;
    diceFace2.loadFromFile(resourcePath() + "dice 2.jpg");

    // sprite face2 saves diceFace2 texture in it and sets its position to be
    // outside of the screen
    Sprite face2(diceFace2);
    face2.setPosition(1100, 700);

    // texture diceFace3 get the third dice face image in it
    Texture diceFace3;
    diceFace3.loadFromFile(resourcePath() + "dice 3.jpg");

    // sprite face3 saves diceFace3 texture in it and sets its position to be
    // outside of the screen
    Sprite face3(diceFace3);
    face3.setPosition(1100, 700);

    // texture diceFace4 get the fourth dice face image in it
    Texture diceFace4;
    diceFace4.loadFromFile(resourcePath() + "dice 4.jpg");

    // sprite face4 saves diceFace4 texture in it and sets its position to be
    // outside of the screen
    Sprite face4(diceFace4);
    face4.setPosition(1100, 700);

    // texture diceFace5 get the fifth dice face image in it
    Texture diceFace5;
    diceFace5.loadFromFile(resourcePath() + "dice 5.jpg");

    // sprite face5 saves diceFace5 texture in it and sets its position to be
    // outside of the screen
    Sprite face5(diceFace5);
    face5.setPosition(1100, 700);

    // texture diceFace6 get the sixth dice face image in it
    Texture diceFace6;
    diceFace6.loadFromFile(resourcePath() + "dice 6.jpg");

    // sprite face6 saves diceFace6 texture in it and sets its position to be
    // outside of the screen
    Sprite face6(diceFace6);
    face6.setPosition(1100, 700);

    // the dice2Face 1 - 6 do the same as the 6 dices above, the difference is
    // that these dices won't move when they are selected, only the top 6 dices
    // will move
    Texture dice2Face1;
    dice2Face1.loadFromFile(resourcePath() + "dice 1.jpg");
    Sprite Face1(dice2Face1);
    Face1.setPosition(1100, 700);

    Texture dice2Face2;
    dice2Face2.loadFromFile(resourcePath() + "dice 2.jpg");
    Sprite Face2(dice2Face2);
    Face2.setPosition(1100, 700);

    Texture dice2Face3;
    dice2Face3.loadFromFile(resourcePath() + "dice 3.jpg");
    Sprite Face3(dice2Face3);
    Face3.setPosition(1100, 700);

    Texture dice2Face4;
    dice2Face4.loadFromFile(resourcePath() + "dice 4.jpg");
    Sprite Face4(dice2Face4);
    Face4.setPosition(1100, 700);

    Texture dice2Face5;
    dice2Face5.loadFromFile(resourcePath() + "dice 5.jpg");
    Sprite Face5(dice2Face5);
    Face5.setPosition(1100, 700);

    Texture dice2Face6;
    dice2Face6.loadFromFile(resourcePath() + "dice 6.jpg");
    Sprite Face6(dice2Face6);
    Face6.setPosition(1100, 700);

    // three of the highlights are needed because there are three dice places
    // The rectangle that highlights the dice when clicked are made here, and
    // their size, colour, opacity, and position are decided
    RectangleShape HighlightEvent1(Vector2f(80, 80));
    HighlightEvent1.setFillColor(Color(200, 200, 0, 150));
    HighlightEvent1.setPosition(1100, 700);

    // The rectangle that highlights the dice when clicked are made here, and
    // their size, colour, opacity, and position are decided
    RectangleShape HighlightEvent2(Vector2f(80, 80));
    HighlightEvent2.setFillColor(Color(200, 200, 0, 150));
    HighlightEvent2.setPosition(1100, 700);

    // The rectangle that highlights the dice when clicked are made here, and
    // their size, colour, opacity, and position are decided
    RectangleShape HighlightEvent3(Vector2f(80, 80));
    HighlightEvent3.setFillColor(Color(200, 200, 0, 150));
    HighlightEvent3.setPosition(1100, 700);

    // Generate 3 random events and their details
    setEventDetails();

edit:
    // Display the header and info of event 1
    Text event1Header(events[0].header, font2);
    event1Header.setCharacterSize(20);
    event1Header.setFillColor(Color(248, 208, 8, 255));
    event1Header.setStyle(Text::Bold);
    event1Header.setPosition(115, 255);

    Text event1Info(events[0].info, font3);
    event1Info.setCharacterSize(13);
    event1Info.setFillColor(Color(248, 208, 8, 255));
    event1Info.setStyle(Text::Bold);
    event1Info.setPosition(110, 305);

    // Display the header and info of event 2
    Text event2Header(events[1].header, font2);
    event2Header.setCharacterSize(20);
    event2Header.setFillColor(Color(22, 219, 142, 255));
    event2Header.setStyle(Text::Bold);
    event2Header.setPosition(385, 255);

    Text event2Info(events[1].info, font3);
    event2Info.setCharacterSize(13);
    event2Info.setFillColor(Color(22, 219, 142, 255));
    event2Info.setStyle(Text::Bold);
    event2Info.setPosition(380, 305);

    // Display the header and info of event 3
    Text event3Header(events[2].header, font2);
    event3Header.setCharacterSize(20);
    event3Header.setFillColor(Color(255, 100, 49, 255));
    event3Header.setStyle(Text::Bold);
    event3Header.setPosition(660, 255);

    Text event3Info(events[2].info, font3);
    event3Info.setCharacterSize(13);
    event3Info.setFillColor(Color(255, 100, 49, 255));
    event3Info.setStyle(Text::Bold);
    event3Info.setPosition(647, 305);

    // the next button is made here, its size, colour, opacity, and position are
    // set
    RectangleShape NextButton(Vector2f(350, 85));
    NextButton.setFillColor(Color(200, 200, 0, 255));
    NextButton.setPosition(1100, 700);

    // the next button's text is made here, its size, colour, boldness, and
    // position are set
    Text NextText("Next", font);
    NextText.setCharacterSize(30);
    NextText.setFillColor(Color::Black);
    NextText.setStyle(Text::Bold);
    NextText.setPosition(1100, 700);

    // checks if a dice has been placed in the boxes
    bool BoxCheck1 = 0;
    bool BoxCheck2 = 0;
    bool BoxCheck3 = 0;

    // These are the end game text for various events. Depending on if the user
    // wins or not, the text will appear
    Text Congratulations("Congratulations", font);
    Congratulations.setCharacterSize(70);
    Congratulations.setFillColor(Color::White);
    Congratulations.setStyle(Text::Bold);
    Congratulations.setPosition(50, 150);

    Text YouDied("You Died", font);
    YouDied.setCharacterSize(80);
    YouDied.setFillColor(Color::Red);
    YouDied.setStyle(Text::Bold);
    YouDied.setPosition(275, 200);

    Text EndSenarioWin("You have Survived Dice Against Survival", font);
    EndSenarioWin.setCharacterSize(25);
    EndSenarioWin.setFillColor(Color::White);
    EndSenarioWin.setStyle(Text::Bold);
    EndSenarioWin.setPosition(60, 350);

    // When money reaches 0, this ending plays
    Text EndSenarioLose1Money("You have Bankrupted your Nation", font);
    EndSenarioLose1Money.setCharacterSize(25);
    EndSenarioLose1Money.setFillColor(Color::Red);
    EndSenarioLose1Money.setStyle(Text::Bold);
    EndSenarioLose1Money.setPosition(100, 400);

    // When food reaches 0, this ending plays
    Text EndSenarioLose1Food("You have Starved your Nation", font);
    EndSenarioLose1Food.setCharacterSize(25);
    EndSenarioLose1Food.setFillColor(Color::Red);
    EndSenarioLose1Food.setStyle(Text::Bold);
    EndSenarioLose1Food.setPosition(100, 400);

    // When knowledge reaches 0, this ending plays
    Text EndSenarioLose1Knowledge(
        "You have lost All of your nations Knowledge", font);
    EndSenarioLose1Knowledge.setCharacterSize(25);
    EndSenarioLose1Knowledge.setFillColor(Color::Red);
    EndSenarioLose1Knowledge.setStyle(Text::Bold);
    EndSenarioLose1Knowledge.setPosition(100, 400);

    // When military reaches 0, this ending plays
    Text EndSenarioLose1Military(
        "You have been raided by a nearby enemy", font);
    EndSenarioLose1Military.setCharacterSize(25);
    EndSenarioLose1Military.setFillColor(Color::Red);
    EndSenarioLose1Military.setStyle(Text::Bold);
    EndSenarioLose1Military.setPosition(100, 400);

    

                       
    // Main game loop starts here, this is the basic sfml window game loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // gets the mouse(x,y) position
        float mousePositionX = Mouse::getPosition(window).x;
        float mousePositionY = Mouse::getPosition(window).y;

        // clears acreen
        window.clear();

        // this if statment checks if we are still on the first turn, if we are
        // then we can skip the starting sequence
        if (turns > 0) {
            // skips the startssequence
            goto skip;
        }
        // draws the background to the window
        window.draw(background);

        // draws the white box to the window
        window.draw(titleBox);

        // the if statement checks if the x position of the titleBox is at its
        // desired position, if it is then the box will move by xMovement
        // (float) times towards to left, until its x position is less that 50,
        // this creates the animation
        if (titleBoxPosition.x >= 50)
            titleBoxPosition.x -= xMovement;
        titleBox.setPosition(titleBoxPosition);

        // if the box has reached its desired location, we can continue drawing
        // other elements to the screen
        if (titleBoxPosition.x <= 50) {
            // draws text and dice image
            window.draw(titlePage);
            window.draw(dice);
            window.draw(textNToContinue);

     // Change system cursor from 'wait' to 'hand'
     // as the opening title animation finishes loading
     if (custom_cursor.loadFromSystem(sf::Cursor::Hand)){
      window.setMouseCursor(custom_cursor);
     }

            // check if the mouse button was pressed and if it was then Npressed
            // becomes true
            if (event.type == Event ::MouseButtonPressed) {
                Npressed = true;
            }
        }

    // title sequence ends here where the skip is
    skip:
        // checks if the starting page is over and if we aren't on the 1 turn
        // anymore
        if (Npressed || turns > 0) {
            // clears screen
            window.clear();
            // starts drawing elemnts of main game play
            // background 2 gets frawn
            window.draw(background2);
            window.draw(Box);
            // same animation as the white box but this time on the black one
            if (BoxPosition.x >= 25) {
                BoxPosition.x -= xMovement;
                Box.setPosition(BoxPosition);
            }
        }

        // check if the black box is placed where it is supposed to be
        if (BoxPosition.x <= 25 || turns > 0) {
            if (turns >= 1) {
                // if the box is placed where it is supposed to be visual
                // elements of the actual gameplay are being drawn
                window.draw(background2);
                Box.setPosition(20, 25);
                window.draw(Box);
            }
            /*NOTE: SFML uses the painter technique, elements
            are drawn ontop of one another on the window*/
            // drawing all the element of the actual game screen
            window.draw(Turn);
            window.draw(Event1);
            window.draw(Event2);
            window.draw(Event3);
            window.draw(diceBox);
            window.draw(event1Box);
            window.draw(event2Box);
            window.draw(event3Box);
            window.draw(event1Header);
            window.draw(event2Header);
            window.draw(event3Header);
            window.draw(event1Info);
            window.draw(event2Info);
            window.draw(event3Info);
            window.draw(face1);
            window.draw(face2);
            window.draw(face3);
            window.draw(face4);
            window.draw(face5);
            window.draw(face6);
            window.draw(Face1);
            window.draw(Face2);
            window.draw(Face3);
            window.draw(Face4);
            window.draw(Face5);
            window.draw(Face6);
            window.draw(Food);
            window.draw(Knowledge);
            window.draw(Military);
            window.draw(Money);
            window.draw(NextButton);
            window.draw(NextText);

            // checks what dice were randomly generated, and based off uses
            // switch statment to place the corrsponding dice image into one of
            // the three dice box spots
            for (int i = 0; i < 3; i++) {
                Vector2f dicePosition(
                    ((i * 0 + 1) * 450) + ((i - 1) * -100), 130);
                switch (diceImages[i]) {
                    // if the diceImages[i] is 1 move dice face 1 into the dice
                    // box
                case 1:
                    face1.setPosition(dicePosition);
                    Face1.setPosition(dicePosition);
                    break;

                    // if the diceImages[i] is 2 move dice face 2 into the dice
                    // box
                case 2:
                    face2.setPosition(dicePosition);
                    Face2.setPosition(dicePosition);
                    break;

                    // if the diceImages[i] is 1 move dice face 1 into the dice
                    // box
                case 3:
                    face3.setPosition(dicePosition);
                    Face3.setPosition(dicePosition);
                    break;

                    // if the diceImages[i] is 4 move dice face 4 into the dice
                    // box
                case 4:
                    face4.setPosition(dicePosition);
                    Face4.setPosition(dicePosition);
                    break;

                    // if the diceImages[i] is 5 move dice face 5 into the dice
                    // box
                case 5:
                    face5.setPosition(dicePosition);
                    Face5.setPosition(dicePosition);
                    break;

                    // if the diceImages[i] is 6 move dice face 6 into the dice
                    // box
                case 6:
                    face6.setPosition(dicePosition);
                    Face6.setPosition(dicePosition);
                    break;
                }
            }
            // draws the highlight
            window.draw(HighlightEvent1);
            window.draw(HighlightEvent2);
            window.draw(HighlightEvent3);

            // checks if the first dice face in the dice box has been clicked,
            // and if it has then increses its counter by one
            if (event.type == Event ::MouseButtonPressed &&
                550 < mousePositionX && mousePositionX < 625 &&
                130 < mousePositionY && mousePositionY < 205)
                EventDiceCounter[0]++;

            // checks if the second dice face in the dice box has been clicked,
            // and if it has then increses its counter by one
            if (event.type == Event ::MouseButtonPressed &&
                450 < mousePositionX && mousePositionX < 525 &&
                130 < mousePositionY && mousePositionY < 205)
                EventDiceCounter[1]++;

            // checks if the third dice face in the dice box has been clicked,
            // and if it has then increses its counter by one
            if (event.type == Event ::MouseButtonPressed &&
                350 < mousePositionX && mousePositionX < 425 &&
                130 < mousePositionY && mousePositionY < 205)
                EventDiceCounter[2]++;
        }

        // checks if the first dice face counter is not divisable by 2, if that
        // is true, the highlight will appear on the first dice
        if (EventDiceCounter[0] % 2 != 0) {
            HighlightEvent1.setPosition(550 - 2.5, 130 - 2.5);
            HighlightEvent2.setPosition(1100, 700);
            HighlightEvent3.setPosition(1100, 700);

            // for loop to run three times, one for ever possible box position
            for (int i = 0; i < 3; i++) {
                // checks which dice place was clicked
                if (event.type == Event ::MouseButtonPressed &&
                    ((i * 270) + 175) < mousePositionX &&
                    mousePositionX < ((i * 270) + 175) + 80 &&
                    450 < mousePositionY && mousePositionY < 525) {
                    // depending on which dice face image (1-6) is in the first
                    // place of the dice box, a centain part of the bool 2D
                    // array becomes true
                    switch (diceImages[0]) {
                    case 1:
                        EventDice[i][0] = true;
                        break;

                    case 2:
                        EventDice[i][1] = true;
                        break;

                    case 3:
                        EventDice[i][2] = true;
                        break;

                    case 4:
                        EventDice[i][3] = true;
                        break;

                    case 5:
                        EventDice[i][4] = true;
                        break;

                    case 6:
                        EventDice[i][5] = true;
                        break;
                    }
                }
            }
            //  if the first dice face counter is  divisable by 2, the highlight
            //  will not appear on the first dice
        } else
            HighlightEvent1.setPosition(1100, 700);

        // checks if the second dice face counter is not divisable by 2, if that
        // is true, the highlight will appear on the second dice
        if (EventDiceCounter[1] % 2 != 0) {
            HighlightEvent2.setPosition(450 - 2.5, 130 - 2.5);
            HighlightEvent1.setPosition(1100, 700);
            HighlightEvent3.setPosition(1100, 700);

            // for loop to run three times, one for ever possible box position
            for (int i = 0; i < 3; i++) {
                // checks which dice place was clicked
                if (event.type == Event::MouseButtonPressed &&
                    ((i * 270) + 175) < mousePositionX &&
                    mousePositionX < ((i * 270) + 175) + 80 &&
                    450 < mousePositionY && mousePositionY < 525) {
                    switch (diceImages[1]) {
                        // depending on which dice face image (1-6) is in the
                        // second place of the dice box, a centain part of the
                        // bool 2D array becomes true
                    case 1:
                        EventDice2[i][0] = true;
                        break;

                    case 2:
                        EventDice2[i][1] = true;
                        break;

                    case 3:
                        EventDice2[i][2] = true;
                        break;

                    case 4:
                        EventDice2[i][3] = true;
                        break;

                    case 5:
                        EventDice2[i][4] = true;
                        break;

                    case 6:
                        EventDice2[i][5] = true;
                        break;
                    }
                }
            }
            // if the first dice face counter is  divisable by 2, the highlight
            // will not appear on the second dice
        } else
            HighlightEvent2.setPosition(1100, 700);

        // checks if the third dice face counter is not divisable by 2, if that
        // is true, the highlight will appear on the third dice
        if (EventDiceCounter[2] % 2 != 0) {
            HighlightEvent3.setPosition(350 - 2.5, 130 - 2.5);
            HighlightEvent1.setPosition(1100, 700);
            HighlightEvent2.setPosition(1100, 700);

            // for loop to run three times, one for ever possible box position
            for (int i = 0; i < 3; i++) {
                // checks which dice place was clicked
                if (event.type == Event::MouseButtonPressed &&
                    ((i * 270) + 175) < mousePositionX &&
                    mousePositionX < ((i * 270) + 175) + 80 &&
                    450 < mousePositionY && mousePositionY < 525) {
                    // depending on which dice face image (1-6) is in the third
                    // place of the dice box, a centain part of the bool 2D
                    // array becomes true
                    switch (diceImages[2]) {
                    case 1:
                        EventDice3[i][0] = true;
                        break;

                    case 2:
                        EventDice3[i][1] = true;
                        break;

                    case 3:
                        EventDice3[i][2] = true;
                        break;

                    case 4:
                        EventDice3[i][3] = true;
                        break;

                    case 5:
                        EventDice3[i][4] = true;
                        break;

                    case 6:
                        EventDice3[i][5] = true;
                        break;
                    }
                }
            }
            //  if the first dice face counter is  divisable by 2, the highlight
            //  will not appear on the third dice
        } else
            HighlightEvent3.setPosition(1100, 700);

        // places that the dice can end up in (3 dice place holders in the
        // events boxes)
        Vector2f dicePosition2(175, 450);
        Vector2f dicePosition3(445, 450);
        Vector2f dicePosition4(715, 450);

        // dice 1
        // placemnt of dice one
        Vector2f dicePositionOfDice1;

        // if the dice value of the dice boxes' first dice is 1, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        if (EventDice[0][0]) {
            face1.setPosition(dicePosition2);
            dicePositionOfDice1 = dicePosition2;
            stats[0] = 1;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' first dice is 2, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        else if (EventDice[0][1]) {
            face2.setPosition(dicePosition2);
            dicePositionOfDice1 = dicePosition2;
            stats[0] = 2;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' first dice is 3, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        else if (EventDice[0][2]) {
            face3.setPosition(dicePosition2);
            dicePositionOfDice1 = dicePosition2;
            stats[0] = 3;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' first dice is 4, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        else if (EventDice[0][3]) {
            face4.setPosition(dicePosition2);
            dicePositionOfDice1 = dicePosition2;
            stats[0] = 4;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' first dice is 5, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        else if (EventDice[0][4]) {
            face5.setPosition(dicePosition2);
            dicePositionOfDice1 = dicePosition2;
            stats[0] = 5;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' first dice is 6, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        else if (EventDice[0][5]) {
            face6.setPosition(dicePosition2);
            dicePositionOfDice1 = dicePosition2;
            stats[0] = 6;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' first dice is 1, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice[1][0]) {
            face1.setPosition(dicePosition3);
            dicePositionOfDice1 = dicePosition3;
            stats[1] = 1;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' first dice is 2, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice[1][1]) {
            face2.setPosition(dicePosition3);
            dicePositionOfDice1 = dicePosition3;
            stats[1] = 2;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' first dice is 3, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice[1][2]) {
            face3.setPosition(dicePosition3);
            dicePositionOfDice1 = dicePosition3;
            stats[1] = 3;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' first dice is 4, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice[1][3]) {
            face4.setPosition(dicePosition3);
            dicePositionOfDice1 = dicePosition3;
            stats[1] = 4;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' first dice is 5, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice[1][4]) {
            face5.setPosition(dicePosition3);
            dicePositionOfDice1 = dicePosition3;
            stats[1] = 5;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' first dice is 6, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice[1][5]) {
            face6.setPosition(dicePosition3);
            dicePositionOfDice1 = dicePosition3;
            stats[1] = 6;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' first dice is 1, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice[2][0]) {
            face1.setPosition(dicePosition4);
            dicePositionOfDice1 = dicePosition4;
            stats[2] = 1;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        // if the dice value of the dice boxes' first dice is 2, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice[2][1]) {
            face2.setPosition(dicePosition4);
            dicePositionOfDice1 = dicePosition4;
            stats[2] = 2;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        // if the dice value of the dice boxes' first dice is 3, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice[2][2]) {
            face3.setPosition(dicePosition4);
            dicePositionOfDice1 = dicePosition4;
            stats[2] = 3;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        // if the dice value of the dice boxes' first dice is 4, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice[2][3]) {
            face4.setPosition(dicePosition4);
            dicePositionOfDice1 = dicePosition4;
            stats[2] = 4;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        // if the dice value of the dice boxes' first dice is 5, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice[2][4]) {
            face5.setPosition(dicePosition4);
            dicePositionOfDice1 = dicePosition4;
            stats[2] = 5;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        // if the dice value of the dice boxes' first dice is 6, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice[2][5]) {
            face6.setPosition(dicePosition4);
            dicePositionOfDice1 = dicePosition4;
            stats[2] = 6;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        // dice 2
        // placemnt of dice two
        Vector2f dicePositionOfDice2;

        // if the dice value of the dice boxes' second dice is 1, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        if (EventDice2[0][0]) {
            face1.setPosition(dicePosition2);
            dicePositionOfDice2 = dicePosition2;
            stats[0] = 1;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' second dice is 2, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        else if (EventDice2[0][1]) {
            face2.setPosition(dicePosition2);
            dicePositionOfDice2 = dicePosition2;
            stats[0] = 2;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' second dice is 3, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        else if (EventDice2[0][2]) {
            face3.setPosition(dicePosition2);
            dicePositionOfDice2 = dicePosition2;
            stats[0] = 3;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' second dice is 4, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        else if (EventDice2[0][3]) {
            face4.setPosition(dicePosition2);
            dicePositionOfDice2 = dicePosition2;
            stats[0] = 4;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' second dice is 5, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        else if (EventDice2[0][4]) {
            face5.setPosition(dicePosition2);
            dicePositionOfDice2 = dicePosition2;
            stats[0] = 5;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' second dice is 6, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        else if (EventDice2[0][5]) {
            face6.setPosition(dicePosition2);
            dicePositionOfDice2 = dicePosition2;
            stats[0] = 6;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' second dice is 1, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice2[1][0]) {
            face1.setPosition(dicePosition3);
            dicePositionOfDice2 = dicePosition3;
            stats[1] = 1;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' second dice is 2, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice2[1][1]) {
            face2.setPosition(dicePosition3);
            dicePositionOfDice2 = dicePosition3;
            stats[1] = 2;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' second dice is 3, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice2[1][2]) {
            face3.setPosition(dicePosition3);
            dicePositionOfDice2 = dicePosition3;
            stats[1] = 3;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' second dice is 4, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice2[1][3]) {
            face4.setPosition(dicePosition3);
            dicePositionOfDice2 = dicePosition3;
            stats[1] = 4;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' second dice is 5, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice2[1][4]) {
            face5.setPosition(dicePosition3);
            dicePositionOfDice2 = dicePosition3;
            stats[1] = 5;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' second dice is 6, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice2[1][5]) {
            face6.setPosition(dicePosition3);
            dicePositionOfDice2 = dicePosition3;
            stats[1] = 6;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' second dice is 1, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice2[2][0]) {
            face1.setPosition(dicePosition4);
            dicePositionOfDice2 = dicePosition4;
            stats[2] = 1;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        // if the dice value of the dice boxes' second dice is 2, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice2[2][1]) {
            face2.setPosition(dicePosition4);
            dicePositionOfDice2 = dicePosition4;
            stats[2] = 2;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        // if the dice value of the dice boxes' second dice is 3, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice2[2][2]) {
            face3.setPosition(dicePosition4);
            dicePositionOfDice2 = dicePosition4;
            stats[2] = 3;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        // if the dice value of the dice boxes' second dice is 4, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice2[2][3]) {
            face4.setPosition(dicePosition4);
            dicePositionOfDice2 = dicePosition4;
            stats[2] = 4;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        // if the dice value of the dice boxes' second dice is 5, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice2[2][4]) {
            face5.setPosition(dicePosition4);
            dicePositionOfDice2 = dicePosition4;
            stats[2] = 5;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        // if the dice value of the dice boxes' second dice is 6, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice2[2][5]) {
            face6.setPosition(dicePosition4);
            dicePositionOfDice2 = dicePosition4;
            stats[2] = 6;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        // dice 3
        // placemnt of dice three
        Vector2f dicePositionOfDice3;

        // if the dice value of the dice boxes' third dice is 1, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        if (EventDice3[0][0]) {
            face1.setPosition(dicePosition2);
            dicePositionOfDice3 = dicePosition2;
            stats[0] = 1;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' third dice is 2, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        else if (EventDice3[0][1]) {
            face2.setPosition(dicePosition2);
            dicePositionOfDice3 = dicePosition2;
            stats[0] = 2;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' third dice is 3, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        else if (EventDice3[0][2]) {
            face3.setPosition(dicePosition2);
            dicePositionOfDice3 = dicePosition2;
            stats[0] = 3;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' third dice is 4, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        else if (EventDice3[0][3]) {
            face4.setPosition(dicePosition2);
            dicePositionOfDice3 = dicePosition2;
            stats[0] = 4;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' third dice is 5, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        else if (EventDice3[0][4]) {
            face5.setPosition(dicePosition2);
            dicePositionOfDice3 = dicePosition2;
            stats[0] = 5;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' third dice is 6, and it in
        // placed in the first event box, set the dice to that box, and store
        // that dice value in the stats[0], also BoxCheck1 is now true because a
        // dice was placed in it
        else if (EventDice3[0][5]) {
            face6.setPosition(dicePosition2);
            dicePositionOfDice3 = dicePosition2;
            stats[0] = 6;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck1 = true;
        }

        // if the dice value of the dice boxes' third dice is 1, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice3[1][0]) {
            face1.setPosition(dicePosition3);
            dicePositionOfDice3 = dicePosition3;
            stats[1] = 1;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' third dice is 2, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice3[1][1]) {
            face2.setPosition(dicePosition3);
            dicePositionOfDice3 = dicePosition3;
            stats[1] = 2;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' third dice is 3, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice3[1][2]) {
            face3.setPosition(dicePosition3);
            dicePositionOfDice3 = dicePosition3;
            stats[1] = 3;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' third dice is 4, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice3[1][3]) {
            face4.setPosition(dicePosition3);
            dicePositionOfDice3 = dicePosition3;
            stats[1] = 4;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' third dice is 5, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice3[1][4]) {
            face5.setPosition(dicePosition3);
            dicePositionOfDice3 = dicePosition3;
            stats[1] = 5;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' third dice is 6, and it in
        // placed in the second event box, set the dice to that box, and store
        // that dice value in the stats[1], also BoxCheck2 is now true because a
        // dice was placed in it
        else if (EventDice3[1][5]) {
            face6.setPosition(dicePosition3);
            dicePositionOfDice3 = dicePosition3;
            stats[1] = 6;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck2 = true;
        }

        // if the dice value of the dice boxes' third dice is 1, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice3[2][0]) {
            face1.setPosition(dicePosition4);
            dicePositionOfDice3 = dicePosition4;
            stats[2] = 1;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        // if the dice value of the dice boxes' third dice is 2, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice3[2][1]) {
            face2.setPosition(dicePosition4);
            dicePositionOfDice3 = dicePosition4;
            stats[2] = 2;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        // if the dice value of the dice boxes' third dice is 3, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice3[2][2]) {
            face3.setPosition(dicePosition4);
            dicePositionOfDice3 = dicePosition4;
            stats[2] = 3;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        // if the dice value of the dice boxes' third dice is 4, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice3[2][3]) {
            face4.setPosition(dicePosition4);
            dicePositionOfDice3 = dicePosition4;
            stats[2] = 4;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        // if the dice value of the dice boxes' third dice is 5, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice3[2][4]) {
            face5.setPosition(dicePosition4);
            dicePositionOfDice3 = dicePosition4;
            stats[2] = 5;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        // if the dice value of the dice boxes' third dice is 6, and it in
        // placed in the third event box, set the dice to that box, and store
        // that dice value in the stats[2], also BoxCheck3 is now true because a
        // dice was placed in it
        else if (EventDice3[2][5]) {
            face6.setPosition(dicePosition4);
            dicePositionOfDice3 = dicePosition4;
            stats[2] = 6;
            setEventEffects(stats[0], stats[1], stats[2]);
            BoxCheck3 = true;
        }

        if (BoxCheck1) {
            Text event1Effect(events[0].effect, font3);
            event1Effect.setCharacterSize(20);
            event1Effect.setFillColor(Color::White);
            event1Effect.setStyle(Text::Italic);
            event1Effect.setPosition(120, 400);
            window.draw(event1Effect);
        }

        if (BoxCheck2) {
            Text event2Effect(events[1].effect, font3);
            event2Effect.setCharacterSize(20);
            event2Effect.setFillColor(Color::White);
            event2Effect.setStyle(Text::Italic);
            event2Effect.setPosition(400, 400);
            window.draw(event2Effect);
        }

        if (BoxCheck3) {
            Text event3Effect(events[2].effect, font3);
            event3Effect.setCharacterSize(20);
            event3Effect.setFillColor(Color::White);
            event3Effect.setStyle(Text::Italic);
            event3Effect.setPosition(670, 400);
            window.draw(event3Effect);
        }

        // if all three boxes have a dice in it, show the next button and text
        // and make the next button bool true
        if (BoxCheck1 && BoxCheck2 && BoxCheck3) {
            NextButton.setPosition(310, 125);
            NextText.setPosition(440, 150);
            nextButtonBool = true;
        }
        // if the next button was pressed after it appeared call the
        // applyEffects();, and mkae the array true
        if (event.type == Event::MouseButtonPressed && 310 < mousePositionX &&
            mousePositionX < 660 && 125 < mousePositionY &&
            mousePositionY < 210 && nextButtonBool) {
            NextPressed[turns] = true;
            applyEffects();
        }
        // if any of the stats are not less that 0 and NextPressed[turns] is
        // true, (which means the user pressed the next button), and less than
        // 10 turns have past, then reset the next button, all the dice faces,
        // and all the highlights and increase the turns by 1, ad reset the game
        // turns
        if (NextPressed[turns] && turns < 10 && food > 0 && military > 0 &&
            money > 0 && knowledge > 0) {
            NextButton.setPosition(1100, 700);
            NextText.setPosition(1100, 700);
            face1.setPosition(1100, 700);
            face2.setPosition(1100, 700);
            face3.setPosition(1100, 700);
            face4.setPosition(1100, 700);
            face5.setPosition(1100, 700);
            face6.setPosition(1100, 700);
            Face1.setPosition(1100, 700);
            Face2.setPosition(1100, 700);
            Face3.setPosition(1100, 700);
            Face4.setPosition(1100, 700);
            Face5.setPosition(1100, 700);
            Face6.setPosition(1100, 700);
            HighlightEvent1.setPosition(1100, 700);
            HighlightEvent2.setPosition(1100, 700);
            HighlightEvent3.setPosition(1100, 700);
            turns++;
            goto start;
        }
        // if ten turns have been completed, the user wins
        if (turns >= 10) {
            window.clear();
            window.draw(Congratulations);
            window.draw(EndSenarioWin);
        }

        // if food goes below 0 then the user loses with the food ending
        if (food <= 0) {
            window.clear();
            window.draw(YouDied);
            window.draw(EndSenarioLose1Food);
        }

        // if military goes below 0 then the user loses with the military ending
        if (military <= 0) {
            window.clear();
            window.draw(YouDied);
            window.draw(EndSenarioLose1Military);
        }

        // if money goes below 0 then the user loses with the money ending
        if (money <= 0) {
            window.clear();
            window.draw(YouDied);
            window.draw(EndSenarioLose1Money);
        }

        // if knowledge goes below 0 then the user loses with the knowledge
        // ending
        if (knowledge <= 0) {
            window.clear();
            window.draw(YouDied);
            window.draw(EndSenarioLose1Knowledge);
        }
        // end game loop
        window.display();
    }
    // return 0
    return 0;
}
