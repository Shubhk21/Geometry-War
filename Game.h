#pragma once
#include "EntityManager.h"
#include<SFML/Audio.hpp>
#include<thread>
struct EnemyConfig {

};

struct PlayerConfig {

};

struct BulletConfig {

};

class Game {

	sf::RenderWindow	m_window;
	EntityManager		m_entityManager;
	sf::Font			m_font;
	sf::Text			m_text_score;
	sf::Text			m_text_life;
	sf::RectangleShape  m_lifelineCover;
	sf::RectangleShape  m_lifelineInner;
	sf::Sound			m_sound;
	sf::SoundBuffer		m_soundBuffer;
	PlayerConfig		m_playerConfig;
	EnemyConfig			m_enemyConfig;
	BulletConfig		m_bulletConfig;
	int					m_score = 0;
	long long			m_currentFrame = 0;
	int					m_lastEnemySpawnTime = 0;
	int					m_lastSpecialWeaponSpawned = 0;
	bool				m_paused = false;
	bool				m_running = true;
	int					m_playerLife;

	std::shared_ptr<Entity> m_player;
	void init(const std::string& config);
	void setPaused(bool paused);

	void sMovement();
	void sCollision();
	void sRender();
	void sUserInput();
	void sEnemySpawn();
	void sLifespan();

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnBullet(const Vec2& mousePos);
	Vec2 getDirection(const Vec2& bulletPosition, const Vec2& reference, int speed);
	void spawnSpecialWeapon();

public:
	Game(const std::string& config);
	void run();
};