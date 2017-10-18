#include "vectrex_simulation.h"

#include <tari/animation.h>

#include "vectrexanimations.h"

static struct {
	int mVectrexID;
	TextureData mVectrexTexture;

	int mBGID;
	TextureData mBGTexture;

} gData;

void loadVectrexSimulation()
{
	instantiateActor(VectrexAnimationHandler);
	
	gData.mVectrexTexture = loadTexture("assets/stormstorm/VECTREX.pkg");
	gData.mVectrexID = playOneFrameAnimationLoop(makePosition(0, 0, 50), &gData.mVectrexTexture);

	gData.mBGTexture = loadTexture("assets/stormstorm/BG.pkg");
	gData.mBGID = playOneFrameAnimationLoop(makePosition(0, 0, 1), &gData.mBGTexture);

}
