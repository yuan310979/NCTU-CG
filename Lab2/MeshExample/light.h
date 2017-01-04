class lightning{
public:
	float x[8], y[8], z[8];
	float ambient[8][3];
	float diffuse[8][3];
	float specular[8][3];

	int counter;
	float environment_ambient[3];

	lightning(const char* light_file);
};