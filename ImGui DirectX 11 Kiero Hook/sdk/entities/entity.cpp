#pragma once
#include <Windows.h>
#include "entity.h"
#include "../math/math.h"
#include <string>
#include <sstream>
#include <client.dll.hpp>
#include <iostream>
#include <thread>

namespace entity {
    VOID LocalEntity::LocalPlayerController()
    {
        this->EntityAddress = *reinterpret_cast<DWORD64*>((uintptr_t)GetModuleHandle("client.dll") + cs2_dumper::offsets::client_dll::dwLocalPlayerController);

        this->iTeamNum = *reinterpret_cast<int*>(this->EntityAddress + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
    }

    VOID CEntity::UpdateName() {
        char* playerNamePtr = (char*)(this->EntityAddress + cs2_dumper::schemas::client_dll::CBasePlayerController::m_iszPlayerName);
        strncpy_s(this->EntityName, playerNamePtr, sizeof(this->EntityName) - 1);
        this->EntityName[sizeof(this->EntityName) - 1] = '\0'; // Null terminate.
    }

    void CEntity::UpdatePos() {
        if (this->EntityPawnAddress + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin > 0x7FFFFF) {
            this->VecOrigin = *(Vec3*)(this->EntityPawnAddress + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
        }
    }

    BOOL CEntity::IsAlive()
    {
        return this->Pawn.iHealth > 0 && this->Entity.bPawnIsAlive == 1;
    }

    BOOL CEntity::TeamCheck()
    {
        return this->Entity.iTeamNum == LocalController.iTeamNum;
    }

    BOOL CEntity::isLocal()
    {
        return this->EntityAddress == LocalController.EntityAddress;
    }

    void CEntity::PlayerWeapon()
    {
        DWORD64 weaponAddress = *reinterpret_cast<DWORD64*>(this->EntityPawnAddress + cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_pClippingWeapon);

        weaponAddress = *reinterpret_cast<DWORD64*>(weaponAddress + 0x10);
        weaponAddress = *reinterpret_cast<DWORD64*>(weaponAddress + 0x20);

        // Read the weapon name into a buffer
        char weaponBuffer[32];
        memcpy(weaponBuffer, reinterpret_cast<void*>(weaponAddress), sizeof(weaponBuffer) - 1);
        weaponBuffer[sizeof(weaponBuffer) - 1] = '\0'; // Null-terminate the string

        // Convert the buffer to a std::string and process it
        std::string weaponName(weaponBuffer);
        std::size_t underscoreIndex = weaponName.find('_');
        if (underscoreIndex != std::string::npos)
            weaponName = weaponName.substr(underscoreIndex + 1);

        this->Pawn.pClippingWeapon = weaponName;
    }

    //EntityName


    // Not functional anymore because of removal of m_pBoneArray. (I will try to figure it out)
    //void CEntity::UpdatePlayerBoneData()
    //{
    //    this->Pawn.GameSceneNode = *reinterpret_cast<DWORD64*>(this->EntityPawnAddress + cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode);
    //    this->Pawn.BoneArray = *reinterpret_cast<DWORD64*>(this->Pawn.GameSceneNode + Offsets->PlayerEntity.m_pBoneArray);
    //
    //    BoneJointData BoneArray[30];
    //    memcpy(BoneArray, reinterpret_cast<void*>(this->Pawn.BoneArray), sizeof(BoneArray));
    //
    //    this->BoneData.BonePosList.clear();
    //
    //    for (int i = 0; i < 30; ++i)
    //    {
    //        Vec2 ScreenPos;
    //        bool IsVisible = View.WorldToScreen(BoneArray[i].Pos, ScreenPos);
    //        this->BoneData.BonePosList.push_back({ BoneArray[i].Pos, ScreenPos, IsVisible });
    //    }
    //}


    VOID UpdatePlayerEntities()
    {
        Entity->EntityPlayerVector.clear();
        Entity->PawnVector.clear();
        for (int index = 1; index < 64; ++index) // MaxEntities = 64
        {
            DWORD64 entityAddress = *reinterpret_cast<DWORD64*>(Entity->EntityListEntry + index * 0x78); // EntityListEntrySize = 0x78


            if (entityAddress == NULL || !entityAddress) {
                std::cout << "EntityAddress found\n";
                continue;
            }

            // Read the pawn index from the entity address
            DWORD pawnIndex = *reinterpret_cast<DWORD*>(entityAddress + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
            std::cout << "pawn index just set\n";
            // Compute the address of the entity pawn list entry
            DWORD64 entityPawnListEntry = *reinterpret_cast<DWORD64*>((uintptr_t)GetModuleHandle("client.dll") + cs2_dumper::offsets::client_dll::dwEntityList);
            std::cout << "pawn list entry just set\n";
            DWORD64 pawnListEntryOffset = 0x10 + 8 * ((pawnIndex & 0x7FFF) >> 9); // PawnListEntryOffsetBase = 0x10, PawnListEntryMultiplier = 8, PawnIndexMask = 0x7FFF, PawnIndexShift = 9
            std::cout << "pawn list entry offset set\n";
            DWORD64 entityPawnAddress = *reinterpret_cast<DWORD64*>(entityPawnListEntry + pawnListEntryOffset);
            std::cout << "set entity pawn address\n";
            entityPawnAddress += 0x78 * (pawnIndex & 0x1FF); // PawnAddressMultiplier = 0x78, PawnSubIndexMask = 0x1FF
            std::cout << "set entity pawn address x2\n";
            entityPawnAddress = *reinterpret_cast<DWORD64*>(entityPawnAddress);

            // Validate the entity address and add to vectors if valid
            if (entityAddress > 0x7FFFFF && entityAddress != 0) // MinValidAddress = 0x7FFFFF
            {
                Entity->EntityPlayerVector.push_back(entityAddress);
                Entity->PawnVector.push_back(entityPawnAddress);
            }
        }
    }

    void PrintPlayerNames()
    {
        std::cout << "updating entities:\n";
        Entity->UpdateEntity();
        UpdatePlayerEntities();
        Entity->UpdatePos();
        std::cout << "updated.\n\n\n\n";

        for (DWORD64 entityAddress : entity::Entity->EntityPlayerVector)
        {
            Vec3 PawnPos = *(Vec3*)(static_cast<uintptr_t>(entityAddress) + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);

            if (PawnPos.x == NAN || PawnPos.y == NAN || PawnPos.z == NAN ) {
                continue;
            }

            char PawnPosString[256];
            snprintf(PawnPosString, sizeof(PawnPosString), "x: %.2f, y: %.2f, z: %.2f", PawnPos.x, PawnPos.y, PawnPos.z);


            std::cout << "Player name: " << PawnPosString << std::endl;
        }
    }
}
