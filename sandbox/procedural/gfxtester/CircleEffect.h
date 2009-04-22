#ifndef CIRCLEEFFECT_H
#define CIRCLEEFFECT_H

#include "Effect.h"
#include <iostream>

struct CircleEffect : public Effect
{
    // coords and size of current point
    float x;
    float y;
    float pointsize;

    // future dots values;
    unsigned long numsteps;
    unsigned long scaling;
        
    // generator instances
    lost::shared_ptr<sinegen> xg;
    lost::shared_ptr<cosgen> yg;
    lost::shared_ptr<sinegen> psg;
    lost::shared_ptr<sinegen> wg;

    // screen pos of whole animation
    unsigned long posx;
    unsigned long posy;

    CircleEffect(RenderEnvironment& renv)
    : Effect(renv)
    {
        float xtime = 3000;
        float ytime = xtime * 1.f;

        wg.reset(new sinegen(lost::shared_ptr<generator>(new constgen(100)),
                             lost::shared_ptr<generator>(new constgen(50)),
                             lost::shared_ptr<generator>(new constgen(xtime*1.0))));

                
        xg.reset(new sinegen(lost::shared_ptr<generator>(new constgen(0)),
                             lost::shared_ptr<generator>(new constgen(100)),
                             lost::shared_ptr<generator>(new constgen(xtime))));
        yg.reset(new cosgen(lost::shared_ptr<generator>(new constgen(0)),
                            lost::shared_ptr<generator>(new constgen(100)),
                            lost::shared_ptr<generator>(new constgen(ytime*.9))));
        psg.reset(new sinegen(lost::shared_ptr<generator>(new constgen(8)), 
                              lost::shared_ptr<generator>(new constgen(8)),
                              lost::shared_ptr<generator>(new constgen(1700))));


        numsteps = 40;
        scaling = 100;
        
        posx = 200;
        posy = 200;
    }
    
    void tick(Uint32 deltaTick)
    {
        x = xg->tick(deltaTick);
        y = yg->tick(deltaTick);
        pointsize = psg->tick(deltaTick);    
    }
    
    void rewind()
    {
        xg->rewind();
        yg->rewind();
        psg->rewind();        
    }
    
    void draw(Uint32 deltaTick)
    {
        set2DProjection();
        
        glColor3f(1.0f, 1.0f, 1.0f);
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
        tick(deltaTick);
        for(unsigned long i=0; i<numsteps; ++i)
        {
            tick(i*scaling);
            rewind();
            glPointSize(pointsize);
            glBegin(GL_POINTS);
                glVertex2f(posx+x, posy+y);
            glEnd();
        }        
    }
};

#endif
