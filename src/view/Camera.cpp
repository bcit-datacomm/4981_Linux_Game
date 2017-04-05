#include "../view/Camera.h"
#include "../player/Player.h"
#include "../view/Window.h"
#include "../player/Marine.h"

/**
* Date: Jan. 30, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: Camera(const int w, const int h)
*       w : View width
*       h : View height
*
* Description: 
*   ctor for camera.
*/
Camera::Camera(const int w, const int h) : viewport() {
    setViewSize(w, h);
}

/**
* Date: Jan. 30, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: move(const int x, const int y)
*       x : player x point
*       y : player y point
*
* Description: 
*   Move camera to player.
*/
void Camera::move(const int x, const int y) {
    viewport.x = ( x + MARINE_HEIGHT / 2 ) - viewport.w / 2;
    viewport.y = ( y + MARINE_WIDTH / 2 ) - viewport.h / 2;
}

/**
* Date: Jan. 30, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: setViewSize(const int w, const int h)
*       w : View width
*       h : View height
*
* Description: 
*   Set camera width and height
*/
void Camera::setViewSize(const int w, const int h) {
    viewport.w = w;
    viewport.h = h;
}
