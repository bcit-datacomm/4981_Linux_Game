 #include "Base.h"

Base::Base() {
	this->setX(100);
	this->setY(100);

	printf("Create Base\n");
}

Base::~Base() {
	printf("Destory Base\n");
}	
void Base::onCollision() {
}
	
void Base::collidingProjectile(int damage) {
	this->health -= damage;
}

std::pair<float, float> Base::addPlayer(unsigned int id){
	this->spawnPoints[id] = std::make_pair(200,200);
	return this->spawnPoints[id];
}

std::pair<float, float> Base::getSpawnPoints(unsigned int id){
	return this->spawnPoints[id];
}
