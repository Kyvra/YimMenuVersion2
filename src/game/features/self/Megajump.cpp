#include "core/commands/LoopedCommand.hpp"
#include "game/gta/Natives.hpp"

namespace YimMenu::Features
{
	class MegaJump : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
             if (!PED::IS_PED_ON_FOOT(PLAYER::PLAYER_PED_ID()))
                 return;
              if (PAD::IS_CONTROL_JUST_PRESSED(0, 22))
               {
				  ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0.f, 0.f, 25.f, 0.f, 0.f, 0.f, 0, false, true, true, false, true);
			   }
		}
	};

	static MegaJump _MegaJump{"megajump", "Mega Jump", "Jump Super High"};
}
