#pragma once
#include "core/commands/Command.hpp"
#include "core/backend/FiberPool.hpp"
#include "game/gta/Stats.hpp"
#include "game/pointers/Pointers.hpp"

namespace YimMenu::Features
{
	class ClubPopularity : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			if (*Pointers.IsSessionStarted)
			{
				FiberPool::Push([] {
					Stats::SetInt("MPx_CLUB_POPULARITY", 1000);
				});
			}
		}
	};
	static ClubPopularity _ClubPopularity{"club_popularity", "Nightclub Popularity At 100%", "Set Nightclub Popularity At 100%"};
	class ClubPayout : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			if (*Pointers.IsSessionStarted)
			{
				FiberPool::Push([] {
					Stats::SetInt("MPX_CLUB_PAY_TIME_LEFT", -1);
				});
			}
		}
	};
	static ClubPayout _ClubPayout{"club_payout", "Nightclub Payout", "Force Next Nightclub Payout"};
	class BailAgent1 : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			if (*Pointers.IsSessionStarted)
			{
				FiberPool::Push([] {
					Stats::SetPackedInt(42270, 1);
				});
			}
		}
	};
	static BailAgent1 _BailAgent1{"bail_agent1", "Bail Enforcement Agent 1", "Agent 1 will fill your safe"};
	class BailAgent2 : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			if (*Pointers.IsSessionStarted)
			{
				FiberPool::Push([] {
					Stats::SetPackedInt(42271, 1);
				});
			}
		}
	};
	static BailAgent2 _BailAgent2{"bail_agent2", "Bail Enforcement Agent 2", "Agent 2 will fill your safe"};
	class CarWash : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			if (*Pointers.IsSessionStarted)
			{
				FiberPool::Push([] {
					Stats::SetPackedInt(24924, 0);
				});
			}
		}
	};
	static CarWash _CarWash{"carwash", "Reset Hands On Car Wash Heat", "Reset Hands On Car Wash Heat"};
	class SmokeWater : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			if (*Pointers.IsSessionStarted)
			{
				FiberPool::Push([] {
					Stats::SetPackedInt(24925, 0); 
				});
			}
		}
	};
	static SmokeWater _SmokeWater{"smokewater", "Reset Smoke On The Water Heat", "Reset Smoke On The Water Heat"};

	
	class Helitours : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			if (*Pointers.IsSessionStarted)
			{
				FiberPool::Push([] {
					Stats::SetPackedInt(24926, 0);
				});
			}
		}
	};
	static Helitours _Helitours{"helitours", "Reset Higgins Helitours Heat", "Reset Higgins Helitours Heat"};
	class SalvageYard : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			if (*Pointers.IsSessionStarted)
			{
				FiberPool::Push([] {
					Stats::SetPackedInt(51051, 100);
				});
			}
		}
	};
	static SalvageYard _SalvageYard{"salvageyard", "Max Salvage Yard Reputation", "Max Salvage Yard Reputation"};
}
