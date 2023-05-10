#include "State.h"


void State::initial_textures(string file)
{
	//loads the current state's texture files
	string tex;
	ifstream ifs("textures/"+ file +"/texturenames.ini");
	if (ifs.is_open()) {
		Texture* temp_textures[20];
		int textures_count = 0;
		while (!ifs.eof()) {
			getline(ifs, tex);
			temp_textures[textures_count] = new Texture;
			temp_textures[textures_count]->loadFromFile("textures/" + file + "/" + tex + ".png");
			textures_count++;
		}
		textures_no = textures_count;
		textures = new Texture* [textures_count];
		for (int i = 0; i < textures_count; i++)
			textures[i] = temp_textures[i];
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
		Texture* temp_textures[20];

		while (!ifs.eof()) {
			getline(ifs, tex);
			
			temp_textures[num] =  new Texture;
			temp_textures[num]->loadFromFile("textures/" + file + "/" + tex + ".png");

			x_size = ceil(temp_textures[num]->getSize().x / 16.0);
			y_size = ceil(temp_textures[num]->getSize().y / 16.0);

			tile_props[num].x_size = x_size;
			tile_props[num].y_size = y_size;

			tile_props[num].properties = new tile_properties* [x_size];
			for (int i = 0; i < x_size; i++)
				tile_props[num].properties[i] = new tile_properties [y_size];

			num++;
		}
		tile_sheets_no = num;
		tile_sheets = new Texture* [num];
		tile_sheets_img = new Image[num];
		for (int i = 0; i < num; i++) {
			tile_sheets[i] = temp_textures[i];
			tile_sheets_img[i] = tile_sheets[i]->copyToImage();
		}
	}	

	ifs.close();
	sheets_no = num;
	//loads tile properties
	for (int sheet = 0; sheet < sheets_no; sheet++) {
		ifs.open("textures/" + file + "/properties/sheet " + to_string(sheet) + ".prop");
		if (ifs.is_open()) {
			string test;
			short props;
			if (!(ifs >> test))
				continue;
			ifs.seekg(ios::beg);
			if (ifs.is_open()) {
				for (int i = 0; i < tile_props[sheet].x_size; i++)
					for (int j = 0; j < tile_props[sheet].y_size; j++) {
						ifs >> props;
						tile_props[sheet].properties[i][j] = { props };
						if (props & 32)
							ifs >> tile_props[sheet].properties[i][j].object_type >> tile_props[sheet].properties[i][j].tool_type;
					}
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
	if (0 != velocity.x && 0 != velocity.y)
		velocity.x *= 0.707, velocity.y *= 0.707;
	return velocity;
}

State::State()
{

}

State::~State()
{
	//destructor for:
	//textures map

	for (int i = 0; i < textures_no ; i++)
		delete textures[i];

	if (textures != nullptr)
		delete[] textures;

	for (int i = 0; i < tile_sheets_no; i++)
		delete tile_sheets[i];

	if (tile_sheets != nullptr)
		delete[] tile_sheets;

	if (tile_sheets_img != nullptr)
		delete[] tile_sheets_img;

	//tile props
	for (int sheet = 0; sheet < sheets_no; sheet++){
		for (int i = 0; i < tile_props[sheet].x_size; i++)
				delete[] tile_props[sheet].properties[i];
		delete[] tile_props[sheet].properties;
	}
}
