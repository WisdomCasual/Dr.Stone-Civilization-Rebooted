#include "State.h"


void State::initial_textures(string file)
{
	//loads the current state's texture files
	string tex;
	ifstream ifs("textures/"+ file +"/texturenames.ini");
	if (ifs.is_open()) {
		while (!ifs.eof()) {
			getline(ifs, tex);
			textures.push_back(new Texture);
			textures.back()->loadFromFile("textures/" + file + "/" + tex + ".png");
		}
	}
	ifs.close();
}

Vector2f State::delta_movement()
{
	// returns direction of movement based on arrow/wasd keys --> {[-1,1],[-1,1]}
	Vector2f velocity{ 0,0 };
	if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
		velocity.x++;
	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
		velocity.x--;
	if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
		velocity.y--;
	if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
		velocity.y++;
	return velocity;
}

State::State()
{

}

State::~State()
{
	//destructor for:
	//textures map
	for (auto tex : textures) {
		delete tex;
	}
	textures.clear();
}
