#include "cAudio.h"

class SfxHandler {
public:
    void init();
    void update();
    void beginmusic();
    void mediumExplo();
    void YouDie();
    void setVol(Settings);
    void kill();
    SfxHandler();
private:
    cAudio::IAudioManager* manager;
    cAudio::IAudioSource* music;
    cAudio::IAudioSource* death;
    list<cAudio::IAudioSource*> fooms;
    Settings settings;
};

SfxHandler::SfxHandler() {
}

void SfxHandler::init() {
    manager = cAudio::createAudioManager(false);
    manager->initialize(manager->getAvailableDeviceName(0));
    
    music = manager->create("discovery","BS-Connected.ogg",true);
    music->setVolume(settings.musicvol);
    death = manager->create("youdie","youdie.wav",true);
    death->setVolume(0.3*settings.soundvol);
};

void SfxHandler::beginmusic() {
    if (not music->isPlaying())
        music->play2d(true);
}

void SfxHandler::setVol(Settings thesettings) {
    settings = thesettings;
}

void SfxHandler::kill() {
    //Delete all IAudio sounds
    manager->releaseAllSources();
    //Shutdown cAudio
    manager->shutDown();
    cAudio::destroyAudioManager(manager);
}

void SfxHandler::YouDie() {
    death->play2d(false);
}

void SfxHandler::mediumExplo() {
    bool exploding = false;
    for (list<cAudio::IAudioSource*>::iterator bang = fooms.begin(); bang != fooms.end(); bang++) {
        if (not exploding and not (*bang)->isPlaying() ) {
            (*bang)->play2d(false);
            exploding = true;
        }
    }
    if (not exploding) {
        cAudio::IAudioSource* explodingness = manager->create("explo04","explo01.wav",true);
        explodingness->setVolume(settings.soundvol);
        explodingness->play2d(false);
        fooms.push_back(explodingness);
    }
}