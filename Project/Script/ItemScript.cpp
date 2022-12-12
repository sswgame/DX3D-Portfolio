#include "pch.h"
#include "ItemScript.h"

#include "PlayerScript.h"

#include <Engine/CLight3D.h>
#include <Engine/CSerializer.h>

ItemScript::ItemScript()
	: CScript{ (int)SCRIPT_TYPE::ITEMSCRIPT }
	, m_eItemType(ItemType::NONE)
	, m_pOwnerObject(nullptr)
	, m_pParticleObject(nullptr)
	, m_fColliderSphereSize(0.f)

{
}

ItemScript::ItemScript(const ItemScript& _origin)
	: CScript{ (int)SCRIPT_TYPE::ITEMSCRIPT }
	, m_eItemType(_origin.m_eItemType)
	, m_pOwnerObject(_origin.m_pOwnerObject)
	, m_pParticleObject(_origin.m_pParticleObject)
	, m_fColliderSphereSize(_origin.m_fColliderSphereSize)

{
}

ItemScript::~ItemScript()
{
}

void ItemScript::SetItemType(ItemType _type)
{
	m_eItemType = _type;

	m_fColliderSphereSize = 20.f;
}

void ItemScript::SetParticleObject(CGameObject* _particle)
{
	m_pParticleObject = _particle;

	m_pOwnerObject->AddChild(m_pParticleObject);
}

void ItemScript::start()
{
}

void ItemScript::update()
{
}

void ItemScript::lateupdate()
{
}

void ItemScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void ItemScript::OnCollision(CGameObject* _OtherObject)
{
	if (nullptr != _OtherObject->GetScript<PlayerScript>())
	{
		if (m_eItemType == ItemType::ITEM_HEALING)
		{
			//_OtherObject->GetScript<PlayerScript>()->Cure()
		}
		else if (m_eItemType == ItemType::ITME_HP_POTION)
		{
			//_OtherObject->GetScript<PlayerScript>()->AddHPPotion()
		}

		m_pOwnerObject->Destroy();
	}
}

void ItemScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void ItemScript::Serialize(YAML::Emitter& emitter)
{
	//emitter << YAML::Key << NAME_OF(m_eItemType) << YAML::Value << m_eItemType;
	m_pOwnerObject->Serialize(emitter);
	m_pParticleObject->Serialize(emitter);
	emitter << YAML::Key << NAME_OF(m_fColliderSphereSize) << YAML::Value << m_fColliderSphereSize;

	CScript::Serialize(emitter);
}

void ItemScript::Deserialize(const YAML::Node& node)
{
	m_pOwnerObject->Deserialize(node);
	m_pParticleObject->Deserialize(node);
	m_fColliderSphereSize = node[NAME_OF(m_fColliderSphereSize)].as<float>();

	CScript::Deserialize(node);
}
