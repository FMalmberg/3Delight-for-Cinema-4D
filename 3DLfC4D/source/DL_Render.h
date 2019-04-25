#ifndef NORRSKEN_H
#define NORRSKEN_H

#include "c4d.h"
#include "DL_API.h"

bool DL_RenderFrame(BaseDocument* doc, long frame, RENDER_MODE mode = PREVIEW_RENDER, bool progressive = false);

#endif