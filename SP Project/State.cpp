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

void State::initial_tile_sheets(string file)
{
	//loads texture sheets
	string tex;
	int num = 0, x_size, y_size;
	ifstream ifs("textures/" + file + "/texturenames.ini");
	if (ifs.is_open()) {
		while (!ifs.eof()) {
			getline(ifs, tex);
			tile_sheets.push_back(new Texture);
			tile_sheets.back()->loadFromFile("textures/" + file + "/" + tex + ".png");

			x_size = ceil(tile_sheets.back()->getSize().x / 16.0);
			y_size = ceil(tile_sheets.back()->getSize().y / 16.0);

			tile_props[num].x_size = x_size;
			tile_props[num].y_size = y_size;

			tile_props[num].properties = new tile_properties* [x_size];
			for (int i = 0; i < x_size; i++)
				tile_props[num].properties[i] = new tile_properties [y_size];

			num++;
		}
	}
	ifs.close();
	sheets_no = num;
	//loads tile properties
	for (int sheet = 0; sheet < sheets_no; sheet++) {
		ifs.open("Maps/" + file + "properties/sheet "+ to_string(sheet) + ".prop");
		string test;
		short priority, props;
		if (!(ifs >> test))
			continue;
		ifs.seekg(ios::beg);
		if (ifs.is_open()) {
			for (int i = 0; i < x_size; i++)
				for (int j = 0; j < y_size; j++) {
					ifs >> priority;
					tile_props[sheet].properties[i][j] = { priority };
				}
		}
		ifs.close();
	}
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

	//tile props
	for (int sheet = 0; sheet < sheets_no; sheet++){
		for (int i = 0; i < tile_props[sheet].x_size; i++)
				delete[] tile_props[sheet].properties[i];
		delete[] tile_props[sheet].properties;
	}
}
