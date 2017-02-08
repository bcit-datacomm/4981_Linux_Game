#include "Base.h"
#include "Marine.h"
#include<random>

Base::Base() {
	this->setX((MAP_WIDTH/2)-BASE_WIDTH);
	this->setY((MAP_HEIGHT/2)-BASE_HEIGHT);
	this->setHeight(BASE_HEIGHT);
	this->setWidth(BASE_WIDTH);

	printf("Create Base\n");
}

Base::~Base() {
	printf("Destory Base\n");
}	
void Base::onCollision() {
	printf("Base collision\n");
}
	
void Base::collidingProjectile(int damage) {
	this->health -= damage;
}

Point Base::getSpawnPoint(){
	
	//random number generator 
	std::random_device rd;
	std::mt19937 eng(rd());

	//range 0 to 3 to be used for choosing North, South, West or East of Base
	std::uniform_int_distribution<> distr(0,3);
	
	int x;
	int y;
	
	//The gab between the spawn point and base.
	int gab = 100;
	

	switch(distr(eng)){
		case 0://North
	
			//y point is fixed
			y = this->getY()-gab;
			//x point is randomly generated;	
			x = distr(eng, decltype(distr)::param_type(this->getX()-gab, this->getX()+this->getWidth()+gab));

			break;
		case 1://South
			y = this->getY()+this->getHeight()+gab;
			x = distr(eng, decltype(distr)::param_type(this->getX()-gab, this->getX()+this->getWidth()+gab));
			break;
		case 2://West
			
			//x point is fixed
			x = this->getX()-gab;
			//y point is randomly generated
			y = distr(eng, decltype(distr)::param_type(this->getY()-gab, this->getY()+this->getHeight()+gab));
			break;
		case 3://East
			x = this->getY()+this->getWidth()+gab;
			y = distr(eng, decltype(distr)::param_type(this->getY()-gab, this->getY()+this->getHeight()+gab));
			break;
	}
	
	//considering the player size
	if(x < this->getX())
		x-=MARINE_WIDTH;
	if(y < this->getY())
		y-=MARINE_HEIGHT;

	return Point(x,y);
}
