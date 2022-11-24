#pragma once

#include <Engine/CScript.h>

class MagmaScript :
    public CScript
{
private:
	float	m_fLifeTime;
	float	m_fAddTime;

	float	m_fExplodeScale;

public:
	void	Update();
	void	LateUpdate();

	CLONE(MagmaScript)


public:
	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;

public:
	MagmaScript();
	virtual ~MagmaScript();
};
