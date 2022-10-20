#pragma once
#include "Cres.h"

#define NAME_OF(variable) #variable

template <typename T>
Ptr<T> LoadAs(const YAML::Node& node)
{
	std::vector<std::string> keyValue;
	for (const auto& data : node)
	{
		keyValue.push_back(data.as<std::string>());
	}

	return CResMgr::GetInst()->Load<T>(ToWString(keyValue[0]), ToWString(keyValue[1]));
}

inline std::pair<std::wstring, std::wstring> GetResourceInfo(const YAML::Node& node)
{
	std::vector<std::string> keyValue;
	for (const auto& data : node)
	{
		keyValue.push_back(data.as<std::string>());
	}

	return {ToWString(keyValue[0]), ToWString(keyValue[1])};
}

//
//namespace YAML
//{
//	template <>
//	struct convert<tLightInfo>
//	{
//		static Node encode(const tLightInfo& resource)
//		{
//			Node node;
//			node.push_back(ToString(resource.GetKey()));
//			node.push_back(ToString(resource.GetRelativePath()));
//			node.SetStyle(EmitterStyle::Flow);
//
//			return node;
//		}
//
//		static bool decode(const Node& node, tLightInfo& resource)
//		{
//			if (false == node.IsSequence() || node.size() != 2)
//			{
//				return false;
//			}
//			resource.SetKey(ToWString(node[0].as<std::string>()));
//			resource.SetRelativePath(ToWString(node[1].as<std::string>()));
//
//			return true;
//		}
//	};
//
//	inline Emitter& operator <<(Emitter& emitter, const tLightInfo& resource)
//	{
//		emitter << Flow;
//		emitter << BeginSeq << ToString(resource.GetKey()) << ToString(resource.GetRelativePath()) << EndSeq;
//		return emitter;
//	}
//}

//CRes
namespace YAML
{
	template <>
	struct convert<CRes>
	{
		static Node encode(const CRes& resource);

		static bool decode(const Node& node, CRes& resource);
	};

	inline Emitter& operator <<(Emitter& emitter, const CRes& resource);
}

//Vec2
namespace YAML
{
	template <>
	struct convert<Vec2>
	{
		static Node encode(const Vec2& vec);

		static bool decode(const Node& node, Vec2& vec);
	};

	inline Emitter& operator <<(Emitter& emitter, const Vec2& vec);
}

//Vec3
namespace YAML
{
	template <>
	struct convert<Vec3>
	{
		static Node encode(const Vec3& vec);

		static bool decode(const Node& node, Vec3& vec);
	};

	inline Emitter& operator <<(Emitter& emitter, const Vec3& vec);
}

//Vec4
namespace YAML
{
	template <>
	struct convert<Vec4>
	{
		static Node encode(const Vec4& vec);

		static bool decode(const Node& node, Vec4& vec);
	};

	inline Emitter& operator <<(Emitter& emitter, const Vec4& vec);
}

//Matrix
namespace YAML
{
	template <>
	struct convert<Matrix>
	{
		static Node encode(const Matrix& mat);

		static bool decode(const Node& node, Matrix& mat);
	};

	inline Emitter& operator <<(Emitter& emitter, const Matrix& mat);
}
