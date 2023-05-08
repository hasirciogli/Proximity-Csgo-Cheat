#include <windows.h>
#include <d3dx9.h>
#include <d3d9.h>


#include "dependencies/imgui/imgui.h"
#include "dependencies/imgui/imgui_internal.h"
#include "dependencies/imgui/imgui_impl_win32.h"
#include "dependencies/imgui/imgui_impl_dx9.h"
#include "core/menu/variables.hpp"


#include <iostream>
#include <algorithm>
#include <string>
#include <list>
#include <math.h>





#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

namespace HNOTIF {

	class HNotifier {
	public:
		

		void runBasement(LPDIRECT3DDEVICE9*);

		inline void addNotification(int idNumber, const char* title, const char* message) {
			variables::Menu_Settings::HNFAnimObject* tmpObj = new variables::Menu_Settings::HNFAnimObject();
			tmpObj->id = idNumber;
			tmpObj->title = title;
			tmpObj->message = message;
			tmpObj->Alpha = 0.f;

			variables::Menu_Settings::h_Notifications.push_front(tmpObj);
		}
	};
}