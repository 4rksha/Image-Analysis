
//! \file tuto7_camera.cpp reprise de tuto7.cpp mais en derivant AppCamera, avec gestion automatique d'une camera.


#include "wavefront.h"
#include "texture.h"
#include "program.h"
#include "uniforms.h"
#include "vec.h"
#include "mat.h"
#include "mesh.h"
#include "orbiter.h"
#include "draw.h"        
#include "app_camera.h" 
#include <string>   
using namespace std;    

class AnimationCurve
{
public:
    AnimationCurve() : m_P(nullptr), m_nbp(0) {}

    //! initialise la courbe d'animation avec une liste de points de C du fichier "nom_fichier"
    void init(const char* nom_fichier);

    void draw(Orbiter& camera);

    const Point& operator[]( const int i) { assert(i>=0); assert(i<m_nbp);  return m_P[i]; }
    int nb_points() const { return m_nbp; }

protected:
    //! Les points de la trajectoire/courbe d'animation que va suivre la fusée
    Point* m_P;
    //! Nombre de point
    int m_nbp;

    Mesh m_curve;

    Vector Bezier(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, const float t);
};

const int NBPB=20;              // NomBre de Points de Bezier (entre 2 points de Contr il y aura NPB points)


//float temps()
//{
//    return float(clock())/CLOCKS_PER_SEC;
//}


// Fonction de calcul
// pc[0..3] sont les 4 points de controles
// t  le  parametre
// p le  point resultat de la courbe
Vector AnimationCurve::Bezier(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, const float t)
{
    float t1,t13,t3;
    Vector p, r0, r1, r2, r3;
    t1 = 1 - t;
    t13 = t1 * t1 * t1;
    t3 = t * t * t;
    p = t13*p0 + 3*t*t1*t1*p1 + 3*t*t*t1*p2 + t3*p3;
    return p;

//    vecMul( r0, t13, p0);
//    vecMul( r1, 3*t*t1*t1, p1);
//    vecMul( r2, 3*t*t*t1, p2);
//    vecMul( r3, t3, p3);
//    vecAdd( p, r0, r1);
//    vecAdd( p, p, r2);
//    vecAdd( p, p, r3);
//    return p;
}


void AnimationCurve::init(const char* nom_fichier)
{
    const int taille=512;
    char txt[taille];
    FILE* f;

    f = fopen( nom_fichier, "r");
    if (f==nullptr)
    {
        printf("ERREUR: ouverture impossible du fichier %s\n", nom_fichier);
        assert(f);
    }

    // donnée temporaire (=point de controle PC) servant pour calculer les points finaux (P)
    int NBPC;
    Point* PC;

    do
    {
        fgets( txt,taille,f);
    }
    while( txt[0]=='#' );
    int err = sscanf( txt, "%d\n", &NBPC);
    assert( err==1 );
    m_nbp = (NBPC/4)*NBPB;	// Nombre de points en tout

    PC = new Point[NBPC];
    m_P = new Point[m_nbp];
    int i;
    for(i=0; i<NBPC; ++i)
    {
        do
        {
            fgets( txt,taille,f);
        }
        while( txt[0]=='#' );
        err = sscanf( txt, "%f %f %f\n", &PC[i].x, &PC[i].y, &PC[i].z);
        assert( err==3 );
    }

    printf("Animation: %d PdC et %d P en tout\n", NBPC, m_nbp);
    //for(i=0;i<NBPC;++i) vecPrint( a.P[i] );


    // Courbe de Bezier
    int c=0, j;
    for (i=0; i<NBPC; i+=4)                 // Pour calculer une courbe de Bezier, on regroupe les points de controle par 4
    {
        for (j=0; j<NBPB; j++)
        {
            m_P[c] = Point(Bezier( Vector(PC[i]),
                                   Vector(PC[(i+1)%NBPC]), Vector(PC[(i+2)%NBPC]), Vector(PC[(i+3)%NBPC]),  float(j)/(NBPB) ) );
            c++;
        }
    }
    delete[] PC;



    m_curve = Mesh(GL_LINES);

    m_curve.color( Color(1, 0, 0));
    for(i=0; i<m_nbp-1; ++i)
    {
        m_curve.vertex( m_P[i] );
        m_curve.vertex( m_P[i+1] );
    }
    m_curve.vertex( m_P[i] );
    m_curve.vertex( m_P[0] );
}


void AnimationCurve::draw(Orbiter& camera)
{
    ::draw( m_curve,  camera);
}

// utilitaire. creation d'une grille / repere.
Mesh make_grid( const int n= 10 )
{
    Mesh grid= Mesh(GL_LINES);
    
    // grille
    grid.color(White());
    for(int x= 0; x < n; x++)
    {
        float px= float(x) - float(n)/2 + .5f;
        grid.vertex(Point(px, 0, - float(n)/2 + .5f)); 
        grid.vertex(Point(px, 0, float(n)/2 - .5f));
    }

    for(int z= 0; z < n; z++)
    {
        float pz= float(z) - float(n)/2 + .5f;
        grid.vertex(Point(- float(n)/2 + .5f, 0, pz)); 
        grid.vertex(Point(float(n)/2 - .5f, 0, pz)); 
    }
    
    // axes XYZ
    grid.color(Red());
    grid.vertex(Point(0, .1, 0));
    grid.vertex(Point(1, .1, 0));
    
    grid.color(Green());
    grid.vertex(Point(0, .1, 0));
    grid.vertex(Point(0, 1, 0));
    
    grid.color(Blue());
    grid.vertex(Point(0, .1, 0));
    grid.vertex(Point(0, .1, 1));
    
    glLineWidth(2);
    
    return grid;
}


class TP : public AppCamera
{
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    TP( ) : AppCamera(1024, 640) {}
    
    void reload_programs()
    {
        if(program == 0)
            program= read_program(shaderName.c_str());
        else
            reload_program(program, shaderName.c_str());

        program_area= program_format_errors(program, program_log);
    
        if(program_log.size() > 0)
            printf("[boom]\n%s\n", program_log.c_str());
        
        program_failed= (program_log.size() > 0);
    }

    // creation des objets de l'application
    int init( )
    {
        m_anim.init(smart_path("data/animation/anim1.ani") );
        // decrire un repere / grille 
        m_repere= make_grid(20);
        chair = read_mesh("data/tp1_iehl/chair.obj");
        femme =read_mesh("data/tp1_iehl/femme.obj");
        m_objet= read_mesh("data/tp1_iehl/table.obj");
        champi = read_mesh("data/tp1_iehl/test.obj");
        donuts = read_mesh("data/tp1_iehl/donuts.obj");
        lampadaire = read_mesh("data/tp1_iehl/lampadaire.obj");
        terrain = read_mesh("data/tp1_iehl/terrain.obj");
        bee = read_mesh("data/tp1_iehl/bee.obj");
        arbre = read_mesh("data/tp1_iehl/rock.obj");
        m_texture= read_texture(0, "data/tp1_iehl/text.jpg");
        textArbre= read_texture(0, "data/tp1_iehl/rocktext.png");
        shaderName="data/tp1_iehl/shader3.glsl";
        vao= 0;
        if(vao == 0)
        {
            glGenVertexArrays(1, &vao);
        }
        reload_programs();
        // si l'objet est "gros", il faut regler la camera pour l'observer entierement :
        // recuperer les points extremes de l'objet (son englobant)
        Point pmin, pmax;
        m_repere.bounds(pmin, pmax);
        // parametrer la camera de l'application, renvoyee par la fonction camera()
        camera().lookat(pmin, pmax);
        
        // etat openGL par defautvao= 0;
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre
        
        glClearDepth(1.f);                          // profondeur par defaut
        glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST);                    // activer le ztest

        return 0;   // ras, pas d'erreur
    }
    
    // destruction des objets de l'application
    int quit( )
    {
        m_objet.release();
        m_repere.release();
        release_program(program);
        return 0;
    }
    
    // dessiner une nouvelle image
    int render( )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*Transform model= Identity();
        Transform view= camera().view();
        Transform projection= camera().projection(window_width(), window_height(), 45);
        
        Transform mvp= projection * view * model;
        glUseProgram(program);
        program_uniform(program, "modelMatrix", model);
        program_uniform(program, "mvpMatrix", mvp);
        program_uniform(program, "viewInvMatrix", view.inverse());*/
        // dessine l'objet, place au centre du monde, pour le point de vue de la camera 
        draw(m_objet, Translation(-1,0,-5), camera());
        draw(femme, Translation(-1.7,0,-5)*Scale(0.005,0.005,0.005)*RotationY(90), camera() );
        draw(chair, Translation(-1,0.23,-4.7)*Scale(0.08,0.08,0.08)*RotationY(180), camera());
        draw(donuts, Translation(-1,0.38,-5), camera());
        draw(champi, Translation(-4,0.2,-2)*Scale(0.1,0.1,0.1), camera());
        draw(champi, Translation(-2,0.2,-3)*Scale(0.1,0.1,0.1)*RotationY(30), camera());
        draw(champi, Translation(-4,0.2,5)*Scale(0.1,0.1,0.1)*RotationY(172), camera() );
        draw(champi, Translation(-2,0.2,3)*Scale(0.1,0.1,0.1)*RotationY(75), camera() );
        draw(champi, Translation(-6,0.2,3)*Scale(0.1,0.1,0.1)*RotationY(240), camera() );
        draw(champi, Translation(-4,0.2,0)*Scale(0.1,0.1,0.1)*RotationY(10), camera() );
        draw(champi, Translation(-6,0.2,-5)*Scale(0.1,0.1,0.1)*RotationY(75), camera() );
        draw(champi, Translation(-3,0.2,-7)*Scale(0.1,0.1,0.1)*RotationY(75), camera() );
        draw(champi, Translation(-5,0.2,7)*Scale(0.1,0.1,0.1)*RotationY(75), camera() );
        draw(champi, Translation(3,0.2,-7)*Scale(0.1,0.1,0.1)*RotationY(75), camera() );
        draw(champi, Translation(5,0.5,-5)*Scale(0.1,0.1,0.1)*RotationY(75), camera() );
        draw(champi, Translation(7,0.2,-3)*Scale(0.1,0.1,0.1)*RotationY(75), camera());
        draw(arbre, Translation(0,5,0), camera() ,textArbre);
        draw(lampadaire, Translation(-3,0.2,-6)*Scale(0.1,0.1,0.1)*RotationY(140), camera());
        draw(lampadaire, Translation(-3,0.2,-4)*Scale(0.1,0.1,0.1), camera());
        draw(lampadaire, Translation(-3,0.2,-2)*Scale(0.1,0.1,0.1), camera());
        draw(lampadaire, Translation(-3,0.2,0)*Scale(0.1,0.1,0.1), camera());
        draw(lampadaire, Translation(-3,0.2,2)*Scale(0.1,0.1,0.1), camera());
        draw(lampadaire, Translation(-3,0.2,4)*Scale(0.1,0.1,0.1), camera());
        draw(lampadaire, Translation(-3,0.2,6)*Scale(0.1,0.1,0.1), camera());
        draw(lampadaire, Translation(1,0.2,-6)*Scale(0.1,0.1,0.1)*RotationY(70), camera());
        draw(lampadaire, Translation(3,0.2,-6)*Scale(0.1,0.1,0.1)*RotationY(90), camera());
        draw(lampadaire, Translation(5,0.5,-6)*Scale(0.1,0.1,0.1)*RotationY(90), camera());
        draw(lampadaire, Translation(7,0.6,-6)*Scale(0.1,0.1,0.1)*RotationY(90), camera());
        draw(terrain, Scale(3,3,3), camera(),m_texture);
    
        draw(bee, tBee1*Translation(2,7,2)*Scale(0.01,0.01,0.01), camera());
        draw(bee, tBee1*Translation(0,7,0)*Scale(0.01,0.01,0.01), camera());
        draw(bee, tBee1*Translation(3,7,0)*Scale(0.01,0.01,0.01), camera());
        
        // dessine aussi le repere, pour le meme point de vue
        //draw(m_repere, Identity(), camera());

        // continuer...draw(bee, tBee1*Translation(2,7,2)*Scale(0.01,0.01,0.01), camera());
        return 1;
    }

    int update(const float time, const float delta ){
        float ts = 10 * time /1000;
        int temps_entier = int (ts);

        int temps_entier_ok = temps_entier % m_anim.nb_points();
        float poids = ts - temps_entier;
        int cible = (temps_entier + 1) % m_anim.nb_points();

        Point pos = m_anim[temps_entier_ok] + (m_anim[cible]-m_anim[temps_entier_ok])*poids;


        int prec1 = (temps_entier +1) % m_anim.nb_points();
        int suiv1 = (cible +1) % m_anim.nb_points();

        Point pos_suiv = m_anim[prec1] + poids * (m_anim[suiv1]-m_anim[prec1]);

        Vector direction = normalize(pos_suiv - pos);
        float ps = dot(direction,Vector(1,0,0));
        float angle;
        if (direction.z>0) angle = -acos(ps);
        else angle = acos(ps);



        tBee1 = Translation((pos_suiv.x+(pos.x-pos_suiv.x)*poids*0.05)/5, 0,  (pos_suiv.z+(pos.z-pos_suiv.z)*poids*0.05)/5)*RotationY((angle/M_PI)*180 +90.0);
        return 1;
    }

protected:
    Mesh m_objet;
    Mesh m_repere;
    GLuint m_texture;
    Mesh champi;
    Mesh donuts;
    Mesh lampadaire;
    GLuint textArbre; 
    Mesh arbre;
    Mesh terrain;
    Mesh bee;
    Mesh chair;
    Mesh femme;
    Transform tBee1=Identity();
    AnimationCurve m_anim;
    string shaderName;
    GLuint program=0;
    GLuint vao;
    std::string program_log;
    int program_area;
    bool program_failed;
};


int main( int argc, char **argv )
{
    // il ne reste plus qu'a creer un objet application et la lancer 
    TP tp;
    tp.run();
    
    return 0;
}
