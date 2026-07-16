/// Libreria del motor
#include "Chilly3D/engine.h"

C3D_Engine_class engine;

int main()
{
	engine.setup(C3D_SETUP_DEBUG, true);
	engine.setup(C3D_SETUP_ANSI, true);
	engine.setup(C3D_SETUP_FULLSCREEN, false);
	engine.setup(C3D_SETUP_DOBLEBUFFER, true);
	engine.init(480, 270);
	while (engine.update()) {}
	engine.dispose();
    return 0;
}
