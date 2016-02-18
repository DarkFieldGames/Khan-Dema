#ifdef _MSC_VER
// We'll also define this to stop MSVC complaining about sprintf().
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace irr;
using namespace std;
//using namespace scene;
//using namespace video;
//using namespace io;
//using namespace gui;

#include "main.h"
#include "animators.h"
#include "collision.h"

int main(int argc,char *argv[]) {
  
	MyEventReceiver receiver;
// create a NULL device to detect screen resolution
    IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);
  
core::dimension2d<u32> deskres;
    bool fullscreen = false;
	int resolutionX;
	int resolutionY;
  
    if (argc >= 3){
	  deskres = core::dimension2d<u32>(atoi(argv[1]),atoi(argv[2]));
    }
    else {
      deskres = core::dimension2d<u32>(800,600);
	}
    if (argc >= 4) {
    char wnum = *argv[3];
      char* wind = argv[3];
      if (wnum == 'f') {
	fullscreen = true;
      }
    }
	nulldevice -> drop();

// now the dimensions can be used to create the real device
 device = createDevice(video::EDT_OPENGL, deskres, 32, fullscreen, false, true, &receiver);

	resolutionX = deskres.Width;
	resolutionY = deskres.Height;

	if (device == 0)
		return 1; // could not create selected driver.

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	
	smgr->setAmbientLight(video::SColorf(0.5,0.5,0.5,1));
    scene::ISceneNode* skybox=smgr->addSkyBoxSceneNode(
        driver->getTexture("backgrounds/stars2.jpg"),
        driver->getTexture("backgrounds/stars2.jpg"),
        driver->getTexture("backgrounds/stars2.jpg"),
        driver->getTexture("backgrounds/stars2.jpg"),
        driver->getTexture("backgrounds/stars2.jpg"),
        driver->getTexture("backgrounds/stars2.jpg"));

    scene::IAnimatedMesh* playership = smgr->getMesh("models/raptor.3ds");
    if (!playership) {
        device->drop();
        return 1;
    }

//	scene::IAnimatedMeshSceneNode* ship = smgr->addAnimatedMeshSceneNode( playership );
    scene::IMeshSceneNode* ship = smgr->addCubeSceneNode( 1, 0, -1, core::vector3df(0,0,410), core::vector3df(0,0,0), core::vector3df(10,50,10) );
	if (ship)
	{
		ship->setMaterialTexture(0, driver->getTexture("models/raptor.tga"));
		ship->setMaterialFlag(video::EMF_LIGHTING, true);
        ship->getMaterial(0).SpecularColor.set(100,100,100,255);
	}
	
    scene::IMeshSceneNode* rock = smgr->addCubeSceneNode( 1, 0, -1, core::vector3df(200,200,400), core::vector3df(0,0,0), core::vector3df(60,60,60) );
    if (rock)
    {
        rock->setMaterialTexture(0, driver->getTexture("rocks/rock.jpg"));
        rock->setMaterialFlag(video::EMF_LIGHTING, true);
        rock->getMaterial(0).SpecularColor.set(100,100,100,255);
    }
//    scene::ISceneNode* lightnode = 0;
//    lightnode = smgr->addLightSceneNode(0, core::vector3df(-200,0,0),
//        video::SColorf(0.2f, 0.2f, 0.2f, 1.0f), 450.0f);
//    lightnode = smgr->addLightSceneNode(0, core::vector3df(1000,0,300),
//        video::SColorf(0.3f, 0.3f, 0.3f, 1.0f), 750.0f);

    
//	scene::ICameraSceneNode* cam = smgr->addCameraSceneNode(0, core::vector3df(0,30,-40), core::vector3df(0,5,0));
	scene::ICameraSceneNode* cam = smgr->addCameraSceneNode();
        cam->setPosition(core::vector3df(0,100,0));
        cam->setTarget(core::vector3df(0,150,200));

	device->getCursorControl()->setVisible(false);

	int lastFPS = -1;
	core::vector3df lastpos;
	core::vector3df newpos;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();

	// This is the movemen speed in units per second.
	const f32 MOVEMENT_SPEED = 550.f;
    u32 firedtime = 0;
    u32 rocktime = 0;

	bool asteroidfield = false;
	bool rkeywasdown = false;

	while(device->run())
	{
		// Work out a frame delta time.
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;

		if(receiver.IsKeyDown(irr::KEY_ESCAPE) or receiver.IsKeyDown(irr::KEY_KEY_Q))
			break;
        
        if (collidingBoxes(ship->getAbsolutePosition(), ship->getAbsolutePosition() , 10,50 ,
                           rock->getAbsolutePosition(), rock->getAbsolutePosition() , 60,60 ) ) {
          cout << rand() << endl;
        };
        
        bool coun =  collidingBoxes(ship->getAbsolutePosition(), ship->getAbsolutePosition() , 50,50 ,
                           rock->getAbsolutePosition(), rock->getAbsolutePosition() , 50,50 );
        
        

		/* Check if keys W, S, A or D are being held down, and move the
		sphere node around respectively. */
		core::vector3df nodePosition = ship->getPosition();
        core::vector3df rockPosition = rock->getPosition();
		
		float maxX = 350.0;
		float minX = -350.0;
		float maxY = 500.0;
		float minY = 0.0;

		if((receiver.IsKeyDown(irr::KEY_KEY_W) or receiver.IsKeyDown(irr::KEY_UP)) and nodePosition.Y <= maxY)
			nodePosition.Y += MOVEMENT_SPEED * frameDeltaTime;
		else if((receiver.IsKeyDown(irr::KEY_KEY_S) or receiver.IsKeyDown(irr::KEY_DOWN)) and nodePosition.Y >= minY)
			nodePosition.Y -= MOVEMENT_SPEED * frameDeltaTime;

		if((receiver.IsKeyDown(irr::KEY_KEY_A) or receiver.IsKeyDown(irr::KEY_LEFT)) and nodePosition.X >= minX)
			nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
		else if((receiver.IsKeyDown(irr::KEY_KEY_D) or receiver.IsKeyDown(irr::KEY_RIGHT)) and nodePosition.X <= maxX)
			nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;

        if(receiver.IsKeyDown(irr::KEY_KEY_I) and rockPosition.Y <= maxY)
            rockPosition.Y += MOVEMENT_SPEED * frameDeltaTime;
        else if(receiver.IsKeyDown(irr::KEY_KEY_K) and rockPosition.Y >= minY)
            rockPosition.Y -= MOVEMENT_SPEED * frameDeltaTime;

        if(receiver.IsKeyDown(irr::KEY_KEY_J) and rockPosition.X >= minX)
            rockPosition.X -= MOVEMENT_SPEED * frameDeltaTime;
        else if(receiver.IsKeyDown(irr::KEY_KEY_L) and rockPosition.X <= maxX)
            rockPosition.X += MOVEMENT_SPEED * frameDeltaTime;

        if ( receiver.IsKeyDown(irr::KEY_KEY_R)) {
			rkeywasdown = true;
		} else {
		  if (rkeywasdown) {
			rkeywasdown = false;
			asteroidfield = !asteroidfield;
		  }
		}
		
		newpos.X = receiver.GetMouseState().Position.X / (float) resolutionX;
		newpos.Y = receiver.GetMouseState().Position.Y / (float) resolutionY;
		if (newpos != lastpos) {
		    core::vector3df proportion = newpos;
		
		    proportion.X *= maxX-minX;
		    proportion.X -= maxX;
		
		    proportion.Y *= minY-maxY;
		    proportion.Y += maxY;
		
		    proportion.Z = nodePosition.Z;
		    
		    if (proportion.X <= minX){ proportion.X = minX; }
		    if (proportion.X >= maxX){ proportion.X = maxX; }
		    if (proportion.Y >= maxY){ proportion.Y = maxY; }
		    if (proportion.Y <= minY){ proportion.Y = minY; }
		
		    ship->setPosition(proportion);
		} else {
		    ship->setPosition(nodePosition);
		}
            rock->setPosition(rockPosition);
		lastpos = newpos;
		
		driver->beginScene(true, true, video::SColor(255,113,113,133));

		smgr->drawAll(); // draw the 3d scene
		device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw tmp(L"Khan Dema [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
		
	}

	/*
	In the end, delete the Irrlicht device.
	*/
	device->drop();
	
	return 0;
}
