/* The Animated class. Animated objects follow Irrlicht spline animators to move around a path. */
class Animated {
private:
    core::array<core::vector3df> points;
protected: 
    list<scene::ISceneNode*>* shots;
    virtual list<scene::ISceneNode*>* createShots()=0; // return the list of scene nodes making up the Animated object.
    virtual core::array<core::vector3df> calculateTragectory()=0; // return the list of points making up the path to follow.
    Animated ( u32 createtime ,u32 duration);
    void init ();
public:
    virtual bool finished(u32)=0; // return true if the path's complete
    u32 created;
    u32 duration;
    void deleteAnimated() ;
    core::vector3df getPosition() ;
    core::vector3df getOldPosition() ;
    std::vector<core::vector3df> positions;
    void addPosition();
};

// returns the current position of the object in space.
core::vector3df Animated::getPosition() {
    return shots->back()->getAbsolutePosition();
};

// returns the previous position of the object in space. Used to interpolate collision detection.
core::vector3df Animated::getOldPosition() {
    if (positions.size() <= 2) {return positions.front();};
    return positions[positions.size()-2];
};

// records the current position, ready to recall it in get OldPosition().
void Animated::addPosition() {
    positions.push_back( getPosition() );
}

Animated::Animated ( u32 createtime ,u32 durationparam) {
    created=createtime;
    duration=durationparam;
}

//  sets the Animated object off along its path.
void Animated::init() {  
    shots=createShots();
    points=calculateTragectory();
    
    // Add an invisible point before the first point, so the animator doesn't start off with zero velocity.
    points.insert(2*points[0]-points[1]);
    
    // Create the animator.
    // TODO: make this more general. At the moment it's hardcoded to smoothness of 0.95.
    scene::ISceneNodeAnimator* anim = 0;
    anim = smgr->createFollowSplineAnimator( created-duration/(points.size()-2.0), points, 1000.0*(points.size()-2.0)/duration,0.95,false);
    shots->back()->addAnimator(anim);
    anim->drop();
    
}
//  drops the Animated scene nodes.
//  there are a few commented out lines; I'm sure we put them in for some segfaultish/memory-leakish reason, but they don't seem to be doing anything any more. C++ is strange.
void Animated::deleteAnimated () {
    for ( list<scene::ISceneNode*>::iterator it1=shots->begin(); it1!=shots->end(); ++it1) {
        (*it1)->remove();
//      (*it1)->drop();
    }
//  shots->back()->drop();

    shots->clear();
}

// A blue glowing bullet.
class Bullet: public Animated, public Item {
private:
    core::vector3df start;
protected: 
    list<scene::ISceneNode*>* createShots();
    core::array<core::vector3df> calculateTragectory();
public:
    bool finished(u32);
public:
    Bullet (core::vector3df,u32);
    void setPosition(core::vector3df);
};

Bullet::Bullet (core::vector3df startparam, u32 createtime):
    Animated(createtime,1800) {
    start=startparam;
    size[0] = 10;
    size[1] = 40;
    
    dead = false;
    health = 1;
    damageNumber = 1;
    damageData.push_back(0); // nothing
    damageData.push_back(0); // bullets
    damageData.push_back(0); // player
    damageData.push_back(50); // asteroids
    
    init();
}

// create the blue bullet, and a glowing blue light.
list<scene::ISceneNode*>* Bullet::createShots () {
    list<scene::ISceneNode*>* shots=new list<scene::ISceneNode*>();

    scene::ISceneNode* shot;

//  attach billboard to light
//  shot = smgr->addCubeSceneNode( 1, 0, -1, core::vector3df(200,200,400), core::vector3df(0,0,0), core::vector3df(10,50,10) );
    /*
    shot = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(50, 50));
    shot->setMaterialFlag(video::EMF_LIGHTING, false);
    shot->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    shot->setMaterialTexture(0, driver->getTexture("sprites/blueshot1.png"));
    */
    shot = smgr->addCubeSceneNode( 1, 0, -1, core::vector3df(0,0,0), core::vector3df(0,0,0), core::vector3df(12,48,0) );
    // texture, lighting, normals.
    shot->setMaterialTexture(0, driver->getTexture("sprites/blueshot2.png"));
    shot->setMaterialFlag(video::EMF_LIGHTING, false);
    shot->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    
    
        
    shots->push_back(shot);
    scene::ISceneNode* light = smgr->addLightSceneNode(shot, core::vector3df(0,0,0),
                video::SColorf(0.0f, 0.2f, 0.2f), 50.0f);

    //shots->push_back(light);

    return shots;
};

core::array<core::vector3df> Bullet::calculateTragectory(){
    core::array<core::vector3df> points(3);
    points.push_back( start );
//  Some pretty strange paths are allowed here, such as the commented-out slowing down. But for now we'll just have two points.
/*  points.push_back( start+core::vector3df(0,200,0) );
    points.push_back( start+core::vector3df(0,300,0) );
    points.push_back( start+core::vector3df(0,350,0) );
    points.push_back( start+core::vector3df(0,375,0) );
    points.push_back( start+core::vector3df(0,387.5,0) );
    points.push_back( start+core::vector3df(0,388,0) );
    points.push_back( start+core::vector3df(0,388,0) );
    points.push_back( start+core::vector3df(0,388,0) );
    points.push_back( start+core::vector3df(0,388,0) );
    points.push_back( start+core::vector3df(0,388,0) );*/
    points.push_back( start+core::vector3df(0,700,0) );
    return points;
}

//  Set the position of the Bullet. I think I used this in debugging once.
void Bullet::setPosition(core::vector3df posit) {
    for ( list<scene::ISceneNode*>::iterator it1=shots->begin(); it1!=shots->end(); ++it1) {
        (*it1)->setPosition(posit);
    }
//    shots->front()->setPosition(posit);
//    shots->back()->setPosition(posit);
}

// return True if it should have finished by now.
bool Bullet::finished (u32 now) {
    return now > created+duration;
}


// Some backup code, probably redundant now.
/*class Bullet {
    scene::ISceneNode* shot1;
    scene::ISceneNode* shot2;
    video::IVideoDriver* driver;
    scene::ISceneManager* smgr;
  public:
    Bullet (video::IVideoDriver*,scene::ISceneManager*,core::vector3df,u32);
    u32 created;
    void deleteBullet() ;
    int timedelay;
};

Bullet::Bullet (video::IVideoDriver* driverparam, scene::ISceneManager* smgrparam, core::vector3df start, u32 createtime) {
    timedelay = 600;
    created = createtime;
    driver = driverparam;
    smgr = smgrparam;
    shot1 = smgr->addLightSceneNode(0, core::vector3df(0,0,0),
                video::SColorf(0.0f, 0.2f, 0.2f, 1.0f), 100.0f);
        scene::ISceneNodeAnimator* anim = 0;
//    anim = smgr->createFlyStraightAnimator(start,
//                        start+core::vector3df(0,600,0), timedelay, false);

    core::array<core::vector3df> points(3);
    points.push_back( start );
    points.push_back( start+core::vector3df(0,200,0) );
    points.push_back( start+core::vector3df(0,300,0) );
    points.push_back( start+core::vector3df(0,350,0) );
    points.push_back( start+core::vector3df(0,375,0) );
    points.push_back( start+core::vector3df(0,387.5,0) );
    points.push_back( start+core::vector3df(0,388,0) );
    points.push_back( start+core::vector3df(0,388,0) );
    points.push_back( start+core::vector3df(0,388,0) );
    points.push_back( start+core::vector3df(0,388,0) );
    points.push_back( start+core::vector3df(0,388,0) );
    points.push_back( start+core::vector3df(0,388,0) );
//    core::vector3df points [3] = { start, start+core::vector3df(100,400,0), start+core::vector3df(0,600,0) };
    anim = smgr->createFollowSplineAnimator( createtime, points, 1000.0*(points.size()-1.0)/timedelay, 0.0,false);
//        shot1->addAnimator(anim);
//        anim->drop();

        // attach billboard to light
    shot2 = smgr->addBillboardSceneNode(shot1, core::dimension2d<f32>(50, 50));
        shot2->setMaterialFlag(video::EMF_LIGHTING, false);
        shot2->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
        shot2->setMaterialTexture(0, driver->getTexture("sprites/blueshot1.png"));
        shot2->addAnimator(anim);
        anim->drop();
};

void Bullet::deleteBullet () {
    if (shot2){ 
    shot2->remove();
    shot2->drop();
    }
    if (shot1){ 
    shot1->remove();
//    shot1->drop();
    }
}*/

//  A randomly generated asteroid. Moves from the top to the bottom of the screen in a straight line.
class Asteroid: public Animated, public Item {
private:
    core::vector3df start;
protected: 
    list<scene::ISceneNode*>* createShots();
    core::array<core::vector3df> calculateTragectory();
public:
    bool finished(u32);
public:
    Asteroid (core::vector3df,u32);
};

// Init some useful variables.
Asteroid::Asteroid (core::vector3df startparam, u32 createtime):
    Animated(createtime,(5000+rand()%10000)) {
    start=startparam;
    size[0] = 60.0;
    size[1] = 60.0;
    pointvalue = 10;
    
    dead = false;
    health = 20;
    damageNumber = 3;
    damageData.push_back(0); // nothing
    damageData.push_back(2); // bullets
    damageData.push_back(5); // player
    damageData.push_back(0); // asteroids
    
    init();
}


// randomly pick an asteroid mesh, rotate it, and scale it a bit.
list<scene::ISceneNode*>* Asteroid::createShots () {
    list<scene::ISceneNode*>* shots=new list<scene::ISceneNode*>();
    int rocknum = rand() % 4 ;
    scene::ISceneNode* shot = 0;
    
    // randomly pick one of the four asteroid meshes.
    if (rocknum == 0) {
        shot = smgr->addAnimatedMeshSceneNode(smgr->getMesh("rocks/rock1.3ds"));
    }
    else if (rocknum == 1) {
        shot = smgr->addAnimatedMeshSceneNode(smgr->getMesh("rocks/rock2.3ds"));
    }
    else if (rocknum == 2) {
        shot = smgr->addAnimatedMeshSceneNode(smgr->getMesh("rocks/rock3.3ds"));
    }
    else if (rocknum == 3) {
        shot = smgr->addAnimatedMeshSceneNode(smgr->getMesh("rocks/rock4.3ds"));
    } else {
        // cube used for collision detection debugging.
        shot = smgr->addCubeSceneNode( 1, 0, -1, core::vector3df(0,0,0), core::vector3df(0,0,0), core::vector3df(60,60,60) );
    }
    if (shot and rocknum <= 3) {
        // rotate by any random angles in 3D.
        shot->setRotation(core::vector3df(rand()%360,rand()%360,rand()%360));
        
        // scale up by a factor between 0 and 2, and multiply the size, health and point value.
        f32 scalefactor = (float)(rand()%2000)/1000.0;
        health *= scalefactor;
        scalefactor *= 1.0/3.5;
        scalefactor += 1;
        size[0] *= scalefactor;
        size[1] *= scalefactor;
        pointvalue *= scalefactor;
        pointvalue *= 10;
        shot->setScale(core::vector3df(3.0*scalefactor,3.0*scalefactor,3.0*scalefactor));
        
        // texture, lighting, normals.
        shot->setMaterialTexture(0, driver->getTexture("rocks/rock.jpg"));
        shot->setMaterialFlag(video::EMF_LIGHTING, true);
        shot->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        shot->getMaterial(0).SpecularColor.set(50,50,50,50);
        
        smgr->setShadowColor(video::SColor(150,0,0,0));
    }

    shots->push_back(shot);
    return shots;
};

// Just two points. Start and finish.
core::array<core::vector3df> Asteroid::calculateTragectory(){
    core::array<core::vector3df> points(3);
    points.push_back( start );
    points.push_back( start+core::vector3df(0,-1400,0) );
    return points;
}

bool Asteroid::finished (u32 now) {
    return now > created+duration;
}

// You'll notice none of these example Animated classes have proper destructors. I'll have to write some at some point.
// But it'll be more useful to finish the load-objects-from-XML code first.