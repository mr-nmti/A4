#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
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
constexpr int INITIAL_WEAPON = KNIFE;
//constexpr int TERRORIST = 1;
//constexpr int COUNTER_TERRORIST = 2;
const string TERRORIST = "terrorist";
const string COUNTER_TERRORIST = "counter-terrorist";
constexpr int AFK = 0;
constexpr int ATK = 1;
constexpr int DEATH_HEALTH = 0;
constexpr bool DEAD = true;
constexpr bool ALIVE = false;
constexpr int ATTACKER = 1;
constexpr int DEFENDER = 2;
constexpr long int INITIAL_MONEY = 1000;
constexpr int INITIAL_HEALTH = 100;
constexpr int INITIAL_TAG = -1;
constexpr bool ROUND_NOT_STARTED = false;
constexpr bool ROUND_STARTED = true;
const string TOKENS_DELIMITER = " ";
const string START_OUTPUT_MESSAGE = "fight!";
const string USER_NOT_FOUND_MESSAGE = "user not available";
const string GO_STATUS_MESSAGE = "ok";
const string ADD_USER_DONE_MESSAGE = "ok";
const string BUY_WEAPON_AFTER_START_MESSAGE = "you can't buy weapons anymore";
const string WEAPON_NOT_FOUND_MESSAGE = "weapon not available";
const string PLAYEY_HAS_THE_WEAPON_MESSAGE = "you already have this weapon";
const string PLAYER_HAS_NOT_ENOUGH_MONEY_MESSAGE = "insufficient money";
const string SUCCESSFUL_WEAPON_BUY_MESSAGE = "weapon bought successfully";
const string PLAYER_NOT_HAVE_THE_WEAPON_MESSAGE = "attacker doesn't have this gun";
const string DEAD_ATTACKER_MESSAGE = "attacker is dead";
const string DEAD_DEFENDER_MESSAGE = "attacked is dead";
const string PLAYERS_ARE_SAME_TEAM_MESSAGE = "you can't shoot this player";
const string SHOOT_BEFORE_START_MESSAGE = "The game hasn't started yet";
const string SUCCESSFUL_SHOOT_MESSAGE = "nice shot";
const string COUNTER_TERRORIST_WIN_MESSAGE = "counter-terrorist won";
const string TERRORIST_WIN_MESSAGE = "terrorist won";
const string COUNTER_SCOREBOARD_MESSAGE = "counter-terrorist players:";
const string TERRORIST_SCOREBOARD_MESSAGE = "terrorist players:";
class Weapon
{
    public:
        Weapon(int type);
        Weapon() : Weapon(UNDEFINED_WEAPON){};

        string get_name() { return name; }
        long int get_price() { return price; }
        int get_damage() { return damage; }
        long int get_kill_prize() { return kill_prize; }
    private:
        string name;
        int type;
        long int price;
        int damage;
        long int kill_prize;
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
        string get_username() { return username; }
        long int get_money() { return money; }
        int get_health() { return health; }
        int get_play_status() { return play_status; }
        int get_tag() { return tag; }
        string get_team() { return team; }
        vector<Weapon> get_weapons() { return weapons; }
        int get_kill_count() { return kill_count; }
        int get_death_count() { return death_count; }
        void set_play_status(int status) { play_status = status; }; 
        void set_tag(int in_tag) { tag = in_tag; }
        void set_bought_weapon(Weapon bought_weapon);
        void set_shoot_status(Player &defender, Weapon weapon);
        void set_health(int damage);
        void set_kill_count() { kill_count++ ;}
        void set_death_count() { death_count++ ;}
        void set_money(long int prize) { money += prize; }
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
    weapons.push_back(Weapon(INITIAL_WEAPON));
}

void Player:: set_bought_weapon(Weapon bought_weapon)
{
   weapons.push_back(bought_weapon);
   money = money - bought_weapon.get_price();
}
void Player:: set_health(int damage)
{
    health -= damage;
    if (health < DEATH_HEALTH)
        health = DEATH_HEALTH;
}

void Player:: set_shoot_status(Player &defender, Weapon weapon)
{
    defender.set_health(weapon.get_damage());
    if (defender.get_health() == DEATH_HEALTH)
    {
        (*this).set_kill_count();
        defender.set_death_count();
        (*this).set_money(weapon.get_kill_prize());
    }
}
class Round
{
    public:
        // todo
        Round(int r_num);
        void add_user_command(string in_username, string in_team);
        int find_player_index_by_username(string in_username);
        int find_weapon_index_by_name(string in_name);
        bool is_weapon_in_player_inventory(string in_username, string in_weapon_name);
        bool is_player_money_sufficient(int user_index, int weapon_index);
        string get_name(int i) { return players[i].get_username(); }
        vector<Player> get_players() { return players; }
        vector<Weapon> get_weapons() { return weapons; }
        Player* get_exact_player(int index) { return &players[index]; }
        bool get_game_status() { return game_status; }
        void buy_command(string in_username, string in_weapon_name);
        void shoot_command(string in_attacker_username, 
               string in_defender_username, string in_weapon_name); 
        void get_money_command(string in_username);
        void get_health_command(string in_username);
        void set_game_status(bool in_status) { game_status = in_status; }
        void go_status_command(int status, string in_username);
        void make_weapons();
        void round_end();
        void score_board_command();

    private:
        bool game_status;
        int round_number;
        vector<Player> players;
        vector<Weapon> weapons;
};

Round:: Round(int r_num)
{
    round_number = r_num;
    game_status = ROUND_NOT_STARTED;
}
void Round:: add_user_command(string in_username, string in_team)
{
    Player new_player(in_username, in_team);
    new_player.set_tag(players.size());
    players.push_back(new_player);
    cout << ADD_USER_DONE_MESSAGE << endl;
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

bool round_already_started_check(Round r, string command)
{
    bool game_started_check = r.get_game_status();
    if (game_started_check == ROUND_STARTED && command == "buy")
        cout << BUY_WEAPON_AFTER_START_MESSAGE << endl;

    else if (game_started_check == ROUND_NOT_STARTED && command == "shoot")
        cout << SHOOT_BEFORE_START_MESSAGE << endl;
    return game_started_check;
}

bool player_has_this_weapon_check(Round r, string in_username, string in_weapon_name)
{
    bool has_weapon = r.is_weapon_in_player_inventory(in_username, in_weapon_name);
    if (has_weapon)
        cout << PLAYEY_HAS_THE_WEAPON_MESSAGE << endl;

    return has_weapon;
}

bool player_has_enough_money(Round r, string in_username, string in_weapon_name)
{
    int user_index = r.find_player_index_by_username(in_username);
    int weapon_index = r.find_weapon_index_by_name(in_weapon_name);
    long int user_money = r.get_players()[user_index].get_money();
    long int weapon_price = r.get_weapons()[weapon_index].get_price();
    bool has_enough = weapon_price <= user_money;
    if (!has_enough)
        cout << PLAYER_HAS_NOT_ENOUGH_MONEY_MESSAGE << endl;

    return has_enough;
}

bool can_buy_weapon(Round r, string in_username, string in_weapon_name)
{
    bool result = false;
    if (user_is_available_check(r, in_username) && weapon_is_available_check(r, in_weapon_name))
    {
        result = ((round_already_started_check(r, "buy") == ROUND_NOT_STARTED) && 
        !player_has_this_weapon_check(r, in_username, in_weapon_name) &&
        player_has_enough_money(r, in_username, in_weapon_name));
    }
    
    return result;
}

void Round:: buy_command( string in_username, string in_weapon_name)
{
    if (can_buy_weapon(*this, in_username, in_weapon_name))
    {
        int player_index = find_player_index_by_username(in_username);
        int weapon_index = find_weapon_index_by_name(in_weapon_name);
        Weapon bought_weapon(weapon_index + 1);
        players[player_index].set_bought_weapon(bought_weapon);
        cout << SUCCESSFUL_WEAPON_BUY_MESSAGE << endl;
    }
}

bool attacker_has_weapon_to_shoot(Round r, string in_attacker_username, string in_weapon_name)
{
    bool has_weapon = r.is_weapon_in_player_inventory(in_attacker_username, in_weapon_name);
    if (!has_weapon)
        cout << PLAYER_NOT_HAVE_THE_WEAPON_MESSAGE << endl;

    return has_weapon;
}

bool is_player_dead_check(Round r, string in_username, int player_type)
{
    int player_index = r.find_player_index_by_username(in_username);
    bool death_condition = (r.get_players()[player_index].get_health() <= DEATH_HEALTH);

    if (death_condition == DEAD && player_type == ATTACKER)
        cout << DEAD_ATTACKER_MESSAGE << endl;
    
    else if (death_condition == DEAD && player_type == DEFENDER)
        cout << DEAD_DEFENDER_MESSAGE << endl;

    return death_condition;
}

bool are_players_in_same_teams(Round r, string in_attacker_username, string in_defender_username)
{
    int attacker_index = r.find_player_index_by_username(in_attacker_username);
    int defender_index = r.find_player_index_by_username(in_defender_username);
    string attacker_team = r.get_players()[attacker_index].get_team();
    string defender_team = r.get_players()[defender_index].get_team();
    bool are_in_same_team = (attacker_team == defender_team);
    if (are_in_same_team)
        cout << PLAYERS_ARE_SAME_TEAM_MESSAGE << endl;

    return are_in_same_team;
}

bool can_shoot(Round r, string in_attacker_username, 
               string in_defender_username, string in_weapon_name)
{
    bool result = false;
    if (user_is_available_check(r, in_attacker_username) && 
    user_is_available_check(r, in_defender_username) &&
    attacker_has_weapon_to_shoot(r, in_attacker_username, in_weapon_name))
    {
        result = ((round_already_started_check(r, "shoot") == ROUND_STARTED) &&
                    !is_player_dead_check(r, in_attacker_username, ATTACKER) &&
                    !is_player_dead_check(r, in_defender_username, DEFENDER) &&
                    !are_players_in_same_teams(r, in_attacker_username, in_defender_username));
    }
    return result;
}

void Round:: shoot_command(string in_attacker_username, 
               string in_defender_username, string in_weapon_name) 
{
    if (can_shoot(*this, in_attacker_username, in_defender_username, in_weapon_name))
    {
        int attacker_index = find_player_index_by_username(in_attacker_username);
        int defender_index = find_player_index_by_username(in_defender_username);
        int weapon_index = find_weapon_index_by_name(in_weapon_name);
        Weapon used_weapon(weapon_index + 1);

       players[attacker_index].set_shoot_status(players[defender_index], used_weapon);
        cout << SUCCESSFUL_SHOOT_MESSAGE << endl;
    }
}

int num_alive_team_members(vector<Player> players, string team)
{
    int number;
    for (int i = 0; i < players.size(); i++)
        if (players[i].get_team() == team && players[i].get_health() != DEATH_HEALTH &&
            players[i].get_play_status() != AFK)
                number++;

    return number;
}

void Round:: round_end()
{
    int num_of_alive_counters = num_alive_team_members(players, COUNTER_TERRORIST);
    int num_of_alive_terrors = num_alive_team_members(players, TERRORIST);
    if (num_of_alive_terrors == 0 || (num_of_alive_terrors != 0 && num_of_alive_counters != 0))
        cout << COUNTER_TERRORIST_WIN_MESSAGE << endl;
    
    if (num_of_alive_counters == 0)
        cout << TERRORIST_WIN_MESSAGE << endl;
    
}

void print_scoreboead(vector<Player> counters, vector<Player> terrors)
{
    cout << COUNTER_SCOREBOARD_MESSAGE << endl;
    for (int i = 0; i < counters.size(); i++)
        cout << counters[i].get_username() << " " << counters[i].get_kill_count() << " " << 
        counters[i].get_death_count() << endl; 
    
        cout << TERRORIST_SCOREBOARD_MESSAGE << endl;
    for (int i = 0; i < terrors.size(); i++)
        cout << terrors[i].get_username() << " " << terrors[i].get_kill_count() << " " << 
        terrors[i].get_death_count() << endl; 
}
bool cmp_players(Player p1, Player p2)
{
    if (p1.get_kill_count() != p2.get_kill_count())
        return (p1.get_kill_count() > p2.get_kill_count());
    else if (p1.get_death_count() != p2.get_death_count())
        return (p1.get_death_count() < p2.get_death_count());
    else
        return (p1.get_username() < p2.get_username());
}

vector<Player> devide_teams(vector<Player> players, string team)
{
    vector<Player> devided_team;
    for (int i = 0; i < players.size(); i++)
        if (players[i].get_team() == team)
            devided_team.push_back(players[i]);

    return devided_team;
}
void Round:: score_board_command()
{
    vector<Player> counter_members = devide_teams(players, COUNTER_TERRORIST);
    vector<Player> terrorist_members = devide_teams(players, TERRORIST);
    sort(counter_members.begin(), counter_members.end(), cmp_players);
    sort(terrorist_members.begin(),terrorist_members.end(), cmp_players);
    
    print_scoreboead(counter_members, terrorist_members);
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
    
    bool game_status = ROUND_NOT_STARTED;
    string command = tokens[0];
    if (command == "start" && game_status == ROUND_NOT_STARTED)
    {
        r.set_game_status(ROUND_STARTED);
        cout << START_OUTPUT_MESSAGE << endl;
    }

    else if (command == "add-user" && game_status == ROUND_NOT_STARTED)
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
        r.buy_command(tokens[1], tokens[2]);

    else if (command == "shoot")
        r.shoot_command(tokens[1], tokens[2], tokens[3]);
    
 
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
        r.round_end();
        
    }int round_nums;

    return 0;
}