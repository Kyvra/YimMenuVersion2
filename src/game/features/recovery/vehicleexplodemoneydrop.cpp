#include "core/commands/LoopedCommand.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/Natives.hpp"
#include "game/gta/Pools.hpp"

namespace YimMenu::Features
{
	class VehicleDropMoney : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		void OnTick() override
		{
			const Vehicle selfVehicle = Self::GetVehicle();
			for (auto veh : Pools::GetVehicles())
			{
				if (!ENTITY::DOES_ENTITY_EXIST(veh.GetHandle()))
					continue;
				if (veh == selfVehicle)
					continue;
				VEHICLE::SET_VEHICLE_DROPS_MONEY_WHEN_BLOWN_UP(
				    veh.GetHandle(),
				    true);
			}
		}

		void OnDisable() override
		{
			for (auto veh : Pools::GetVehicles())
			{
				if (ENTITY::DOES_ENTITY_EXIST(veh.GetHandle()))
				{
					VEHICLE::SET_VEHICLE_DROPS_MONEY_WHEN_BLOWN_UP(
					    veh.GetHandle(),
					    false);
				}
			}
		}
	};

	static VehicleDropMoney _VehicleDropMoney{
	    "vehicledropcash",
	    "Vehicle Drop Cash",
	    "Makes vehicles drop cash when blown up (USE SPARINGLY)"};
}
