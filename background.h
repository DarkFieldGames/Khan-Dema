// Background events. Stars, nebulae and so forth.

class Star: public Animated {
private:
    core::vector3df start;
protected: 
    list<scene::ISceneNode*>* createShots();
    core::array<core::vector3df> calculateTragectory();
public:
    bool finished(u32);
public:
    Star (core::vector3df,u32);
};

Star::Star (core::vector3df startparam, u32 createtime):
    Animated(createtime,15000) {
    start=startparam;
    init();
}

list<scene::ISceneNode*>* Star::createShots () {
    list<scene::ISceneNode*>* shots=new list<scene::ISceneNode*>();
    int size = 2+(rand()%5);
    scene::ISceneNode* shot = smgr->addBillboardSceneNode(shot, core::dimension2d<f32>(size,size));
    shot->setMaterialFlag(video::EMF_LIGHTING, false);
    shot->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    shot->setMaterialTexture(0, driver->getTexture("backgrounds/star.tga"));
    
    shots->push_back(shot);

    return shots;
};

core::array<core::vector3df> Star::calculateTragectory(){
    core::array<core::vector3df> points(3);
    points.push_back( start );
    points.push_back( start+core::vector3df(0,-1400,0) );
    return points;
}

bool Star::finished (u32 now) {
    return now > created+duration;
}


class Nebula: public Animated {
private:
    core::vector3df start;
protected: 
    list<scene::ISceneNode*>* createShots();
    core::array<core::vector3df> calculateTragectory();
public:
    bool finished(u32);
public:
    Nebula (core::vector3df,u32);
};

Nebula::Nebula (core::vector3df startparam, u32 createtime):
    Animated(createtime,17000) {
    start=startparam;
    init();
}

list<scene::ISceneNode*>* Nebula::createShots () {
    list<scene::ISceneNode*>* shots=new list<scene::ISceneNode*>();
    int size = (rand()%1280);
    scene::ISceneNode* shot = smgr->addBillboardSceneNode(shot, core::dimension2d<f32>(size,(800*size)/1280));
    shot->setMaterialFlag(video::EMF_LIGHTING, false);
    shot->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    shot->setMaterialTexture(0, driver->getTexture("backgrounds/clouds1.png"));
    
    shots->push_back(shot);

    return shots;
};

core::array<core::vector3df> Nebula::calculateTragectory(){
    core::array<core::vector3df> points(3);
    points.push_back( start );
    points.push_back( start+core::vector3df(0,-2400,0) );
    return points;
}

bool Nebula::finished (u32 now) {
    return now > created+duration;
}