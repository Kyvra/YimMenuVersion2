#include "core/commands/Command.hpp"
#include "game/gta/Natives.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/gta/Stats.hpp"

namespace YimMenu::Features
{
	class UnlockFestiveGift : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			if (*Pointers.IsSessionStarted)
			{
				//2025
				Stats::SetPackedBool(28346, true);
				Stats::SetPackedBool(54767, true);
				//2024
				Stats::SetPackedBool(54652, true);
				//2023
				Stats::SetPackedBool(42218, true);
				Stats::SetPackedBool(42169, true);
		        Stats::SetPackedBool(42172, true);
				Stats::SetPackedBool(36781, true);
				Stats::SetPackedBool(42219, true);
				Stats::SetPackedBool(36777, true);
				Stats::SetPackedBool(36778, true);
				Stats::SetPackedBool(36779, true);
				Stats::SetPackedBool(42173, true);
				Stats::SetPackedBool(42174, true);
				Stats::SetPackedBool(42175, true);
				//2022
				Stats::SetPackedBool(36821, true);
				Stats::SetPackedBool(36822, true);
				//2021
				Stats::SetPackedBool(32292, true);
				Stats::SetPackedBool(32293, true);
				STATS::STAT_SAVE(0, 0, 3, 0);
			}
		}
	};
	static UnlockFestiveGift _UnlockFestiveGift{
	    "Unlock_Festive_Gifts",
	    "Unlock Festive Gifts",
	    "Unlock Festive Gifts"};
}
