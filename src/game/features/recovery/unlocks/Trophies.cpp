#include "core/commands/Command.hpp"
#include "game/gta/Natives.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/gta/Stats.hpp"

namespace YimMenu::Features
{
	class UnlockMansion_T : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			if (*Pointers.IsSessionStarted)
			{
				Stats::SetPackedBool(54787, true);
				Stats::SetPackedBool(54788, true);
				Stats::SetPackedBool(54789, true);
				Stats::SetPackedBool(54790, true);
				Stats::SetPackedBool(54791, true);
				Stats::SetPackedBool(54792, true);
				Stats::SetPackedBool(54793, true);
				Stats::SetPackedBool(54794, true);
				Stats::SetPackedBool(54795, true);
				Stats::SetPackedBool(54796, true);
				Stats::SetPackedBool(54797, true);
				Stats::SetPackedBool(54798, true);
				Stats::SetPackedBool(54799, true);
				Stats::SetPackedBool(54800, true);
				Stats::SetPackedBool(54801, true);
				Stats::SetPackedBool(54802, true);
				Stats::SetPackedBool(54803, true);
				Stats::SetPackedBool(54804, true);
				Stats::SetPackedBool(54805, true);
				Stats::SetPackedBool(54806, true);
				Stats::SetPackedBool(54807, true);
				Stats::SetPackedBool(54808, true);
				Stats::SetPackedBool(54809, true);
				Stats::SetPackedBool(54810, true);
				Stats::SetPackedBool(54811, true);
				Stats::SetPackedBool(54812, true);
				Stats::SetPackedBool(54813, true);
				Stats::SetPackedBool(54814, true);
				Stats::SetPackedBool(54815, true);
				Stats::SetPackedBool(54816, true);
				Stats::SetPackedBool(54817, true);
			}
			else
			{
				Notifications::Show("YimMenuV2", "You Must be Online", NotificationType::Error);
			}
		}
	};
	static UnlockMansion_T _UnlockMansion_T{
	    "unlockallmansiontrophies",
	    "Unlock Mansion Trophies",
	    "Unlocks all mansion trophies"};

}
