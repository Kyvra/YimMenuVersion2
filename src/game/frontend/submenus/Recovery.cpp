#include "Recovery.hpp"
#include "Recovery/HeistModifier.hpp"
#include "Recovery/StatEditor.hpp"
#include "Recovery/Transactions.hpp"
#include "Recovery/DailyActivities.hpp"
#include "game/frontend/items/Items.hpp"

namespace YimMenu::Submenus
{
	Recovery::Recovery() :
		#define ICON_FA_SACK_DOLLAR "\xef\x93\x80"
	    Submenu::Submenu("Recovery", ICON_FA_SACK_DOLLAR)
	{
		auto main = std::make_shared<Category>("Main");
		auto businesses = std::make_shared<Category>("Businesses");
		auto casino = std::make_shared<Category>("Casino");
		auto unlocks = std::make_shared<Category>("Unlocks");
		auto generalUnlocks = std::make_shared<Group>("General");
		auto trophies = std::make_shared<Group>("Trophies");
		auto skip = std::make_shared<Group>("Skipper");
		auto Unreleased = std::make_shared<Group>("Unreleased");
		auto requestServices = std::make_shared<Group>("Request Services");

		auto generalGroup = std::make_shared<Group>("General");
		auto gunvanGroup = std::make_shared<Group>("Gun Van");
		auto businessGroup = std::make_shared<Group>("General");
		auto casinoSlots = std::make_shared<Group>("Slot Machines");
		//auto casinoWheel = std::make_shared<Group>("Lucky Wheel");
		//auto casinoBlackJack = std::make_shared<Group>("Blackjack");
		//auto casinoRoulette = std::make_shared<Group>("Roulette");

		generalGroup->AddItem(std::make_shared<BoolCommandItem>("playallmissionssolo"_J));
		generalGroup->AddItem(std::make_shared<BoolCommandItem>("unlockgtaplus"_J));
		generalGroup->AddItem(std::make_shared<BoolCommandItem>("overriderpmultiplier"_J));
		generalGroup->AddItem(std::make_shared<ConditionalItem>("overriderpmultiplier"_J, std::make_shared<FloatCommandItem>("rpmultiplierinput"_J)));
		generalGroup->AddItem(std::make_shared<CommandItem>("maxskills"_J));
		generalGroup->AddItem(std::make_shared<IntCommandItem>("rpslider"_J, "Character Level"));
		generalGroup->AddItem(std::make_shared<CommandItem>("applyrp"_J));
		generalGroup->AddItem(std::make_shared<IntCommandItem>("lscarmeetrank"_J, "LS Car Meet Rank"));
		generalGroup->AddItem(std::make_shared<CommandItem>("applylscarmeetrank"_J));

		generalGroup->AddItem(std::make_shared<BoolCommandItem>("freechangeappearance"_J));
		generalGroup->AddItem(std::make_shared<BoolCommandItem>("nochangeappearancecooldown"_J));
		generalGroup->AddItem(std::make_shared<BoolCommandItem>("allowgenderchange"_J));
		generalGroup->AddItem(std::make_shared<BoolCommandItem>("peddropcash"_J));
		generalGroup->AddItem(std::make_shared<BoolCommandItem>("vehicledropcash"_J));

		
		gunvanGroup->AddItem(std::make_shared<ListCommandItem>("gunvanslot"_J));
		gunvanGroup->AddItem(std::make_shared<ListCommandItem>("gunvanweapon"_J));
		gunvanGroup->AddItem(std::make_shared<CommandItem>("setgunvanweapon"_J));
		gunvanGroup->AddItem(std::make_shared<CommandItem>("removegunvanweapon"_J));
		gunvanGroup->AddItem(std::make_shared<ListCommandItem>("gunvanlocation"_J));
		gunvanGroup->AddItem(std::make_shared<CommandItem>("setgunvanlocation"_J));
		gunvanGroup->AddItem(std::make_shared<CommandItem>("teleportgunvan"_J));
		gunvanGroup->AddItem(std::make_shared<BoolCommandItem>("gunvanskins"_J));
		gunvanGroup->AddItem(std::make_shared<BoolCommandItem>("gunvanblip"_J));

		
		businessGroup->AddItem(std::make_shared<ListCommandItem>("businesssafe"_J));
		businessGroup->AddItem(std::make_shared<CommandItem>("claimsafeearnings"_J));

		casinoSlots->AddItem(std::make_shared<BoolCommandItem>("casinomanipulaterigslotmachines"_J));
		
		generalUnlocks->AddItem(std::make_shared<CommandItem>("unlockallcareerprogress"_J));
		generalUnlocks->AddItem(std::make_shared<CommandItem>("resetcareerprogress"_J));
		generalUnlocks->AddItem(std::make_shared<CommandItem>("unlockalltattoos"_J));
		generalUnlocks->AddItem(std::make_shared<CommandItem>("BunkerUnlocks"_J));
		generalUnlocks->AddItem(std::make_shared<CommandItem>("unlockallAwards"_J));
		generalUnlocks->AddItem(std::make_shared<CommandItem>("unlock_paints"_J));
		generalUnlocks->AddItem(std::make_shared<CommandItem>("unlock_packed_bools"_J));
		generalUnlocks->AddItem(std::make_shared<CommandItem>("unlock_misc"_J));
		generalUnlocks->AddItem(std::make_shared<CommandItem>("enable_hidden_liveries"_J));

		skip->AddItem(std::make_shared<CommandItem>("SkipDrDreeContract"_J));
		skip->AddItem(std::make_shared<CommandItem>("SkipJenetteTheMutette"_J));
		skip->AddItem(std::make_shared<CommandItem>("SkipCluckinBellRaid"_J));


		trophies->AddItem(std::make_shared<CommandItem>("unlockallmansiontrophies"_J));

		requestServices->AddItem(std::make_shared<CommandItem>("requestmoc"_J));
		requestServices->AddItem(std::make_shared<CommandItem>("requestavenger"_J));
		requestServices->AddItem(std::make_shared<CommandItem>("requestterrorbyte"_J));
		requestServices->AddItem(std::make_shared<CommandItem>("requestkosatka"_J));
		requestServices->AddItem(std::make_shared<CommandItem>("requestacidlab"_J));
		requestServices->AddItem(std::make_shared<CommandItem>("requestacidbike"_J));
		requestServices->AddItem(std::make_shared<CommandItem>("requestammodrop"_J));
		requestServices->AddItem(std::make_shared<CommandItem>("requestbullshark"_J));
		requestServices->AddItem(std::make_shared<CommandItem>("requestairstrike"_J));

		Unreleased->AddItem(std::make_shared<CommandItem>("Unreleased_content"_J));
		Unreleased->AddItem(std::make_shared<ListCommandItem>("luckyclover_location"_J));
		Unreleased->AddItem(std::make_shared<CommandItem>("teleport_luckyclover"_J));
		




		
		main->AddItem(generalGroup);
		main->AddItem(gunvanGroup);
		main->AddItem(requestServices);
		businesses->AddItem(businessGroup);
		casino->AddItem(casinoSlots);
		unlocks->AddItem(generalUnlocks);
		unlocks->AddItem(trophies);
		unlocks->AddItem(skip);
		unlocks->AddItem(Unreleased);

		
		AddCategory(std::move(main));
		AddCategory(std::move(businesses));
		AddCategory(std::move(casino));
		AddCategory(std::move(unlocks));
		AddCategory(BuildStatEditorMenu());
		AddCategory(BuildTransactionsMenu());
		AddCategory(BuildHeistModifierMenu());
		AddCategory(BuildDailyActivitiesMenu());
	}
}
