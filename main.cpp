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
		/// EUREKA!!! ALLI ESTA EL PIXEL EN PANTALLA!
		hal_video_putPixel(100, 100, 0xFFFF);
		/// Cuadrado de 20x20 en (100,100), color rojo puro,
		/// bien visible para confirmar que el render funciona.
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				hal_video_putPixel(
					100 + x, 100 + y,
					C3D_RGB565(50+(x*2),100+(y*2), 100+x+y)
				);
			}
		}
		/// Hasta aqui dejo el codigo!
		/// Mañana continuo
		/// Solo nos faltaria hacer las correcciones a FULLSCREE!
		/// Hasta la próxima amigos
	}
	engine.dispose();
    return 0;
}
