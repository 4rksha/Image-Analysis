
//! \file tuto7.cpp reprise de tuto6.cpp mais en derivant App::init(), App::quit() et bien sur App::render().

#include "wavefront.h"
#include "texture.h"

#include "orbiter.h"
#include "draw.h"        
#include "app.h"  
#include "CharacterController.h"
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
        m_terrain.mesh= read_mesh("data/projet/sale2.obj");
        m_terrain2.mesh= read_mesh("data/projet/terrain2.obj");
        lit.mesh= read_mesh("data/projet/litdéformé.obj");
        

        m_texture= read_texture(0, "data/debug2x2red.png");
        m_text_terrain= read_texture(0, "data/projet/sale2text.png");
        text_lit = read_texture(0, "data/projet/lit.png");
        m_text_terrain2= read_texture(0, "data/projet/textureTerrain.png");

        shad.init();
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
        glDeleteTextures(1, &m_texture);
        shad.quit();
        
        return 0;
    }
    
    // dessiner une nouvelle image
    int render( )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Point luxPosition=(Point)CC.Position;
        Point Direction=(Point)CC.direction();
        shad.edraw(m_objet.mesh,CC.getCh2w()*Scale(0.4,0.4,0.4),m_view,m_texture,luxPosition,Direction);
        shad.edraw(lit.mesh, Scale(0.3,0.3,0.3) ,m_view,text_lit,luxPosition,Direction);
        shad.edraw(m_terrain.mesh, Identity() ,m_view,m_text_terrain,luxPosition,Direction);
        shad.edraw(m_terrain2.mesh, Translation(0,-2,0)*Scale(20,10,20) ,m_view,m_text_terrain2,luxPosition,Direction);
        
        return 1;
    }

    int update(const float time, const float delta ){
        CC.update(delta);
        m_view=CC.getCam();
        return 1;
    }


protected:
    Objet m_objet;
    Objet m_terrain;
    Objet m_terrain2;
    Objet lit;
    GLuint m_texture;
    GLuint m_text_terrain;
    GLuint m_text_terrain2;
    GLuint text_lit;
    CharacterController CC;
    Orbiter m_view;
    shader shad;
};


int main( int argc, char **argv )
{
    // il ne reste plus qu'a creer un objet application et la lancer 
    TP tp;
    tp.run();
    
    return 0;
}
