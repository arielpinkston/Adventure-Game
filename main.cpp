#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <random>

using namespace std;

enum en_DIRS {NORTH, EAST, SOUTH, WEST};
enum en_ROOMS {SPORTSHOP, CASINO, CARPARK, LOBBY, RESTAURANT, CORRIDOR, STOREROOM, POOL, GARDEN, POND, PUMPROOM, RECROOM, GREENHOUSE, STAFFROOM, COURTYARD, HALLWAY, MENSRR, WOMENSRR, SHOPPINGCENTER, MEDICALOFFICE};
enum en_VERBS {GET, DROP, USE, OPEN, CLOSE, EXAMINE, INVENTORY, LOOK};
enum en_NOUNS {STORE_DOOR, MAGNET, METER, ROULETTE, MONEY, FISHROD, CARKEYS, GOGGLES, BOUQUET, STONE, WRENCH, SHEARS, DONUT, BANDAGE, MPIECE};

const int NONE= -1;
const int DIRS= 4;
const int ROOMS= 20;
const int VERBS= 9;
const int NOUNS= 15;

bool debug = false;

class words {
private:
    string word;
    int code;
  public:
    words() {
        word = "";
        code = 0;
      }
    words(string w, int c) {
        word = w;
        code = c;
    }
    ~words() {} // destructor
    void setWord(string w) {
        word = w;
    }

    void setCode(int c) {
        code = c;
    }

    string getWord() {
        return word;
    }

    int getCode() {
        return code;
    }
};

class room {
private:
    string description;
    string descriptionMore;
    int exits[DIRS];
public:
    room() {
      description = "";
      descriptionMore = "";
      for (int i = 0; i < DIRS; i++) {
          setExit(i, NONE);
      }
    }
    room(string desc, string descMore) {
        description = desc;
        descriptionMore = descMore;
        for (int i = 0; i < DIRS; i++) {
            setExit(i, NONE);
        }
    }
    ~room() {} // destructor
    void setDescription(string desc) {
        description = desc;
    }

    void setDescriptionMore(string descMore) {
        descriptionMore = descMore;
    }

    void setExit(int index, int roomCode) {
        exits[index] = roomCode;
    }

    string getDescription() {
        return description;
    }

    string getDescriptionMore() {
        return descriptionMore;
    }

    int getExit(int index) {
      return exits[index];
    }
};

class noun {
private:
    string word;
    string description;
    string descriptionMore;
    int code;
    int location;
    bool can_carry;
public:
    noun() {
      word = "";
      description = "";
      descriptionMore = "";
      code = 0;
      location = 0;
      can_carry = false;
    }
    noun(string w, string desc, string descMore, int c, int loc, bool carry) {
        word = w;
        description = desc;
        descriptionMore = descMore;
        code = c;
        location = loc;
        can_carry = carry;
    }
    ~noun() {} // destructor
    void setWord(string w) {
        word = w;
    }

    void setDescription(string desc) {
        description = desc;
    }

    void setDescriptionMore(string descMore) {
        descriptionMore = descMore;
    }

    void setCode(int c) {
        code = c;
    }

    void setLocation(int loc) {
        location = loc;
    }

    void setCanCarry(bool carry) {
        can_carry = carry;
    }

    string getWord() {
        return word;
    }

    string getDescription() {
        return description;
    }

    string getDescriptionMore() {
        return descriptionMore;
    }    

    int getCode() {
        return code;
    }

    int getLocation() {
        return location;
    }

    bool getCanCarry() {
        return can_carry;
    }
};

vector<noun> inventory;

void set_rooms(room *rms) {
    rms[SPORTSHOP].setDescription("sports shop");
    rms[SPORTSHOP].setExit(NORTH, SHOPPINGCENTER);
    rms[SPORTSHOP].setExit(EAST, NONE);
    rms[SPORTSHOP].setExit(SOUTH, CARPARK);
    rms[SPORTSHOP].setExit(WEST, NONE);

    rms[CASINO].setDescription("bustling casino");
    rms[CASINO].setExit(NORTH, NONE);
    rms[CASINO].setExit(EAST, RECROOM);
    rms[CASINO].setExit(SOUTH, LOBBY);
    rms[CASINO].setExit(WEST, NONE);

    rms[CARPARK].setDescription("car park");
    rms[CARPARK].setExit(NORTH, SPORTSHOP);
    rms[CARPARK].setExit(EAST, LOBBY);
    rms[CARPARK].setExit(SOUTH, STAFFROOM);
    rms[CARPARK].setExit(WEST, NONE);

    rms[LOBBY].setDescription("hotel lobby");
    rms[LOBBY].setExit(NORTH, CASINO);
    rms[LOBBY].setExit(EAST, RESTAURANT);
    rms[LOBBY].setExit(SOUTH, CORRIDOR);
    rms[LOBBY].setExit(WEST, CARPARK);

    rms[RESTAURANT].setDescription("restaurant");
    rms[RESTAURANT].setExit(NORTH, RECROOM);
    rms[RESTAURANT].setExit(EAST, NONE);
    rms[RESTAURANT].setExit(SOUTH, GREENHOUSE);
    rms[RESTAURANT].setExit(WEST, LOBBY);

    rms[CORRIDOR].setDescription("corridor");
    rms[CORRIDOR].setExit(NORTH, LOBBY);
    rms[CORRIDOR].setExit(EAST, GREENHOUSE);
    rms[CORRIDOR].setExit(SOUTH, GARDEN);
    rms[CORRIDOR].setExit(WEST, STAFFROOM);

    rms[STOREROOM].setDescription ("secret store room");
    rms[STOREROOM].setExit(NORTH, NONE);
    rms[STOREROOM].setExit(EAST, NONE);
    rms[STOREROOM].setExit(SOUTH, NONE);
    rms[STOREROOM].setExit(WEST, NONE);

    rms[POOL].setDescription("swimming pool area");
    rms[POOL].setExit(NORTH, STAFFROOM);
    rms[POOL].setExit(EAST, GARDEN);
    rms[POOL].setExit(SOUTH, PUMPROOM);
    rms[POOL].setExit(WEST, HALLWAY);

    rms[GARDEN].setDescription("tranquil garden");
    rms[GARDEN].setExit(NORTH, CORRIDOR);
    rms[GARDEN].setExit(EAST, POND);
    rms[GARDEN].setExit(SOUTH, NONE);
    rms[GARDEN].setExit(WEST, POOL);

    rms[POND].setDescription("patio with a fish pond");
    rms[POND].setExit(NORTH, GREENHOUSE);
    rms[POND].setExit(EAST, NONE);
    rms[POND].setExit(SOUTH, COURTYARD);
    rms[POND].setExit(WEST, GARDEN);

    rms[PUMPROOM].setDescription("damp pump room");
    rms[PUMPROOM].setExit(NORTH, POOL);
    rms[PUMPROOM].setExit(EAST, NONE);
    rms[PUMPROOM].setExit(SOUTH, NONE);
    rms[PUMPROOM].setExit(WEST, NONE);

    rms[RECROOM].setDescription("rec room filled with board games");
    rms[RECROOM].setExit(NORTH, NONE);
    rms[RECROOM].setExit(EAST, NONE);
    rms[RECROOM].setExit(SOUTH, RESTAURANT);
    rms[RECROOM].setExit(WEST, CASINO);

    rms[GREENHOUSE].setDescription("humid greenhouse");
    rms[GREENHOUSE].setExit(NORTH, RESTAURANT);
    rms[GREENHOUSE].setExit(EAST, NONE);
    rms[GREENHOUSE].setExit(SOUTH, POND);
    rms[GREENHOUSE].setExit(WEST, CORRIDOR);

    rms[STAFFROOM].setDescription("low lit break room with half empty vending machines");
    rms[STAFFROOM].setExit(NORTH, CARPARK);
    rms[STAFFROOM].setExit(EAST, CORRIDOR);
    rms[STAFFROOM].setExit(SOUTH, POOL);
    rms[STAFFROOM].setExit(WEST, NONE);

    rms[COURTYARD].setDescription("sprawling courtyard filled with blooming flowers");
    rms[COURTYARD].setExit(NORTH, POND);
    rms[COURTYARD].setExit(EAST, NONE);
    rms[COURTYARD].setExit(SOUTH, NONE);
    rms[COURTYARD].setExit(WEST, NONE);

    rms[MENSRR].setDescription("men's restroom");
    rms[MENSRR].setExit(NORTH, NONE);
    rms[MENSRR].setExit(EAST, NONE);
    rms[MENSRR].setExit(SOUTH, HALLWAY);
    rms[MENSRR].setExit(WEST, NONE);

    rms[HALLWAY].setDescription("hallway");
    rms[HALLWAY].setExit(NORTH, MENSRR);
    rms[HALLWAY].setExit(EAST, POOL);
    rms[HALLWAY].setExit(SOUTH, WOMENSRR);
    rms[HALLWAY].setExit(WEST, MEDICALOFFICE);

    rms[WOMENSRR].setDescription("women's restroom");
    rms[WOMENSRR].setExit(NORTH, HALLWAY);
    rms[WOMENSRR].setExit(EAST, NONE);
    rms[WOMENSRR].setExit(SOUTH, NONE);
    rms[WOMENSRR].setExit(WEST, NONE);

    rms[SHOPPINGCENTER].setDescription("crowded shopping center lined with stores");
    rms[SHOPPINGCENTER].setExit(NORTH, NONE);
    rms[SHOPPINGCENTER].setExit(EAST, NONE);
    rms[SHOPPINGCENTER].setExit(SOUTH, SPORTSHOP);
    rms[SHOPPINGCENTER].setExit(WEST, NONE);

    rms[MEDICALOFFICE].setDescription("quiet clinic filled with medical supplies");
    rms[MEDICALOFFICE].setExit(NORTH, NONE);
    rms[MEDICALOFFICE].setExit(EAST, HALLWAY);
    rms[MEDICALOFFICE].setExit(SOUTH, NONE);
    rms[MEDICALOFFICE].setExit(WEST, NONE);
}

void set_directions(words *dir) {
    dir[NORTH].setCode(NORTH);
    dir[NORTH].setWord("NORTH");

    dir[EAST].setCode(EAST);
    dir[EAST].setWord("EAST");

    dir[SOUTH].setCode(SOUTH);
    dir[SOUTH].setWord("SOUTH");

    dir[WEST].setCode(WEST);
    dir[WEST].setWord("WEST");
}

void set_verbs(words *vbs) {
//enum en_VERBS {GET, DROP, USE, OPEN , CLOSE, EXAMINE, INVENTORY, LOOK};
    vbs[GET].setCode(GET);
    vbs[GET].setWord("GET");
    vbs[DROP].setCode(DROP);
    vbs[DROP].setWord("DROP");
    vbs[USE].setCode(USE);
    vbs[USE].setWord("USE");
    vbs[OPEN].setCode(OPEN);
    vbs[OPEN].setWord("OPEN");
    vbs[CLOSE].setCode(CLOSE);
    vbs[CLOSE].setWord("CLOSE");
    vbs[EXAMINE].setCode(EXAMINE);
    vbs[EXAMINE].setWord("EXAMINE");
    vbs[INVENTORY].setCode(INVENTORY);
    vbs[INVENTORY].setWord("INVENTORY");
    vbs[LOOK].setCode(LOOK);
    vbs[LOOK].setWord("LOOK");
}

void set_nouns(noun *nns) {
//enum en_NOUNS {STORE_DOOR, MAGNET, METER , ROULETTE, MONEY, FISHROD};
    nns[STORE_DOOR].setWord("DOOR");
    nns[STORE_DOOR].setCode(STORE_DOOR);
    nns[STORE_DOOR].setDescription(" a closed store room door");
    nns[STORE_DOOR].setCanCarry(false);
    nns[STORE_DOOR].setLocation(CORRIDOR);

    nns[MAGNET].setWord("MAGNET");
    nns[MAGNET].setCode(MAGNET);
    nns[MAGNET].setDescription(" a magnet");
    nns[MAGNET].setCanCarry(true);
    nns[MAGNET].setLocation(LOBBY);

    nns[METER].setWord("METER");
    nns[METER].setCode(METER);
    nns[METER].setDescription(" a parking meter");
    nns[METER].setCanCarry(false);
    nns[METER].setLocation(CARPARK);

    nns[ROULETTE].setWord("ROULETTE");
    nns[ROULETTE].setCode(ROULETTE);
    nns[ROULETTE].setDescription(" a roulette wheel");
    nns[ROULETTE].setCanCarry(false);
    nns[ROULETTE].setLocation(CASINO);

    nns[MONEY].setWord("MONEY");
    nns[MONEY].setCode(MONEY);
    nns[MONEY].setDescription(" some money");
    nns[MONEY].setCanCarry(true);
    nns[MONEY].setLocation(CASINO);

    nns[FISHROD].setWord("ROD");
    nns[FISHROD].setCode(FISHROD);
    nns[FISHROD].setDescription(" a fishing rod");
    nns[FISHROD].setCanCarry(false);
    nns[FISHROD].setLocation(SPORTSHOP);

    // CARKEYS
    nns[CARKEYS].setWord("CARKEYS");
    nns[CARKEYS].setCode(CARKEYS);
    nns[CARKEYS].setDescription(" carkeys");
    nns[CARKEYS].setCanCarry(true);
    nns[CARKEYS].setLocation(CARPARK);
    nns[CARKEYS].setDescriptionMore("the keys are attached to a crowded keyring full of tassles, keychains, odds, and ends");

    // GOGGLES
    nns[GOGGLES].setWord("GOGGLES");
    nns[GOGGLES].setCode(GOGGLES);
    nns[GOGGLES].setDescription(" neon pink goggles");
    nns[GOGGLES].setCanCarry(true);
    nns[GOGGLES].setLocation(POOL);

    // BOUQUET
    nns[BOUQUET].setWord("BOUQUET");
    nns[BOUQUET].setCode(BOUQUET);
    nns[BOUQUET].setDescription(" a bouquet of flowers");
    nns[BOUQUET].setCanCarry(true);
    nns[BOUQUET].setLocation(COURTYARD);

    // STONE
    nns[STONE].setWord("STONE");
    nns[STONE].setCode(STONE);
    nns[STONE].setDescription(" a round, smooth stone");
    nns[STONE].setCanCarry(true);
    nns[STONE].setLocation(POND);

    // WRENCH
    nns[WRENCH].setWord("WRENCH");
    nns[WRENCH].setCode(WRENCH);
    nns[WRENCH].setDescription(" a wrench");
    nns[WRENCH].setCanCarry(true);
    nns[WRENCH].setLocation(PUMPROOM);

    // SHEARS
    nns[SHEARS].setWord("SHEARS");
    nns[SHEARS].setCode(SHEARS);
    nns[SHEARS].setDescription(" a pair of metal shears");
    nns[SHEARS].setCanCarry(true);
    nns[SHEARS].setLocation(GREENHOUSE);

    // DONUT
    nns[DONUT].setWord("DONUT");
    nns[DONUT].setCode(DONUT);
    nns[DONUT].setDescription(" a half eaten donut");
    nns[DONUT].setCanCarry(true);
    nns[DONUT].setLocation(STAFFROOM);

    // MPIECE
    nns[MPIECE].setWord("MPIECE");
    nns[MPIECE].setCode(MPIECE);
    nns[MPIECE].setDescription(" a silver monopoly piece");
    nns[MPIECE].setCanCarry(true);
    nns[MPIECE].setLocation(RECROOM);

    // BANDAGE
    nns[BANDAGE].setWord("BANDAGE");
    nns[BANDAGE].setCode(BANDAGE);
    nns[BANDAGE].setDescription(" a roll of bandages");
    nns[BANDAGE].setCanCarry(true);
    nns[BANDAGE].setLocation(MEDICALOFFICE);
}

void section_command(string Cmd, string &wd1, string &wd2) {
    string sub_str;
    vector<string> words;
    char search = ' ';
    short int i, j;

    for (i = 0; i < Cmd.size(); i++) {
        if (Cmd.at(i) != search) {
            sub_str.insert(sub_str.end(), Cmd.at(i));
        }
        if (i == Cmd.size() - 1) {
            words.push_back(sub_str);
            sub_str.clear();
        }
        if (Cmd.at(i) == search) {
            words.push_back(sub_str);
            sub_str.clear();
        }
    }

    for (i = words.size() - 1; i > 0; i--) {
        if (words.at(i) == "") {
            words.erase(words.begin() + i);
        }
    }

    for (i = 0; i < words.size(); i++) {
        for (j = 0; j < words.at(i).size(); j++) {
            if(islower(words.at(i).at(j))) {
                words.at(i).at(j) = toupper(words.at(i).at(j));
            }
        }
    }

    if (words.size() == 0) {
        cout << "No command given " << endl;
    }
    if (words.size() == 1) {
        wd1 = words.at(0);
    }
    if (words.size() == 2) {
        wd1 = words.at(0);
        wd2 = words.at(1);
    }
    if (words.size() > 2) {
        cout << "Command too long. Only type one or two words (direction or verb and noun)" << endl;
    }
}

void look_around(int loc, room *rms, words *dir, noun *nns) {
    int i;
    cout << "I am in a " << rms[loc].getDescription() << "." << endl;

    for (i = 0; i < DIRS; i++) {
        if (rms[loc].getExit(i) != NONE) {
            cout << "There is an exit " << dir[i].getWord() << " to a " << 
            rms[rms[loc].getExit(i)].getDescription() << "." << endl;
        }
    }

    //The look command should check which objects (nouns) are in the current 
    //room and report them to the player.
    for (i = 0; i < NOUNS; i++) {
        if (nns[i].getLocation() == loc) {
        cout << "I see" << nns[i].getDescription() << "." << endl;
        }
    }

}

bool parser(int &loc, string wd1, string wd2, words *dir, words *vbs, room *rms, noun *nns) {
    int VERB_ACTION = NONE;
    int NOUN_MATCH = NONE;
    static bool doorisOpen = false; //false is closed door.

    int i;

    for (i = 0; i < DIRS; i++) {
        if (wd1 == dir[i].getWord()) {
            if (rms[loc].getExit(dir[i].getCode()) != NONE) {
                loc = rms[loc].getExit(dir[i].getCode());

                if (loc == STOREROOM || loc == CORRIDOR) {
                    nns[STORE_DOOR].setLocation(loc);
                }
                return true;
                cout << "I am now in a " << rms[loc].getDescription() << "." << endl;
            } else {
                cout << "No exit that way." << endl;
                return false;
            }
        }
    }

    for (i = 0; i < VERBS; i ++) {
        if (wd1 == vbs[i].getWord()) {
            VERB_ACTION = vbs[i].getCode();
            break;
        }
    }

    if (wd2 != "") {
        for (i = 0; i < NOUNS; i ++) {
            if (wd2 == nns[i].getWord()) {
                NOUN_MATCH = nns[i].getCode();
                break;
            }
        }
    }

    if (VERB_ACTION == NONE) {
        cout << "No valid command entered." << endl;
        return true;
    }

    if (VERB_ACTION == LOOK) {
        look_around(loc, rms, dir, nns);
        return true;
    }

    if (VERB_ACTION == OPEN) {
        if (NOUN_MATCH == STORE_DOOR) {
            if (loc == CORRIDOR || loc == STOREROOM) {
                if (doorisOpen == false) {
                    doorisOpen = true;
                    rms[CORRIDOR].setExit(EAST, STOREROOM);
                    rms[STOREROOM].setExit(WEST, CORRIDOR);
                    // nns[STORE_DOOR].setDescription().clear();
                    nns[STORE_DOOR].setDescription(" an open store room door to the east");
                    cout << "I have opened the door." << endl;
                    return true;
                } else if (doorisOpen == true) {
                    cout << "The door is already open." << endl;
                    return true;
                }
            } else {
                cout << "There is no door to open here." << endl;
                return true;
            }
        } else {
            cout << "Opening that is not possible." << endl;
            return true;
        }
    }

    if (VERB_ACTION == CLOSE) {
        if (NOUN_MATCH == STORE_DOOR) {
            if (loc == CORRIDOR || loc == STOREROOM) {
                if (doorisOpen == true) {
                  doorisOpen = false;
                  rms[CORRIDOR].setExit(EAST, NONE);
                  rms[STOREROOM].setExit(WEST, NONE);
                  nns[STORE_DOOR].setDescription(" a closed store room door");
                  cout << "I have closed the door." << endl;
                  return true;
                }
                else if (doorisOpen == false) {
                  cout << "The door is already closed." << endl;
                  return true;
                }
            }
            else {
              cout << "There is no door to close here."  << endl;
              return true;
            }
          }
        else {
          cout << "Closing that is no possible" << endl;
          return true;
        }
    }

    if (VERB_ACTION == GET) {
        if (NOUN_MATCH != NONE) {
            if (nns[NOUN_MATCH].getCanCarry()) {
                inventory.push_back(nns[NOUN_MATCH]);
                cout << "I picked up" << nns[NOUN_MATCH].getDescription() << endl;
                nns[NOUN_MATCH].setLocation(-1);
                return true;
            } else {
                cout << "I can't pick that up." << endl;
                return true;
            }
        } else {
            cout << "Nothing to pick up." << endl;
            return true;
        }
    }

    if (VERB_ACTION == INVENTORY) {
        cout << "Current inventory: " << endl;
        for (int i = 0; i < inventory.size(); ++i) {
            cout << "  " << inventory[i].getDescription() << endl;
          }
    }

    if (VERB_ACTION == DROP) {
        if (NOUN_MATCH != NONE) {
            cout << "Dropping" << nns[NOUN_MATCH].getDescription() << endl;
            nns[NOUN_MATCH].setLocation(loc);
            for (auto it = inventory.begin(); it != inventory.end(); ++it) {
                if (it->getCode() == NOUN_MATCH) {
                    inventory.erase(it);
                    break;
                }
            }
        }
    }

    if (VERB_ACTION == EXAMINE) {

        if (NOUN_MATCH != NONE) { 
          if (nns[NOUN_MATCH].getCanCarry() == true) {
            if (nns[NOUN_MATCH].getLocation() == loc) {
                    cout << "You examine the" << nns[NOUN_MATCH].getDescription() << ": " << nns[NOUN_MATCH].getDescriptionMore() << "." << endl;
            }
          }
        } 
        else {
            cout << "Nothing to examine." << endl;
        }

    }

    return false;
}

int main() {
    if (debug) cout << "starting main()" << endl;

    string command;
    string word_1;
    string word_2;

    room rooms[ROOMS];
    set_rooms(rooms);

    words directions[DIRS];
    set_directions(directions);

    words verbs[VERBS];
    set_verbs(verbs);

    noun nouns[NOUNS];
    set_nouns(nouns);

    int location = CARPARK;

    while (word_1 != "QUIT") { // Change "QUIT" to quit the loop
        command.clear();
        cout << "Availible commands:";
        cout << "\n  LOOK\n  GET\n  DROP\n  OPEN DOOR\n  CLOSE DOOR\n  INVENTORY\n  To travel, type desired direction (ex: SOUTH)\n";
        cout << "What shall I do? ";
        getline(cin, command);

        word_1.clear();
        word_2.clear();

        section_command(command, word_1, word_2);
        if (word_1 != "QUIT") {
            parser(location, word_1, word_2, directions, verbs, rooms, nouns);
        }
    }

        return 0;
}
