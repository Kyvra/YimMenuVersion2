#include "core/commands/LoopedCommand.hpp"
#include "game/gta/ScriptGlobal.hpp"
#include "game/gta/Natives.hpp"

namespace YimMenu::Features
{
	class BecomeCEO : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;
		virtual void OnTick() override
		{
			*ScriptGlobal(1892798).At(PLAYER::PLAYER_ID(), 615).At(10).As<int*>() = 0;
			*ScriptGlobal(1892798).At(PLAYER::PLAYER_ID(), 615).At(11).As<int*>() = 1;
		}
	};
	static BecomeCEO _BecomeCEO{
	    "Become_CEO",
	    "Register as CEO",
	    "Force to Become A Boss"};

	class BecomeMC : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;
		virtual void OnTick() override
		{
			*ScriptGlobal(1892798).At(PLAYER::PLAYER_ID(), 615).At(10).As<int*>() = 0;
			*ScriptGlobal(1892798).At(PLAYER::PLAYER_ID(), 615).At(10).At(433).As<int*>() = 1;
		}
	};
	static BecomeMC _BecomeMC{
	    "Become_MC",
	    "Register as MC",
	    "Force to Become A Boss"};
}