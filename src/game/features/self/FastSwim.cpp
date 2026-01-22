#include "core/commands/LoopedCommand.hpp"
#include "game/gta/Natives.hpp"

namespace YimMenu::Features
{
	class FastSwim : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			Ped ped = PLAYER::PLAYER_PED_ID();

			if (!PED::IS_PED_SWIMMING(ped))
				return;

			if (PAD::IS_CONTROL_PRESSED(0, 21))
			{
				PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(PLAYER::PLAYER_ID(), 2.5f);
			}
			else
			{
				PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(PLAYER::PLAYER_ID(), 1.0f);
			}
		}

		virtual void OnDisable() override
		{
			PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(PLAYER::PLAYER_ID(), 1.0f);
		}
	};

	static FastSwim _FastSwim{"fastswim", "Fast Swim", "Swim faster while holding sprint"};
}
