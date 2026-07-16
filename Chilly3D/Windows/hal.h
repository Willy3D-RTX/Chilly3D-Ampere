#ifndef C3D_HAL_H
#define C3D_HAL_H

#include <windows.h>

static HWND		__hwnd			= nullptr;
static HDC		__hdcMem		= nullptr;
static HBITMAP	__hBitmap		= nullptr;
static HBITMAP	__hBitmapOld	= nullptr;

/*
	Funcion para crear una ventana en Microsoft Windows.
*/
C3D_API void hal_video_createWindow(void)
{
	message.info(__FILE__, __LINE__, "Iniciando la capa de abstracion del subsistema de Video");
	message.info(
		__FILE__,
		__LINE__,
		"HAL: WINDOW.MODE=%dx%d; FULLSCREEN=%d; DOBLEBUFFER=%d",
		root.video.width,
		root.video.height,
		root.video.fullscreen,
		root.video.doblebuffer
	);
	/// *** FALTA METER EL CODIGO PARA CREAR LA VENTANA ***
	/// Mañana continuamos desde aqui!!!
	/// Hasta la proxima
}

/*
	Funcion para intercambiar buffer de video.
*/
C3D_API void hal_video_swapBuffer(void)
{

}

/*
	Funcion para presentar en buffer en pantalla.
*/
C3D_API void hal_video_present(void)
{

}

/*
	Funcion para cerrar y limpiar la memoria de Video.
*/
C3D_API void hal_video_dispose(void)
{
	message.info(__FILE__, __LINE__, "Cerrando la capa de abstraccion del subsistema de Video!");
}

/*
	Funcion para hacer delays en milisegundos.
*/
C3D_API void hal_cpu_delayMS(uint64_t _milliseconds)
{

}

/*
	Funcion para obtener los milisegundos desde que se inicio el motor grafico.
*/
C3D_API uint64_t hal_cpu_getTicks(void)
{

}

#endif // C3D_HAL_H
