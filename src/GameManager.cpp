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
		this->deleteZombie(z.first);
	}
	for (const auto& o : this->objectManager) {
		this->deleteObject(o.first);
	}
	for (const auto& m : this->turretManager) {
		this->deleteTurret(m.first);
	}
}

// Render all objects in level
void GameManager::renderObjects(SDL_Renderer* gRenderer, float camX, float camY) {

	for (const auto& m : this->marineManager) {
		m.second->texture.render(gRenderer, m.second->getX()-camX, m.second->getY()-camY,
								 NULL, m.second->getAngle());
	}
	for (const auto& o : this->objectManager) {
		o.second->texture.render(gRenderer, o.second->getX()-camX, o.second->getY()-camY);
	}
	for (const auto& z : this->zombieManager) {
		z.second->texture.render(gRenderer, z.second->getX()-camX, z.second->getY()-camY, 
								 NULL, z.second->getAngle());
	}

    for (const auto& m : this->turretManager) {
		m.second->texture.render(gRenderer, m.second->getX()-camX, m.second->getY()-camY, 
								 NULL, m.second->getAngle());
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

// Create marine add it to manager, returns marine id
bool GameManager::createMarine(SDL_Renderer* gRenderer, float x, float y) {
	unsigned int id = 0;
	if (!this->marineManager.empty()) {
		id = this->marineManager.rbegin()->first + 1;
	}
	this->marineManager[id] = new Marine();
	if (!this->marineManager[id]->texture.loadFromFile("assets/texture/arrow.png", gRenderer)) {
		printf("Failed to load the player texture!\n");
		this->deleteMarine(id);
		return false;
	}
	this->marineManager[id]->setPosition(x,y);
	return true;
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

unsigned int GameManager::addZombie(Zombie* newZombie) {
	unsigned int id = 0;
	if (!this->zombieManager.empty()) {
		id = this->zombieManager.rbegin()->first + 1;
	}
	zombieManager[id] = newZombie;
	return id;
}

// Deletes zombie from level
void GameManager::deleteZombie(unsigned int id) {
	if (this->zombieManager.count(id)) {
		delete this->zombieManager.find(id)->second;
	}
	this->zombieManager.erase(id);
}

unsigned int GameManager::addObject(Object* newObject) {
	unsigned int id = 0;
	if (!this->objectManager.empty()) {
		id = this->objectManager.rbegin()->first + 1;
	}
	objectManager[id] = newObject;
	return id;
}

// Deletes zombie from level
void GameManager::deleteObject(unsigned int id) {
	if (this->objectManager.count(id)) {
		delete this->objectManager.find(id)->second;
	}
	this->objectManager.erase(id);
}

// Returns Collision Handler
CollisionHandler* GameManager::getCollisionHandler() {
    return this->collisionHandler;
}

// Update colliders to current state
void GameManager::updateCollider() {

	delete this->collisionHandler->quadtreeMov;
	delete this->collisionHandler->quadtreePro;
	delete this->collisionHandler->quadtreeDam;

	this->collisionHandler->quadtreeMov = new Quadtree(0, {0,0,2000,2000});
	this->collisionHandler->quadtreePro = new Quadtree(0, {0,0,2000,2000});
	this->collisionHandler->quadtreeDam = new Quadtree(0, {0,0,2000,2000});
	

	for (const auto& m : this->marineManager) {
		this->collisionHandler->quadtreeMov->insert(&m.second->movementHitBox);
		this->collisionHandler->quadtreePro->insert(&m.second->projectileHitBox);
		this->collisionHandler->quadtreeDam->insert(&m.second->damageHitBox);
	}
	std::vector<HitBox*> projectileColliders;
	for (const auto& m : this->marineManager) {
		projectileColliders.push_back(&m.second->projectileHitBox);
	}
	for (const auto& z : this->zombieManager) {
		this->collisionHandler->quadtreeMov->insert(&z.second->movementHitBox);
		this->collisionHandler->quadtreePro->insert(&z.second->projectileHitBox);
		this->collisionHandler->quadtreeDam->insert(&z.second->damageHitBox);
	}
	for (const auto& o : this->objectManager) {
		this->collisionHandler->quadtreeMov->insert(&o.second->movementHitBox);
		this->collisionHandler->quadtreePro->insert(&o.second->projectileHitBox);
		this->collisionHandler->quadtreeDam->insert(&o.second->damageHitBox);
	}
  	for (const auto& m : this->turretManager) {
		this->collisionHandler->quadtreeMov->insert(&m.second->movementHitBox);
		this->collisionHandler->quadtreePro->insert(&m.second->projectileHitBox);
		this->collisionHandler->quadtreeDam->insert(&m.second->damageHitBox);
	}
  
}


