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
	message.info(
		__FILE__,
		__LINE__,
		"Iniciando la capa de abstracion del subsistema de Video"
	);
	message.info(
		__FILE__,
		__LINE__,
		"HAL: WINDOW.MODE=%dx%d; FULLSCREEN=%d; DOBLEBUFFER=%d",
		root.video.width,
		root.video.height,
		root.video.fullscreen,
		root.video.doblebuffer
	);
	/// ancho del monitor primario.
	root.video.monitor[0].w = GetSystemMetrics(SM_CXSCREEN);
	/// alto del monitor primario.
	root.video.monitor[0].h = GetSystemMetrics(SM_CYSCREEN);
	message.info(
		__FILE__,
		__LINE__,
		"Monitor: %dx%d",
		root.video.monitor[0].w,
		root.video.monitor[0].h
	);
	/// Crear y reservar memoria dinamica para el buffer principal.
	root.video.dat_0 = (uint16_t*)calloc(
		root.video.width *root.video.height,
		sizeof(uint16_t)
	);
	if (root.video.doblebuffer == true)
	{
		/// Crear y reservar memoria dinamica para el buffer secundario (solo
		/// para doblebuffer.
		root.video.dat_1 = (uint16_t*)calloc(
			root.video.width * root.video.height,
			sizeof(uint16_t)
		);
		if (root.video.dat_1 == nullptr)
		{
			message.error(
				__FILE__,
				__LINE__,
				"Memoria insuficiente! No se pudo crear el doblebuffer de video"
			);
			root.flags.error = true;
			return;
		}
	}
	if (root.video.dat_0 == nullptr)
	{
		message.error(
			__FILE__,
			__LINE__,
			"Memoria insuficiente! No se creo el buffer de video"
		);
		root.flags.error = true;
		return;
	}

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
	if (root.video.dat_0 != nullptr)
	{
		message.info(
			__FILE__,
			__LINE__,
			"Liberando buffer de principal de video."
		);
		free(root.video.dat_0);
		root.video.dat_0 = nullptr;
	}
	if (root.video.dat_1 != nullptr)
	{
		message.info(
			__FILE__,
			__LINE__,
			"Liberando buffer de secundario de video."
		);
		free(root.video.dat_1);
		root.video.dat_1 = nullptr;
		/// HOLA @eliasartiga8469
		/// Espero Haberlo escrito bien :)
	}
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
