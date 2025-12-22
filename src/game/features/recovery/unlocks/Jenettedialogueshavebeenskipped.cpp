#include "core/commands/Command.hpp"
#include "game/gta/Natives.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/gta/Stats.hpp"

namespace YimMenu::Features
{
	class SkipJenetteTheMutette : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			if (*Pointers.IsSessionStarted)
			{
				Stats::SetPackedBool(51192, 51195);
				STATS::STAT_SAVE(0, 0, 3, 0);
			}
			else
			{
				Notifications::Show("Please join any freemode session and reload the script.", "Success! Jenette dialogues have been skipped!", NotificationType::Error);
			}
		}
	};
	static SkipJenetteTheMutette _SkipJenetteTheMutette{
	    "SkipJenetteTheMutette",
	    "Skip Jenette The Mutette",
	    "Skip JenetteTheMutette"};

}
