#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

constexpr int ALL_WEAPON_NUMBER = 3;
constexpr int UNDEFINED_WEAPON = 0;
constexpr int HEAVY_WEAPON = 1;
constexpr int PISTOL = 2;
constexpr int KNIFE = 3;
constexpr int UNBUYABLE = -1;
constexpr long int HEAVY_WEAPON_PRICE = 3000;
constexpr int HEAVY_WEAPON_DAMAGE = 45;
constexpr long int HEAVY_WEAPON_KILL_PRIZE = 100;
constexpr long int PISTOL_PRICE = 400;
constexpr int PISTOL_DAMAGE = 20;
constexpr long int PISTOL_KILL_PRIZE = 200;
constexpr int KNIFE_DAMAGE = 35;
constexpr long int KNIFE_KILL_PRIZE = 500;
constexpr int INITIAL_WEAPON = KNIFE;
constexpr int AFK = 0;
constexpr int ATK = 1;
constexpr int DEATH_HEALTH = 0;
constexpr bool DEAD = true;
constexpr bool ALIVE = false;
constexpr int ATTACKER = 1;
constexpr int DEFENDER = 2;
constexpr long int INITIAL_MONEY = 1000;
constexpr int INITIAL_HEALTH = 100;
constexpr long int ROUND_END_WINNER_PRIZE = 2700;
constexpr long int ROUND_END_LOSER_PRIZE = 2400;
constexpr long int MAXIMUM_MONEY = 10000;
constexpr bool ROUND_NOT_STARTED = false;
constexpr bool ROUND_STARTED = true;
const string TERRORIST = "terrorist";
const string COUNTER_TERRORIST = "counter-terrorist";
const string TOKENS_DELIMITER = " ";
const string START_OUTPUT_MESSAGE = "fight!";
const string USER_NOT_FOUND_MESSAGE = "user not available";
const string GO_STATUS_MESSAGE = "ok";
const string ADD_USER_DONE_MESSAGE = "ok";
const string BUY_WEAPON_AFTER_START_MESSAGE = "you can't buy weapons anymore";
const string WEAPON_NOT_FOUND_MESSAGE = "weapon not available";
const string PLAYEY_ALREADY_HAS_THE_WEAPON_MESSAGE = "you already have this weapon";
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
const string SCOREBOARD_TOKENS_DELIMITER = " ";
class Weapon
{
    public:
        Weapon(int type);

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
            price = HEAVY_WEAPON_PRICE;
            damage = HEAVY_WEAPON_DAMAGE;
            kill_prize = HEAVY_WEAPON_KILL_PRIZE;
            break;
        
        case PISTOL:
            name = "pistol";
            type = PISTOL;
            price = PISTOL_PRICE;
            damage = PISTOL_DAMAGE;
            kill_prize = PISTOL_KILL_PRIZE;
            break;

        case KNIFE:
            name = "knife";
            type = KNIFE;
            price = UNBUYABLE;
            damage = KNIFE_DAMAGE;
            kill_prize = KNIFE_KILL_PRIZE;
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
        Player(string input_username, string in_team);

        string get_username() { return username; }
        long int get_money() { return money; }
        int get_health() { return health; }
        int get_play_status() { return play_status; }
        string get_team() { return team; }
        vector<Weapon> get_weapons() { return weapons; }
        int get_kill_count() { return kill_count; }
        int get_death_count() { return death_count; }

        void set_play_status(int status) { play_status = status; }; 
        void set_bought_weapon(Weapon bought_weapon);
        void set_shoot_modifies(Player &defender, Weapon weapon);
        void set_health(int damage);
        void set_kill_count() { kill_count++ ;}
        void set_death_count() { death_count++ ;}
        void set_money(long int prize);
        void modify_after_round(string winner_team);

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

Player::Player(string input_username, string in_team)
{
    username = input_username;
    team = in_team;

    money = INITIAL_MONEY;
    health = INITIAL_HEALTH;
    play_status = ATK;
    death_count = 0;
    kill_count = 0;
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

void Player:: set_money(long int prize)
{
    money += prize;
    if (MAXIMUM_MONEY < money)
        money = MAXIMUM_MONEY;
}

void Player:: set_shoot_modifies(Player &defender, Weapon weapon)
{
    defender.set_health(weapon.get_damage());
    if (defender.health == DEATH_HEALTH)
    {
        this->set_kill_count();
        this->set_money(weapon.get_kill_prize());
        defender.set_death_count();
        for (int i = 0; i < defender.weapons.size() - 1; i++)
            defender.weapons.pop_back();
    }
}

void Player:: modify_after_round(string winner_team)
{
    this->health = INITIAL_HEALTH;
    if (this->team == winner_team)
        this->set_money(ROUND_END_WINNER_PRIZE);
    else
        this->set_money(ROUND_END_LOSER_PRIZE);
}

class Game
{
    public:
        Game() { game_status = ROUND_NOT_STARTED; }
        vector<Player> get_players() { return players; }
        vector<Weapon> get_weapons() { return weapons; }
        bool get_game_status() { return game_status; }
        void get_money_command(string input_username);
        void get_health_command(string input_username);
        void set_game_status(bool in_status) { game_status = in_status; }

        int find_player_index_by_username(string input_username);
        int find_weapon_index_by_name(string in_name);
        bool is_weapon_in_player_inventory(string input_username, string input_weapon_name);
        bool is_player_money_sufficient(int user_index, int weapon_index);
        void add_user_command(string input_username, string in_team);
        void buy_command(string input_username, string input_weapon_name);
        void shoot_command(string input_attacker_username, 
               string in_defender_username, string input_weapon_name); 
        void go_status_command(int status, string input_username);
        void make_weapons();
        void round_finished();
        void score_board_command();

    private:
        bool game_status;
        int round_number;
        vector<Player> players;
        vector<Weapon> weapons;
};

void Game:: make_weapons()
{
    for(int i = 1; i <= ALL_WEAPON_NUMBER; i++)
    {
        Weapon new_weapon(i);
        weapons.push_back(new_weapon);
    }
}

void Game:: add_user_command(string input_username, string in_team)
{
    Player new_player(input_username, in_team);
    players.push_back(new_player);
    cout << ADD_USER_DONE_MESSAGE << endl;
}

int Game:: find_player_index_by_username(string input_username)
{
    for (int i = 0; i < players.size(); i++)
        if (input_username == players[i].get_username())
            return i;

    return -1;
}

int Game:: find_weapon_index_by_name(string in_name)
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

void Game:: get_money_command(string input_username)
{
    int user_index = find_player_index_by_username(input_username);
    cout << players[user_index].get_money() << endl;
} 

void Game:: get_health_command(string input_username)
{
    int user_index = find_player_index_by_username(input_username);
    cout << players[user_index].get_health() << endl;
}

void Game:: go_status_command(int status, string input_username)
{
    int user_index = find_player_index_by_username(input_username);
    if (status == AFK)
        players[user_index].set_play_status(AFK);
    else if (status == ATK)
        players[user_index].set_play_status(ATK);

    cout << GO_STATUS_MESSAGE << endl;
}


bool Game:: is_weapon_in_player_inventory(string input_username, string input_weapon_name)
{
    int user_index = find_player_index_by_username(input_username);

    for (int i = 0; i < players[user_index].get_weapons().size(); i++)
        if (input_weapon_name == players[user_index].get_weapons()[i].get_name())
            return true;
    
    return false;
}

bool is_player_afk(Player player)
{
    return (player.get_play_status() == AFK);
}

bool is_user_available_check(Game game, string input_username)
{
    int user_index = game.find_player_index_by_username(input_username);
    if (player_not_found(user_index)) 
    {
        cout << USER_NOT_FOUND_MESSAGE << endl;
        return false;
    }
    if (is_player_afk(game.get_players()[user_index]))
    {
        cout << USER_NOT_FOUND_MESSAGE << endl;     
        return false;
    }

    return true;
}

bool is_weapon_available_check(Game game, string input_weapon_name)
{
    int weapon_index = game.find_weapon_index_by_name(input_weapon_name);
    if (weapon_not_found(weapon_index))
    {
        cout << WEAPON_NOT_FOUND_MESSAGE << endl;
        return false;
    }
    
    return true;
}

bool round_already_started_check(Game game, string command)
{
    bool game_started_check = game.get_game_status();
    if (game_started_check == ROUND_STARTED && command == "buy")
        cout << BUY_WEAPON_AFTER_START_MESSAGE << endl;

    else if (game_started_check == ROUND_NOT_STARTED && command == "shoot")
        cout << SHOOT_BEFORE_START_MESSAGE << endl;

    return game_started_check;
}

bool player_has_this_weapon_check(Game game, string input_username, string input_weapon_name)
{
    bool has_weapon = game.is_weapon_in_player_inventory(input_username, input_weapon_name);
    if (has_weapon)
        cout << PLAYEY_ALREADY_HAS_THE_WEAPON_MESSAGE << endl;

    return has_weapon;
}

bool player_has_enough_money(Game game, string input_username, string input_weapon_name)
{
    int user_index = game.find_player_index_by_username(input_username);
    int weapon_index = game.find_weapon_index_by_name(input_weapon_name);
    long int user_money = game.get_players()[user_index].get_money();
    long int weapon_price = game.get_weapons()[weapon_index].get_price();
    bool has_enough = (weapon_price <= user_money);
    if (!has_enough)
        cout << PLAYER_HAS_NOT_ENOUGH_MONEY_MESSAGE << endl;

    return has_enough;
}

bool can_buy_weapon(Game game, string input_username, string input_weapon_name)
{
    bool result = false;
    if (is_user_available_check(game, input_username) && is_weapon_available_check(game, input_weapon_name))
    {
        result = ((round_already_started_check(game, "buy") == ROUND_NOT_STARTED) && 
        !player_has_this_weapon_check(game, input_username, input_weapon_name) &&
        player_has_enough_money(game, input_username, input_weapon_name));
    }
    
    return result;
}

void Game:: buy_command(string input_username, string input_weapon_name)
{
    if (can_buy_weapon(*this, input_username, input_weapon_name))
    {
        int player_index = find_player_index_by_username(input_username);
        int weapon_index = find_weapon_index_by_name(input_weapon_name);
        Weapon bought_weapon(weapon_index + 1);
        players[player_index].set_bought_weapon(bought_weapon);
        cout << SUCCESSFUL_WEAPON_BUY_MESSAGE << endl;
    }
}

bool attacker_has_weapon_to_shoot(Game game, string input_attacker_username, string input_weapon_name)
{
    bool has_weapon = game.is_weapon_in_player_inventory(input_attacker_username, input_weapon_name);
    if (!has_weapon)
        cout << PLAYER_NOT_HAVE_THE_WEAPON_MESSAGE << endl;

    return has_weapon;
}

bool is_player_dead_check(Game game, string input_username, int player_type)
{
    int player_index = game.find_player_index_by_username(input_username);
    bool death_status = (game.get_players()[player_index].get_health() <= DEATH_HEALTH);

    if (death_status == DEAD && player_type == ATTACKER)
        cout << DEAD_ATTACKER_MESSAGE << endl;
    
    else if (death_status == DEAD && player_type == DEFENDER)
        cout << DEAD_DEFENDER_MESSAGE << endl;

    return death_status;
}

bool are_players_in_same_team(Game game, string input_attacker_username, string in_defender_username)
{
    int attacker_index = game.find_player_index_by_username(input_attacker_username);
    int defender_index = game.find_player_index_by_username(in_defender_username);
    string attacker_team = game.get_players()[attacker_index].get_team();
    string defender_team = game.get_players()[defender_index].get_team();
    bool are_in_same_team = (attacker_team == defender_team);
    if (are_in_same_team)
        cout << PLAYERS_ARE_SAME_TEAM_MESSAGE << endl;

    return are_in_same_team;
}

bool can_shoot(Game game, string input_attacker_username, 
               string in_defender_username, string input_weapon_name)
{
    bool result = false;
    if (is_user_available_check(game, input_attacker_username) && 
    is_user_available_check(game, in_defender_username) &&
    attacker_has_weapon_to_shoot(game, input_attacker_username, input_weapon_name))
    {
        result = ((round_already_started_check(game, "shoot") == ROUND_STARTED) &&
                    !is_player_dead_check(game, input_attacker_username, ATTACKER) &&
                    !is_player_dead_check(game, in_defender_username, DEFENDER) &&
                    !are_players_in_same_team(game, input_attacker_username, in_defender_username));
    }

    return result;
}

void Game:: shoot_command(string input_attacker_username, 
               string in_defender_username, string input_weapon_name) 
{
    if (can_shoot(*this, input_attacker_username, in_defender_username, input_weapon_name))
    {
        int attacker_index = find_player_index_by_username(input_attacker_username);
        int defender_index = find_player_index_by_username(in_defender_username);
        int weapon_index = find_weapon_index_by_name(input_weapon_name);
        Weapon used_weapon(weapon_index + 1);

       players[attacker_index].set_shoot_modifies(players[defender_index], used_weapon);
        cout << SUCCESSFUL_SHOOT_MESSAGE << endl;
    }
}

int num_alive_team_members(vector<Player> players, string team)
{
    int number = 0;
    for (int i = 0; i < players.size(); i++)
        if (players[i].get_team() == team && players[i].get_health() != DEATH_HEALTH &&
            players[i].get_play_status() != AFK)
                number++;

    return number;
}

void Game:: round_finished()
{
    int num_of_alive_counters = num_alive_team_members(players, COUNTER_TERRORIST);
    int num_of_alive_terrors = num_alive_team_members(players, TERRORIST);
    string winner_team;
    if (num_of_alive_terrors == 0 || (num_of_alive_terrors != 0 && num_of_alive_counters != 0))
    {    
        winner_team = COUNTER_TERRORIST;
        cout << COUNTER_TERRORIST_WIN_MESSAGE << endl;
    }
    if (num_of_alive_counters == 0)
    {   
        winner_team = TERRORIST;
        cout << TERRORIST_WIN_MESSAGE << endl;
    }
    for (int i = 0; i < players.size(); i++)
        players[i].modify_after_round(winner_team);
    
    game_status = ROUND_NOT_STARTED;
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

void Game:: score_board_command()
{
    vector<Player> counter_members = devide_teams(players, COUNTER_TERRORIST);
    vector<Player> terrorist_members = devide_teams(players, TERRORIST);
    sort(counter_members.begin(), counter_members.end(), cmp_players);
    sort(terrorist_members.begin(),terrorist_members.end(), cmp_players);

    print_scoreboead(counter_members, terrorist_members);
}

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

void get_command(vector<string> tokens, Game &game)
{   
    
    bool game_status = ROUND_NOT_STARTED;
    string command = tokens[0];
    if (command == "start" && game_status == ROUND_NOT_STARTED)
    {
        game.set_game_status(ROUND_STARTED);
        cout << START_OUTPUT_MESSAGE << endl;
    }

    else if (command == "add-user" && game_status == ROUND_NOT_STARTED)
        game.add_user_command(tokens[1], tokens[2]);
    
    else if (command == "get-money")
        game.get_money_command(tokens[1]);
    
    else if (command == "get-health")
        game.get_health_command(tokens[1]);

    else if (command == "go-afk")
        game.go_status_command(AFK, tokens[1]);

    else if (command == "go-atk")
        game.go_status_command(ATK, tokens[1]);

    else if (command == "buy")
        game.buy_command(tokens[1], tokens[2]);

    else if (command == "shoot")
        game.shoot_command(tokens[1], tokens[2], tokens[3]);
    
    else if (command == "score-board")
        game.score_board_command();
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
    int num_rounds;
    cin >> num_rounds;
    Game game;
    game.make_weapons();
    string line;
    vector<string> tokens;// name should be changed
    for (int i = 0; i < num_rounds; i++)
    {
        int round_command_number;
        while(1)
        {
            getline(cin, line);
            tokens = parse_line(line);
            if (tokens[0] == "round")
            {
                round_command_number = stoi(tokens[1]);
                break;
            }
            get_command(tokens, game);
        }
        for (int j = 0; j < round_command_number; j++)
        {
            getline(cin, line);
            tokens = parse_line(line);
            get_command(tokens, game);
        }
        game.round_finished();
    }
    string remaining_command;
    while(cin >> remaining_command)
        if (remaining_command == "score-board")
            game.score_board_command();
    int round_nums;

    return 0;
}