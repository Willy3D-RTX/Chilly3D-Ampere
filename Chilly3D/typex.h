#ifndef C3D_TYPEX_H
#define C3D_TYPEX_H
#define C3D_TYPEX_BUFFER_SIZE 255

#define C3D_VIDEO_WIDTH_MIN 320
#define C3D_VIDEO_HEIGHT_MIN 200

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>

/*
	Comprobacion de la plataforma del sistema.
*/
#if defined(CONFIG_IDF_TARGET_ESP32P4)
	/// IDF-ESP para la placa ESP32P4
#elif defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	/// Microsoft Windows
	#define C3D_PLATFORM_WINDOW
	#define C3D_API_PRINTF printf
#elif define(ANDROID)
	/// Sistemas Android
#elif defined(__linux__)
	/// Linux
#elif defined(__APPLE__) && defined(__MACH__)
	/// Apple
#else
	#error "Plataforma desconocida o no compatible con el motor Chilly3D-Engine"
#endif /// Fin de las comprobaciones de la plataforma.

#if C3D_API_BPP == 8
	typedef uint8_t C3D_Pixel;
#elif C3D_API_BPP == 16
	typedef uint16_t C3D_Pixel;
#elif C3D_API_BPP == 24
	typedef uint32_t C3D_Pixel;
#elif C3D_API_BPP == 32
	typedef uint32_t C3D_Pixel;
#else
	#error "Error en C3D_API_BPP: Debe valer 8, 16, 24 o 32 bits!"
#endif // C3D_API_BPP

/*
	Lista de colores de los mensajes.
*/
typedef enum
{
	BLACK = 0,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE
} C3D_MessageColor_t;

/*

	Lista de configuraciones del motor Chilly3d-Engine.
*/
typedef enum {
	C3D_SETUP_DEBUG = 0,
	C3D_SETUP_ANSI,
	C3D_SETUP_WIDTH,
	C3D_SETUP_HEIGHT,
	C3D_SETUP_FULLSCREEN,
	C3D_SETUP_DOBLEBUFFER
} C3D_Setup_t;

/*
	Estructura de datos para el control de banderas internas del motor.
*/
typedef struct
{
	bool debug = false;
	bool ansi = false;
	bool loop = false; /// Esta flag activa pone en marcha al motor grafico.
	bool error = false;
} C3D_Flags_t;

typedef struct
{
	uint16_t w = 0;
	uint16_t h = 0;
	uint16_t freq = 0;
} C3D_Monitor_t;

/*
	Estructura de datos para el subsistema de Video.
*/
typedef struct
{
	bool fullscreen = false;
	bool doblebuffer = false;
	uint16_t width = 0;
	uint16_t height = 0;
	uint16_t* dat_0 = nullptr; /// Buffer principal de video.
	uint16_t* dat_1 = nullptr; /// Almacen para el doblebuffer de video.
	C3D_Monitor_t monitor[5] = {};
} C3D_Video_t;

/*
	Estructura raiz del proyecto.
*/
typedef struct
{
	char prompt[64] = C3D_API_TITLE;
	char error[C3D_TYPEX_BUFFER_SIZE] = {};

	C3D_Flags_t flags = {};

	C3D_Video_t video = {};
} C3D_Root_t;

/*
	Root del proyecto.
*/
C3D_Root_t root = {};

/*
	Clase de C3D_Message_class, controla el envio de mensajes del sistema.

	Exiten varios tipos de mensajes del sitema.

	1.- info(): Informacion general.
	2.- engine(): Informacion del motor grafico.
	3.- error(): Informacion de errores del motor grafico y/o sus dependencias.
	4.- event(): Informacion de eventos del sistema, pulsar boton, etc.
*/
class C3D_Message_class {
private:
	char str[C3D_TYPEX_BUFFER_SIZE] = {};

public:
	void setColor(C3D_MessageColor_t _text, C3D_MessageColor_t _bg)
	{
		if (root.flags.ansi == false) { return; }
		C3D_API_PRINTF("\x1b[%d;%dm", 30 + _text, 40 + _bg);
	}

	void reset(void)
	{
		C3D_API_PRINTF("\x1b[0m");
	}

	void info(const char* _file, uint32_t _line, const char* _message, ...)
	{
		if (root.flags.debug == false) { return; }
		va_list arg;
		va_start(arg, _message);
		vsnprintf(this->str, sizeof(str), _message, arg);
		va_end(arg);
		this->setColor(YELLOW, BLACK);
		C3D_API_PRINTF("[INFO  ] ");
		this->setColor(WHITE, BLACK);
		C3D_API_PRINTF("@");
		this->setColor(MAGENTA, BLACK);
		C3D_API_PRINTF("%s", root.prompt);
		this->setColor(YELLOW, BLACK);
		C3D_API_PRINTF("> ");
		this->setColor(WHITE, BLACK);
		C3D_API_PRINTF("%s.\n", this->str);
		this->setColor(WHITE, BLACK);
		C3D_API_PRINTF("(%d, %s)\n\n", _line, _file);
	}

	void engine(const char* _file, uint32_t _line, const char* _message, ...)
	{
		va_list arg;
		va_start(arg, _message);
		vsnprintf(this->str, sizeof(str), _message, arg);
		va_end(arg);
		this->setColor(MAGENTA, BLACK);
		C3D_API_PRINTF("[ENGINE] ");
		this->setColor(WHITE, BLACK);
		C3D_API_PRINTF("@");
		this->setColor(MAGENTA, BLACK);
		C3D_API_PRINTF("%s", root.prompt);
		this->setColor(MAGENTA, BLACK);
		C3D_API_PRINTF("> ");
		this->setColor(WHITE, BLACK);
		C3D_API_PRINTF("%s.\n", this->str);
		this->setColor(WHITE, BLACK);
		C3D_API_PRINTF("(%d, %s)\n\n", _line, _file);
	}

	void error(const char* _file, uint32_t _line, const char* _message, ...)
	{
		va_list arg;
		va_start(arg, _message);
		vsnprintf(this->str, sizeof(str), _message, arg);
		va_end(arg);
		this->setColor(RED, BLACK);
		C3D_API_PRINTF("[ERROR ] ");
		this->setColor(WHITE, BLACK);
		C3D_API_PRINTF("@");
		this->setColor(MAGENTA, BLACK);
		C3D_API_PRINTF("%s", root.prompt);
		this->setColor(RED, BLACK);
		C3D_API_PRINTF("> ");
		this->setColor(WHITE, BLACK);
		C3D_API_PRINTF("%s.\n", this->str);
		this->setColor(WHITE, BLACK);
		C3D_API_PRINTF("(%d, %s)\n\n", _line, _file);
	}

	void event(const char* _file, uint32_t _line, const char* _message, ...)
	{
		if (root.flags.debug == false) { return; }
		va_list arg;
		va_start(arg, _message);
		vsnprintf(this->str, sizeof(str), _message, arg);
		va_end(arg);
		this->setColor(CYAN, BLACK);
		C3D_API_PRINTF("[EVENT ] ");
		this->setColor(WHITE, BLACK);
		C3D_API_PRINTF("@");
		this->setColor(MAGENTA, BLACK);
		C3D_API_PRINTF("%s", root.prompt);
		this->setColor(CYAN, BLACK);
		C3D_API_PRINTF("> ");
		this->setColor(WHITE, BLACK);
		C3D_API_PRINTF("%s.\n", this->str);
		this->setColor(WHITE, BLACK);
		C3D_API_PRINTF("(%d, %s)\n\n", _line, _file);
	}

	void dispose(void)
	{
		root.flags.ansi = false;
		root.flags.debug = false;
		this->reset();
	}
};

C3D_Message_class message;

#if defined(C3D_PLATFORM_WINDOW)
	#include "Windows/hal.h"
#endif // defined(C3D_PLATFORM_WINDOW)

#endif // C3D_TYPEX_H
