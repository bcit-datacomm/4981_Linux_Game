#include "GameManager.h"
#include "HitBox.h"

GameManager::GameManager() {
	
}
	
GameManager::~GameManager() {
	for (const auto& m : this->marineManager) {
		this->deleteMarine(m.first);
	}
}

void GameManager::renderObjects(SDL_Renderer* gRenderer, float camX, float camY) {

	for (const auto& m : this->marineManager) {
		m.second->texture.render(gRenderer, m.second->getX()-camX, m.second->getY()-camY);
	}
	
}

unsigned int GameManager::createMarine() {
	unsigned int id = 0;
	if (!this->marineManager.empty()) {
		id = this->marineManager.rbegin()->first + 1;
	}
	this->marineManager[id] = new Marine();
	return id;
}

void GameManager::deleteMarine(unsigned int id) {
	if (this->marineManager.count(id)) {
		delete this->marineManager.find(id)->second;
	}
	this->marineManager.erase(id);
}

bool GameManager::addMarine(unsigned int id, Marine* newMarine) {
	if (this->marineManager.count(id)) {
		return false;
	} else {
		this->marineManager[id] = newMarine;
		return true;
	}
}
	
Marine* GameManager::getMarine(unsigned int id) {
	return this->marineManager.find(id)->second;
}

void GameManager::updateCollider(CollisionHandler* ch) {
	std::vector<HitBox*> moveColliders;
	for (const auto& m : this->marineManager) {
		moveColliders.push_back(&m.second->movementHitBox);
	}
	ch->updateMoveColliders(moveColliders);
}


