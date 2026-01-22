#include "core/commands/Command.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/pointers/Pointers.hpp"
#include "game/gta/Stats.hpp"
#include "core/backend/FiberPool.hpp"

namespace YimMenu::Features
{
	class CompleteWSameCrew : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			if (*Pointers.IsSessionStarted)
			{
				FiberPool::Push([] {
					Stats::SetPackedInt(42100, 5);
				});
			}
			else
			{
				Notifications::Show("YimMenuV2", "You must be online", NotificationType::Error);
			}
		}
	};
	static CompleteWSameCrew _CompleteWSameCrew{
	    "CompleteWSameCrewheist",
	    "Complete all heists with the same crew",
	    "Complete all heists with the same crew"};
	}
