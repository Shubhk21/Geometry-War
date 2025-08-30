#pragma once
#include<memory>
#include<string>
#include "Components.h"

class Entity {
	friend class EntityManager;
	bool	m_active = true;
	size_t  m_id = 0;
	std::string m_tag = "default";

	Entity(const size_t id, const std::string& tag);
public:

	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CTransform> CTransform;
	std::shared_ptr<CCollision> CCollision;
	std::shared_ptr<CScore> CScore;
	std::shared_ptr<CLifespan> CLifespan;
	std::shared_ptr<CInput> CInput;

	bool isActive() const;
	std::string getTag() const;
	rsize_t getId() const;
	void destroy();

};