#include <math.h>
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
#include <algorithm>  
#include <iostream>

struct Ray
{
    Point o;                // origine
    Vector d;               // direction
    float tmax;             // intervalle [0 tmax]
    
    // le rayon est un segment, on connait origine et extremite, et tmax= 1
    Ray( const Point& origine, const Point& extremite ) : o(origine), d(Vector(origine, extremite)), tmax(1) {}
    
    // le rayon est une demi droite, on connait origine et direction, et tmax= \inf
    Ray( const Point& origine, const Vector& direction ) : o(origine), d(direction), tmax(FLT_MAX) {}
    
    // renvoie le point sur le rayon pour t
    Point point( const float t ) const { return o + t * d; }
};

struct Hit
{
    float t;            // p(t)= o + td, position du point d'intersection sur le rayon 
    int cercle_id;    // indice du cercle dans le mesh
    
    // pas d'intersection
    Hit( ) : t(FLT_MAX), cercle_id(-1) {}   
    // intersection 
    Hit( const float _t, const int _id ) : t(_t), cercle_id(_id) {}
    
    // renvoie vrai si l'intersection est valide
    operator bool ( ) { return (cercle_id != -1); }
};

struct Sphere
{
    Point c;
    float r;
    Color diffuse;
    int id;
    
    Sphere(Point _c, float _r,int _id,Color _diffuse = Color(0,0,0,1)): c(_c),r(_r),diffuse(_diffuse),id(_id) {}

    Hit intersect( const Ray& ray, const float tmax ) const
    {
	float a=dot(ray.d,ray.d);
	float b=dot(2*ray.d,ray.o - c);
	float k=dot(ray.o-c,ray.o - c)-(r*r);
	float delta=b*b-4*a*k;
	if(delta<0) return Hit();
	if(delta==0)
	{
		float t=-b/(2*a);
		if(t<0 || t>1) return Hit();
		return Hit(t, id);
	}
	if(delta>0)
	{
		float t1=(-b+sqrt(delta))/(2*a);
        	float t2=(-b-sqrt(delta))/(2*a);
		if((t1<0 || t1>tmax) && (t2<0 || t2>tmax)) return Hit();
		else if(t1<0 || t1>tmax) return Hit(t2, id);
		else if(t2<0 || t2>tmax) return Hit(t1, id);
		else return Hit(std::min(t1,t2), id);

	} 
	return Hit();
    }

    Vector normal( const Point& p ) const   
    {
        return normalize(Vector(c, p));
    }

    Color diffuse_color( const Mesh& mesh, const Hit& hit )
    {
        return diffuse;
    }

};

struct Source
{
    Point s;
    Color emission;
    Source(Point _s,Color _emission) : s(_s), emission(_emission){}
};



int main( )
{
    const char *mesh_filename= "data/tp1_iehl/sphere.obj";
    const char *orbiter_filename= "data/tp1_iehl/cornell_orbiter.txt";
    
    // cree l'image resultat
    Image image(1024, 768);
    
    // charge une camera
    Orbiter camera;
    camera=Orbiter();
    //if(camera.read_orbiter(orbiter_filename) < 0)
    //    return 1; // erreur, pas de camera, pas d'image
    
    // charge des triangles
    Mesh mesh= read_mesh(mesh_filename);
    Source sources(Point(3,1,0),Color(3,3,3,1));

    
    // recupere les triangles du mesh
    std::vector<Sphere> spheres;
    {
            spheres.push_back( Sphere(Point(0,0,0),1.0,0,Color(1,0,0)) );
	    spheres.push_back( Sphere(Point(0,-19,-5),18.0,1,Color(0,1,0)) );
    }
    
    // recupere les transformations standards
    camera.projection(image.width(), image.height(), 45);
    Transform model= Identity();
    Transform view= camera.view();
    Transform projection= camera.projection();
    Transform viewport= camera.viewport();
    Transform inv= Inverse(viewport * projection * view * model);
    
    
    // c'est parti, parcours tous les pixels de l'image
    for(int y= 0; y < image.height(); y++)
    for(int x= 0; x < image.width(); x++)
    {
        // generer le rayon sur le centre du pixel (x, y)
        Point origine= inv(Point(x + .5f, y + .5f, 0));
        Point extremite= inv(Point(x + .5f, y + .5f, 1));
        Ray ray(origine, extremite);
        
        // calculer les intersections avec tous les triangles
        Hit hit;
        hit.t= 1;   // intersections dans l'intervalle [0 .. 1]
        for(int i= 0; i < int(spheres.size()); i++)
        {
            Hit h= spheres[i].intersect(ray, hit.t);
            if(h)
                // ne conserve que l'intersection la plus proche de l'origine du rayon
                // triangle::intersect(ray, tmax= hit.t) ne renvoie vrai que si t < hit.t
                hit= h;
                
            /* remarque : Hit() definit un operateur de conversion vers bool qui renvoie vrai si hit.triangle_id != -1
                if(h) { ... }
                est equivalent a :
                if(h.triangle_id != -1) { ... }
                
                et on peut ecrire tout ca de maniere encore plus compacte :
                if(Hit h= triangles[i].intersect(ray, hit.t))
                    hit= h;
             */
        }
        if(hit)
	{
            // pixel rouge en cas d'intersection
	    Point s= sources.s;
	    Color emission= sources.emission;
	    // position du point d'intersection
	    Point p= ray.o + hit.t * ray.d;
	    // direction de p vers la source s
	    Vector l= normalize(Vector(p, s));
	    // interpoler la normale au point d'intersection
	    Vector pn= spheres[hit.cercle_id].normal(p);
            Hit hitlum;
            hitlum.t=1;
            Ray ray2(sources.s,p+ 0.001 * pn);
            int n=0;
            for(int i= 0; i < int(spheres.size()); i++)
		{
		    Hit h= spheres[i].intersect(ray2, hitlum.t);
		    if(h)
		    {
		        // ne conserve que l'intersection la plus proche de l'origine du rayon
		        // triangle::intersect(ray, tmax= hit.t) ne renvoie vrai que si t < hit.t
		        hitlum= h;
			n++;
		    }
		        
		    /* remarque : Hit() definit un operateur de conversion vers bool qui renvoie vrai si hit.triangle_id != -1
		        if(h) { ... }
		        est equivalent a :
		        if(h.triangle_id != -1) { ... }
		        
		        et on peut ecrire tout ca de maniere encore plus compacte :
		        if(Hit h= triangles[i].intersect(ray, hit.t))
		            hit= h;
		     */
		}
	    // calculer la lumiere reflechie vers la camera / l'origine du rayon
	    float cos_theta= std::abs(dot(pn, l));
	    Color fr= spheres[hit.cercle_id].diffuse_color(mesh, hit) / M_PI;
	    Color color= emission * fr * cos_theta;
        if(n==0)
	    {
	        Color c(color,1);
	        image(x, y)= c;
            }
            else image(x,y)=Color(0,0,0);
       }
    }
    
    // enregistre l'image
    write_image(image, "render.png");
    return 0;
}
