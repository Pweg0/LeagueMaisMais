#include "PluginSDK.h"
#include "Color.h"

PluginSetup("Pwego");

IMenu* MainMenu;

IMenu* ComboMenu;
IMenuOption* ComboQ;
IMenuOption* ComboW;
IMenuOption* ComboE;
IMenuOption* ComboR;


IMenu* JungleMenu;
IMenuOption* JungleQ;
IMenuOption* JungleW;
IMenuOption* JungleE;
IMenuOption* ManaJManager;

IMenu* FarmMenu;
IMenuOption* FarmQ;
IMenuOption* FarmW;
IMenuOption* FarmE;
IMenuOption* ManaFManager;

IMenu* KillSteal;
IMenuOption* KR;
IMenuOption* KQ;
IMenuOption* KW;
IMenuOption* KE;

IMenu* DrawMenu;
IMenuOption* DrawReady;
IMenuOption* DrawQ;
IMenuOption* DrawW;
IMenuOption* DrawE;
IMenuOption* DrawR;

ISpell2* Q;
ISpell2* W;
ISpell2* E;
ISpell2* R;

void Menu()
{
	MainMenu = GPluginSDK->AddMenu("Sejuani By Pwego");

	ComboMenu = MainMenu->AddMenu("Combo Settings");
	ComboQ = ComboMenu->CheckBox("Use Q Combo", true);
	ComboW = ComboMenu->CheckBox("Use W Combo", true);
	ComboE = ComboMenu->CheckBox("Use E Combo", true);
	ComboR = ComboMenu->CheckBox("Use R Combo", true);

	FarmMenu = MainMenu->AddMenu("LaneClear Settings");
	FarmQ = FarmMenu->CheckBox("Use Q Farm", true);
	FarmW = FarmMenu->CheckBox("Use W Farm", true);
	FarmE = FarmMenu->CheckBox("Use E Farm", false);
	ManaFManager = FarmMenu->AddInteger("Mana Manager", 1, 100, 40);


	JungleMenu = MainMenu->AddMenu("Jungle Settings");
	JungleQ = JungleMenu->CheckBox("Use q Jungle", true);
	JungleW = JungleMenu->CheckBox("Use W Jungle", true);
	JungleE = JungleMenu->CheckBox("Use E Jungle", true);
	ManaJManager = JungleMenu->AddInteger("Mana Manager", 1, 100, 40);

	KillSteal = MainMenu->AddMenu("KillSteal Options");
	KQ = KillSteal->CheckBox("Use Q For KS", true);
	KW = KillSteal->CheckBox("Use W For KS", true);
	KE = KillSteal->CheckBox("Use E For KS", true);
	KR = KillSteal->CheckBox("Use R For KS", true);

	DrawMenu = MainMenu->AddMenu("Drawning Settings");
	DrawReady = DrawMenu->CheckBox("Drawing Ready skills", true);
	DrawQ = DrawMenu->CheckBox("Drawing Q", true);
	DrawW = DrawMenu->CheckBox("Drawing W", true);
	DrawE = DrawMenu->CheckBox("Drawing E", true);
	DrawR = DrawMenu->CheckBox("Drawing R", true);


}
void Sejuani()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, false, true, (kCollidesWithHeroes | kCollidesWithMinions | kCollidesWithYasuoWall));
	Q->SetSkillshot(0.f, 70.f, 1600.f, 900.f);
	//std::string(GEntityList->Player()->GetSpellBook()->GetName(kSlotQ)) == "SejuaniArcticAssault";

	W = GPluginSDK->CreateSpell2(kSlotW, kCircleCast, false, true, (kCollidesWithNothing));
	W->SetSkillshot(0.25f, 0.f, 1000.f, 0.f);
	//std::string(GEntityList->Player()->GetSpellBook()->GetName(kSlotW)) == "SejuaniNorthernWinds";

	E = GPluginSDK->CreateSpell2(kSlotE, kTargetCast, false, false, (kCollidesWithNothing));
	E->SetSkillshot(0.25f, 0.f, 1000.f, 0.f);
	//std::string(GEntityList->Player()->GetSpellBook()->GetName(kSlotE)) == "SejuaniWintersClaw";

	R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, true, false, (kCollidesWithYasuoWall));
	R->SetSkillshot(0.25f, 110.f, 1600.f, 1170.f);
	//std::string(GEntityList->Player()->GetSpellBook()->GetName(kSlotR)) == "SejuaniGlacialPrisonStart";
}
void Combo()
{
	auto player = GEntityList->Player();
	if (ComboQ->Enabled() && Q->IsReady())
	{
		auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
		if (target != nullptr && target->IsValidTarget() && !target->IsDead() && target->IsHero())
		{
			Q->CastOnTarget(target);
		}
	}
	if (ComboE->Enabled() && E->IsReady())
	{
		auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, E->Range());
		if (target != nullptr && target->IsValidTarget() && !target->IsDead() && target->IsHero())
		{
			E->CastOnTarget(target);
		}
	}
	if (ComboW->Enabled() && W->IsReady())
	{
		auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, W->Range());
		if (target != nullptr && target->IsValidTarget() && !target->IsDead() && target->IsHero())
		{
			W->CastOnTarget(target);
		}
	}
	if (ComboR->Enabled() && R->IsReady())
	{
		auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, R->Range());
		if (target != nullptr && target->IsValidTarget() && target->IsHero() && !target->IsDead())
		{
			Vec3 pos;
			int sla;
			GPrediction->FindBestCastPosition(R->Range(), 1600, false, false, true, pos, sla);

			R->CastOnPosition(pos);

		}
	}

}
void LaneClear()
{
	auto ManaManager = GEntityList->Player()->ManaPercent();
	if (ManaManager > ManaFManager->GetInteger())
	{
		for (auto Creep : GEntityList->GetAllMinions(false, true, false))
		{
			if (Creep->IsEnemy(GEntityList->Player()) && Creep->IsValidTarget() && !Creep->IsDead())
			{
				if (FarmQ->Enabled() && Q->IsReady() && Creep->IsValidTarget(GEntityList->Player(), Q->Range()))
				{
					Q->CastOnTarget(Creep);
				}
				if (FarmW->Enabled() && W->IsReady() && Creep->IsValidTarget(GEntityList->Player(), W->Range()))
				{
					W->CastOnTarget(Creep);
				}
				if (FarmE->Enabled() && E->IsReady() && Creep->IsValidTarget(GEntityList->Player(), E->Range()))
				{
					E->CastOnTarget(Creep);
				}
			}
		}
	}
}
void JungleClear()
{
	auto ManaManager = GEntityList->Player()->ManaPercent();
	if (ManaManager > ManaJManager->GetInteger())
	{
		for (auto CreepJ : GEntityList->GetAllMinions(false, false, true))
		{
			if (CreepJ->IsEnemy(GEntityList->Player()) && CreepJ->IsValidTarget() && !CreepJ->IsDead())
			{
				if (JungleQ->Enabled() && Q->IsReady() && CreepJ->IsValidTarget(GEntityList->Player(), Q->Range()))
				{
					Q->CastOnTarget(CreepJ);
				}
				if (JungleW->Enabled() && W->IsReady() && CreepJ->IsValidTarget(GEntityList->Player(), W->Range()))
				{
					W->CastOnTarget(CreepJ);
				}
				if (JungleE->Enabled() && E->IsReady() && CreepJ->IsValidTarget(GEntityList->Player(), E->Range()))
				{
					E->CastOnTarget(CreepJ);
				}
			}
		}
	}
}
void Kill()
{
	for (auto Heros : GEntityList->GetAllHeros(false, true))
	{
		auto RKill = GDamage->GetSpellDamage(GEntityList->Player(), Heros, kSlotR);
		auto QKill = GDamage->GetSpellDamage(GEntityList->Player(), Heros, kSlotQ);
		auto WKill = GDamage->GetSpellDamage(GEntityList->Player(), Heros, kSlotW);
		auto EKill = GDamage->GetSpellDamage(GEntityList->Player(), Heros, kSlotE);

		{
			if (Heros != nullptr && !Heros->IsDead() && Heros->IsValidTarget())
			{
				if (KR->Enabled() && R->IsReady() && Heros->IsValidTarget(GEntityList->Player(), R->Range()) && RKill > Heros->GetHealth())
				{
					R->CastOnTarget(Heros);
				}
				if (KQ->Enabled() && Q->IsReady() && Heros->IsValidTarget(GEntityList->Player(), Q->Range()) && QKill > Heros->GetHealth())
				{
					Q->CastOnTarget(Heros);
				}
				if (KW->Enabled() && W->IsReady() && Heros->IsValidTarget(GEntityList->Player(), W->Range()) && WKill > Heros->GetHealth())
				{
					W->CastOnTarget(Heros);
				}
				if (KE->Enabled() && E->IsReady() && Heros->IsValidTarget(GEntityList->Player(), E->Range()) && EKill > Heros->GetHealth())
				{
					E->CastOnTarget(Heros);
				}
			}
		}
	}
}
void Drawing()
{
	auto player = GEntityList->Player();
	if (DrawReady->Enabled())
	{
		if (Q->IsReady() && DrawQ->Enabled())
		{
			GRender->DrawOutlinedCircle(player->GetPosition(), Vec4(225, 225, 0, 225), Q->Range());
		}

		if (W->IsReady() && DrawW->Enabled())
		{
			GRender->DrawOutlinedCircle(player->GetPosition(), Vec4(225, 225, 0, 225), W->Range());
		}
		if (E->IsReady() && DrawE->Enabled())
		{
			GRender->DrawOutlinedCircle(player->GetPosition(), Vec4(225, 255, 0, 255), E->Range());
		}
		if (R->IsReady() && DrawR->Enabled())
		{
			GRender->DrawOutlinedCircle(player->GetPosition(), Vec4(225, 225, 0, 225), R->Range());
		}
	}

	else
	{
		if (DrawQ->Enabled())
		{
			GRender->DrawOutlinedCircle(player->GetPosition(), Vec4(225, 225, 0, 225), Q->Range());
		}

		if (DrawW->Enabled())
		{
			GRender->DrawOutlinedCircle(player->GetPosition(), Vec4(225, 225, 0, 225), W->Range());
		}
		if (E->IsReady() && DrawE->Enabled())
		{
			GRender->DrawOutlinedCircle(player->GetPosition(), Vec4(225, 255, 0, 255), E->Range());
		}
		if (DrawR->Enabled())
		{
			GRender->DrawOutlinedCircle(player->GetPosition(), Vec4(225, 225, 0, 225), R->Range());
		}
	}
}
PLUGIN_EVENT(void) OnGameUpdate()
{
	Kill();
	if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
	{
		Combo();
	}
	if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
	{
		LaneClear();
		JungleClear();
	}


}

PLUGIN_EVENT(void) OnRender()
{
	Drawing();
}

// Called when plugin is first loaded
PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{
	// Initializes global interfaces for core access
	PluginSDKSetup(PluginSDK);
	Menu();
	Sejuani();
	GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->AddEventHandler(kEventOnRender, OnRender);

	IUnit* player = GEntityList->Player();

	if (strcmp(player->ChampionName(), "Sejuani") == 0)
	{
		GGame->PrintChat("<b><font color=\"#e40aff\">Sejuani - 1.0<b><font color=\"#01CCF8\"> by </font></b> Pwego<font color=\"#313131\"> - Loaded</font></b>");

	}
	else
	{
		GRender->NotificationEx(Color::Red().Get(), 2, true, true, "Sejuani unloaded. You dont play the Champion!");
	}
}

// Called when plugin is unloaded
PLUGIN_API void OnUnload()
{
	MainMenu->Remove();
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);

}

