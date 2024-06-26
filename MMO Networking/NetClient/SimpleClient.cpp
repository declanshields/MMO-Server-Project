#include <iostream>

#include <engine_net.h>

enum class CustomMsgTypes : uint32_t
{
	FireBullet,
	MovePlayer
};

class CustomClient : public Engine::Network::client_interface<CustomMsgTypes>
{
public:
	bool FireBullet(float x, float y)
	{
		Engine::Network::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::FireBullet;
		msg << x << y;
		Send(msg);
	}
};

int main()
{
	CustomClient c;
	c.Connect("community.onelonecoder.com", 60000);
	c.FireBullet(2.0f, 5.0f);

	return 0;
}