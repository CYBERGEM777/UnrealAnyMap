# UnrealAnyMap
UnrealAnyMap is a tiny header-only C++ library that provides a heterogeneous map class that lets you map keys of any type to values of any type. One key type can map to values of any number of arbitrary types, in a typesafe manner.

```cpp
AnyMap.Add("map", 1);
AnyMap.Add("any", FName(TEXT("type")));
AnyMap.Add(2, "to");
AnyMap.Add(3, FVector(1, 1, 1));
AnyMap.Add(FIntPoint(0, 0), FLinearColor::Blue);
AnyMap.Add(FIntPoint(1, 0), FString(TEXT("any type!")));
```

Too lazy to write and/or maintain a struct? Want to quickly pack a bunch of arbitrary data into a TQueue? Just want to show the compiler *who's really boss*? We've got you covered here at __CYBERGEM *Experimental Software Division.*__

__WARNING!__ This software hasn't been tested much yet! Use at own risk! 😊

Suggestions, feature requests, bug reports? [Drop by the Discord server.](http://discord.gg/XdSMcKQa5F)

## Requirements
UnrealAnyMap is implemented using std::any, therefore C++17 or greater is required. If you need to set up your Unreal project to compile using C++17, [see the note below.](#setup-cpp17)

## Installation
Clone the repository to your project's Public source directory:

```bash
cd [YOUR PROJECT SOURCE DIR]/Public/
git clone https://github.com/CYBERGEM-CORPORATION/UnrealAnyMap
```

Or you might prefer to add it as a submodule (if your source dir is part of a git repository):

```bash
cd [YOUR PROJECT SOURCE DIR]/Public/
git submodule add https://github.com/CYBERGEM-CORPORATION/UnrealAnyMap
```

## Use Example

```cpp
#include "UnrealAnyMap/UnrealAnyMap.h"

. . .

Cybergem::FUnrealAnyMap AnyMap;

// Add values to the map.
//
// Key types can be any type that can be used as a key by TMap
// (e.g. types that implement GetTypeHash()).
//
// Values can be any copy constructible type.
AnyMap.Add("name",      FString(TEXT("Tom Bombadil")));
AnyMap.Add("alignment", FName(TEXT("Chaotic_Neutral")));
AnyMap.Add("level",     9999);
AnyMap.Add("health",    1000000.f);
AnyMap.Add("position",  FVector(3, 7, 17));

// Add another key type:
AnyMap.Add(FIntPoint(0, 0), FName(TEXT("Cloak")));
AnyMap.Add(FIntPoint(1, 0), FName(TEXT("Hat")));
AnyMap.Add(FIntPoint(0, 1), FName(TEXT("Boots")));

// And another!
AnyMap.Add(0, FString(TEXT("Wake now, my merry friends!")));
AnyMap.Add(1, FString(TEXT("Long tales are thirsty.")));
AnyMap.Add(2, FString(TEXT("Keep to the green grass.")));

// Variables that will be populated with the values in the map.
FString Name;
FName Alignment;
int32 Level;
float Health;
FVector Position;

FName InventorySlot0;
FName InventorySlot1;
FName InventorySlot2;

FString Dialog0;
FString Dialog1;
FString Dialog2;

// Get copies of the map values.
//
// If a reference of the wrong type is passed to Get() it returns false
// and the out variable is not populated with any value.
bool bGotAll = AnyMap.Get("name", Name);
bGotAll &=     AnyMap.Get("alignment", Alignment);
bGotAll &=     AnyMap.Get("level", Level);
bGotAll &=     AnyMap.Get("health", Health);
bGotAll &=     AnyMap.Get("position", Position);

bGotAll &=     AnyMap.Get(FIntPoint(0, 0), InventorySlot0);
bGotAll &=     AnyMap.Get(FIntPoint(1, 0), InventorySlot1);
bGotAll &=     AnyMap.Get(FIntPoint(0, 1), InventorySlot2);

bGotAll &=     AnyMap.Get(0, Dialog0);
bGotAll &=     AnyMap.Get(1, Dialog1);
bGotAll &=     AnyMap.Get(2, Dialog2);

// Returns false because key "foo" is not in map.
bool bError0 = AnyMap.Get("foo", Level);

// Returns false because Level type does not match type of value with key "position"
bool bError1 = AnyMap.Get("position", Level);

// Get pointers to the map values and modify them.
//
// Returns nullptr if key not in map or template type parameter does not match value.
if (int32* _Level = AnyMap.Find<int32>("level"))
{
	*_Level += 1;
}

if (float* _Health = AnyMap.Find<float>("health"))
{
	*_Health += 21;
}

// Remove a key value pair from the map:
AnyMap.Remove("position");

// Get the number of values in the map with a single key type:
int32 NumItems = AnyMap.Num<FIntPoint>();

// Empty the map of all keys of a single type:
AnyMap.Empty<FIntPoint>();

// Get the number of key types in the map:
int32 NumKeyTypes = AnyMap.NumKeyTypes();

// Empty the map completely:
AnyMap.Empty();
```

## <a name="setup-cpp17"></a>Setting Up Your Project To Compile With C++17 (Or Greater)
*If you know of a better way to do this, please contact me.*

From [the instructions provided here by _YAF_Lightbringer:](https://forums.unrealengine.com/t/whats-needed-for-using-c-17/124529/4)

Open the VS project UE_4.2x\Engine\Source\Programs\UnrealBuildTool\UnrealBuildTool.csproj

Depending on which .NET SDK is intalled on your machine, you might need to change the target framework.

Find the file Platform\Windows\VCToolChain.cs in that solution.

Go to the end of the method AppendCLArguments_Global (it’s around line 537 at the time of writing) and add:

```
Arguments.Add("/std:c++latest");
```

there (or whatever c++ standard you want the compiler to use) and then build the solution.

It should build to UE_4.2x\Engine\Binaries\DotNET\UnrealBuildTool.exe

Now add these lines to YourProject.Build.cs:

```
PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
CppStandard = CppStandardVersion.Latest;
```

and finally, build your project.

## License
Published under the [MIT](https://choosealicense.com/licenses/mit/) license.

