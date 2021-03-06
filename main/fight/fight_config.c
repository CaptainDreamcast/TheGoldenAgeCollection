#include "fight_config.h"

#include <prism/mugendefreader.h>

static struct {
	double mDefaultAttackDamageDoneToPowerMultiplier;
	double mDefaultAttackDamageReceivedToPowerMultiplier;

} gData;

static void loadRules(MugenDefScript* tScript) {
	gData.mDefaultAttackDamageDoneToPowerMultiplier = getMugenDefFloatOrDefault(tScript, "Rules", "Default.Attack.LifeToPowerMul", 0.7);
	gData.mDefaultAttackDamageReceivedToPowerMultiplier = getMugenDefFloatOrDefault(tScript, "Rules", "Default.GetHit.LifeToPowerMul", 0.6);
}

static void loadMugenConfig(void* tData) {
	(void)tData;

	MugenDefScript script = loadMugenDefScript("assets/main/fight/data/mugen.cfg"); // TODO: proper path
	loadRules(&script);
	unloadMugenDefScript(script);
}

ActorBlueprint DreamMugenConfig = {
	.mLoad = loadMugenConfig,
};

double getDreamDefaultAttackDamageDoneToPowerMultiplier()
{
	return gData.mDefaultAttackDamageDoneToPowerMultiplier;
}

double getDreamDefaultAttackDamageReceivedToPowerMultiplier()
{
	return gData.mDefaultAttackDamageReceivedToPowerMultiplier;
}

