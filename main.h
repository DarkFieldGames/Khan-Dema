// Here begin the evil global variables.
//I'm still reasonably surprised that this is working ,with these things cooperating between files. That'll probably come back to haunt me at some point.
static IrrlichtDevice *device;
static video::IVideoDriver *driver;
static scene::ISceneManager *smgr;
static gui::IGUIEnvironment *env;
static io::IFileSystem *fileio;
static ITimer *timer;
static ILogger *logs;
// Here end the evil global variables.

using namespace gui;

// printing a core::vector3df to stdout.
void coutVector(core::vector3df vectorness, bool endil) {
    cout << vectorness.X << " ";
    cout << vectorness.Y << " ";
    cout << vectorness.Z << " ";
    if (endil) { cout << endl; };
}

void coutVector(core::vector3df vectorness) {
    cout << vectorness.X << " ";
    cout << vectorness.Y << " ";
    cout << vectorness.Z << " ";
    cout << endl; 
}