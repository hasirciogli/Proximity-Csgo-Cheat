#include "core/hooks/hooks.hpp"
#include "core/features/features.hpp"

unsigned long __stdcall hooks::findmdl::hook(char* path) noexcept {
	visuals::custom_models::replace_model(path);		// We will change (or not) the path in draw_models()

	return findmdl::original(interfaces::mdl_cache, path);
}
