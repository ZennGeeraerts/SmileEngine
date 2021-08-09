#include <SmileEngine.h>

class Sandbox : public Smile::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Smile::Application* Smile::CreateApplication()
{
	return new Sandbox();
}