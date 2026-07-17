#ifndef C3D_HAL_H
#define C3D_HAL_H
#define C3D_HAL_LINES 421

#include <windows.h>

static HWND		__hwnd			= nullptr;
/// DC de la ventana (para presentar el framebuffer).
static HDC		__hdc			= nullptr;
/// Reservado para uso futuro (doblebuffer via GDI).
static HDC		__hdcMem		= nullptr;
/// Reservado para uso futuro (doblebuffer via GDI).
static HBITMAP	__hBitmap		= nullptr;
/// Reservado para uso futuro (doblebuffer via GDI).
static HBITMAP	__hBitmapOld	= nullptr;

/// Descriptor del formato de pixel (RGB565, 16bpp) usado por StretchDIBits
/// para volcar directamente root.video.dat_0 a la ventana, sin copias extra.
struct C3D_BitmapInfo565_t
{
	BITMAPINFOHEADER header;
	DWORD masks[3];
};
static C3D_BitmapInfo565_t __bmi = {};

/*
	Procedimiento de ventana (WndProc). Solo necesita reaccionar a cerrar
	la ventana; el pintado normal se hace desde hal_video_present().
*/
static LRESULT CALLBACK __wndProc(
	HWND _hwnd,
	UINT _msg,
	WPARAM _wParam,
	LPARAM _lParam
)
{
	switch (_msg)
	{
		case WM_DESTROY:
			message.event(
				__FILE__,
				__LINE__,
				"Se destruyo la instancia de la ventana"
			);
			///root.flags.loop = false;
			PostQuitMessage(0);
			return 0;

		case WM_ERASEBKGND:
			/// Evita que Windows borre el fondo antes de
			/// cada pintado (flicker).
			return 1;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(_hwnd, &ps);
			StretchDIBits(
				hdc,
				0, 0, root.video.width, root.video.height,
				0, 0, root.video.width, root.video.height,
				root.video.dat_0,
				(BITMAPINFO*)&__bmi,
				DIB_RGB_COLORS,
				SRCCOPY
			);
			EndPaint(_hwnd, &ps);
			return 0;
		}

		case WM_KEYDOWN:
			if (_wParam == VK_F12)
			{
				message.event(
					__FILE__,
					__LINE__,
					"Se presiono el boton de salida de emergecia!"
				);
				root.events.type = C3D_EVENTS_EXIT_EMERGENCY;
				PostQuitMessage(0);
			}
			return 0;
	}
	return DefWindowProc(_hwnd, _msg, _wParam, _lParam);
}

/*
	Funcion para gestionar los eventos de sistema.
	En esta funcion quiero detectar las teclas que se presionan!
	Por ejemplo A, S, D, W... F1, ..., F12, etc.
*/
C3D_Api void hal_cpu_events(void)
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		/// F12 es el boton de cierre de emergencia!
		if (msg.message == WM_QUIT)
		{
			message.event(
				__FILE__,
				__LINE__,
				"Se presiono el boton Close [x] de windows!"
			);
			root.events.type = C3D_EVENTS_QUIT;
			return;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		/// Muchas gracias por su apoyo @thejotap3
		/// Esten pendientes al codigo, espero ponerlo en marcha
		/// hoy mismo amigos! Saludos :)
	}
}

/*
	Funcion para crear una ventana en Microsoft Windows.
*/
C3D_Api void hal_video_createWindow(void)
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
	/// ------------------------------------------------------------
	/// Registrar la clase de ventana.
	/// ------------------------------------------------------------
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	WNDCLASSEX wc   = {};
	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = __wndProc;
	wc.hInstance     = hInstance;
	wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpszClassName = "C3D_WindowClass";

	if (!RegisterClassEx(&wc))
	{
		message.error(__FILE__, __LINE__, "No se pudo registrar la clase de ventana");
		root.flags.error = true;
		return;
	}

	/// ------------------------------------------------------------
	/// Calcular estilo, tamano y posicion segun fullscreen o ventana.
	/// ------------------------------------------------------------
	DWORD style = root.video.fullscreen
		? WS_POPUP
		: (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX);

	int winW, winH, posX, posY;

	if (root.video.fullscreen == true)
	{
		winW = root.video.monitor[0].w;
		winH = root.video.monitor[0].h;
		posX = 0;
		posY = 0;
	}
	else
	{
		RECT rect = { 0, 0, root.video.width, root.video.height };
		AdjustWindowRect(&rect, style, FALSE);
		winW = rect.right - rect.left;
		winH = rect.bottom - rect.top;
		posX = CW_USEDEFAULT;
		posY = CW_USEDEFAULT;
	}

	__hwnd = CreateWindowEx(
		0,
		"C3D_WindowClass",
		root.prompt,
		style,
		posX, posY,
		winW, winH,
		nullptr, nullptr,
		hInstance,
		nullptr
	);

	if (__hwnd == nullptr)
	{
		message.error(__FILE__, __LINE__, "No se pudo crear la ventana");
		root.flags.error = true;
		return;
	}

	ShowWindow(__hwnd, SW_SHOW);
	__hdc = GetDC(__hwnd);

	/// ------------------------------------------------------------
	/// Describir el formato de pixel RGB565 para StretchDIBits.
	/// ------------------------------------------------------------
	__bmi.header.biSize        = sizeof(BITMAPINFOHEADER);
	__bmi.header.biWidth       = root.video.width;
	__bmi.header.biHeight      = -(int)root.video.height; ///negativo=top-down.
	__bmi.header.biPlanes      = 1;
	__bmi.header.biBitCount    = 16;
	__bmi.header.biCompression = BI_BITFIELDS;
	__bmi.masks[0] = 0xF800; /// R
	__bmi.masks[1] = 0x07E0; /// G
	__bmi.masks[2] = 0x001F; /// B
	message.info(__FILE__, __LINE__, "Ventana creada correctamente");
}

/*
	Funcion para intercambiar buffer de video.
*/
C3D_Api void hal_video_swapBuffer(void)
{
	if (root.video.doblebuffer == true)
	{
		memcpy(
			root.video.dat_0,
			root.video.dat_1,
			(size_t)root.video.width * root.video.height * sizeof(uint16_t)
		);
	}
}

/*
	Funcion para presentar en buffer en pantalla.
	Tambien procesa los mensajes pendientes de la ventana (equivalente a
	un "poll de eventos"), por lo que debe llamarse una vez por frame.
*/
C3D_Api void hal_video_present(void)
{
//	MSG msg;
//	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
//	{
//		if (msg.message == WM_QUIT)
//		{
//			root.flags.loop = false;
//			return;
//		}
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
	hal_cpu_events();
	hal_video_swapBuffer();
	StretchDIBits(
		__hdc,
		0, 0, root.video.width, root.video.height,
		0, 0, root.video.width, root.video.height,
		root.video.dat_0,
		(BITMAPINFO*)&__bmi,
		DIB_RGB_COLORS,
		SRCCOPY
	);
}

/*
	Funcion para cerrar y limpiar la memoria de Video.
*/
C3D_Api void hal_video_dispose(void)
{
	message.info(
		__FILE__,
		__LINE__,
		"Cerrando la capa de abstraccion del subsistema de Video!"
	);
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
	if (__hdc != nullptr)
	{
		ReleaseDC(__hwnd, __hdc);
		__hdc = nullptr;
	}
	if (__hwnd != nullptr)
	{
		DestroyWindow(__hwnd);
		__hwnd = nullptr;
	}
}

/*
	Funcion para hacer delays en milisegundos.
*/
C3D_Api void hal_cpu_delayMS(uint64_t _milliseconds)
{
	Sleep((DWORD)_milliseconds);
}

/*
	Funcion para obtener los milisegundos desde que se inicio el motor grafico.
*/
C3D_Api uint64_t hal_cpu_getTicks(void)
{
	return (uint64_t)GetTickCount64();
}

/*
	Devuelve el buffer sobre el que se debe DIBUJAR en este momento.
	Si hay doblebuffer, se dibuja sobre dat_1 (dat_0 es lo que ya esta
	en pantalla); si no, se dibuja directo sobre dat_0.
*/
static C3D_Api uint16_t* __drawTarget(void)
{
	return root.video.doblebuffer ? root.video.dat_1 : root.video.dat_0;
}

/*
	Funcion para dibujar pixeles en el buffer de video
	Nota: Se debe tener cuidado con el doblebuffer!
*/
C3D_Api void hal_video_putPixel(int _x, int _y, C3D_Pixel _color)
{
	if (_x < 0 || _y < 0 || _x >= root.video.width || _y >= root.video.height)
	{
		return;
	}
	uint16_t* buffer = __drawTarget();
	buffer[(_y * root.video.width) + _x] = (uint16_t)_color;
}

/*
	Funcion para obtener pixeles en el buffer de video
	Nota: Se debe tener cuidado con el doblebuffer!
*/
C3D_Api C3D_Pixel hal_video_getPixel(int _x, int _y)
{
	if (_x < 0 || _y < 0 || _x >= root.video.width || _y >= root.video.height)
	{
		return (C3D_Pixel)0;
	}
	uint16_t* buffer = __drawTarget();
	return (C3D_Pixel)buffer[(_y * root.video.width) + _x];
}

/*
	Funcion para limpiar el buffer de video.
*/
C3D_Api void hal_video_clear(C3D_Pixel _color)
{
	uint16_t* buffer = __drawTarget();
	uint32_t total = (uint32_t)root.video.width * (uint32_t)root.video.height;
	for (uint32_t i = 0; i < total; i++)
	{
		buffer[i] = (uint16_t)_color;
	}
}
#endif // C3D_HAL_H
