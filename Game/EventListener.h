#ifndef _EVENTLISTENER_H

#define _EVENTLISTENER_H

#include "spiderDataTypes.h"
#include "KeyCodes.h"

namespace spider3d
{

	enum SEVENT_TYPE	{ SMOUSE_EVENT = 0 ,	SKEY_EVENT , SNONE};

	enum SMOUSE_EVENT_TYPE
	{
		//! Left mouse button was pressed down.
		LEFT_MOUSE_PRESSED_DOWN = 0,

		//! Right mouse button was pressed down.
		RIGHT_MOUSE_PRESSED_DOWN,

		//! Middle mouse button was pressed down.
		MIDDLE_MOUSE_PRESSED_DOWN,

		//! Left mouse button was left up.
		LEFT_MOUSE_LEFT_UP,

		//! Right mouse button was left up.
		RIGHT_MOUSE_LEFT_UP,

		//! Middle mouse button was left up.
		MIDDLE_MOUSE_LEFT_UP,

		//! The mouse cursor changed its position.
		SMOUSE_MOVED,

        //! The mouse cursor changed its position.
		SMOUSE_DRAGGED,
        
		//! The mouse wheel was moved. Use Wheel value in event data to find out
		//! in what direction and how fast.
		SMOUSE_WHEEL,

		//! No real event. Just for convenience to get number of events
		COUNT
	};





	typedef struct Event
	{
		typedef struct MouseInput
		{
			//! X position of mouse cursor
			si32 X;

			//! Y position of mouse cursor
			si32 Y;

			//! mouse wheel delta, usually 1.0 or -1.0.
			/** Only valid if event was EMIE_MOUSE_WHEEL */
			sf32 Wheel;

			//! Type of mouse event
			SMOUSE_EVENT_TYPE Event;
		}MouseInput;

		//! Any kind of keyboard event.
		typedef struct KeyInput
		{
            KeyInput()
            {
                PressedDown = false;
                Control     = false;
                Shift       = false;
                Alt         = false;
            }
			//! Character corresponding to the key (0, if not a character)
			wchar_t Char;

			//! Key which has been pressed or released
			KEY_CODE Key;

			//! If not true, then the key was left up
			sbool PressedDown;

			//! True if shift was also pressed
			sbool Shift;

			//! True if ctrl was also pressed
			sbool Control;
            
            sbool Alt;
		}KeyInput;

		Event()
		{
			//eventType = SNONE;
			keyInput.Key = KEY_LBUTTON;
		}

		MouseInput mouseInput;

		KeyInput   keyInput;

		SEVENT_TYPE eventType;

	}Event;


	class EventListener
	{
	public:

		virtual sbool OnEvent( Event& ) = 0;

	};

}

#endif