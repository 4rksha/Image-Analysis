
//! \file tuto7.cpp reprise de tuto6.cpp mais en derivant App::init(), App::quit() et bien sur App::render().

#include "wavefront.h"
#include "texture.h"

#include "orbiter.h"
#include "draw.h"        
#include "app.h"  
#include "CharacterController.h"
#include "glcore.h"



class TP : public App
{
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    TP( ) : App(1024, 640) {}
    
    // creation des objets de l'application
    int init( )
    {
        m_objet= read_mesh("data/cube.obj");
        m_terrain= read_mesh("data/tp1_iehl/terrain.obj");
        

        m_texture= read_texture(0, "data/debug2x2red.png");

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
        m_objet.release();
        glDeleteTextures(1, &m_texture);
        
        return 0;
    }
    
    // dessiner une nouvelle image
    int render( )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        draw(m_objet, CC.getCh2w(),m_view,m_texture);
        draw(m_terrain, Identity() ,m_view,m_texture);
        
        return 1;
    }

    int update(const float time, const float delta ){
        CC.update(delta);
        m_view=CC.getCam();
        return 1;
    }


protected:
    Mesh m_objet;
    Mesh m_terrain;
    GLuint m_texture;
    CharacterController CC;
    Orbiter m_view;
};


int main( int argc, char **argv )
{
    // il ne reste plus qu'a creer un objet application et la lancer 
    TP tp;
    tp.run();
    
    return 0;
}
