// Define some values that we'll use to identify individual GUI controls.
enum
{
    GUI_ID_QUIT_BUTTON = 101,
    GUI_ID_NEW_GAME_BUTTON,
    GUI_ID_CONTROLS_BUTTON
};

// EventReceiver handles all keyboard and mouse input, and GUI events.
class MyEventReceiver : public IEventReceiver
{
public:
//    MyEventReceiver(SAppContext & context) : Context(context) { }

    struct SAppContext {
        bool Controls;
        bool NewGame;
    } Context;
    
    struct SMouseState
    {
        core::position2di Position;
        bool LeftButtonDown;
        SMouseState() : LeftButtonDown(false) { }
    } MouseState;
    
    virtual bool OnEvent(const SEvent& event)
    {
        if (event.EventType == EET_GUI_EVENT) {
            s32 id = event.GUIEvent.Caller->getID();

            switch(event.GUIEvent.EventType)
            {
            case EGET_BUTTON_CLICKED:
                switch(id)
                {
                case GUI_ID_QUIT_BUTTON:
                    device->closeDevice();
                    return true;

                case GUI_ID_NEW_GAME_BUTTON:
                {
                    Context.NewGame = true;
                }
                return true;

                case GUI_ID_CONTROLS_BUTTON:
                {
                    Context.Controls = true;
                }
                return true;

                default:
                    Context.NewGame = false;
                    Context.Controls = false;
                    return false;
                }
                break;

            default:
                break;
            }
        }
        
        //////////////////////////////////////////////////////
      
//  Remember the mouse state

        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
        {
            switch(event.MouseInput.Event)
            {
                case EMIE_LMOUSE_PRESSED_DOWN:
                    MouseState.LeftButtonDown = true;
                break;

                case EMIE_LMOUSE_LEFT_UP:
                    MouseState.LeftButtonDown = false;
                break;

                case EMIE_MOUSE_MOVED:
                    MouseState.Position.X = event.MouseInput.X;
                    MouseState.Position.Y = event.MouseInput.Y;
                break;

                default:
                    // We won't use the wheel
                break;
            }
        }
      
        // Remember whether each key is down or up
        else if (event.EventType == irr::EET_KEY_INPUT_EVENT)
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

        return false;
    }

    // This is used to check whether a key is being held down
    virtual bool IsKeyDown(EKEY_CODE keyCode) const
    {
        return KeyIsDown[keyCode];
    }
    
    
    const SMouseState & GetMouseState(void) const
    {
        return MouseState;
    }
    
    
    MyEventReceiver()
    {
        Context.Controls = false;
        Context.NewGame = false;
        for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
    }

private:
    // We use this array to store the current state of each key
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

