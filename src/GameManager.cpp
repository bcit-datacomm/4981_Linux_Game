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
	for (const auto& z : this->zombieManager) {
		delete z.second;
	}

}

// Render all objects in level
void GameManager::renderObjects(SDL_Renderer* gRenderer, float camX, float camY) {

	for (const auto& m : this->marineManager) {
		//m.second->setAngle();
		m.second->texture.render(gRenderer, m.second->getX()-camX, m.second->getY()-camY, NULL, m.second->getAngle());
	}
	for (const auto& o : this->objectManager) {
		o.second->texture.render(gRenderer, o.second->getX()-camX, o.second->getY()-camY, NULL, NULL);
	}
	for (const auto& z : this->zombieManager) {
		z.second->texture.render(gRenderer, z.second->getX()-camX, z.second->getY()-camY, NULL, z.second->getAngle());
	}



}

// Update marine movements. health, and actions
void GameManager::updateMarines(const float& delta) {
	for (const auto& m : this->marineManager) {
		m.second->move((m.second->getDX()*delta), (m.second->getDY()*delta), this->collisionHandler);
	}
}

// Update zombie movements.
void GameManager::updateZombies(const float& delta) {
	for (const auto& z : this->zombieManager) {
		z.second->generateRandomMove();
		z.second->move((z.second->getDX()*delta), (z.second->getDY()*delta), this->collisionHandler);
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

// Update colliders to current state
void GameManager::updateCollider() {
	std::vector<HitBox*> moveColliders;
	for (const auto& m : this->marineManager) {
		moveColliders.push_back(&m.second->movementHitBox);
	}
	std::vector<HitBox*> projectileColliders;
	for (const auto& m : this->marineManager) {
		projectileColliders.push_back(&m.second->projectileHitBox);
	}
	for (const auto& z : this->zombieManager) {
		moveColliders.push_back(&z.second->movementHitBox);
		projectileColliders.push_back(&z.second->projectileHitBox);
	}
	for (const auto& o : this->objectManager) {
		moveColliders.push_back(&o.second->movementHitBox);
		projectileColliders.push_back(&o.second->projectileHitBox);
	}
	this->collisionHandler->updateColliders(moveColliders, projectileColliders);
}

unsigned int GameManager::addZombie(Zombie* newZombie) {
	unsigned int id = 0;
	if (!this->zombieManager.empty()) {
		id = this->zombieManager.rbegin()->first + 1;
	}
	zombieManager[id] = newZombie;
	return id;
}

unsigned int GameManager::addObject(Object* newObject) {
	unsigned int id = 0;
	if (!this->objectManager.empty()) {
		id = this->objectManager.rbegin()->first + 1;
	}
	objectManager[id] = newObject;
	return id;
}

