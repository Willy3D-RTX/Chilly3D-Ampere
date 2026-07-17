#ifndef C3D_ENGINE_H
#define C3D_ENGINE_H
#define C3D_ENGINE_LINES 175

#define C3D_API_TITLE "Chilly3D-Engine"
#define C3D_API_VERSION_MAJOR 1
#define C3D_API_VERSION_MINOR 0
#define C3D_API_BPP 16 /// Bits por pixel

#define C3D_API_COMPILED_LINES (\
		C3D_ENGINE_LINES +\
		C3D_TYPEX_LINES+\
		C3D_HAL_LINES\
		)

#include "typex.h"

class C3D_Engine_class {
private:

public:
	void setup(C3D_Setup_t _setup, int _value)
	{
		if (_setup == C3D_SETUP_DEBUG)
		{
			message.engine(
				__FILE__,
				__LINE__,
				"Opciones del motor: DEBUG=%d",
				_value
			);
			root.flags.debug = (bool)_value;
			return;
		}
		if (_setup == C3D_SETUP_ANSI)
		{
			message.engine(
				__FILE__,
				__LINE__,
				"Opciones del motor: ANSI=%d",
				 _value
			);
			root.flags.ansi = (bool)_value;
			return;
		}
		if (_setup == C3D_SETUP_WIDTH)
		{
			message.engine(
				__FILE__,
				__LINE__,
				"Opciones del motor: VIDEO.WIDTH=%d",
				_value
			);
			root.video.width = _value;
			return;
		}
		if (_setup == C3D_SETUP_HEIGHT)
		{
			message.engine(
				__FILE__,
				__LINE__,
				"Opciones del motor: VIDEO.HEIGHT=%d",
				_value
			);
			root.video.height = _value;
			return;
		}
		if (_setup == C3D_SETUP_FULLSCREEN)
		{
			message.engine(
				__FILE__,
				__LINE__,
				"Opciones del motor: VIDEO.FULLSCREEN=%d",
				_value
			);
			root.video.fullscreen = (bool)_value;
			return;
		}
		if (_setup == C3D_SETUP_DOBLEBUFFER)
		{
			message.engine(
				__FILE__,
				__LINE__,
				"Opciones del motor: VIDEO.DOBLEBUFFER=%d",
				_value
			);
			root.video.doblebuffer = (bool)_value;
			return;
		}
	}

	void init(uint16_t _width, uint16_t _height)
	{
		message.engine(
			__FILE__,
			__LINE__,
			"Inciando %s, Version %d.%d",
			C3D_API_TITLE,
			C3D_API_VERSION_MAJOR,
			C3D_API_VERSION_MINOR
		);
		if (_width < C3D_VIDEO_WIDTH_MIN || _height < C3D_VIDEO_HEIGHT_MIN)
		{
			message.error(
				__FILE__,
				__LINE__,
				"Error en la resolucion de pantalla"
			);
			_width = C3D_VIDEO_WIDTH_MIN;
			_height = C3D_VIDEO_HEIGHT_MIN;
			message.info(
				__FILE__,
				__LINE__,
				"Ajustando: VIDEO.MODE=%dx%d",
				_width,
				_height
			);
		}
		root.video.width = _width;
		root.video.height = _height;
		hal_video_createWindow();
		if (root.flags.error == true)
		{
			message.error(
				__FILE__,
				__LINE__,
				"Error al poner en marcha a %s",
				C3D_API_TITLE
			);
		}
		else
		{
			/// Si todo sale bien, root.flags.loop es 'true'
			/// Solo por depuracion no lo vamos a activar hasta que
			/// Terminemos de escribir todo el codigo minimo y funcional
			/// para poner en marcha el motor grafico.
			root.flags.loop = true;
			message.engine(
				__FILE__,
				__LINE__,
				"Lineas compiladas: %d",
				C3D_API_COMPILED_LINES
			);
		}
	}

	bool update(void)
	{
		hal_video_present();
		hal_video_clear(0);
		if (root.events.type == C3D_EVENTS_QUIT) { root.flags.loop = false; }
		return root.flags.loop;
	}

	void dispose(void)
	{
		message.engine(
			__FILE__,
			__LINE__,
			"Cerrando %s..",
			C3D_API_TITLE
		);
		hal_video_dispose();
		message.dispose();
		message.engine(
			__FILE__,
			__LINE__,
			"%s se ha finalizado con exito!",
			C3D_API_TITLE
		);
	}
};

#endif // C3D_ENGINE_H
