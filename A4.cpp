#include <iostream>
#include <vector>
#include <string>

/*

A) get_command refactor?
B) every round should get infinite commands, not just one!
C) class round should be added which contains whole game?

*/
using namespace std;

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
constexpr int GAME_NOT_STARTED = 0;
constexpr int GAME_STARTED = 1;
const string TOKENS_DELIMITER = " ";
const string START_OUTPUT_MESSAGE = "fight!";
const string USER_NOT_FOUND_MESSAGE = "user not available";

class Weapon
{
    public:
        Weapon(int type);
        Weapon() : Weapon(UNDEFINED_WEAPON){};

    private:
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
            type = HEAVY_WEAPON;
            price = 3000;
            damage = 45;
            kill_prize = 100;
            break;
        
        case PISTOL:
            type = PISTOL;
            price = 400;
            damage = 20;
            kill_prize = 200;
            break;

        case KNIFE:
            type = KNIFE;
            price = UNBUYABLE;
            damage = 35;
            kill_prize = 500;
            break;
        
        default:
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
    private:
        string username;
        string team;
        long int money;
        int health;
        int play_status;
        int death_count;
        int kill_count;
        vector<Weapon> weapons;
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
}

class Round
{
    public:
        // todo
        Round(int r_num) { round_number = r_num; }
        void add_user_command(string in_username, string in_team);
        int find_player_by_username(string in_username);
        string get_name(int i) { return players[i].get_username(); }
        void get_money_command(string in_username);
    private:
        int round_number;
        vector<Player> players;
        vector<Weapon> weapons;
};

void Round:: add_user_command(string in_username, string in_team)
{
    Player new_player(in_username, in_team);
    players.push_back(new_player);
}

int Round:: find_player_by_username(string in_username)
{
    for (int i = 0; i < players.size(); i++)
        if (in_username == players[i].get_username())
            return i;

    cout << USER_NOT_FOUND_MESSAGE << endl;
    return -1;
}

bool user_not_found(int index)
{
    return (index == -1);
}
void Round:: get_money_command(string in_username)
{
    int user_index = find_player_by_username(in_username);
    if (user_not_found(user_index))
        return;

    cout << players[user_index].get_money() << endl;
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
        game_status = GAME_STARTED;
        cout << START_OUTPUT_MESSAGE << endl;
    }

    else if (command == "add-user")
        r.add_user_command(tokens[1], tokens[2]);
    
    else if (command == "get-money")
    {
        r.get_money_command(tokens[1]);
    }
    else if (command == "get-health")
    {
        //todo
    }

    else if (command== "go-afk")
    {
        //todo
    }

    else if (command == "go-atk")
    {
        //todo
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