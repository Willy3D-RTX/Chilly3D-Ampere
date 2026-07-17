/// Libreria del motor
/// Si el codigo les parece raro, es por que estoy mezclando c y c++.
/// Este dato es para lo mas purista del codigo c/c++. ;)

#include "Chilly3D/engine.h"

C3D_Engine_class engine;

int main()
{
	engine.setup(C3D_SETUP_DEBUG, true);
	engine.setup(C3D_SETUP_ANSI, true);
	engine.setup(C3D_SETUP_FULLSCREEN, false);
	engine.setup(C3D_SETUP_DOBLEBUFFER, true);
	engine.init(480, 270);
	while (engine.update())
	{
		hal_video_putPixel(100, 100, 0xFFFF);
	}
	engine.dispose();
    return 0;
}
