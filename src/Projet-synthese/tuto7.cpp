
//! \file tuto7.cpp reprise de tuto6.cpp mais en derivant App::init(), App::quit() et bien sur App::render().

#include "wavefront.h"
#include "texture.h"

#include "orbiter.h"
#include "draw.h"        
#include "app.h"  
#include "CharacterController.h"
#include "audio.h" 
#include "glcore.h"
#include "shader.h"
#include "Objet.h"



class TP : public App
{
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    TP( ) : App(1024, 640) {}
    
    // creation des objets de l'application
    int init( )
    {
        m_objet.mesh= read_mesh("data/cube.obj");
        m_terrain.mesh= read_mesh("data/projet/mesh/sale2.obj");
        m_terrain2.mesh= read_mesh("data/projet/mesh/terrain2.obj");
        lit.mesh= read_mesh("data/projet/mesh/litdéformé.obj");
        cheval.mesh=read_mesh("data/projet/mesh/chevalabasculedéformé.obj");
        coussin1.mesh=read_mesh("data/projet/mesh/coussindéformé.obj");
        coussin2.mesh=read_mesh("data/projet/mesh/coussin.obj");
        canape.mesh=read_mesh("data/projet/mesh/canapédéformé.obj");
        vase.mesh=read_mesh("data/projet/mesh/vase.obj");
        vaseC.mesh=read_mesh("data/projet/mesh/vasecassé.obj");
        CouteauSang.mesh=read_mesh("data/projet/mesh/couteausang.obj");

        m_texture= read_texture(0, "data/debug2x2red.png");
        m_text_terrain= read_texture(0, "data/projet/img/sale2text.png");
        text_lit = read_texture(0, "data/projet/img/lit.png");
        m_text_terrain2= read_texture(0, "data/projet/img/textureTerrain.png");
        textChev=read_texture(0, "data/projet/img/textCheval.png");
        textCoussin=read_texture(0, "data/projet/img/textCoussin1.png");
        tCanape=read_texture(0, "data/projet/img/textCanapé.png");
        tVase=read_texture(0, "data/projet/img/testVase1.png");
        tCouteau=read_texture(0, "data/projet/img/couteau.png");

        m_objet.setNumberCube(1);
        Point pmin,pmax;
        m_objet.mesh.bounds(pmin, pmax);
        m_objet.cube[0].set_position(pmax,pmin);
        lit.setNumberCube(1);
        lit.mesh.bounds(pmin,pmax);
        m_objet.cube[0].set_position(pmax,pmin);

        shad.init();
        aud.audio_Init();
        // etat openGL par defaut
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre
        
        glClearDepth(1.f);                          // profondeur par defaut
        glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST);                    // activer le ztest         
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);  
        glEnable(GL_TEXTURE_2D);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);       
        CC.getCam().lookAt((Point)CC.position(),5);
        m_view=CC.getCam();
        
        

        return 0;   // ras, pas d'erreur
    }
    
    // destruction des objets de l'application
    int quit( )
    {
        m_objet.mesh.release();
        lit.mesh.release();
        m_terrain.mesh.release();
        m_terrain2.mesh.release();
        cheval.mesh.release();
        coussin1.mesh.release();
        coussin2.mesh.release();
        canape.mesh.release();
        vase.mesh.release();
        vaseC.mesh.release();
        CouteauSang.mesh.release();
        glDeleteTextures(1, &m_texture);
        glDeleteTextures(1, &m_text_terrain2);
        glDeleteTextures(1, &m_text_terrain);
        glDeleteTextures(1, &textChev);
        glDeleteTextures(1, &textCoussin);
        glDeleteTextures(1, &tCanape);
        glDeleteTextures(1, &tVase);
        glDeleteTextures(1, &text_lit);
        glDeleteTextures(1, &tCouteau);
        shad.quit();
        aud.audio_Quit();
        
        return 0;
    }

    void box_transform(Transform T,Objet m)
    {
        for(int i=0;i<m.cube.size();i++)
        {
            Cube c=m.cube[i];
            c.set_position(T(c.get_pmax()),T(c.get_pmin()));
        }
    }
    
    // dessiner une nouvelle image
    int render( )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Point luxPosition=(Point)CC.Position;
        Point Direction=(Point)CC.direction();
        shad.edraw(m_objet.mesh,CC.getCh2w()*Scale(0.4,0.4,0.4),m_view,m_texture,luxPosition,Direction);
        box_transform(CC.getCh2w()*Scale(0.4,0.4,0.4),m_objet);
        shad.edraw(lit.mesh, Translation(0,0,-6)*Scale(0.3,0.3,0.3) ,m_view,text_lit,luxPosition,Direction);
        box_transform(Translation(0,0,-6)*Scale(0.3,0.3,0.3),lit);
        shad.edraw(m_terrain.mesh, Identity() ,m_view,m_text_terrain,luxPosition,Direction);
        box_transform(Identity(),m_terrain);
        shad.edraw(m_terrain2.mesh, Translation(0,-2,0)*Scale(20,10,20) ,m_view,m_text_terrain2,luxPosition,Direction);
        box_transform(Translation(0,-2,0)*Scale(20,10,20),m_terrain2);
        shad.edraw(cheval.mesh,Translation(2,0,3)*Scale(0.2,0.2,0.2),m_view,textChev,luxPosition,Direction);
        shad.edraw(coussin1.mesh,Translation(4,0,2)*Scale(0.3,0.3,0.3),m_view,textCoussin,luxPosition,Direction);
        shad.edraw(coussin2.mesh,Translation(4,0,3.5)*Scale(0.3,0.3,0.3),m_view,textCoussin,luxPosition,Direction);
        shad.edraw(canape.mesh,Translation(5.5,0,3.5)*Scale(0.5,0.5,0.5),m_view,tCanape,luxPosition,Direction);
        shad.edraw(vase.mesh,Translation(5,0,-6)*Scale(0.2,0.2,0.2),m_view,tVase,luxPosition,Direction);
        shad.edraw(vaseC.mesh,Translation(5.5,0.1,-5)*RotationX(-75)*Scale(0.2,0.2,0.2),m_view,tVase,luxPosition,Direction);
        shad.edraw(CouteauSang.mesh,Translation(-1,0.7,-7)*RotationX(-90)*Scale(0.05,0.05,0.05),m_view,tCouteau,luxPosition,Direction);
        shad.edraw(CouteauSang.mesh,Translation(1,0.7,-7)*RotationX(90)*RotationY(180)*Scale(0.05,0.05,0.05),m_view,tCouteau,luxPosition,Direction);
        return 1;
    }

    int CalculateContact(Objet m)
    {
        int n=0;
        for(int i=0;i<m.cube.size();i++)
        {
            n=m_objet.cube[0].calculate_collision(m.cube[i]);
        }
        return n;
    }

    int update(const float time, const float delta ){
        int n=0;
        n=CalculateContact(m_terrain);
        n=CalculateContact(lit);

        if(n==0)
        {
            CC.update(delta);
        }
        m_view=CC.getCam();
        static int controleMusic=0;

        if((key_state('s') || key_state('z') || key_state('q') || key_state('d')) && controleMusic==0)
        {
            controleMusic=1;
            aud.play_audio(aud.getWav());
        }
        if(!(key_state('s') || key_state('z') || key_state('q') || key_state('d')) && controleMusic==1)
        {
            controleMusic=0;
            aud.pause_audio(aud.getWav());
        }
        return 1;
    }


protected:
    Objet m_objet;
    Objet m_terrain;
    Objet m_terrain2;
    Objet lit;
    Objet cheval;
    Objet coussin1;
    Objet coussin2;
    Objet canape;
    Objet vase;
    Objet vaseC;
    Objet CouteauSang;
    GLuint m_texture;
    GLuint m_text_terrain;
    GLuint m_text_terrain2;
    GLuint text_lit;
    GLuint textChev;
    GLuint textCoussin;
    GLuint tCanape;
    GLuint tVase;
    GLuint tCouteau;
    CharacterController CC;
    Orbiter m_view;
    shader shad;
    audio aud;
};


int main( int argc, char **argv )
{
    // il ne reste plus qu'a creer un objet application et la lancer 
    TP tp;
    tp.run();
    
    return 0;
}
