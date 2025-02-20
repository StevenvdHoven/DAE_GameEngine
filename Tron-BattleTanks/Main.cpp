#include "../Minigin/GameEngine.h"


using namespace Engine;

void load()
{

}

int main(int, char* []) {
	GameEngine engine("../Data/");
	engine.Run(load);
	return 0;
}