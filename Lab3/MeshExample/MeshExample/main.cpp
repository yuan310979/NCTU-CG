#include <GL/glew.h>
#include "mesh.h"
#include "glut.h"
#include "view.h"
#include "scene.h"
#include "light.h"
#include "texture.h"

#include "FreeImage.h"
#include <windows.h>
#include "ShaderLoader.h"
#include <string>

mesh *object;
view *view_setting;

scene *s;
lightning *l;
texture *t[100];

int windowSize[2];
float zoom, angle;

GLhandleARB	PhongShadingShader;
GLhandleARB	HairSimulationShader;
float length = 0.5;
float gravity[] = {0.0f, -0.1f, 0.0f, 0.0f};
int segment_num = 15;

void light();
void display(); 
void reshape(GLsizei , GLsizei );
void keyboard(unsigned char, int, int);
void LoadTexture();
void LoadShader();
void draw(int);

int main(int argc, char** argv)
{
	zoom = 0;
	angle = 0;

	view_setting = new view("../Peter/Peter.view");
	s = new scene("../Peter/Peter.scene");
	l = new lightning("../Peter/Peter.light");

	glutInit(&argc, argv);
	glutInitWindowSize(view_setting->viewport[2], view_setting->viewport[3]);
	glutInitWindowPosition(view_setting->viewport[0], view_setting->viewport[1]);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("HairSimulator");
	auto result = glewInit();
	if (result != GLEW_OK){
		std::cerr << glewGetErrorString(result);
		std::exit(0);
	}
	//glewExperimental = GL_TRUE;
	LoadTexture();
	LoadShader();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}

void LoadShader(){
	PhongShadingShader = glCreateProgram();
	if (PhongShadingShader != 0){
		ShaderLoad(PhongShadingShader, "../MeshExample/PhongShading.vert", GL_VERTEX_SHADER);
		ShaderLoad(PhongShadingShader, "../MeshExample/PhongShading.frag", GL_FRAGMENT_SHADER);
	}
	HairSimulationShader = glCreateProgram();
	if (HairSimulationShader != 0){
		ShaderLoad(HairSimulationShader, "../MeshExample/HairSimulation.vert", GL_VERTEX_SHADER);
		ShaderLoad(HairSimulationShader, "../MeshExample/HairSimulation.geom", GL_GEOMETRY_SHADER);
		ShaderLoad(HairSimulationShader, "../MeshExample/HairSimulation.frag", GL_FRAGMENT_SHADER);
	}
}

void LoadTexture(){
	for (int i = 0; i < s->tex_counter; i++){
		if (s->tex_type[i] == 1 || s->tex_type[i] == 2){
			t[i] = new texture();
			t[i]->LoadTexture(s->tex_name[i][0]);
			printf("Load %s\n", s->tex_name[i]);
		}
		else if (s->tex_type[i] == 3){ //cube load
			t[i] = new texture();
			t[i]->LoadEnvTexture(s->tex_name[i][0], s->tex_name[i][1], s->tex_name[i][2], s->tex_name[i][3], s->tex_name[i][4], s->tex_name[i][5]);
			printf("Load CUBE %s\n", s->tex_name[i][4]);
		}
	}
}

void keyboard(unsigned char key, int x, int y){
	switch (key){
	//why zoom value not changed
	case 'w':
		printf("Zoom in\n");
		view_setting->eye[0] += (view_setting->vat[0] - view_setting->eye[0]) / 10.0;
		view_setting->eye[1] += (view_setting->vat[1] - view_setting->eye[1]) / 10.0;
		view_setting->eye[2] += (view_setting->vat[2] - view_setting->eye[2]) / 10.0;
		break;
		//printf("Zoom innnnn %d\n", zoom);
	case 's':
		printf("Zoom out\n");
		view_setting->eye[0] -= (view_setting->vat[0] - view_setting->eye[0]) / 10.0;
		view_setting->eye[1] -= (view_setting->vat[1] - view_setting->eye[1]) / 10.0;
		view_setting->eye[2] -= (view_setting->vat[2] - view_setting->eye[2]) / 10.0;
		break;
	case 'a':
		printf("Turn left\n");
		angle -= 30;
		break;
	case 'd':
		printf("Turn right\n");
		angle += 30;
		break;
	case 'r':
		printf("Increase length\n");
		length += 0.1;
		break;
	case 'f':
		printf("Decrease length\n");
		length -= 0.1;
		break;
	case 't':
		printf("Increase segment number\n");
		segment_num += 1;
		break;
	case 'g':
		printf("Decrease segment number\n");
		segment_num -= 1;
		break;
	case 'y':
		printf("Increase the y of gravity vector\n");
		gravity[1] += 0.1;
		break;
	case 'h':
		printf("Decrease the y of gravity vector\n");
		gravity[1] -= 0.1;
		break;
	}
	glutPostRedisplay();
}

void light()
{
	glShadeModel(GL_SMOOTH);

	// z buffer enable
	glEnable(GL_DEPTH_TEST);

	// enable lighting
	glEnable(GL_LIGHTING);

	for (int i = 0; i < l->counter; i++){
		GLfloat light_specular[] = { l->specular[i][0], l->specular[i][1], l->specular[i][2], 1.0 };
		GLfloat light_diffuse[] = { l->diffuse[i][0], l->diffuse[i][1], l->diffuse[i][2], 1.0 };
		GLfloat light_ambient[] = { l->ambient[i][0], l->ambient[i][1], l->ambient[i][2], 1.0 };
		GLfloat light_position[] = { l->x[i], l->y[i], l->z[i], 1.0 };

		glEnable(GL_LIGHT0 + i);
		glLightfv(GL_LIGHT0 + i, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0 + i, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0 + i, GL_AMBIENT, light_ambient);
	}
	glEnable(GL_LIGHT_MODEL_AMBIENT);
	GLfloat ambient[] = { l->environment_ambient[0], l->environment_ambient[1], l->environment_ambient[2]};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	
}

void draw(int m)
{
	int lastMaterial = -1;
	for (size_t i = 0; i < s->object[m]->fTotal; ++i)
	{
		glPushMatrix();

		// set material property if this face used different material
		if (lastMaterial != s->object[m]->faceList[i].m)
		{
			lastMaterial = (int)s->object[m]->faceList[i].m;
			glMaterialfv(GL_FRONT, GL_AMBIENT, s->object[m]->mList[lastMaterial].Ka);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, s->object[m]->mList[lastMaterial].Kd);
			glMaterialfv(GL_FRONT, GL_SPECULAR, s->object[m]->mList[lastMaterial].Ks);
			glMaterialfv(GL_FRONT, GL_SHININESS, &s->object[m]->mList[lastMaterial].Ns);

			//you can obtain the texture name by object->mList[lastMaterial].map_Kd
			//load them once in the main function before mainloop
			//bind them in display function here
		}
		glRotatef(angle, s->rotate[m][0], s->rotate[m][1], s->rotate[m][2]);
		glTranslatef(s->transfer[m][0], s->transfer[m][1], s->transfer[m][2]);
		glRotatef(s->angle[m], s->rotate[m][0], s->rotate[m][1], s->rotate[m][2]);
		glScalef(s->scale[m][0], s->scale[m][1], s->scale[m][2]);

		// do texture mapping
		if (s->obj_tex_type[m] == 0){
			//do nothing
		}
		else if (s->obj_tex_type[m] == 1){
			//single texture
			glEnable(GL_TEXTURE_2D);
			t[s->obj_tex_index[m]]->Bind();
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		}
		else if (s->obj_tex_type[m] == 2){
			//multi texture
			//bind texture 0
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			t[s->obj_tex_index[m] - 1]->Bind();
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
			//bind texture 1
			glActiveTexture(GL_TEXTURE1);
			glEnable(GL_TEXTURE_2D);
			t[s->obj_tex_index[m]]->Bind();
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
		}
		else if (s->obj_tex_type[m] == 3){
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_GEN_R);
			glEnable(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, t[s->obj_tex_index[m]]->texObject[0]);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		}

		glBegin(GL_TRIANGLES);
		for (size_t j = 0; j < 3; ++j)
		{
			if (s->obj_tex_type[m] == 1){
				float* point = s->object[m]->tList[s->object[m]->faceList[i][j].t].ptr;
				glTexCoord2f(point[0], point[1]);
			}
			else if (s->obj_tex_type[m] == 2){
				float* point = s->object[m]->tList[s->object[m]->faceList[i][j].t].ptr;
				glMultiTexCoord2f(GL_TEXTURE0, point[0], point[1]);
				glMultiTexCoord2f(GL_TEXTURE1, point[0], point[1]);
			}
			else if (s->obj_tex_type[m] == 3){
				float* point = s->object[m]->tList[s->object[m]->faceList[i][j].t].ptr;
				glTexCoord2f(point[0], point[1]);
			}
			glNormal3fv(s->object[m]->nList[s->object[m]->faceList[i][j].n].ptr);
			glVertex3fv(s->object[m]->vList[s->object[m]->faceList[i][j].v].ptr);
		}
		glEnd();
		glPopMatrix();

		if (s->obj_tex_type[m] == 1) //single
		{
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else if (s->obj_tex_type[m] == 2) //multi
		{
			//unbind texture 1
			glActiveTexture(GL_TEXTURE1);
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			//unbind texture 0
			glActiveTexture(GL_TEXTURE0);
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else if (s->obj_tex_type[m] == 3) //cube
		{
			glDisable(GL_TEXTURE_CUBE_MAP);
			glDisable(GL_TEXTURE_GEN_R);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_GEN_S);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
	}
}

void display()
{
	// clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);      //清除用color
	glClearDepth(1.0f);                        // Depth Buffer (就是z buffer) Setup
	glEnable(GL_DEPTH_TEST);                   // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                    // The Type Of Depth Test To Do
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//這行把畫面清成黑色並且清除z buffer

	// viewport transformation
	glViewport(view_setting->viewport[0], view_setting->viewport[1], view_setting->viewport[2], view_setting->viewport[3]);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(view_setting->fovy, (GLfloat)windowSize[0] / (GLfloat)windowSize[1], view_setting->dnear, view_setting->dfar);
	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(view_setting->eye[0], view_setting->eye[1], view_setting->eye[2] + zoom,// eye
		view_setting->vat[0], view_setting->vat[1], view_setting->vat[2],     // center
		view_setting->vup[0], view_setting->vup[1], view_setting->vup[2]);    // up


	//注意light位置的設定，要在gluLookAt之後
	light();
	glUseProgram(PhongShadingShader);
	glUniform1i(glGetUniformLocation(PhongShadingShader, "light_num"), l->counter);
	glUniform1i(glGetUniformLocation(PhongShadingShader, "colorTexture"), 0);
	draw(0);

	glDepthMask(GL_FALSE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(HairSimulationShader);
	//glUniform1i(glGetUniformLocation(HairSimulationShader, "seg_num"), segment_num);
	glUniform1f(glGetUniformLocation(HairSimulationShader, "length"), length);
	glUniform4f(glGetUniformLocation(HairSimulationShader, "gravity"), gravity[0], gravity[1], gravity[2], gravity[3]);
	
	glUniform1i(glGetUniformLocation(HairSimulationShader, "seg_num"), segment_num);
	draw(1);
	/*
	float tmp = 0.0f;
	for (int i = 0; i < segment_num; i++){
		glUniform1i(glGetUniformLocation(HairSimulationShader, "num"), i);
		glUniform1f(glGetUniformLocation(HairSimulationShader, "color"), tmp);
		draw(1);
		printf("%f\n", tmp);
		tmp += 1.0f / (float)(segment_num - 1);
	}*/
	
	glDepthMask(GL_TRUE);
	glUseProgram(0);
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
	windowSize[0] = w;
	windowSize[1] = h;
}

