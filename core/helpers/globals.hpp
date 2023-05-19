#pragma once

namespace globals {
	// Set to true when pressing the full update button. If true, does full_update in fsn and reverts to false. full_update() is called in fsn so its more thread safe.
	inline bool forcing_update = false;

	// Used in fireevent hook
	inline bool round_ended = false;


	inline bool double_tap_aim = false;
	inline bool double_tap_aim_check = false;
	inline int tochargeamount = 0;
	inline int shift_ticks = 0;
	inline int tocharge = 0;
	inline int fixed_tickbase = 0;
	inline int tickbase_shift = 0;


	inline bool should_recharge = false;
	inline bool shot_command = false;
	inline bool aimbot_working = false;
}
