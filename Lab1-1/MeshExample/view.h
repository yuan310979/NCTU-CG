#include <iostream>

class view{
public:
	float eye[3], vat[3], vup[3], fovy, dnear, dfar, viewport[4];
	view(const char* view_file);

};
