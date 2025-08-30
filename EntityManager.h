#pragma once
#include<memory>
#include<vector>
#include<map>
#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;


class EntityManager {

	EntityVec	m_entities;
	EntityVec	m_toaddEntities;
	EntityMap	m_entityMap;
	size_t		m_totalEntities = 0;


public:

	EntityManager();
	std::shared_ptr<Entity> addEntities(const std::string& tag);
	EntityVec getEntities();
	EntityVec getEntities(const std::string tag);
	void updateEntities();
};
