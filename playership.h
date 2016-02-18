// The player ship.
class PlayerShip: public Item {
private:
    scene::ISceneNode* node;
    scene::ISceneNode* shield;
    bool shielddropped;
public:
    u32 deathtime;
    bool active;
    bool immune;
    int lives;
    scene::IAnimatedMeshSceneNode* ship;
    PlayerShip();
    void deleteShip(u32);
    bool update(u32);
    core::vector3df getPosition();
    void setPosition(core::vector3df);
    void init();
};

PlayerShip::PlayerShip () {
    size[0] = 50;
    size[1] = 50;
    lives = 2;
    
    damageNumber = 2;
    damageData.push_back(0); // nothing
    damageData.push_back(0); // bullets
    damageData.push_back(0); // player
    damageData.push_back(50); // asteroids
}

// Create another ship. Called when the game starts, or after death.
void PlayerShip::init() {
    dead = false;
    active = true;
    immune = true;
    shielddropped = false;
    health = 1;
    scene::IAnimatedMesh* playership = smgr->getMesh("models/raptor.3ds");
    if (!playership) {
    };
    node = smgr->addSphereSceneNode(0.0000001);
    shield = smgr->addSphereSceneNode(60);
    if (shield) {
        shield->setMaterialTexture(0, driver->getTexture("models/shield.tga"));
        shield->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
        shield->setMaterialFlag(video::EMF_LIGHTING, true);
        scene::ISceneNodeAnimator* anim =
                        smgr->createRotationAnimator(core::vector3df(0,1,0));
                if (anim)
                {
                        shield->addAnimator(anim);
                        anim->drop();
                }
    }
    ship = smgr->addAnimatedMeshSceneNode( playership );
    if (ship)
    {
        ship->setPosition(core::vector3df(0,0,0));
        ship->setRotation(core::vector3df(0,180,0));
        ship->setMaterialTexture(0, driver->getTexture("models/raptor.tga"));
        ship->setMaterialFlag(video::EMF_LIGHTING, true);
        ship->getMaterial(0).SpecularColor.set(100,100,100,255);
    };
    
    device->getCursorControl()->setPosition((s32)(driver->getCurrentRenderTargetSize().Width/2),(s32)(driver->getCurrentRenderTargetSize().Height * 9/10)); 
};

core::vector3df PlayerShip::getPosition() {
    return node->getPosition();
}

void PlayerShip::setPosition(core::vector3df posit) {
    node->setPosition(posit);
    if (active) {
        ship->setPosition(posit);
    }
    if (immune) {
        shield->setPosition(posit + core::vector3df(0,10,0));
    }
}

// Called on death.
void PlayerShip::deleteShip (u32 now) {
  if (active) {
    if (immune) {
      shield->remove();
      shielddropped = true;
      immune = false;
    }
    ship->remove();
    active = false;
    deathtime = now;
    lives--;
  }
}

// Called every tick. Restores dead ships, handles shielding delay, returns whether there are lives left.
bool PlayerShip::update (u32 now) {
  if (not active and now > deathtime+1500 and lives >= 0) {
    init();
  }
  if (immune and now > deathtime+4500) {
    immune = false;
  }
  if (not immune and not shielddropped) {
      shield->remove();
      shielddropped = true;
  }
  
  return (lives >= 0);
}