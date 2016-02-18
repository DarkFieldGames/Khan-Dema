// Particle effects.

class ParticleFX {
public:
  list<scene::IParticleSystemSceneNode*>* pnodes;
  virtual list<scene::IParticleSystemSceneNode*>* createPnodes(core::vector3df)=0;
  scene::ISceneNode* light;
  u32 lifetime;
  u32 emittime;
  u32 inittime;
  void deleteEmitter();
  void deleteSystem();
  bool lit;
  bool killed;
  void init();
};

void ParticleFX::init(  ) {
    killed = false;
};

// drop all the particle system emitter, and prepare to delete the scene nodes.
void ParticleFX::deleteEmitter() {
    if (not killed) {
        if (lit) { 
            light->remove();
        };
        for ( list<scene::IParticleSystemSceneNode*>::iterator it1=pnodes->begin(); it1!=pnodes->end(); ++it1) {
            (*it1)->setEmitter(0);
            scene::ISceneNodeAnimator* anim = smgr->createDeleteAnimator( 800 );
            (*it1)->addAnimator(anim);
            anim->drop();
            killed = true;
            //it1 = pnodes->erase(it1);
    //      (*it1)->drop();
        }
    }
};

// delete the particle system scene nodes.
// not actually used any more, I don't think. It was segfaulting.
void ParticleFX::deleteSystem() {
      //smgr->getRootSceneNode()->removeChild(ps);
    //ps->drop();
};

// The spray of blue particles when a bullet hits something.
class BulletHitSpray: public ParticleFX {
public:
    BulletHitSpray( core::vector3df, u32 now );
    list<scene::IParticleSystemSceneNode*>* createPnodes(core::vector3df);
};


BulletHitSpray::BulletHitSpray(core::vector3df initpos , u32 now) {
    lifetime = 1000;
    emittime = 100;
    lit = false;
    pnodes = createPnodes(initpos);
    inittime = now;    
}

// Create the scene node.
list<scene::IParticleSystemSceneNode*>* BulletHitSpray::createPnodes(core::vector3df initpos) {
    list<scene::IParticleSystemSceneNode*>* pnodes=new list<scene::IParticleSystemSceneNode*>();
    
    scene::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false);

    scene::IParticleEmitter* em;
    em = ps->createPointEmitter(
            //core::aabbox3d<f32>(-7,0,-7,7,1,7), // emitter size
            core::vector3df(0.0f,-0.3f,0.0f),   // initial direction
            25,50,                             // emit rate
            video::SColor(0,255,255,255),       // darkest color
            video::SColor(0,255,255,255),       // brightest color
            300,900,45,                         // min and max age, angle
            core::dimension2df(5.f,5.f),         // min size
            core::dimension2df(10.f,10.f));        // max size

    ps->setEmitter(em); // this grabs the emitter
    em->drop(); // so we can drop it here without deleting it

    scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(video::SColor(0,0,0,0),100);
    ps->addAffector(paf); // same goes for the affector
    paf->drop();

    ps->setPosition(initpos);
    ps->setScale(core::vector3df(2,2,2));
    ps->setMaterialFlag(video::EMF_LIGHTING, false);
    ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialTexture(0, driver->getTexture("sprites/scatter.png"));
    ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
    
    pnodes->push_back(ps);
    
    init();
    return pnodes;
};

// A medium-sized explosion.
class MediumExplosion: public ParticleFX {
public:
    MediumExplosion( core::vector3df, u32 now );
    list<scene::IParticleSystemSceneNode*>* createPnodes(core::vector3df);
};


MediumExplosion::MediumExplosion(core::vector3df initpos , u32 now) {
    lifetime = 1500;
    emittime = 100;
    lit = true;
    pnodes = createPnodes(initpos);
    inittime = now;    
}

// Create the light, the main billboard of the explosion, and the smaller dots.
list<scene::IParticleSystemSceneNode*>* MediumExplosion::createPnodes(core::vector3df initpos) {
    light = smgr->addLightSceneNode(0, initpos, video::SColorf(0.87f, 0.71f, 0.52f), 100.0f);
    
    // create the main billboard
    
    list<scene::IParticleSystemSceneNode*>* pnodes=new list<scene::IParticleSystemSceneNode*>();
    
    scene::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false);

    scene::IParticleEmitter* em;
    em = ps->createBoxEmitter(
            core::aabbox3d<f32>(5,-5,5,-5,1,0), // emitter size
            core::vector3df(0.0f,-0.05f,0.0f),   // initial direction
            45,60,                             // emit rate
            video::SColor(0,255,255,255),       // darkest color
            video::SColor(0,255,255,255),       // brightest color
            400,500,360,                         // min and max age, angle
            core::dimension2df(100.f,100.f),         // min size
            core::dimension2df(200.f,200.f));        // max size

    ps->setEmitter(em); // this grabs the emitter
    em->drop(); // so we can drop it here without deleting it

    scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(video::SColor(0,0,0,0),1500);
    ps->addAffector(paf); // same goes for the affector
    paf->drop();

    ps->setPosition(initpos);
    ps->setScale(core::vector3df(2,2,2));
    ps->setMaterialFlag(video::EMF_LIGHTING, false);
    ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialTexture(0, driver->getTexture("sprites/explodeheat.tga"));
    ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
    
    pnodes->push_back(ps);
    
    // Create the smaller dots
    
    ps = smgr->addParticleSystemSceneNode(false);
    
    em = ps->createBoxEmitter(
            core::aabbox3d<f32>(3,-3,3,-3,3,-3), // emitter size
            core::vector3df(0.0f,-0.3f,0.0f),   // initial direction
            500,1000,                             // emit rate
            video::SColor(0,255,255,255),       // darkest color
            video::SColor(0,255,255,255),       // brightest color
            300,900,360,                         // min and max age, angle
            core::dimension2df(5.f,5.f),         // min size
            core::dimension2df(10.f,10.f));        // max size

    ps->setEmitter(em); // this grabs the emitter
    em->drop(); // so we can drop it here without deleting it

    paf = ps->createFadeOutParticleAffector(video::SColor(0,0,0,0),100);
    ps->addAffector(paf); // same goes for the affector
    paf->drop();

    ps->setPosition(initpos);
    ps->setScale(core::vector3df(2,2,2));
    ps->setMaterialFlag(video::EMF_LIGHTING, false);
    ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialTexture(0, driver->getTexture("sprites/explodeheat.tga"));
    ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
    
    pnodes->push_back(ps);
    
    init();
    return pnodes;
};


