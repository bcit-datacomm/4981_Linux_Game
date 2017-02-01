 #ifndef CAMERA_HPP
#define CAMERA_HPP
#include "Player.hpp"
#include "Window.hpp"

class Camera
{
public:
	
	void setViewSize(float w, float h); // Set camera width and height
	void move(float x, float  y); // moves the camera based on player
	
	Camera(float w, float h);
	~Camera();
	
	float getX();
	float getY();
	
private:
	
	SDL_Rect viewport;
	
	float x = 0; // x coordinate of the camera
	float y = 0; // y coordinate of the camera
	float w = 0; // width of the camera
	float h = 0; // height of the camera
};

#endif
