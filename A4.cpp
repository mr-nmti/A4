#include <iostream>
#include <vector>
#include <string>

/*

A) get_command refactor?
B) every round should get infinite commands, not just one!
C) class round should be added which contains whole game?

*/

// chagne go_status_command argumant to play_status or sth else


/*
A) functionilze buy error handling
B) find_player_index_by_username inside Round??    should give buy function the Round obj
*/
using namespace std;

constexpr int ALL_WEAPON_NUMBER = 3;
constexpr int UNDEFINED_WEAPON = 0;
constexpr int HEAVY_WEAPON = 1;
constexpr int PISTOL = 2;
constexpr int KNIFE = 3;
constexpr int UNBUYABLE = -1;

//constexpr int TERRORIST = 1;
//constexpr int COUNTER_TERRORIST = 2;
const string TERRORIST = "terrorist";
const string COUNTER_TERRORIST = "counter-terrorist";
constexpr int AFK = 0;
constexpr int ATK = 1;
constexpr long int INITIAL_MONEY = 1000;
constexpr int INITIAL_HEALTH = 100;
constexpr int INITIAL_TAG = -1;
constexpr int GAME_NOT_STARTED = 0;
constexpr int GAME_STARTED = 1;
const string TOKENS_DELIMITER = " ";
const string START_OUTPUT_MESSAGE = "fight!";
const string USER_NOT_FOUND_MESSAGE = "user not available";
const string GO_STATUS_MESSAGE = "ok";
const string BUY_WEAPON_AFTER_START_MESSAGE = "you can't buy weapons anymore";
const string WEAPON_NOT_FOUND_MESSAGE = "weapon not available";
class Weapon
{
    public:
        Weapon(int type);
        Weapon() : Weapon(UNDEFINED_WEAPON){};

        string get_name() { return name; }
    private:
        string name;
        int type;
        long int price;
        int damage;
        int kill_prize;
};

Weapon::Weapon(int in_type)
{
    switch (in_type)
    {
        case HEAVY_WEAPON:
            name = "heavy";
            type = HEAVY_WEAPON;
            price = 3000;
            damage = 45;
            kill_prize = 100;
            break;
        
        case PISTOL:
            name = "pistol";
            type = PISTOL;
            price = 400;
            damage = 20;
            kill_prize = 200;
            break;

        case KNIFE:
            name = "knife";
            type = KNIFE;
            price = UNBUYABLE;
            damage = 35;
            kill_prize = 500;
            break;
        
        default:
            name = "";
            type = UNDEFINED_WEAPON;
            price = UNDEFINED_WEAPON;
            damage = UNDEFINED_WEAPON;
            kill_prize = UNDEFINED_WEAPON;
    }
}




class Player
{
    public:
        Player(string in_username, string in_team);
        //todo
        string get_username() { return username; }
        long int get_money() { return money; }
        int get_health() { return health; }
        int get_play_status() { return play_status; }
        int get_tag() { return tag; }
        vector<Weapon> get_weapons() { return weapons; }
        void set_play_status(int status) { play_status = status; }; 
        void set_tag(int in_tag) { tag = in_tag; }
    private:
        string username;
        string team;
        long int money;
        int health;
        int play_status;
        int death_count;
        int kill_count;
        vector<Weapon> weapons;
        int tag;
};

Player::Player(string in_username, string in_team)
{
    username = in_username;
    team = in_team;
    money = INITIAL_MONEY;
    health = INITIAL_HEALTH;
    play_status = ATK;
    death_count = 0;
    kill_count = 0;
    tag = INITIAL_TAG;
}

class Round
{
    public:
        // todo
        Round(int r_num) { round_number = r_num; }
        void add_user_command(string in_username, string in_team);
        int find_player_index_by_username(string in_username);
        int find_weapon_index_by_name(string in_name);
        bool is_weapon_in_player_inventory(string in_username, string in_weapon_name);
        string get_name(int i) { return players[i].get_username(); }
        vector<Player> get_players() { return players; }
        int get_game_status() { return game_status; }

        void get_money_command(string in_username);
        void get_health_command(string in_username);
        void set_game_status(int in_status) { game_status = in_status; }
        void go_status_command(int status, string in_username);
        void make_weapons();
    private:
        int game_status;
        int round_number;
        vector<Player> players;
        vector<Weapon> weapons;
};

void Round:: add_user_command(string in_username, string in_team)
{
    Player new_player(in_username, in_team);
    new_player.set_tag(players.size());
    players.push_back(new_player);
}

int Round:: find_player_index_by_username(string in_username)
{
    for (int i = 0; i < players.size(); i++)
        if (in_username == players[i].get_username())
            return i;

    
    return -1;
}

int Round:: find_weapon_index_by_name(string in_name)
{
    for (int i = 0; i < weapons.size(); i++)
        if (in_name == weapons[i].get_name())
            return i;

    return -1;
}
bool player_not_found(int index)
{
    return (index == -1);
}
bool weapon_not_found(int index)
{
    return (index == -1);
}
void Round:: get_money_command(string in_username)
{
    int user_index = find_player_index_by_username(in_username);
    cout << players[user_index].get_money() << endl;
} 

void Round:: get_health_command(string in_username)
{
    int user_index = find_player_index_by_username(in_username);
    cout << players[user_index].get_health() << endl;
}

void Round:: go_status_command(int status, string in_username)
{
    int user_index = find_player_index_by_username(in_username);
    if (status == AFK)
        players[user_index].set_play_status(AFK);
    else if (status == ATK)
        players[user_index].set_play_status(ATK);

    cout << GO_STATUS_MESSAGE << endl;
}

void Round:: make_weapons()
{
    for(int i = 1; i <= ALL_WEAPON_NUMBER; i++)
    {
        Weapon new_weapon(i);
        weapons.push_back(new_weapon);
    }
}

bool Round:: is_weapon_in_player_inventory(string in_username, string in_weapon_name)
{
    int user_index = find_player_index_by_username(in_username);

    for (int i = 0; i < players[user_index].get_weapons().size(); i++)
        if (in_weapon_name == players[user_index].get_weapons()[i].get_name())
            return true;
    
    return false;
}

bool is_player_afk(Player player)
{
    return (player.get_play_status() == AFK);
}

bool user_is_available_check(Round r, string in_username)
{
    int user_index = r.find_player_index_by_username(in_username);
    if (player_not_found(user_index)) 
    {
        cout << USER_NOT_FOUND_MESSAGE << endl;
        return false;
    }
    if (is_player_afk(r.get_players()[user_index]))
    {
        cout << USER_NOT_FOUND_MESSAGE << endl;     
        return false;
    }

    return true;
}

bool weapon_is_available_check(Round r, string in_weapon_name)
{
    int weapon_index = r.find_weapon_index_by_name(in_weapon_name);
    if (weapon_not_found(weapon_index))
    {
        cout << WEAPON_NOT_FOUND_MESSAGE << endl;
        return false;
    }
    
    return true;
}

bool game_already_started_error(Round r)
{
    cout << BUY_WEAPON_AFTER_START_MESSAGE << endl;
    return (r.get_game_status() == GAME_STARTED);
}

bool player_has_this_weapon_check(Round r, string in_username, string in_weapon_name)
{
    return r.is_weapon_in_player_inventory(in_username, in_weapon_name);
}

bool can_buy_weapon(Round r, string in_username, string in_weapon)
{
    //int buyer_index = r.find_player_index_by_username(in_username);
    //int buyer_index = r.find_player_index_by_username(in_username);
    //if (user_is_available_check(r, in_username))
    //{
        // todo
    return true;
    //}
}
//void Round:: buy_command(string in_username, string in_weapon)
//{
 //   if ()
//}

bool is_buying_available(int play_status)
{
    return (play_status == GAME_NOT_STARTED);
}
/******************************************************************************/

vector<string> parse_line(string line)
{
    vector<string> tokens;
    size_t pos = 0;

    while ((pos = line.find(TOKENS_DELIMITER)) != string::npos) 
    {
        tokens.push_back(line.substr(0, pos));
        line.erase(0, pos + TOKENS_DELIMITER.length());
    }   
    tokens.push_back(line.substr(0, pos));

    return tokens;
}

void get_command(vector<string> tokens, Round &r)
{   
    
    int game_status = GAME_NOT_STARTED;
    string command = tokens[0];
    if (command == "start" && game_status == GAME_NOT_STARTED)
    {
        r.set_game_status(GAME_STARTED);
        cout << START_OUTPUT_MESSAGE << endl;
    }

    else if (command == "add-user")
        r.add_user_command(tokens[1], tokens[2]);
    
    else if (command == "get-money")
        r.get_money_command(tokens[1]);
    
    else if (command == "get-health")
        r.get_health_command(tokens[1]);

    else if (command == "go-afk")
        r.go_status_command(AFK, tokens[1]);

    else if (command == "go-atk")
        r.go_status_command(ATK, tokens[1]);

    else if (command == "buy")
    {
        // must be functionilze
        //if (is_buying_available(game_status))

    }
 
}
void get_input()
{
    int round = 4;
    string line;
    for (int i = 0; i < round; i++)
    {
        vector<string> tokens;// name should be changed
        getline(cin, line);
        tokens = parse_line(line);
    }
} 
int main()
{
    Round r(3);
    r.make_weapons();
    string line;
    vector<string> tokens;// name should be changed
    while(1)
    {
        getline(cin, line);
        tokens = parse_line(line);
        get_command(tokens, r);
        
    }int round_nums;

    return 0;
}