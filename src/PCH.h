#pragma once

#define WiIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <stdio.h>
#include <string>
#include <vector>
#include <memory>

#include <Core/Type.h>
#include <Core/Log.h>
#include <Core/LinerMath.h>

#define SAFE_RELEASE(x) if(x) { (x)->Release(); x = nullptr; }
#define SAFE_DELETE(x) if(x) { delete x; x = nullptr; }