#include "../view/Camera.h"
#include "../player/Player.h"
#include "../view/Window.h"
#include "../player/Marine.h"

Camera::Camera(const int w, const int h) : viewport() {
    setViewSize(w, h);
}

Camera::~Camera() {

}

// Move camera to player
void Camera::move(const int x, const int y) {
    viewport.x = ( x + MARINE_HEIGHT / 2 ) - viewport.w / 2;
    viewport.y = ( y + MARINE_WIDTH / 2 ) - viewport.h / 2;
}

// Set camera width and height
void Camera::setViewSize(const int w, const int h) {
    viewport.w = w;
    viewport.h = h;
}
