#pragma once

#include <vector>
#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>

using std::vector;
using std::list;
using std::map;
using std::make_pair;
using std::wstring;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::pair;
using std::priority_queue;
using std::queue;


using std::mutex;
using std::thread;
using std::atomic;
using std::unique_lock;
using std::condition_variable;
using namespace std::chrono_literals;

#include <typeinfo>
#include <assert.h>
#include <random>


// FileSystem
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
using namespace std::experimental;
using namespace std::experimental::filesystem;



#include <wrl.h>
using namespace Microsoft::WRL;

#include <d3d11.h>					// DirectX11
#include <d3dcompiler.h>			// Shader ������
#include <DirectXMath.h>			// DX Math
#include <DirectXPackedVector.h>	

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")


#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;


// Fbx Loader
#include <FBXLoader/fbxsdk.h>
#ifdef _DEBUG
#pragma comment(lib, "FBXLoader/x64/debug/libfbxsdk-md.lib")
#else
#pragma comment(lib, "FBXLoader/x64/release/libfbxsdk-md.lib")
#endif


#include "define.h"
#include "struct.h"
#include "func.h"

#include "CSingleton.h"