#include<iostream>
#include"Game.h"
#include<cmath>
#include<numbers>


Game::Game(const std::string& config) {
	init(config);
}

void Game::init(const std::string& config) {
	if (!m_font.loadFromFile("fonts/arial.ttf")) {
		std::cerr << "Could not open the font!\n";
		exit(-1);
	}
	m_window.create(sf::VideoMode::getDesktopMode(), "Game Window", sf::Style::Fullscreen);
	m_window.setFramerateLimit(60);
	m_text_score.setFont(m_font);
	m_text_score.setString("SCORE : 0");
	m_text_score.setCharacterSize(24);
	m_text_score.setFillColor(sf::Color::White);
	m_text_score.setPosition(10, 10);

	m_text_life.setFont(m_font);
	m_text_life.setString("LIFE ");
	m_text_life.setCharacterSize(24);
	m_text_life.setFillColor(sf::Color::White);
	m_text_life.setPosition(m_window.getSize().x - 310, 10);

	m_playerLife = 100;
	m_lifelineCover.setSize({ 210,40 });
	m_lifelineInner.setSize({ 110,40 });
	m_lifelineCover.setPosition(m_window.getSize().x - 250, 2);
	m_lifelineInner.setPosition(m_window.getSize().x - 250, 2);
	m_lifelineCover.setFillColor(sf::Color::Black);
	m_lifelineInner.setFillColor(sf::Color::Green);
	m_lifelineCover.setOutlineColor(sf::Color::White);
	m_lifelineCover.setOutlineThickness(1.0f);
	spawnPlayer();
}

void Game::run() {
	while (m_running) {
		m_entityManager.updateEntities();

		if (!m_paused) {
			sEnemySpawn();

			sMovement();

			sCollision();

			sLifespan();

			m_currentFrame++;
		}

		sUserInput();

		sRender();
	}
}

void Game::setPaused(bool p) {
	m_paused = p;
}

void Game::spawnPlayer() {

	float px = (float)(m_window.getSize().x) / 2;
	float py = (float)(m_window.getSize().y) / 2;

	auto player = m_entityManager.addEntities("player");

	player->CTransform = std::make_shared<CTransform>(Vec2(px, py), Vec2(0.0f, 0.0f), 0.0f);

	player->CInput = std::make_shared<CInput>();

	player->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color::Black, sf::Color::Red, 4.0f);

	player->CCollision = std::make_shared<CCollision>(32.0f);

	m_player = player;
}

void Game::sEnemySpawn() {

	if (m_currentFrame - m_lastEnemySpawnTime > 100) {
		spawnEnemy();
	}

}

void Game::sRender() {

	m_window.clear();

	m_text_score.setString("SCORE : " + std::to_string(m_score));

	m_window.draw(m_text_score);

	m_window.draw(m_text_life);

	m_window.draw(m_lifelineCover);

	m_window.draw(m_lifelineInner);

	for (auto e : m_entityManager.getEntities()) {

		e->cShape->circle.setPosition(e->CTransform->position.x, e->CTransform->position.y);

		e->CTransform->angle += 1.0f;

		e->cShape->circle.setRotation(e->CTransform->angle);

		m_window.draw(e->cShape->circle);

	}

	m_window.display();
}

void Game::sUserInput() {

	sf::Event event;

	while (m_window.pollEvent(event)) {

		if (event.type == sf::Event::Closed) {
			m_running = false;
		}

		if (event.type == sf::Event::KeyPressed) {

			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player->CInput->up = true;
				break;
			case sf::Keyboard::A:
				m_player->CInput->left = true;
				break;
			case sf::Keyboard::S:
				m_player->CInput->down = true;
				break;
			case sf::Keyboard::D:
				m_player->CInput->right = true;
				break;
			case sf::Keyboard::Escape:
				m_running = false;
				break;
			case sf::Keyboard::P:
				m_paused = (m_paused ? false : true);
				if (!m_soundBuffer.loadFromFile("sound/smb_pause.wav"))
					return exit(0);
				m_sound.setBuffer(m_soundBuffer);
				m_sound.play();
				break;
			default:
				break;
			}
		}

		if (event.type == sf::Event::KeyReleased) {
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player->CInput->up = false;
				break;
			case sf::Keyboard::A:
				m_player->CInput->left = false;
				break;
			case sf::Keyboard::S:
				m_player->CInput->down = false;
				break;
			case sf::Keyboard::D:
				m_player->CInput->right = false;
				break;
			default:
				break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed) {

			if (event.mouseButton.button == sf::Mouse::Left) {
				if (m_paused) continue;
				if (!m_soundBuffer.loadFromFile("sound/smb_bump.wav"))
					return exit(0);
				m_sound.setBuffer(m_soundBuffer);
				m_sound.play();
				spawnBullet(Vec2((float)event.mouseButton.x, (float)event.mouseButton.y));
			}
			if (event.mouseButton.button == sf::Mouse::Right) {
				if (m_paused || m_currentFrame - m_lastSpecialWeaponSpawned < 600) continue;
				if (!m_soundBuffer.loadFromFile("sound/smb_fireworks.wav"))
					return exit(0);
				m_sound.setBuffer(m_soundBuffer);
				m_sound.play();
				spawnSpecialWeapon();
			}

		}
	}

}


void Game::sMovement() {

	m_player->CTransform->velocity.y = 0;
	m_player->CTransform->velocity.x = 0;
	m_lifelineInner.setSize({ 2.1f * m_playerLife,40.0f });

	if (m_player->CInput->up) {
		m_player->CTransform->velocity.y = -5;
	}

	if (m_player->CInput->down) {
		m_player->CTransform->velocity.y = 5;
	}

	if (m_player->CInput->left) {
		m_player->CTransform->velocity.x = -5;
	}

	if (m_player->CInput->right) {
		m_player->CTransform->velocity.x = 5;
	}

	for (auto& e : m_entityManager.getEntities()) {
		e->CTransform->position.x += e->CTransform->velocity.x;
		e->CTransform->position.y += e->CTransform->velocity.y;
	}

}


Vec2 Game::getDirection(const Vec2& bulletPosition, const Vec2& reference, int speed) {

	float xDir = bulletPosition.x - reference.x;

	float yDir = bulletPosition.y - reference.y;

	float theta = atan2f(yDir, xDir);

	return Vec2(speed * cos(theta), speed * sin(theta));

}


void Game::spawnBullet(const Vec2& position) {

	auto bullet = m_entityManager.addEntities("bullet");

	bullet->cShape = std::make_shared<CShape>(10.0f, 8, sf::Color(255, 255, 255, 255), sf::Color(0, 0, 0, 255), 0.0f);

	Vec2 direction = getDirection(position, m_player->CTransform->position, 6);

	bullet->CTransform = std::make_shared<CTransform>(Vec2(m_player->CTransform->position.x, m_player->CTransform->position.y), direction, 0.0f);

	bullet->CCollision = std::make_shared<CCollision>(10.0f);

	bullet->CLifespan = std::make_shared<CLifespan>(100, 100);
}

void Game::spawnEnemy() {

	auto enemy = m_entityManager.addEntities("enemy");

	float ex = (float)(rand() % m_window.getSize().x);
	float ey = (float)(rand() % m_window.getSize().y);

	float dx = (float)(rand() % m_window.getSize().x);
	float dy = (float)(rand() % m_window.getSize().y);

	Vec2 direction = getDirection(Vec2(dx, dy), Vec2(ex, ey), 5);

	enemy->CTransform = std::make_shared<CTransform>(Vec2(ex, ey), direction, 0.0f);

	int points = (3 + (rand() % 6));

	enemy->cShape = std::make_shared<CShape>(32.0f, points, sf::Color((int)ex % 256, (int)ey % 256, (int)dx % 256), sf::Color::White, 2.0f);

	enemy->CCollision = std::make_shared<CCollision>(32.0f);

	m_lastEnemySpawnTime = m_currentFrame;

}

void Game::sCollision() {


	for (auto& b : m_entityManager.getEntities("bullet")) {

		for (auto& e : m_entityManager.getEntities("enemy")) {

			float Dx = e->CTransform->position.x - b->CTransform->position.x;

			float Dy = e->CTransform->position.y - b->CTransform->position.y;

			float distance = b->CCollision->radius + e->CCollision->radius;

			if (distance * distance >= ((Dx * Dx) + (Dy * Dy))) {
				b->destroy();
				e->destroy();
				spawnSmallEnemies(e);
				m_score += (int)e->cShape->circle.getPointCount();
				if (!m_soundBuffer.loadFromFile("sound/smb_bowserfire.wav"))
					return exit(0);
				m_sound.setBuffer(m_soundBuffer);
				m_sound.play();
			}
		}

	}

	for (auto& e : m_entityManager.getEntities("enemy")) {

		if ((((int)(e->CTransform->position.x) + (int)(e->CCollision->radius)) >= (int)m_window.getSize().x) || (((int)(e->CTransform->position.x) - (int)(e->CCollision->radius)) <= 0)) {
			e->CTransform->velocity.x *= -1;
		}

		if ((((int)(e->CTransform->position.y) + (int)(e->CCollision->radius)) >= (int)m_window.getSize().y) || (((int)(e->CTransform->position.y) - (int)(e->CCollision->radius)) <= 0)) {
			e->CTransform->velocity.y *= -1;
		}

		float Dx = e->CTransform->position.x - m_player->CTransform->position.x;

		float Dy = e->CTransform->position.y - m_player->CTransform->position.y;

		float distance = m_player->CCollision->radius + e->CCollision->radius;

		if (distance * distance >= ((Dx * Dx) + (Dy * Dy))) {
			m_player->CTransform->position = Vec2((float)(m_window.getSize().x) / 2, (float)(m_window.getSize().y) / 2);
			e->destroy();
			m_playerLife -= e->cShape->circle.getPointCount();
		}
	}

}

void Game::sLifespan() {

	for (auto& b : m_entityManager.getEntities("bullet")) {

		b->CLifespan->remaining--;

		sf::Uint8 alpha = (b->cShape->circle.getFillColor().a) - 2;

		b->cShape->circle.setFillColor(sf::Color(255, 255, 255, alpha));

		if (b->CLifespan->remaining <= 0) {
			b->destroy();
		}

	}

	for (auto& se : m_entityManager.getEntities("smallenemy")) {

		se->CLifespan->remaining--;

		sf::Uint8 alpha = (se->cShape->circle.getFillColor().a) - 8;

		se->cShape->circle.setFillColor(sf::Color(se->cShape->circle.getFillColor().r, se->cShape->circle.getFillColor().g, se->cShape->circle.getFillColor().b, alpha));

		if (se->CLifespan->remaining <= 0) {
			se->destroy();
		}

	}

	if (m_playerLife <= 0) {
		if (!m_soundBuffer.loadFromFile("sound/smb_mariodie.wav"))
			return exit(0);
		m_sound.setBuffer(m_soundBuffer);
		m_sound.play();
		std::cout << "GAME OVER! ---- FINAL SCORE :" << m_score << std::endl;
		while (m_sound.getStatus() == sf::Sound::Playing) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		exit(0);
	}

}

void Game::spawnSpecialWeapon() {

	float theta = 0.0f;

	for (int i = 0;i < 24;i++) {

		auto bullet = m_entityManager.addEntities("bullet");

		bullet->cShape = std::make_shared<CShape>(10.0f, 8, sf::Color(255, 255, 255, 255), sf::Color(0, 0, 0, 255), 0.0f);

		bullet->CTransform = std::make_shared<CTransform>(Vec2(m_player->CTransform->position.x, m_player->CTransform->position.y), Vec2(6 * cos(theta * PI / 180.0f), 6 * sin(theta * PI / 180.0f)), 0.0f);

		bullet->CCollision = std::make_shared<CCollision>(10.0f);

		bullet->CLifespan = std::make_shared<CLifespan>(100, 100);

		theta += 15.0f;
	}

	m_lastSpecialWeaponSpawned = m_currentFrame;

}


void Game::spawnSmallEnemies(std::shared_ptr<Entity> enemy) {

	float theta = 0.0f;

	int points = (int)enemy->cShape->circle.getPointCount();

	float angle_jump = 360 / (float)points;

	sf::Color enemy_color = enemy->cShape->circle.getFillColor();

	for (int i = 0;i < points;i++) {

		auto smallenemy = m_entityManager.addEntities("smallenemy");

		smallenemy->cShape = std::make_shared<CShape>(20.0f, points, enemy_color, sf::Color::White, 1.0f);

		smallenemy->CTransform = std::make_shared<CTransform>(Vec2(enemy->CTransform->position.x, enemy->CTransform->position.y), Vec2(5.0f * cos(theta * PI / 180.0f), 5.0f * sin(theta * PI / 180.0f)), 0.0f);

		smallenemy->CCollision = std::make_shared<CCollision>(20.0f);

		smallenemy->CLifespan = std::make_shared<CLifespan>(20, 20);

		theta += angle_jump;
	}

}