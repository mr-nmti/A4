#include <iostream>
#include <vector>
#include <string>

using namespace std;

constexpr int UNDEFINED_WEAPON = 0;
constexpr int HEAVY_WEAPON = 1;
constexpr int PISTOL = 2;
constexpr int KNIFE = 3;
constexpr int UNBUYABLE = -1;
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

int main()
{
    return 0;
}