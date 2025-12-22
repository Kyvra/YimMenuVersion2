#include "core/commands/Command.hpp"
#include "game/gta/Natives.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/gta/Stats.hpp"

namespace YimMenu::Features
{
	class BunkerUnlocks : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			if (*Pointers.IsSessionStarted)
			{
				Stats::SetPackedBool(15381, 15382);
				Stats::SetPackedBool(15428, 15439);
				Stats::SetPackedBool(15447, 15474);
				Stats::SetPackedBool(15491, 15499);
				STATS::STAT_SAVE(0, 0, 3, 0);
			}
			else
			{
				Notifications::Show("All Bunker research unlocked!", "All Bunker Research Only Please join any freemode session.", NotificationType::Error);
			}
		}
	};
	static BunkerUnlocks _BunkerUnlocks{
	    "BunkerUnlocks",
	    "Bunker Unlocks",
	    "Bunkerunlocks"};

}
