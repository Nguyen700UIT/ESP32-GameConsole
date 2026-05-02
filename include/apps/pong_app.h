#pragma once

#include "console/button.h"

namespace pong {

void enter();
void tick();
void exit();
void onButtonPressed(console::Button button);
void onButtonReleased(console::Button button);

}
