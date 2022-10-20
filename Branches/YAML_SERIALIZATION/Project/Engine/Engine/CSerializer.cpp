#include "pch.h"
#include "CSerializer.h"

YAML::Node YAML::convert<CRes>::encode(const CRes& resource)
{
	Node node;
	node.push_back(ToString(resource.GetKey()));
	node.push_back(ToString(resource.GetRelativePath()));
	node.SetStyle(EmitterStyle::Flow);

	return node;
}

bool YAML::convert<CRes>::decode(const Node& node, CRes& resource)
{
	if (false == node.IsSequence() || node.size() != 2)
	{
		return false;
	}
	resource.SetKey(ToWString(node[0].as<std::string>()));
	resource.SetRelativePath(ToWString(node[1].as<std::string>()));

	return true;
}

YAML::Emitter& YAML::operator<<(Emitter& emitter, const CRes& resource)
{
	emitter << Flow;
	emitter << BeginSeq << ToString(resource.GetKey()) << ToString(resource.GetRelativePath()) << EndSeq;
	return emitter;
}

YAML::Node YAML::convert<Vector2>::encode(const Vec2& vec)
{
	Node node;
	node.push_back(vec.x);
	node.push_back(vec.y);
	node.SetStyle(EmitterStyle::Flow);

	return node;
}

bool YAML::convert<Vector2>::decode(const Node& node, Vec2& vec)
{
	if (false == node.IsSequence() || node.size() != 2)
	{
		return false;
	}
	vec.x = node[0].as<float>();
	vec.y = node[1].as<float>();

	return true;
}

YAML::Emitter& YAML::operator<<(Emitter& emitter, const Vec2& vec)
{
	emitter << Flow;
	emitter << BeginSeq << vec.x << vec.y << EndSeq;
	return emitter;
}

YAML::Node YAML::convert<Vector3>::encode(const Vec3& vec)
{
	Node node;
	node.push_back(vec.x);
	node.push_back(vec.y);
	node.SetStyle(EmitterStyle::Flow);

	return node;
}

bool YAML::convert<Vector3>::decode(const Node& node, Vec3& vec)
{
	if (false == node.IsSequence() || node.size() != 3)
	{
		return false;
	}
	vec.x = node[0].as<float>();
	vec.y = node[1].as<float>();
	vec.z = node[2].as<float>();

	return true;
}

YAML::Emitter& YAML::operator<<(Emitter& emitter, const Vec3& vec)
{
	emitter << Flow;
	emitter << BeginSeq << vec.x << vec.y << vec.z << EndSeq;
	return emitter;
}

YAML::Node YAML::convert<Vector4>::encode(const Vec4& vec)
{
	Node node;
	node.push_back(vec.x);
	node.push_back(vec.y);
	node.SetStyle(EmitterStyle::Flow);

	return node;
}

bool YAML::convert<Vector4>::decode(const Node& node, Vec4& vec)
{
	if (false == node.IsSequence() || node.size() != 4)
	{
		return false;
	}
	vec.x = node[0].as<float>();
	vec.y = node[1].as<float>();
	vec.z = node[2].as<float>();
	vec.w = node[3].as<float>();

	return true;
}

YAML::Emitter& YAML::operator<<(Emitter& emitter, const Vec4& vec)
{
	emitter << Flow;
	emitter << BeginSeq << vec.x << vec.y << vec.z << vec.w << EndSeq;
	return emitter;
}

YAML::Node YAML::convert<Matrix>::encode(const Matrix& mat)
{
	Node node;
	node.push_back(mat._11);
	node.push_back(mat._12);
	node.push_back(mat._13);
	node.push_back(mat._14);
	node.push_back(mat._21);
	node.push_back(mat._22);
	node.push_back(mat._23);
	node.push_back(mat._24);
	node.push_back(mat._31);
	node.push_back(mat._32);
	node.push_back(mat._33);
	node.push_back(mat._34);
	node.push_back(mat._41);
	node.push_back(mat._42);
	node.push_back(mat._43);
	node.push_back(mat._44);
	node.SetStyle(EmitterStyle::Flow);

	return node;
}

bool YAML::convert<Matrix>::decode(const Node& node, Matrix& mat)
{
	if (false == node.IsSequence() || node.size() != 16)
	{
		return false;
	}
	mat._11 = node[0].as<float>();
	mat._12 = node[1].as<float>();
	mat._13 = node[2].as<float>();
	mat._14 = node[3].as<float>();
	mat._21 = node[4].as<float>();
	mat._22 = node[5].as<float>();
	mat._23 = node[6].as<float>();
	mat._24 = node[7].as<float>();
	mat._31 = node[8].as<float>();
	mat._32 = node[9].as<float>();
	mat._33 = node[10].as<float>();
	mat._34 = node[11].as<float>();
	mat._41 = node[12].as<float>();
	mat._42 = node[13].as<float>();
	mat._43 = node[14].as<float>();
	mat._44 = node[15].as<float>();

	return true;
}

YAML::Emitter& YAML::operator<<(Emitter& emitter, const Matrix& mat)
{
	emitter << Flow;
	emitter << BeginSeq;
	emitter << mat._11 << mat._12 << mat._13 << mat._14;
	emitter << mat._21 << mat._22 << mat._23 << mat._24;
	emitter << mat._31 << mat._32 << mat._33 << mat._34;
	emitter << mat._41 << mat._42 << mat._43 << mat._44;
	emitter << EndSeq;
	return emitter;
}
