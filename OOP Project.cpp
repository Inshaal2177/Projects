#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// ================= UI SYSTEM =================
void uiHeader(string t)
{
    cout << "\n========================================\n";
    cout << " " << t << "\n";
    cout << "========================================\n";
}

void uiLine()
{
    cout << "----------------------------------------\n";
}

// ================= BASE =================
class Character
{
protected:
    string name;

public:
    int health;
    Character(string n, int h) : name(n), health(h) {}

    virtual void attackMenu(Character &enemy) = 0;
    virtual void display() = 0;

    friend void battle(Character &p, Character &e);
};

// ================= PLAYER =================
class Player : public Character
{
protected:
    int energy;

public:
    int healPotions;
    int energyPotions;
    int gold;
    int battlePotions;
    bool power = false, speed = false, invis = false;
    int pendingPotion = 0;

    Player(string n, int h, int e, int g)
        : Character(n, h), energy(e), gold(g),
          battlePotions(0), healPotions(3), energyPotions(1) {}

    void addGold(int g)
    {
        gold += g;
        cout << "[REWARD] +" << g << " Gold | Total: " << gold << "\n";
    }

    void display() override
    {
        uiLine();
        cout << " PLAYER: " << name << "\n";
        cout << " HP: " << health
             << " | Energy: " << energy
             << " | Gold: " << gold << "\n";
        cout << " BattleP: " << battlePotions
             << " | HealP: " << healPotions
             << " | EnergyP: " << energyPotions << "\n";
        uiLine();
    }

    void regenEnergy()
    {
        int regen = 8;
        energy += regen;
        cout << "[REGEN] +" << regen << " Energy | Energy: " << energy << "\n";
    }

    void healMenu()
    {
        if (healPotions <= 0)
        {
            cout << "\n[INFO] No heal potions\n";
            return;
        }
        cout << "\n[HEAL] Use Heal Potion? (1=Yes / 0=No): ";
        int c;
        cin >> c;
        if (!cin)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            return;
        }
        if (c == 1)
        {
            health += 40;
            healPotions--;
            cout << "[HEAL] +40 HP | HP now: " << health << "\n";
        }
    }

    void energyMenu()
    {
        if (energyPotions <= 0)
        {
            cout << "\n[INFO] No energy potions\n";
            return;
        }
        cout << "\n[ENERGY] Use Energy Potion? (1=Yes / 0=No): ";
        int c;
        cin >> c;
        if (!cin)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            return;
        }
        if (c == 1)
        {
            energy += 40;
            energyPotions--;
            cout << "[ENERGY] +40 Energy | Energy now: " << energy << "\n";
        }
    }

    void potionMenu()
    {
        if (battlePotions <= 0)
        {
            cout << "\n[INFO] No battle potions\n";
            return;
        }

        cout << "\n[POTION MENU]\n";
        cout << "1 Power (+50% dmg)  2 Speed (extra hit)  3 Invis (dodge)  0 Skip\nChoice: ";

        int c;
        cin >> c;
        if (!cin)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            return;
        }

        if (c >= 1 && c <= 3)
        {
            pendingPotion = c;
            string names[] = {"", "Power", "Speed", "Invisibility"};
            cout << "[QUEUE] " << names[c] << " potion queued for next attack\n";
        }
    }
};

// ================= SHOP =================
class Shop
{
public:
    void buy(Player &p)
    {
        int c;
        uiHeader("SHOP");

        while (true)
        {
            cout << "Gold: " << p.gold << "\n\n";
            cout << "1 Battle Potion  (50g)\n";
            cout << "2 Heal Potion    (40g)\n";
            cout << "3 Energy Potion  (40g)\n";
            cout << "4 Exit\n";
            cout << "Choice: ";
            cin >> c;
            if (!cin)
            {
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }

            if (c == 4)
                break;

            if (c == 1)
            {
                if (p.gold >= 50)
                {
                    p.gold -= 50;
                    p.battlePotions++;
                    cout << "[SHOP] Battle Potion +1 | Gold left: " << p.gold << "\n";
                }
                else
                    cout << "[SHOP] Not enough gold (need 50)\n";
            }
            else if (c == 2)
            {
                if (p.gold >= 40)
                {
                    p.gold -= 40;
                    p.healPotions++;
                    cout << "[SHOP] Heal Potion +1 | Gold left: " << p.gold << "\n";
                }
                else
                    cout << "[SHOP] Not enough gold (need 40)\n";
            }
            else if (c == 3)
            {
                if (p.gold >= 40)
                {
                    p.gold -= 40;
                    p.energyPotions++;
                    cout << "[SHOP] Energy Potion +1 | Gold left: " << p.gold << "\n";
                }
                else
                    cout << "[SHOP] Not enough gold (need 40)\n";
            }
            else
            {
                cout << "[SHOP] Invalid choice\n";
            }
        }
    }
};

// ================= WARRIOR =================
class Warrior : public Player
{
public:
    Warrior(string n) : Player(n, 120, 100, 120) {}

    void attackMenu(Character &enemy) override
    {
        int c;

        while (true)
        {
            uiHeader("WARRIOR TURN");
            cout << " Energy: " << energy << "\n\n";
            cout << "1 Light Attack   (Cost  5 | DMG 10)\n";
            cout << "2 Heavy Attack   (Cost 15 | DMG 25)\n";
            cout << "3 Special Attack (Cost 30 | DMG 40)\n";
            cout << "4 HP -> Energy   (Trade 20HP for 25E)\n";
            cout << "Choose: ";
            cin >> c;
            if (!cin)
            {
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }

            if (c == 1 && energy >= 5)
            {
                energy -= 5;
                enemy.health -= 10;
                cout << "[ACTION] Light Attack | Cost: 5E | Damage: 10\n";
                cout << "Enemy HP: " << enemy.health << " | Your Energy: " << energy << "\n";
                break;
            }
            else if (c == 2 && energy >= 15)
            {
                energy -= 15;
                enemy.health -= 25;
                cout << "[ACTION] Heavy Attack | Cost: 15E | Damage: 25\n";
                cout << "Enemy HP: " << enemy.health << " | Your Energy: " << energy << "\n";
                break;
            }
            else if (c == 3 && energy >= 30)
            {
                energy -= 30;
                enemy.health -= 40;
                cout << "[ACTION] Special Attack | Cost: 30E | Damage: 40\n";
                cout << "Enemy HP: " << enemy.health << " | Your Energy: " << energy << "\n";
                break;
            }
            else if (c == 4 && health > 20)
            {
                health -= 20;
                energy += 25;
                cout << "[TRADE] -20 HP / +25 Energy | HP: " << health << " | Energy: " << energy << "\n";
                return;
            }
            else if (c >= 1 && c <= 4)
                cout << "[ERROR] Not enough energy or HP too low\n";
            else
                cout << "[ERROR] Invalid choice\n";
        }
    }
};

// ================= MAGE =================
class Mage : public Player
{
public:
    Mage(string n) : Player(n, 90, 120, 120) {}

    void attackMenu(Character &enemy) override
    {
        int c;

        while (true)
        {
            uiHeader("MAGE TURN");
            cout << " Energy: " << energy << "\n\n";
            cout << "1 Spark      (10E | 15 DMG)\n";
            cout << "2 Fireball   (20E | 30 DMG)\n";
            cout << "3 Lightning  (35E | 50 DMG)\n";
            cout << "4 HP -> Energy\n";
            cout << "Choose: ";
            cin >> c;
            if (!cin)
            {
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }

            if (c == 1 && energy >= 10)
            {
                energy -= 10;
                enemy.health -= 15;
                cout << "[SPELL] Spark! | Enemy HP: " << enemy.health << "\n";
                break;
            }
            else if (c == 2 && energy >= 20)
            {
                energy -= 20;
                enemy.health -= 30;
                cout << "[SPELL] Fireball! | Enemy HP: " << enemy.health << "\n";
                break;
            }
            else if (c == 3 && energy >= 35)
            {
                energy -= 35;
                enemy.health -= 50;
                cout << "[SPELL] Lightning Strike! | Enemy HP: " << enemy.health << "\n";
                break;
            }
            else if (c == 4 && health > 20)
            {
                health -= 20;
                energy += 25;
                cout << "[TRADE] -20 HP / +25 Energy | HP: " << health << " | Energy: " << energy << "\n";
                return;
            }
            else if (c >= 1 && c <= 4)
                cout << "[ERROR] Not enough energy or HP too low\n";
            else
                cout << "Invalid\n";
        }
    }
};

// ================= ASSASSIN =================
class Assassin : public Player
{
public:
    Assassin(string n) : Player(n, 85, 110, 120) {}

    void attackMenu(Character &enemy) override
    {
        int c;

        while (true)
        {
            uiHeader("ASSASSIN TURN");
            cout << " Energy: " << energy << "\n\n";
            cout << "1 Slash       ( 8E | 18 DMG)\n";
            cout << "2 Crit        (20E | 35 DMG)\n";
            cout << "3 Shadow Kill (40E | 60 DMG)\n";
            cout << "4 HP -> Energy\n";
            cout << "Choose: ";
            cin >> c;
            if (!cin)
            {
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }

            if (c == 1 && energy >= 8)
            {
                energy -= 8;
                enemy.health -= 18;
                cout << "[ACTION] Slash! | Enemy HP: " << enemy.health << "\n";
                break;
            }
            else if (c == 2 && energy >= 20)
            {
                energy -= 20;
                enemy.health -= 35;
                cout << "[ACTION] Crit! | Enemy HP: " << enemy.health << "\n";
                break;
            }
            else if (c == 3 && energy >= 40)
            {
                energy -= 40;
                enemy.health -= 60;
                cout << "[ACTION] Shadow Kill! | Enemy HP: " << enemy.health << "\n";
                break;
            }
            else if (c == 4 && health > 20)
            {
                health -= 20;
                energy += 25;
                cout << "[TRADE] -20 HP / +25 Energy | HP: " << health << " | Energy: " << energy << "\n";
                return;
            }
            else if (c >= 1 && c <= 4)
                cout << "[ERROR] Not enough energy or HP too low\n";
            else
                cout << "Invalid\n";
        }
    }
};

// ================= ENEMY =================

int randRange(int lo, int hi)
{
    return lo + rand() % (hi - lo + 1);
}

string enemyQuip(string name)
{
    string quips[] = {
        name + " snarls and lunges!",
        name + " swings wildly!",
        name + " charges at you!",
        name + " strikes hard!"};
    return quips[rand() % 4];
}

class Enemy : public Character
{
public:
    Enemy(string n, int h) : Character(n, h) {}

    void attackMenu(Character &enemy) override
    {

        int dmg = randRange(12, 18);
        enemy.health -= dmg;

        cout << "\n[ENEMY TURN]\n";
        cout << enemyQuip(name) << "\n";
        cout << "Damage: " << dmg << " | Your HP: " << enemy.health << "\n";
    }

    void display() override
    {
        cout << " ENEMY: " << name << " | HP: " << health << "\n";
    }
};

// ================= BOSS =================
class Boss : public Enemy
{
    int turnCount;

public:
    Boss(string n) : Enemy(n, 350), turnCount(0) {}

    void attackMenu(Character &enemy) override
    {
        turnCount++;
        cout << "\n[BOSS TURN]\n";

        if (turnCount % 2 == 0)
        {
            int dmg = randRange(28, 35);
            enemy.health -= dmg;
            cout << name << " unleashes an ULTIMATE STRIKE!\n";
            cout << "Damage: " << dmg << " | Your HP: " << enemy.health << "\n";
        }
        else
        {
            int dmg = randRange(16, 22);
            enemy.health -= dmg;
            cout << name << " attacks!\n";
            cout << "Damage: " << dmg << " | Your HP: " << enemy.health << "\n";
        }
    }
};

// ================= BATTLE =================
void battle(Character &p, Character &e)
{
    uiHeader("BATTLE START");
    cout << " VS: ";
    e.display();
    cout << "\n";

    Player *pl = dynamic_cast<Player *>(&p);

    while (p.health > 0 && e.health > 0)
    {

        uiHeader("STATUS");
        p.display();
        e.display();

        if (pl)
        {
            pl->regenEnergy();
            pl->healMenu();
            pl->energyMenu();
            pl->potionMenu();
        }

        // ================= TURN EFFECT FLAGS =================
        bool powerActive = false;
        bool speedActive = false;
        bool invisActive = false;

        if (pl && pl->pendingPotion != 0)
        {
            if (pl->pendingPotion == 1)
            {
                powerActive = true;
                cout << "[POWER] +50% damage this turn!\n";
            }
            else if (pl->pendingPotion == 2)
            {
                speedActive = true;
                cout << "[SPEED] You'll strike twice this turn!\n";
            }
            else if (pl->pendingPotion == 3)
            {
                invisActive = true;
                cout << "[INVIS] Enemy will miss their attack!\n";
            }

            pl->battlePotions--;
            pl->pendingPotion = 0;
        }

        // ================= PLAYER ATTACK =================
        int before = e.health;
        p.attackMenu(e);
        int damage = before - e.health;
        cout << "[LOG] Damage Dealt: " << damage << "\n";

        // ================= POWER EFFECT =================
        if (powerActive && damage > 0)
        {
            int bonus = damage / 2;
            e.health -= bonus;
            cout << "[POWER BONUS] +" << bonus << " extra damage!\n";
            cout << "Enemy HP: " << e.health << "\n";
        }

        // ================= SPEED EFFECT =================
        if (speedActive && e.health > 0)
        {
            cout << "[SPEED] Bonus attack!\n";
            int before2 = e.health;
            p.attackMenu(e);
            int dmg2 = before2 - e.health;
            cout << "[LOG] Bonus Damage Dealt: " << dmg2 << "\n";
        }

        if (e.health <= 0)
            break;

        // ================= ENEMY TURN =================
        uiHeader("ENEMY TURN");

        if (invisActive)
        {
            cout << "You vanish into the shadows... Enemy swings at nothing!\n";
        }
        else
        {
            int beforeP = p.health;
            e.attackMenu(p);
            cout << "[LOG] Damage Taken: " << (beforeP - p.health) << "\n";
        }
    }

    // ================= RESULT =================
    uiHeader("BATTLE END");
    if (p.health > 0)
    {
        cout << "*** YOU WIN! ***\n";
        cout << "HP remaining: " << p.health << "\n";
    }
    else
    {
        cout << "*** YOU LOSE... ***\n";
        cout << "Better luck next time.\n";
    }
}

// ================= MAIN =================
int main()
{
    srand((unsigned int)time(0));

    string name;
    cout << "Enter Name: ";
    cin >> name;

    int c;
    cout << "\n1 Warrior (HP:120 | Energy:100)\n";
    cout << "2 Mage    (HP: 90 | Energy:120)\n";
    cout << "3 Assassin(HP: 85 | Energy:110)\n";
    cout << "Choose class: ";
    cin >> c;

    Character *player;
    if (c == 1)
    {
        player = new Warrior(name);
        cout << "[INFO] Warrior chosen � tough and reliable.\n";
    }
    else if (c == 2)
    {
        player = new Mage(name);
        cout << "[INFO] Mage chosen � high damage, fragile.\n";
    }
    else
    {
        player = new Assassin(name);
        cout << "[INFO] Assassin chosen � high burst, risky.\n";
    }

    Enemy goblin("Goblin", 70);
    Boss dragon("Dragon");

    // --- Round 1: Goblin ---
    battle(*player, goblin);
    if (player->health <= 0)
    {
        delete player;
        return 0;
    }

    Player *pl = dynamic_cast<Player *>(player);
    Shop shop;

    if (pl)
    {

        int reward = 80 + (pl->health / 10) * 2;
        pl->addGold(reward);
        shop.buy(*pl);
    }

    // --- Round 2: Dragon ---
    battle(*player, dragon);
    if (player->health <= 0)
    {
        delete player;
        return 0;
    }

    if (pl)
    {
        int reward = 150 + (pl->health / 10) * 3;
        pl->addGold(reward);
        shop.buy(*pl);
    }

    // --- Optional: God ---
    cout << "\nA mysterious portal opens... Face the GOD? (1=Yes / 0=No): ";
    int g;
    cin >> g;

    if (g == 1)
    {
        Boss god("GOD");
        battle(*player, god);

        if (player->health > 0 && pl)
        {
            int reward = 250 + (pl->health / 10) * 5;
            pl->addGold(reward);
            shop.buy(*pl);
        }
    }

    // --- Final Summary ---
    uiHeader("FINAL STATS");
    player->display();

    if (player->health > 0)
    {
        cout << "\n ADVENTURE COMPLETE! Well fought, " << name << "!\n";
    }

    delete player;
    return 0;
}
