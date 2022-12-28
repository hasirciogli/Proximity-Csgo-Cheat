#include "dependencies/utilities/csgo.hpp"
#include <locale.h>

void console::initialize(const char* title) {
	AllocConsole();

	int codepage = 1254;
	SetConsoleOutputCP(codepage);
	SetConsoleCP(codepage);

	freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(0)), "conin$", "r", static_cast<_iobuf*>(__acrt_iob_func(0)));
	freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(1)), "conout$", "w", static_cast<_iobuf*>(__acrt_iob_func(1)));
	freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(2)), "conout$", "w", static_cast<_iobuf*>(__acrt_iob_func(2)));

	SetConsoleTitleA(title);
}

void console::release() {
	fclose(static_cast<_iobuf*>(__acrt_iob_func(0)));
	fclose(static_cast<_iobuf*>(__acrt_iob_func(1)));
	fclose(static_cast<_iobuf*>(__acrt_iob_func(2)));

	FreeConsole();
}
