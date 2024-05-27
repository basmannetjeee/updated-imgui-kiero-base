#pragma once
// ImGui
#include "../../imgui/imgui.h"

// Offsets
#include <client.dll.hpp>
#include <offsets.hpp>

// Self explanatory
#include <cmath>
#include <vector>


struct view_matrix_t {
	float* operator[ ](int index) {
		return matrix[index];
	}
	float matrix[4][4];
};


// Borrowed

class Vec2
{
public:
	float x, y;
public:
	Vec2() :x(0.f), y(0.f) {}
	Vec2(float x_, float y_) :x(x_), y(y_) {}
	Vec2(ImVec2 ImVec2_) :x(ImVec2_.x), y(ImVec2_.y) {}
	Vec2 operator=(ImVec2 ImVec2_)
	{
		x = ImVec2_.x;
		y = ImVec2_.y;
		return *this;
	}
	Vec2 operator+(Vec2 Vec2_)
	{
		return { x + Vec2_.x,y + Vec2_.y };
	}
	Vec2 operator-(Vec2 Vec2_)
	{
		return { x - Vec2_.x,y - Vec2_.y };
	}
	Vec2 operator*(Vec2 Vec2_)
	{
		return { x * Vec2_.x,y * Vec2_.y };
	}
	Vec2 operator/(Vec2 Vec2_)
	{
		return { x / Vec2_.x,y / Vec2_.y };
	}
	Vec2 operator*(float n)
	{
		return { x * n, y * n };
	}
	Vec2 operator/(float n)
	{
		return { x / n, y / n };
	}
	bool operator==(Vec2 Vec2_)
	{
		return x == Vec2_.x && y == Vec2_.y;
	}
	bool operator!=(Vec2 Vec2_)
	{
		return x != Vec2_.x || y != Vec2_.y;
	}
	ImVec2 ToImVec2()
	{
		return ImVec2(x, y);
	}
	float Length()
	{
		return sqrtf(powf(x, 2) + powf(y, 2));
	}
	float DistanceTo(const Vec2& Pos)
	{
		return sqrtf(powf(Pos.x - x, 2) + powf(Pos.y - y, 2));
	}
};


// Borrowed

class Vec3
{
public:
	float x, y, z;
public:
	Vec3() :x(0.f), y(0.f), z(0.f) {}
	Vec3(float x_, float y_, float z_) :x(x_), y(y_), z(z_) {}
	Vec3 operator+(Vec3 Vec3_)
	{
		return { x + Vec3_.x,y + Vec3_.y,z + Vec3_.z };
	}
	Vec3 operator-(Vec3 Vec3_)
	{
		return { x - Vec3_.x,y - Vec3_.y,z - Vec3_.z };
	}
	Vec3 operator*(Vec3 Vec3_)
	{
		return { x * Vec3_.x,y * Vec3_.y,z * Vec3_.z };
	}
	Vec3 operator/(Vec3 Vec3_)
	{
		return { x / Vec3_.x,y / Vec3_.y,z / Vec3_.z };
	}
	Vec3 operator*(float n)
	{
		return { x * n,y * n,z * n };
	}
	Vec3 operator/(float n)
	{
		return { x / n,y / n,z / n };
	}
	bool operator==(Vec3 Vec3_)
	{
		return x == Vec3_.x && y == Vec3_.y && z == Vec3_.z;
	}
	bool operator!=(Vec3 Vec3_)
	{
		return x != Vec3_.x || y != Vec3_.y || z != Vec3_.z;
	}
	float Length()
	{
		return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
	}
	float DistanceTo(const Vec3& Pos)
	{
		return sqrtf(powf(Pos.x - x, 2) + powf(Pos.y - y, 2) + powf(Pos.z - z, 2));
	}
};

class CNetworkedQuantizedFloat
{
public:
	std::float_t m_Value;
	int m_nEncoder;
	bool m_bUnflattened;
};

class CNetworkVelocityVector
{
public:
	Vec3 m_vecValue;                    // 0x0000
	CNetworkedQuantizedFloat m_vecX;    // 0x0010
	CNetworkedQuantizedFloat m_vecY;    // 0x0018
	CNetworkedQuantizedFloat m_vecZ;    // 0x0020
};

class CNetworkViewOffsetVector
{
public:
	Vec3 m_vecValue;                    // 0x0000
	CNetworkedQuantizedFloat m_vecX;    // 0x0010
	CNetworkedQuantizedFloat m_vecY;    // 0x0018
	CNetworkedQuantizedFloat m_vecZ;    // 0x0020
};

class CNetworkOriginQuantizedVector
{
public:
	Vec3 m_vecValue;                    // 0x0000
	CNetworkedQuantizedFloat m_vecX;    // 0x0010
	CNetworkedQuantizedFloat m_vecY;    // 0x0018
	CNetworkedQuantizedFloat m_vecZ;    // 0x0020
};

class CNetworkOriginCellCoordQuantizedVector
{
public:
	Vec3 m_vecValue;                    // 0x0000
	unsigned int m_cellX;               // 0x0010
	unsigned int m_cellY;               // 0x0012
	unsigned int m_cellZ;               // 0x0014
	unsigned int m_nOutsideWorld;       // 0x0016
	CNetworkedQuantizedFloat m_vecX;    // 0x0018
	CNetworkedQuantizedFloat m_vecY;    // 0x0020
	CNetworkedQuantizedFloat m_vecZ;    // 0x0028
};


// Partially SKIDDED

class ScreenProjection {
public:

	BOOL WorldToScreen(const Vec3& Pos, Vec2& ToPos) const
	{
		float(*ViewMatrix)[4][4] = reinterpret_cast<float(*)[4][4]>((uintptr_t)GetModuleHandle("client.dll") + cs2_dumper::offsets::client_dll::dwViewMatrix);

		if ((uintptr_t)GetModuleHandle("client.dll") + cs2_dumper::offsets::client_dll::dwViewMatrix == 0)
			return FALSE;

		const float w = (*ViewMatrix)[3][0] * Pos.x + (*ViewMatrix)[3][1] * Pos.y + (*ViewMatrix)[3][2] * Pos.z + (*ViewMatrix)[3][3];
		if (w <= 0.01)
			return FALSE;

		const float invW = 1.0f / w;
		const float SightX = ImGui::GetIO().DisplaySize.x * 0.5f;
		const float SightY = ImGui::GetIO().DisplaySize.y * 0.5f;

		ToPos.x = SightX + (((*ViewMatrix)[0][0] * Pos.x + (*ViewMatrix)[0][1] * Pos.y + (*ViewMatrix)[0][2] * Pos.z + (*ViewMatrix)[0][3]) * invW * SightX);
		ToPos.y = SightY - (((*ViewMatrix)[1][0] * Pos.x + (*ViewMatrix)[1][1] * Pos.y + (*ViewMatrix)[1][2] * Pos.z + (*ViewMatrix)[1][3]) * invW * SightY);

		return TRUE;
	}
};