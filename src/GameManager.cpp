#include "GameManager.h"
#include "HitBox.h"

GameManager::GameManager() {
	this->collisionHandler = new CollisionHandler();
}

GameManager::~GameManager() {
	delete this->collisionHandler;
	for (const auto& m : this->marineManager) {
		this->deleteMarine(m.first);
	}
}

// Render all objects in level
void GameManager::renderObjects(SDL_Renderer* gRenderer, float camX, float camY) {

	for (const auto& m : this->marineManager) {
		//m.second->setAngle();
		m.second->texture.render(gRenderer, m.second->getX()-camX, m.second->getY()-camY, NULL, m.second->getAngle());
	}

    // render all turrets on the map
    for (const auto& m : this->turretManager) {
		m.second->texture.render(gRenderer, m.second->getX()-camX, m.second->getY()-camY, NULL, m.second->getAngle());
	}
}

// Update marine movements. health, and actions
void GameManager::updateMarines(const float& delta) {
	for (const auto& m : this->marineManager) {
		m.second->move((m.second->getDX()*delta), (m.second->getDY()*delta), this->collisionHandler);
	}
}

// Create marine add it to manager, returns marine id
unsigned int GameManager::createMarine() {
	unsigned int id = 0;
	if (!this->marineManager.empty()) {
		id = this->marineManager.rbegin()->first + 1;
	}
	this->marineManager[id] = new Marine();
	return id;
}

// Deletes marine from level
void GameManager::deleteMarine(unsigned int id) {
	if (this->marineManager.count(id)) {
		delete this->marineManager.find(id)->second;
	}
	this->marineManager.erase(id);
}

// Adds marine to level
bool GameManager::addMarine(unsigned int id, Marine* newMarine) {
	if (this->marineManager.count(id)) {
		return false;
	} else {
		this->marineManager[id] = newMarine;
		return true;
	}
}

// Get a marine by its id
Marine* GameManager::getMarine(unsigned int id) {
	return this->marineManager.find(id)->second;
}

// Create Turret add it to manager, returns tower id
unsigned int GameManager::createTurret() {
    unsigned int id = 0;
	if (!this->turretManager.empty()) {
		id = this->turretManager.rbegin()->first + 1;
	}
	this->turretManager[id] = new Turret();
	return id;
}

// Deletes tower from level
void GameManager::deleteTurret(unsigned int id) {
    if (this->turretManager.count(id)) {
		delete this->turretManager.find(id)->second;
	}
	this->turretManager.erase(id);
}

// Adds tower to level
bool GameManager::addTurret (unsigned int id, Turret* newTurret) {
    if (this->turretManager.count(id)) {
		return false;
	} else {
		this->turretManager[id] = newTurret;
		return true;
	}
}

// Get a tower by its id
Turret* GameManager::getTurret(unsigned int id) {
    return this->turretManager.find(id)->second;
}

// Update colliders to current state
void GameManager::updateCollider() {
	std::vector<HitBox*> moveColliders;
	for (const auto& m : this->marineManager) {
		moveColliders.push_back(&m.second->movementHitBox);
	}
    // adding all turrets into moveColliders vector
	for (const auto& m : this->turretManager) {
		moveColliders.push_back(&m.second->movementHitBox);
	}
	std::vector<HitBox*> projectileColliders;
	for (const auto& m : this->marineManager) {
		moveColliders.push_back(&m.second->projectileHitBox);
	}
    // adding all turrets into projectileColliders vector
	for (const auto& m : this->turretManager) {
		moveColliders.push_back(&m.second->projectileHitBox);
	}
	this->collisionHandler->updateColliders(moveColliders, projectileColliders);
}
