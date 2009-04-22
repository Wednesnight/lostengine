#ifndef CUBEEFFECT_H
#define CUBEEFFECT_H

#include "Effect.h"
#include <GLUT/glut.h>
#include "../lib/generator.h"


struct CubeEffect : public Effect
{
    lost::shared_ptr<generator> anglegen;
    lost::shared_ptr<generator> xgen;
    lost::shared_ptr<generator> ygen;
    lost::shared_ptr<generator> rgen;
    lost::shared_ptr<generator> ggen;
    lost::shared_ptr<generator> bgen;
//    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};

    float globalx;
    float globaly;

    float circletime;                    

    CubeEffect(RenderEnvironment& renv)
    : Effect(renv)
    {
        anglegen.reset(new sinegen(lost::shared_ptr<generator>(new constgen(0)),
                                   lost::shared_ptr<generator>(new constgen(100)),
                                   lost::shared_ptr<generator>(new constgen(2000))));
                            
        circletime = 7000;                    
                                   
        xgen.reset(new sinegen(lost::shared_ptr<generator>(new constgen(0)),
                                   lost::shared_ptr<generator>(new constgen(5)),
                                   lost::shared_ptr<generator>(new constgen(circletime))));
        ygen.reset(new cosgen(lost::shared_ptr<generator>(new constgen(0)),
                                   lost::shared_ptr<generator>(new constgen(5)),
                                   lost::shared_ptr<generator>(new constgen(circletime*1.0f))));

        rgen.reset(new sinegen(lost::shared_ptr<generator>(new constgen(.5)),
                                   lost::shared_ptr<generator>(new constgen(.5)),
                                   lost::shared_ptr<generator>(new constgen(3000))));

        ggen.reset(new cosgen(lost::shared_ptr<generator>(new constgen(.7)),
                                   lost::shared_ptr<generator>(new constgen(.2)),
                                   lost::shared_ptr<generator>(new constgen(5000))));

        bgen.reset(new cosgen(lost::shared_ptr<generator>(new constgen(.1)),
                                   lost::shared_ptr<generator>(new constgen(2)),
                                   lost::shared_ptr<generator>(new constgen(7000))));

        globalx;
        globaly;
    }
    
    void draw(Uint32 delta)
    {
        GLdouble eyeX = 0;
        GLdouble eyeY = 0;
        GLdouble eyeZ = -10;

        GLdouble centerX = 0;
        GLdouble centerY = 0;
        GLdouble centerZ = 0;

        GLdouble upX = 0;
        GLdouble upY = 1;
        GLdouble upZ = 0;
    
        set3DProjection(eyeX, eyeY, eyeZ,
                        centerX, centerY, centerZ,
                        upX, upY, upZ);

        GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat mat_shininess[] = {50.0};
        GLfloat light_position[] = {0.0, 1.0, -1.0, 0.0};
        GLfloat white_light[] = {1.0, 1.0, 1.0, 0.0};
        GLfloat lmodel_ambient[] = {0.1, 0.1, 0.1, 0.0};

        glEnable(GL_DEPTH_TEST);
        glShadeModel(GL_SMOOTH);

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
        glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);

        anglegen->tick(delta);
        xgen->tick(delta);
        ygen->tick(delta);
        rgen->tick(delta);
        ggen->tick(delta);
        bgen->tick(delta);

        float anglespeed = 80;
        float cubestepspeed = circletime / 20;
        for(float i = 0; i<20; i+=1)
        {
            glColorMaterial(GL_FRONT, GL_DIFFUSE);
            float curr = rgen->tick(delta);rgen->rewind();
            float curg = ggen->tick(delta);ggen->rewind();
            float curb = bgen->tick(delta);bgen->rewind();
            glColor3f(curr, curg, curb);
            float localaangle = anglegen->tick(anglespeed*i);
            anglegen->rewind();
            float curx = xgen->tick(cubestepspeed*i);xgen->rewind();
            float cury = ygen->tick(cubestepspeed*i);ygen->rewind();
            drawCube(curx, cury, localaangle, localaangle);            
        }


        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
    }
    
    void drawCube(float posx, float posy, float xangle, float yangle)
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(posx, posy, 0.0);
        glRotatef(xangle, 1,0,0);
        glRotatef(yangle, 0,1,0);
        glutSolidCube(.8); //, 16, 16);    
        glPopMatrix();
    }
};

#endif
