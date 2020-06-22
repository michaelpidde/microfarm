#pragma once

#define Import __declspec(dllimport)

#include "..\win32api.h"
#include <stdio.h>
#include <string.h>

Import
int ME_init();
Import
int ME_load_asset_class(char *dir, char *prefix);