#pragma once

#include "Modules/ResourceLoader/ResourceLoader.h"
#include "Utility/Containers/String.h"
#include "Utility/Typedefs.h"

#define S_LOG(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define S_LOG_FUNC(fmt, ...) printf("[%s] ", __FUNCTION__);\
printf(fmt, ##__VA_ARGS__); printf("\n");

