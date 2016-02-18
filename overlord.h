// THE OVERLORD! This tells everything else what to do.
// At the moment, it does a bit too much. I'll be passing a few of its functions over to the EventHandler class once I write it.

class Overlord {
public:
    int gamestate;
    int score;
    gui::IGUIStaticText* scorelabel;
    gui::IGUIStaticText* liveslabel;
    
    SfxHandler sounds;
    Settings settings;
    
    Overlord(MyEventReceiver*,Settings,SfxHandler);
    void BeginGame(u32);
    void EndGame();
    void UpdateState(u32, f32);
    void UpdateWorld(u32, f32);
    void UpdatePlayer(u32, f32);
    void InitStars();
    void MakeGUI();
    void ShowControls();
    void KillGUI();
    gui::IGUIElement* title;
    gui::IGUIElement* controldetails;
    bool showcontrols;
    gui::IGUIButton* newgame;
    gui::IGUIButton* controls;
    gui::IGUIButton* quitgame;
    
    MyEventReceiver* receiver;
    
    
private:
    s32 resolutionX;
    s32 resolutionY;
    
    float maxX;
    float minX;
    float maxY;
    float minY;
  
    f32 MOVEMENT_SPEED;
    u32 firedtime;
    u32 rocktime;
    u32 starTime;
    u32 nebtime;
    u32 nebrandom;
    
    PlayerShip* playership;
    
    list<Bullet> bullets;
    list<Asteroid> asteroids;
    list<Star> stars;
    list<Nebula> nebulae;
    list<BulletHitSpray> bulletfx;
    list<MediumExplosion> medexfx;
    gui::IGUIElement* gameover;
    
    bool asteroidfield;
    bool backgr;
    float density;
    
    bool rkeywasdown;
    bool bkeywasdown;
    bool killkeywasdown;
    bool killroids;
    
    bool stillplaying;
    bool gameovershown;
    
    core::vector3df lastpos;
    core::vector3df newpos;
};

Overlord::Overlord (MyEventReceiver* receivepointer, Settings thesettings, SfxHandler thesounds) {
    settings = thesettings;
    sounds = thesounds;
    sounds.setVol(settings);
    
    MOVEMENT_SPEED = 550.f;
    firedtime = 0;
    starTime = 0;
    rocktime = 15000;
    nebtime = 15000;
    nebrandom = 15000;
    gamestate = 0;
    density = 300;    
    backgr = true;
    gameovershown = false;
    killkeywasdown = false;
    killroids = false;
    
    score = 0;
    
    maxX = 350.0;
    minX = -350.0;
    maxY = 500.0;
    minY = 0.0;
    receiver = receivepointer;
    resolutionX = settings.resolution.Width;
    resolutionY = settings.resolution.Height;
    gamestate = 0;
    
     // Create the lives and score boxes, but don't put any text in them yet.
     scorelabel = env->addStaticText(L"  ", core::rect<s32>(resolutionX-200,50,resolutionX-50,100), true);
     scorelabel->setOverrideColor(video::SColor(255,255,255,255));
     scorelabel->setTextAlignment(EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT);
     scorelabel->setDrawBorder(false);
     liveslabel = env->addStaticText(L"  ", core::rect<s32>(50,50,100,100), true);
     liveslabel->setOverrideColor(video::SColor(255,255,255,255));
     liveslabel->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
     liveslabel->setDrawBorder(false);
     
     InitStars();
     MakeGUI();
    //BeginGame(0);
    
    sounds.init();
    
};

// Begin the game!
void Overlord::BeginGame(u32 now) {
    score = 0;
    if (gamestate != 1) {
        device->getCursorControl()->setVisible(false);

        playership = new PlayerShip();
        playership->init(); // Create the player.
        
        asteroidfield = true;
        rkeywasdown = false;
        bkeywasdown = false;
        killkeywasdown = false;
        backgr = true;
        
        density = 500;
        // asteroid density is calculated from time since last death. This resets the clock.
        playership->deathtime = now; 
        
        gamestate = 1;
        killroids = true;
        
        sounds.beginmusic();
    }
};

// End the game.
void Overlord::EndGame() {
    if (gamestate = 1) {
    device->getCursorControl()->setVisible(true);
    
    gameover = env->addImage(driver->getTexture("gui/game_over.tga"),
                core::position2d<int>((resolutionX/2) -150,(resolutionY/2)-60));
    gameovershown = true;
    gamestate = 2; // This gets picked up by UpdateState.
    }
};

// Update the game state.
void Overlord::UpdateState(u32 now, f32 frameDeltaTime) {
    // if it's been in the Gameover state for 2 seconds, bring the gui back
    if (gamestate == 2 and now > 2000 + playership->deathtime) {
      density = 500;    
      gamestate = 0;
      MakeGUI();
    };
    
    // if the gui's in place, check for button presses
    if (gamestate==0) {
        if (receiver->Context.NewGame or receiver->IsKeyDown(irr::KEY_KEY_N) or receiver->IsKeyDown(irr::KEY_KEY_C)) { KillGUI(); BeginGame (now); };
        if (receiver->Context.Controls) { ShowControls (); };
    };
    receiver->Context.NewGame = false;
    receiver->Context.Controls = false;
}

// Update the objects in the universe.
// Pretty much all of this function will be handled by the EventHandler, once I write it.
void Overlord::UpdateWorld(u32 now, f32 frameDeltaTime) {
  
    // pick up Q or ESC button presses
    if( receiver->IsKeyDown(irr::KEY_ESCAPE) or receiver->IsKeyDown(irr::KEY_KEY_Q)) {
        killkeywasdown = true;
    } else {
        if (killkeywasdown) {
            killkeywasdown = false;
            if (gamestate == 1) { // if in game,
              playership->deleteShip(now);
              EndGame(); // end the game
            } else { // if not in game,
                device->closeDevice(); // quit the program
            }
        }
    }
    
    // spawn new asteroids
    if (asteroidfield and (now-rocktime)>density) {
        rocktime = now;
        int asterxval = (int)(minX-200) + rand()%(int)(maxX-minX+400);
        asteroids.push_back(Asteroid(core::vector3df(asterxval,900,0),now ) );
    }

    // spawn background stars
    if (backgr and (now-starTime)>2) {
        starTime = now;
        int starxval = 2*(int)minX + rand()%(int)(2.5*maxX-2.5*minX);
        stars.push_back(Star(core::vector3df(starxval,900,200),now ) );
        starxval = 2*(int)minX + rand()%(int)(2*maxX-2*minX);
        stars.push_back(Star(core::vector3df(starxval,900,200),now ) );
        starxval = 2*(int)minX + rand()%(int)(2*maxX-2*minX);
        stars.push_back(Star(core::vector3df(starxval,900,200),now ) );
    }
    // spawn background nebulae
    if (backgr and (now-nebtime)>nebrandom) {
        nebtime = now;
        int xval = 2*(int)minX + rand()%(int)(2*maxX-2*minX);
        nebulae.push_back(Nebula(core::vector3df(xval,1500,300),now ) );
        nebrandom = 3500 + rand()%7500;
    }

    // check each type of object; delete dead or expired ones
    
    for ( list<Bullet>::iterator it1=bullets.begin(); it1!=bullets.end(); ++it1) {
        if ( it1->finished(now) or it1->dead ) { 
            it1->deleteAnimated();
            it1 = bullets.erase(it1);
        };
    }
    for ( list<Asteroid>::iterator it1=asteroids.begin(); it1!=asteroids.end(); ++it1) {
        if ( it1->finished(now) ) { 
            it1->deleteAnimated();
            it1 = asteroids.erase(it1);
        } else 
        if ( it1->dead ) { 
            medexfx.push_back(MediumExplosion(it1->getPosition(),now)); // Foom!
            sounds.mediumExplo();
            score += it1->pointvalue;
            it1->deleteAnimated();
            it1 = asteroids.erase(it1);
        };
    }
    
    
    // For some strange strange reason, this doesn't delete all the asteroids unless it happens three times.
    if ( killroids ) {
        for ( list<Asteroid>::iterator it1=asteroids.begin(); it1!=asteroids.end(); ++it1) {
            it1->deleteAnimated();
            it1 = asteroids.erase(it1);
        }  
        for ( list<Asteroid>::iterator it1=asteroids.begin(); it1!=asteroids.end(); ++it1) {
            it1->deleteAnimated();
            it1 = asteroids.erase(it1);
        }  
        for ( list<Asteroid>::iterator it1=asteroids.begin(); it1!=asteroids.end(); ++it1) {
            it1->deleteAnimated();
            it1 = asteroids.erase(it1);
        }  
    }
    
    killroids = false;
    
    for ( list<Star>::iterator it1=stars.begin(); it1!=stars.end(); ++it1) {
        if ( it1->finished(now) ) { 
            it1->deleteAnimated();
            it1 = stars.erase(it1);
        };
    }
    for ( list<Nebula>::iterator it1=nebulae.begin(); it1!=nebulae.end(); ++it1) {
        if ( it1->finished(now) ) { 
            it1->deleteAnimated();
            it1 = nebulae.erase(it1);
        };
    }
    for ( list<BulletHitSpray>::iterator it1=bulletfx.begin(); it1!=bulletfx.end(); ++it1) {

        if ( it1->inittime+it1->emittime < now ) {
            it1->deleteEmitter();
            if (it1->inittime+it1->lifetime < now) {
                it1->deleteSystem();
                it1 = bulletfx.erase(it1);
            }
        };
    }
    for ( list<MediumExplosion>::iterator it1=medexfx.begin(); it1!=medexfx.end(); ++it1) {

        if ( it1->inittime+it1->emittime < now ) {
            it1->deleteEmitter();
            if (it1->inittime+it1->lifetime < now) {
                it1->deleteSystem();
                it1 = medexfx.erase(it1);
            }
        };
    }
    
    // Record positions of bullets and asteroids, for interpolating collision detection.
    for ( list<Bullet>::iterator it1=bullets.begin(); it1!=bullets.end(); ++it1) {
      it1->addPosition();
//          it1->setPosition(it1->getPosition());
      //coutVector( it1->getPosition() , true );
      
    };

    for ( list<Asteroid>::iterator it1=asteroids.begin(); it1!=asteroids.end(); ++it1) {
      it1->addPosition();
    };
    

};

// Update the player
void Overlord::UpdatePlayer(u32 now, f32 frameDeltaTime) {
  
    // Slightly kludgy way of showing the score and lives remaining.
    // TODO: this should probably use core::stringw instead.
    wstringstream StrStream;
    StrStream << score;
    wstring ScoreString = StrStream.str();
    scorelabel->setText(ScoreString.c_str());
    
    wstringstream StrStream2;
    StrStream2 << (playership->lives);
    wstring ScoreString2 = StrStream2.str();
    liveslabel->setText(ScoreString2.c_str());

    // This was the old way, but it wouldn't compile in MinGW.
/*    wchar_t buf[100]; 
    int len = swprintf( buf, 100, L"%d", score);
    scorelabel->setText(buf);
    len = swprintf( buf, 100, L"%d", playership->lives);
    liveslabel->setText(buf);*/
    density = 500000.0/pow(500*(now-playership->deathtime),0.5);
    
    // check collision detection between asteroids and bullets.
    for ( list<Asteroid>::iterator roid1=asteroids.begin(); roid1!=asteroids.end(); ++roid1) {
        for ( list<Bullet>::iterator bull1=bullets.begin(); bull1!=bullets.end(); ++bull1) {
            core::vector3df ping = bull1->getOldPosition();
            if ( collidingBoxes(roid1->getPosition(), roid1->getOldPosition(),roid1->size[0], roid1->size[1],bull1->getPosition(), bull1->getOldPosition(), bull1->size[0], bull1->size[1]) ) {
                bull1->isHit(roid1->damageNumber);
                roid1->isHit(bull1->damageNumber);
                bulletfx.push_back(BulletHitSpray(bull1->getPosition(),now ) );
            };
        };
//      Check if it's colliding with the player ship.
        if ( playership->active and collidingBoxes(roid1->getPosition(), roid1->getPosition(), roid1->size[0], roid1->size[1], 
                    playership->getPosition(), playership->getPosition(), playership->size[0],playership->size[1]) ) {
            roid1->isHit(playership->damageNumber);
            if (not playership->immune) {
                playership->isHit(roid1->damageNumber);
            };
        };
    }; 
    
    
    /* Check if keys W, S, A or D are being held down, and move the
    player node around respectively. */
    core::vector3df nodePosition = playership->getPosition();
    
    if((receiver->IsKeyDown(irr::KEY_KEY_W) or receiver->IsKeyDown(irr::KEY_UP)) and nodePosition.Y <= maxY)
        nodePosition.Y += MOVEMENT_SPEED * frameDeltaTime;
    else if((receiver->IsKeyDown(irr::KEY_KEY_S) or receiver->IsKeyDown(irr::KEY_DOWN)) and nodePosition.Y >= minY)
        nodePosition.Y -= MOVEMENT_SPEED * frameDeltaTime;

    if((receiver->IsKeyDown(irr::KEY_KEY_A) or receiver->IsKeyDown(irr::KEY_LEFT)) and nodePosition.X >= minX)
        nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
    else if((receiver->IsKeyDown(irr::KEY_KEY_D) or receiver->IsKeyDown(irr::KEY_RIGHT)) and nodePosition.X <= maxX)
        nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;


    if ( receiver->IsKeyDown(irr::KEY_KEY_R)) {
        rkeywasdown = true;
    } else {
        if (rkeywasdown) {
            rkeywasdown = false;
            asteroidfield = !asteroidfield;
        }
    }
    
    // L key exports the whole scene to debug.irr . Useful for checking memory leaks.
    if (receiver->IsKeyDown(irr::KEY_KEY_L)) {
        cout << "Debug" << endl;
        smgr->saveScene("debug.irr");
    }
    
    // B key toggles background events (stars and so forth)
    if ( receiver->IsKeyDown(irr::KEY_KEY_B)) {
        bkeywasdown = true;
    } else {
        if (bkeywasdown) {
            bkeywasdown = false;
            backgr = !backgr;
        }
    }
    
    // Move the ship to the mouse position.
    // TODO: limit the max speed.
    newpos.X = receiver->GetMouseState().Position.X / (float) resolutionX;
    newpos.Y = receiver->GetMouseState().Position.Y / (float) resolutionY;
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
    
        playership->setPosition(proportion);
    } else {
        playership->setPosition(nodePosition);
    }
    lastpos = newpos;
    
    // If the Z key or LMB is down, shoot bullets.
    if(( receiver->IsKeyDown(irr::KEY_KEY_Z) or receiver->MouseState.LeftButtonDown) and (now-firedtime)>100 and playership->active ) {
        firedtime = now;
        bullets.push_back(Bullet(nodePosition+core::vector3df(22,45,-6),now ) );
        bullets.push_back(Bullet(nodePosition+core::vector3df(-22,45,-6),now ) );
    }
    
    // If the player's dead, explode and delete the player ship.
    if ( playership->dead ) {
        medexfx.push_back(MediumExplosion(playership->getPosition(),now));
        sounds.YouDie();
        playership->deleteShip(now);
        playership->dead = false;
    }
    
    // Check if the game's still going.
    if (not playership->update(now)) {
      EndGame();
    };
};

// Create the GUI.
void Overlord::MakeGUI() {
    if (gameovershown) {
        gameover->remove();
    }
    showcontrols = false;
    title = env->addImage(driver->getTexture("gui/title1.png"),
                          core::position2d<int>((resolutionX/2 - 180), 1*resolutionY/7 ));
    
      core::rect<s32> newgamebox  = core::rect<s32>(resolutionX/5,( 4*resolutionY/7)    ,resolutionX/5 + 168,(4*resolutionY/7)    +34); 
      core::rect<s32> controlbox  = core::rect<s32>(resolutionX/5,( 4*resolutionY/7)+ 60,resolutionX/5 + 159,(4*resolutionY/7) +60+34); 
      core::rect<s32> quitgamebox = core::rect<s32>(resolutionX/5,( 4*resolutionY/7)+120,resolutionX/5 +  76,(4*resolutionY/7)+120+34); 
      newgame = env->addButton(newgamebox,0,GUI_ID_NEW_GAME_BUTTON); 
      controls = env->addButton(controlbox,0,GUI_ID_CONTROLS_BUTTON); 
      quitgame = env->addButton(quitgamebox,0,GUI_ID_QUIT_BUTTON); 
    
      newgame->setImage(driver->getTexture("gui/smallnewgame.png"));
      newgame->setUseAlphaChannel(true); 
      newgame->setDrawBorder(0);
      newgame->setDrawBorder(0);
      controls->setImage(driver->getTexture("gui/smallcontrols.png"));
      controls->setUseAlphaChannel(true); 
      controls->setDrawBorder(0);
      quitgame->setImage(driver->getTexture("gui/smallquit.png"));
      quitgame->setUseAlphaChannel(true); 
      quitgame->setDrawBorder(0);
}

// show the Controls diagram.
void Overlord::ShowControls() {
    if (not showcontrols) {
      controldetails = env->addImage(driver->getTexture("gui/controldiag.png"),
                        core::position2d<int>((9*resolutionX/10)-304,( (7*resolutionY/7)-340-(50*resolutionY/550))));
//  core::rect<s32> controlbox  = core::rect<s32>(resolutionX/5,( 4*resolutionY/7)+ 60,resolutionX/5 + 159,(4*resolutionY/7) +60+34); 
        showcontrols = true;
    }
}

void Overlord::KillGUI() {
    // remove the GUI.
    if (showcontrols) {
        controldetails->remove();
    }
    title->remove();
    newgame->remove();
    controls->remove();
    quitgame->remove();
}

// Create a starry background when the program loads up.
// The rest of the time, stars just drop from the top of the screen at a constant rate.
void Overlord::InitStars() {
    for(int i= 0; i < 750; i++) {
        int starxval = 2*(int)minX + rand()%(int)(2*maxX-2*minX);
        int staryval = 2*(int)minY + rand()%(int)(2*maxY-2*minY);
        stars.push_back(Star(core::vector3df(starxval,staryval,200),0 ) );
    }
}
