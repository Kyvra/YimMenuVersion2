#include "core/commands/LoopedCommand.hpp"
#include "core/commands/FloatCommand.hpp"
#include "game/gta/Natives.hpp"
#include "game/backend/Self.hpp"

namespace YimMenu::Features
{
	class SwimInAir : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;
		//Basic needs improved
		virtual void OnTick() override
		{
			int ped = PLAYER::PLAYER_PED_ID();
			if (!ENTITY::DOES_ENTITY_EXIST(ped))
				return;
			PED::SET_PED_CONFIG_FLAG(ped, 65, true);
			Vector3 vel = ENTITY::GET_ENTITY_VELOCITY(ped);
			bool barelyMoving = fabsf(vel.x) < 0.05f && fabsf(vel.y) < 0.05f && fabsf(vel.z) < 0.02f;
			if (barelyMoving)
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(ped, 1, 0.03f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, false, true, true, false, true);
			}
		}
		virtual void OnDisable() override
		{
			int ped = PLAYER::PLAYER_PED_ID();
			if (ENTITY::DOES_ENTITY_EXIST(ped))
			PED::SET_PED_CONFIG_FLAG(ped, 65, false);
		}
	};
	static SwimInAir _SwimInAir{
	    "swiminair",
	    "Swim In Air",
	    "Freely move around in the air like you're swimming"};
}
