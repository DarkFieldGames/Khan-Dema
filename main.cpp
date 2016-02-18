#ifdef _MSC_VER
// We'll also define this to stop MSVC complaining about sprintf().
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <stdio.h>
#include <stdlib.h>
#include <irrlicht.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <ctime>
#include <map>

#include "/usr/include/tinyxml.h"
//#include "/usr/include/tinystr.h"

using namespace irr;
using namespace std;

#include "main.h"
#include "settings.h"
#include "gui.h"
#include "sound.h"

#include "xmlread.h"

#include "items.h"
#include "animators.h"
#include "collision.h"

#include "background.h"
#include "particlefx.h"
#include "playership.h"

#include "overlord.h"

int main(int argc,char *argv[]) {
    // set random seed to something random
    srand(time(NULL));
    
    MyEventReceiver *receiver = new MyEventReceiver;
    
    Settings settings;
    settings.get();
    
    // parse arguments
    if (argc >= 3){
        settings.resolution = core::dimension2d<u32>(atoi(argv[1]),atoi(argv[2]));
    }
    if (argc >= 4) {
        char wnum = *argv[3];
        if (wnum == 'w') {
          settings.windowed = 1;
        } else if (wnum == 'f') {
            settings.windowed = 0;
        }
    }
    settings.write();

    // now the dimensions can be used to create the real device
    if (settings.rendername == "software") {
        device = createDevice(video::EDT_SOFTWARE, settings.resolution, 32, !((bool)settings.windowed), false, true, receiver);
    } else if (settings.rendername == "software2") {
        device = createDevice(video::EDT_BURNINGSVIDEO, settings.resolution, 32, !((bool)settings.windowed), false, true, receiver);
    } else {
        device = createDevice(video::EDT_OPENGL, settings.resolution, 32, !((bool)settings.windowed), false, true, receiver);
    }

    if (device == 0)
        return 1; // could not create selected driver.

    // Set global pointers to the VideoDriver, SceneManager and GUIEnvironment
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    env = device->getGUIEnvironment();
    
    // Set some GUI things
    IGUISkin* skin = env->getSkin();
    IGUIFont* font = env->getFont("fontlucida.png");
    if (font) {
            skin->setFont(font);
    };
    device->setResizable(true);
    
    smgr->setAmbientLight(video::SColorf(0.2,0.2,0.2,1));
    
    
    // Create the Overlord! This tells everything else what to do.
    SfxHandler sounds;
    Overlord control (receiver, settings, sounds);

    // Some optional point lights
    /*
    scene::ISceneNode* lightnode1 = 0;
    lightnode1 = smgr->addLightSceneNode(0, core::vector3df(-200,0,-300),
        video::SColorf(0.2f, 0.2f, 0.2f, 0.2f), 450.0f);
    scene::ISceneNode* lightnode2 = 0;
    lightnode2 = smgr->addLightSceneNode(0, core::vector3df(200,400,-300),
        video::SColorf(0.3f, 0.3f, 0.3f, 0.3f), 350.0f);
    */

    // The camera. A useful thing to have.
    // This should really be part of Overlord, I reckon. I'll sort that out some time.
    scene::ICameraSceneNode* cam = smgr->addCameraSceneNode();
    int fpos = 70;
    int tilt = 200;
        cam->setPosition(core::vector3df(0,fpos,-800));
        cam->setTarget(core::vector3df(0,fpos+tilt,0));
        cam->setFOV(3.141592654/4.0); 


    // In order to do framerate independent movement, we have to know
    // how long it was since the last frame
    u32 then = device->getTimer()->getTime();
    int lastFPS = -1;
    
    smgr->setShadowColor(video::SColor(150,0,0,0));

    // To begin the game when the program starts, uncomment this line.
//    control.KillGUI(); control.BeginGame(0);
    
    // Each game frame:
    while(device->run()) {
        
        // Work out a frame delta time.
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
        then = now;
        
        
        control.UpdateState(now,frameDeltaTime); // Update the game state
        
        control.UpdateWorld(now,frameDeltaTime); // Update the objects in the universe
        
        if (control.gamestate == 1) // If the game's playing, update the player position etc
            control.UpdatePlayer(now,frameDeltaTime);
        
        driver->beginScene(true, true, video::SColor(255,0,0,0));
        smgr->drawAll(); // draw the 3d scene
        env->drawAll();  // draw the GUI
        driver->endScene();

        int fps = driver->getFPS();

        // Set the window title
        if (lastFPS != fps)
        {
            core::stringw tmp(L"Khan Dema [");
            tmp += driver->getName();
            tmp += L"] fps: ";
            tmp += fps;

            device->setWindowCaption(tmp.c_str());
            lastFPS = fps;
        }
    // End of frame.
    }

    // In the end, delete the Irrlicht device.
    device->drop();
    control.sounds.kill();
    return 0;
}
