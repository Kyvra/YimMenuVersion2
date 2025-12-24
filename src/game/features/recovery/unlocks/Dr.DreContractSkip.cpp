#include "core/commands/Command.hpp"
#include "game/gta/Natives.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/gta/Stats.hpp"

namespace YimMenu::Features
{
	class SkipDrDreeContract : public Command
		{
			using Command::Command;
			virtual void OnCall() override
			{
				if (*Pointers.IsSessionStarted)
				{
				    Stats::SetInt("MPX_FIXER_STORY_BS", 4095);
					STATS::STAT_SAVE(0, 0, 3, 0);
				}
				else
				{
				    Notifications::Show("Success! All preps skipped.\nNext mission: Don't F* With Dre", "Dr. Dre Contract Skip Please join any freemode session.", NotificationType::Error);
				}
			}
		};
	static SkipDrDreeContract _SkipDrDreeContract{
	    "SkipDrDreeContract",
	    "Skip Dr Dree Contract",
	    "Skip Dr Dree Contract"};

}
