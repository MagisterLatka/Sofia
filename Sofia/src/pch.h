#pragma once

#include <Sofia/Platform.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <array>
#include <algorithm>
#include <chrono>
#include <random>
#include <functional>
#include <filesystem>
#include <memory>
#include <exception>
#include <bitset>
#include <queue>
#include <map>

#include <Sofia/Log.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#if defined(SOF_PLATFORM_WINDOWS)
#	ifndef NOMIXMAX
#		define NOMINMAX
#	endif
#	define WIN32_LEAN_AND_MEAN
#	define NOGDICAPMASKS
#	define NOSYSMETRICS
#	define NOMENUS
#	define NOICONS
#	define NOSYSCOMMANDS
#	define NORASTEROPS
#	define OEMRESOURCE
#	define NOATOM
#	define NOCLIPBOARD
#	define NOCOLOR
#	define NOCTLMGR
#	define NODRAWTEXT
#	define NOKERNEL
#	define NONLS
#	define NOMEMMGR
#	define NOMETAFILE
#	define NOOPENFILE
#	define NOSCROLL
#	define NOSERVICE
#	define NOSOUND
#	define NOTEXTMETRIC
#	define NOWH
#	define NOCOMM
#	define NOKANJI
#	define NOHELP
#	define NOPROFILER
#	define NODEFERWINDOWPOS
#	define NOMCX
#	define NORPC
#	define NOPROXYSTUB
#	define NOIMAGE
#	define NOTAPE
#	include <Windows.h>
#endif
