#include "ComponentRegistery.h"
#include "BoxCollider2D.h"
#include "CircleCollider.h"
#include "ImageRenderer.h"
#include "PhysicsBody.h"
#include "TextRenderer.h"
#include "Transform.h"

using namespace Engine;

ComponentRegistery::ComponentRegistery()
{
	Register<BoxCollider2D>("BoxCollider");
	Register<CircleCollider>("CircleCollider");
	Register<ImageRenderer>("ImageRenderer");
	Register<PhysicsBody>("PhyscisBody");
	Register<TextRenderer>("TextRenderer");
	Register<Transform>("Transform");
}
