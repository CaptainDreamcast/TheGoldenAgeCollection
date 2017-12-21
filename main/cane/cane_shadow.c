#include "cane_shadow.h"

#include <tari/mugenanimationhandler.h>

#include "cane_resources.h"


int addCaneShadow(Position* referencePosition) {
	int id = addMugenAnimation(getMugenAnimation(getCaneAnimations(), 4), getCaneSprites(), makePosition(0, 32, -2));
	setMugenAnimationBasePosition(id, referencePosition);

	return id;
}

void removeCaneShadow(int id) {
	removeMugenAnimation(id);
}