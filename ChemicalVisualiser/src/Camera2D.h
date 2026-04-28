#include <Slate/Renderer.h>

class Camera2D : public Slate::Camera
{
public:
	Camera2D();
	void Update(float ts) override;
public:
	float Zoom = 1.0f;
	float Speed = 1.0f;
	bool InvertX = false;
	bool InvertY = false;
};