#include "scene.h"

#include <GL/glut.h>

#include <obj/load.h>
#include <obj/draw.h>
#define RADIUS 20
#define SKYBOXSIZE 500

typedef GLubyte Pixel[3];

Pixel* images[4];

void set_textures(Scene* sc)
{
	int i;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	char texture_filenames[][32] = {
        "models/textures/moon.png",
        "models/textures/ground.jpg",
		"models/textures/mars.png",
		"models/textures/skybox.png"
    };

	for (i = 0; i < 4; ++i) {
        sc->texture_id[i] = load_texture(texture_filenames[i], images[i]);
    }

	/*glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL); NEM KELL IDE MERT NEM LESZ FÉNY*/

	glEnable(GL_TEXTURE_2D);
}

void init_scene(Scene* scene)
{
	
	glBindTexture(GL_TEXTURE_2D, scene->texture_id[0]);

    load_model(&(scene->cube), "models/moon.obj");
	
	glBindTexture(GL_TEXTURE_2D, scene->texture_id[2]);
	load_model(&(scene->mars), "models/mars.obj");

    scene->material.ambient.red = 0.5f;
    scene->material.ambient.green = 0.5f;
    scene->material.ambient.blue = 0.5f;

    scene->material.diffuse.red = 0.5f;
    scene->material.diffuse.green = 0.5f;
    scene->material.diffuse.blue = 0.5f;

    scene->material.specular.red = 0.5f;
    scene->material.specular.green = 0.5f;
    scene->material.specular.blue = 0.5f;

    scene->material.shininess = 10.0f;
	
}
void init_lightdata(Ldata* ldata)
{
	ldata->r = 0.0;
	ldata->g = 0.0;
	ldata->b = 0.0;
	ldata->f = 10.0;
	
}

void changeLdata(Ldata* ldata,float intensity)
{
	const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	
	if(ldata->r > 1 && intensity > 0)
	{
		printf("\nLight intensity reached the maximum");
	}
	else if(ldata->r <= 0 && intensity < 0)
	{
		printf("\nLight intensity reached the minimum %f",ldata->r);
	}
	else
	{
		ldata->r += intensity;
		ldata->g += intensity;
		ldata->b += intensity;
		printf("\nLight changed with %f",intensity);
		
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHTING);
	
		float ambient_light[] = { ldata->r, ldata->g, ldata->b, ldata->f };
		float diffuse_light[] = { ldata->r, ldata->g, ldata->b, ldata->f };
		float specular_light[] = { ldata->r, ldata->g, ldata->b, ldata->f };
    
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light);
		glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light);
	}
	printf("\nR: %f\nG: %f\nB: %f",ldata->r,ldata->g,ldata->b);
}

void changeLightrotation(Ldata* ldata,float h)
{
	ldata->anglex = (ldata->anglex + h);
	ldata->angley = (ldata->anglex + h);
	float position[] = {cos(ldata->anglex)*180/M_PI, ldata->anglez,sin(ldata->angley)*180/M_PI, 1.0f };
		
	printf("\nLightpos x: %f y: %f z %f\nLightangle x:%f\nLightangle y:%f",position[0],position[1],position[2],cos(ldata->anglex)*180/M_PI,sin(ldata->angley)*180/M_PI);
	
	glLightfv(GL_LIGHT1, GL_POSITION, position);
}

void set_lighting(Ldata* ldata)
{
	glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
	
    float ambient_light[] = { ldata->r, ldata->g, ldata->b, ldata->f };
    float diffuse_light[] = { ldata->r, ldata->g, ldata->b, ldata->f };
    float specular_light[] = { ldata->r, ldata->g, ldata->b, ldata->f };
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void draw_scene(const Scene* scene,Ldata* ldata)
{
    set_material(&(scene->material));
	set_lighting(&ldata);
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	
	const double moonrotatespeed = t*25.0;
	const double marsrotatespeed = t*10.0;
	
    const double x = t*0.01;
	const double y = t*0.01;
	const double z = t*5.0;
	
	/*glBegin(GL_LINES);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);*/

    glEnd();
	
	glBindTexture(GL_TEXTURE_2D, scene->texture_id[0]);
	glPushMatrix();
		glRotatef(moonrotatespeed,0,0,1);
		glScalef(0.4,0.4,0.4);
		glTranslatef((RADIUS/2)*cos(x),RADIUS*sin(y),0);
		draw_model(&(scene->cube));
	glPopMatrix();
	
	
	glBindTexture(GL_TEXTURE_2D, scene->texture_id[2]);
	
	glPushMatrix();
		glRotatef(marsrotatespeed,0,0,1);
		glScalef(1.2,1.2,1.2);
		draw_model(&(scene->mars));
	glPopMatrix();
	
	glBindTexture(GL_TEXTURE_2D, scene->texture_id[3]);
	
	
	/*Up*/
	glBegin(GL_QUADS);

	glTexCoord2f(0.25, 1.0); glVertex3f(-SKYBOXSIZE,SKYBOXSIZE, SKYBOXSIZE);
	glTexCoord2f(0.5, 1.0); glVertex3f(SKYBOXSIZE,SKYBOXSIZE, SKYBOXSIZE);
	glTexCoord2f(0.5, 2.0/3); glVertex3f(SKYBOXSIZE,-SKYBOXSIZE, SKYBOXSIZE);
	glTexCoord2f(0.25, 2.0/3); glVertex3f(-SKYBOXSIZE,-SKYBOXSIZE, SKYBOXSIZE);

    glEnd();
	
	/*Down*/
	glBegin(GL_QUADS);

	glTexCoord2f(0.25, 1.0/3); glVertex3f(-SKYBOXSIZE,SKYBOXSIZE, -SKYBOXSIZE);
	glTexCoord2f(0.5, 1.0/3); glVertex3f(SKYBOXSIZE,SKYBOXSIZE, -SKYBOXSIZE);
	glTexCoord2f(0.5, 0.0); glVertex3f(SKYBOXSIZE,-SKYBOXSIZE, -SKYBOXSIZE);
	glTexCoord2f(0.25, 0.0); glVertex3f(-SKYBOXSIZE,-SKYBOXSIZE, -SKYBOXSIZE);

    glEnd();
	
	/*Back*/
	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 2.0/3); glVertex3f(-SKYBOXSIZE,-SKYBOXSIZE, SKYBOXSIZE);
	glTexCoord2f(0.25, 2.0/3); glVertex3f(-SKYBOXSIZE,SKYBOXSIZE,SKYBOXSIZE);
	glTexCoord2f(0.25, 1.0/3); glVertex3f(-SKYBOXSIZE, SKYBOXSIZE, -SKYBOXSIZE);
	glTexCoord2f(0.0, 1.0/3); glVertex3f(-SKYBOXSIZE,-SKYBOXSIZE, -SKYBOXSIZE);

    glEnd();
	
	/*Front*/
	glBegin(GL_QUADS);

	glTexCoord2f(0.5, 2.0/3); glVertex3f(SKYBOXSIZE,-SKYBOXSIZE, SKYBOXSIZE);
	glTexCoord2f(0.75, 2.0/3); glVertex3f(SKYBOXSIZE,SKYBOXSIZE,SKYBOXSIZE);
	glTexCoord2f(0.75, 1.0/3); glVertex3f(SKYBOXSIZE, SKYBOXSIZE, -SKYBOXSIZE);
	glTexCoord2f(0.5, 1.0/3); glVertex3f(SKYBOXSIZE,-SKYBOXSIZE, -SKYBOXSIZE);

    glEnd();
	
	/*Left*/
	glBegin(GL_QUADS);

	glTexCoord2f(0.25, 2.0/3); glVertex3f(-SKYBOXSIZE,-SKYBOXSIZE, SKYBOXSIZE);
	glTexCoord2f(0.5, 2.0/3); glVertex3f(SKYBOXSIZE,-SKYBOXSIZE,SKYBOXSIZE);
	glTexCoord2f(0.5, 1.0/3); glVertex3f(SKYBOXSIZE, -SKYBOXSIZE, -SKYBOXSIZE);
	glTexCoord2f(0.25, 1.0/3); glVertex3f(-SKYBOXSIZE,-SKYBOXSIZE, -SKYBOXSIZE);

    glEnd();
	
	/*Right*/
	glBegin(GL_QUADS);

	glTexCoord2f(0.75, 2.0/3); glVertex3f(-SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	glTexCoord2f(1.0, 2.0/3); glVertex3f(SKYBOXSIZE, SKYBOXSIZE,SKYBOXSIZE);
	glTexCoord2f(1.0, 1.0/3); glVertex3f(SKYBOXSIZE, SKYBOXSIZE, -SKYBOXSIZE);
	glTexCoord2f(0.75, 1.0/3); glVertex3f(-SKYBOXSIZE, SKYBOXSIZE, -SKYBOXSIZE);

    glEnd();
}



void draw_skybox(const Scene* scene)
{
	
	
}