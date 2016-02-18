/* The Item class. Item objects have health, point values, and collision detection related things. */

class Item {
public:
    float size [2];
    int health;
    int damageNumber;
    vector<int> damageData;
    void isHit(int);
    bool dead;
    int pointvalue;
};

void Item::isHit(int hitness) {
    if (hitness <= damageData.size()) {
        health -= damageData[hitness];
    };
    if (health <= 0) {
        dead = true;
    };
}