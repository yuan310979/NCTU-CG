#include <GL/glew.h>
#include "mesh.h"
#include "glut.h"
#include "view.h"
#include "scene.h"
#include "light.h"
#include "texture.h"

#include "FreeImage.h"

mesh *object;
view *view_setting;

scene *s;
lightning *l;
texture *t[100];

int windowSize[2];
float zoom, angle;
int select;
int initial_x, initial_y;
int now_x, now_y;
int move[2];
float transmittance = 0.0;
float reflectance = 0.0;

void light();
void display(); 
void reshape(GLsizei , GLsizei );
void keyboard(unsigned char, int, int);
void click(int, int, int, int);
void mouse(int, int);
void LoadTexture();
void draw(int);
void draw_reverse(int);

int main(int argc, char** argv)
{
	zoom = 0;
	angle = 0;

	view_setting = new view("CornellBox.view");
	s = new scene("CornellBox.scene");
	l = new lightning("CornellBox.light");

	glutInit(&argc, argv);
	glutInitWindowSize(view_setting->viewport[2], view_setting->viewport[3]);
	glutInitWindowPosition(view_setting->viewport[0], view_setting->viewport[1]);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ACCUM);
	glutCreateWindow("Mesh Example");
	auto result = glewInit();
	if (result != GLEW_OK){
		std::cerr << glewGetErrorString(result);
		std::exit(0);
	}
	//glewExperimental = GL_TRUE;
	LoadTexture();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(click);
	glutMotionFunc(mouse);
	glutMainLoop();

	return 0;
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

void mouse(int x, int y){
	move[0] = x - initial_x;
	move[1] = y - initial_y;
	if (select != -1){
		s->transfer[select][0] += move[0] / 75.0;
		s->transfer[select][1] += move[1] / 75.0;
	}
	glutPostRedisplay();
}

void click(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN){
			initial_x = x;
			initial_y = y;
		}
}

void keyboard(unsigned char key, int x, int y){
	/*float normalize = sqrt((view_setting->vat[0] - view_setting->eye[0])*(view_setting->vat[0] - view_setting->eye[0]) + 
							(view_setting->vat[1] - view_setting->eye[1])*(view_setting->vat[1] - view_setting->eye[1]) + 
							(view_setting->vat[2] - view_setting->eye[2])*(view_setting->vat[2] - view_setting->eye[2]));
	float dir[3] = { (view_setting->vat[0] - view_setting->eye[0]) / normalize,
		(view_setting->vat[1] - view_setting->eye[1]) / normalize,
		(view_setting->vat[2] - view_setting->eye[2]) / normalize };*/

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
		angle += 10;
		break;
	case 'd':
		printf("Turn right\n");
		angle -= 10;
		break;
	case 'r':
		if (reflectance < 1.0)
			reflectance += 0.1;
		else
			reflectance = 1.0;
		printf("reflectance: %f\n", reflectance);
		break;
	case 'f':
		if (reflectance > 0.0)
			reflectance -= 0.1;
		else
			reflectance = 0.0;
		printf("reflectance: %f\n", reflectance);
		break;
	case 't':
		if (transmittance < 1.0)
			transmittance += 0.1;
		else
			transmittance = 1.0;
		printf("transmittance: %f\n", transmittance);
		break;
	case 'g':
		if (transmittance > 0.0)
			transmittance -= 0.1;
		else
			transmittance = 0.0;
		printf("transmittance: %f\n", transmittance);
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
		/*printf("Counter: %d\n", l->counter);
		printf("%f %f %f\n ", l->x, l->y, l->z);
		printf("%f %f %f\n ", l->	[0], l->ambient[1], l->ambient[2]);
		printf("%f %f %f\n ", l->diffuse[0], l->diffuse[1], l->diffuse[2]);
		printf("%f %f %f\n ", l->specular[0], l->specular[1], l->specular[2]);
		printf("%f %f %f")*/
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

void draw_reverse(int m)
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
		glTranslatef(s->transfer[m][0] + (-20 - s->transfer[m][0])*2, 0, 0);
		glScalef(-1.0, 1.0, 1.0);
		glTranslatef(-(s->transfer[m][0]), 0, 0);
		glTranslatef(s->transfer[m][0], s->transfer[m][1], s->transfer[m][2]);
		glRotatef(s->angle[m], s->rotate[m][0], s->rotate[m][1], s->rotate[m][2]);
		//glRotatef(180, s->rotate[m][0], s->rotate[m][1], s->rotate[m][2]);
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
			//s->object[m]->vList[s->object[m]->faceList[i][j].v].ptr[0] += ((-20 - s->object[m]->vList[s->object[m]->faceList[i][j].v].ptr[0]) * 2);
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
	/*glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);*/
	glEnable(GL_STENCIL_TEST);
	glClearStencil(0);
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);//這行把畫面清成黑色並且清除z buffer 並清除stencil buffer
	glClear(GL_ACCUM_BUFFER_BIT);

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
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	draw(3);

	glFrontFace(GL_CCW);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_FALSE);
	glStencilFunc(GL_NOTEQUAL, 0, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	draw(2);

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	draw(3);
	glAccum(GL_ACCUM, transmittance);

	glFrontFace(GL_CW);
	glClear(GL_COLOR_BUFFER_BIT);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	draw_reverse(0);
	draw_reverse(1);
	glAccum(GL_ACCUM, reflectance);
	
	glAccum(GL_RETURN, 1.0);

	glFrontFace(GL_CCW);
	glStencilFunc(GL_ALWAYS, 0, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	draw(0);
	draw(1);

	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
	windowSize[0] = w;
	windowSize[1] = h;
}

