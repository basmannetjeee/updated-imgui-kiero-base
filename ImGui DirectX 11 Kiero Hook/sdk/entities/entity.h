#pragma once

#include <string>
#include <client.dll.hpp>
#include <vector>
#include "../math/math.h"


namespace entity {
    class PlayerEntity
    {
    public:
        int iTeamNum;
        int bPawnIsAlive;
        std::string iszPlayerName;
        DWORD hPlayerPawn;
        DWORD PawnIndex;
    };

    class PlayerPawn
    {
    public:
        Vec3 vOldOrigin;
        std::string pClippingWeapon;
        int Armor;
        int iHealth;
        DWORD64 GameSceneNode;
        DWORD64 BoneArray;
    };

    class LocalEntity
    {
    public:
        DWORD64 EntityAddress;
        DWORD64 PawnAddress;
        int iTeamNum;
        DWORD uHandle;
        Vec3 vOldOrigin;
        VOID LocalPlayerController();
    };

    class WeaponEntity
    {
    public:
        DWORD64 EntityAddress;
        DWORD64 GameSceneNode;
        DWORD64 WeaponAddress;
        std::string pClippingWeapon;
        int Index;
        Vec3 ItemPos3D;
        Vec2 ItemPos2D;
    };

    class CEntity
    {
    public:
        Vec3 VecOrigin;
        char EntityName[128];
        PlayerEntity Entity;
        LocalEntity Local;
        PlayerPawn Pawn;
        WeaponEntity Weapon;
        DWORD64 EntityList;
        DWORD64 EntityListEntry;
        DWORD64 EntityAddress;
        DWORD64 EntityPawnAddress;
        std::vector<DWORD64> EntityPlayerVector;
        std::vector<DWORD64> PawnVector;
        std::vector<DWORD64> EntityWeaponVector;
        DWORD64 EntityPawnListEntry;
        BOOL IsAlive();
        VOID UpdateName();
        VOID UpdatePos();
        VOID UpdateEntity();
        VOID PlayerWeapon();
        //VOID UpdatePlayerBoneData();
        BOOL TeamCheck();
        BOOL isLocal();
    };

    inline VOID CEntity::UpdateEntity()
    {
        this->EntityList = *reinterpret_cast<DWORD64*>((uintptr_t)GetModuleHandle("client.dll") + cs2_dumper::offsets::client_dll::dwEntityList);
        this->EntityListEntry = *reinterpret_cast<DWORD64*>(this->EntityList + 0x10);
        this->EntityPawnListEntry = (uintptr_t)GetModuleHandle("client.dll") + cs2_dumper::offsets::client_dll::dwEntityList;
    }

    inline CEntity* Entity = new CEntity;
    inline WeaponEntity* Weapon = new WeaponEntity;
    inline LocalEntity LocalController;
    VOID PlayerEntities();
    VOID EnableCheats();

    void PrintPlayerNames();
}