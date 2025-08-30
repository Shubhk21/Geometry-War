#include"EntityManager.h"

EntityManager::EntityManager() {}

std::shared_ptr<Entity> EntityManager::addEntities(const std::string& tag) {
	auto e = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
	m_toaddEntities.push_back(e);
	return e;
}

EntityVec EntityManager::getEntities() {
	return m_entities;
}

EntityVec EntityManager::getEntities(const std::string tag) {
	return m_entityMap[tag];
}

void EntityManager::updateEntities() {

	for (auto e : m_toaddEntities) {
		m_entities.push_back(e);
		m_entityMap[e->getTag()].push_back(e);
	}

	m_toaddEntities.clear();

	auto vptr = std::remove_if(m_entities.begin(), m_entities.end(), [](std::shared_ptr<Entity> et) {return !et->isActive();});

	m_entities.erase(vptr, m_entities.end());

	for (auto& st_to_vec : m_entityMap) {

		auto mptr = std::remove_if(st_to_vec.second.begin(), st_to_vec.second.end(), [](std::shared_ptr<Entity> et) {return !et->isActive();});

		st_to_vec.second.erase(mptr, st_to_vec.second.end());
	}
	return;
}