#include "core/commands/Command.hpp"
#include "core/frontend/Notifications.hpp"
#include "core/backend/FiberPool.hpp"
#include "game/pointers/Pointers.hpp"
#include "game/gta/Stats.hpp"


namespace YimMenu::Features
{
	class AllCollectibles : public Command
	{
		using Command::Command;

		void OnCall() override
		{
			if (*Pointers.IsSessionStarted)
			{
				FiberPool::Push([] {
					Stats::SetPackedBoolRange(26811, 26910, true); //Action Figures
					Stats::SetPackedBoolRange(34262, 34361, true); //LD Organics Product
					Stats::SetPackedBoolRange(30230, 30239, true); //Movie Props
					Stats::SetPackedBoolRange(26911, 26964, true); //Playing Cards
					Stats::SetPackedBoolRange(28099, 28148, true); //Signal Jammers
					Stats::SetPackedBoolRange(36630, 36654, true); //Snowmen
					Stats::SetPackedBoolRange(51302, 51337, true); //Yuanbao
					Stats::SetPackedBoolRange(54737, 54761, true); //Lucky Clovers
				});
			}
			else
			{
				Notifications::Show("YimMenuV2", "You must be online", NotificationType::Error);
			}
		}
	};
	static AllCollectibles _AllCollectibles{
	    "all_Collectibles",
	    "All Collectibles",
	    "Unlock All Collectibles"};
	}

