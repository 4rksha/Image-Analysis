#include <iostream>
#include <vector>
#include "vec.h"
#include "mat.h"
#include "app_camera.h"
#include "mesh.h"
#include "draw.h"

using namespace std;

class Tp : public AppCamera {


    public:
    Mesh line;
    vector<Mesh> VectorN;
    vector<Mesh> Circle;
    vector<Mesh> Tube;
    vector<Point> lPoint;
    vector<vector<Vector>> vecteur;
    int value=0;
    void IncreaseLod();
    void DecreaseLod();

    Tp(int width, int height) : AppCamera(width, height){}

    int init()
    {
        generateLine();
        calculateVector();
        generateVector();
        //generateCircle();
        camera().lookat(Point(-3,-3,0),Point(7,7,0));
        return 0;
    }

    int quit()
    {
        return 0;
    }

    void generateCircle()
    {
        Circle.resize(0);
        for(unsigned int i=0;i<vecteur.size();i++)
        {
            Mesh CirclePoint=Mesh(GL_LINES);
            Vector p0;
            p0.x=lPoint[i+1].x-lPoint[i].x;
            p0.y=lPoint[i+1].y-lPoint[i].y;
            p0.z=lPoint[i+1].z-lPoint[i].z;
            for(int j=60;j<=360;j=j+60)
            {
                Transform T0 = Rotation(p0,j-60);
                Transform T = Rotation(p0,j);
                Vector v0= vecteur[i].at(1);
                v0=T0(v0);
                Vector v= vecteur[i].at(1);
                v=T(v);
                CirclePoint.vertex(lPoint[i]+(Point)v0);
                CirclePoint.vertex(lPoint[i]+(Point)v);
            }
            Circle.push_back(CirclePoint);
        }
    }

    void generatetube()
    {
        
    }

    void generateVector()
    {
        VectorN.resize(0);
        for(unsigned int i=0;i<vecteur.size();i++)
        {
            Mesh VectorN1=Mesh(GL_LINES);
            VectorN1.vertex(lPoint[i]);
            VectorN1.vertex(lPoint[i]+vecteur[i].at(1));
            VectorN.push_back(VectorN1);
        }
        Mesh VectorN1=Mesh(GL_LINES);
        VectorN1.vertex(lPoint[lPoint.size()-1]);
        VectorN1.vertex(lPoint[lPoint.size()-1]+(Point)vecteur[vecteur.size()-1].at(1));
        VectorN.push_back(VectorN1);
    }

    void generateLine()
    {
        line=Mesh(GL_LINES);
        for(unsigned int i=0;i<lPoint.size()-1;i++)
        {
            line.vertex(lPoint[i]);
            line.vertex(lPoint[i+1]);
        }
    }
    int update(const float time, const float delta )
    {
        if(key_state(SDLK_KP_PLUS))
        {
            IncreaseLod();
            generateLine();
            calculateVector();
            generateVector();
            //generateCircle();
            clear_key_state(SDLK_KP_PLUS);
        }
        if(key_state(SDLK_KP_MINUS))
        {
            DecreaseLod();
            generateLine();
            calculateVector();
            generateVector();
            //generateCircle();
            clear_key_state(SDLK_KP_MINUS);
        }
        
        return 0;
    }

    int render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw(line,Identity(),camera());
        for(unsigned int i=0;i<VectorN.size();i++)
        {
            draw(VectorN[i],Identity(),camera());
        }
        for(unsigned int i=0;i<Circle.size();i++)
        {
            draw(Circle[i],Identity(),camera());
        }
        return 1;
    }
    void calculateVector();
};


void Tp::calculateVector()
{
    vecteur.resize(0);
    vector<Vector> p;
    Vector p0;
    p0.x=lPoint[1].x-lPoint[0].x;
    p0.y=lPoint[1].y-lPoint[0].y;
    p0.z=lPoint[1].z-lPoint[0].z;
    p0=normalize(p0);
    p.push_back(p0);
    p0=Vector(1,0,0);
    Vector v=cross(p[0],p0);
    if(v.z==0 && v.x==0 && v.y==0)
    {
        p0=Vector(0,1,0);
        v=cross(p[0],p0);
    }
    p0=v;
    p0=normalize(p0);
    p.push_back(p0);
    vecteur.push_back(p);
    for(unsigned int i=1;i<lPoint.size()-1;i++)
    {
        vector<Vector> p1;
        p0.x=lPoint[i+1].x-lPoint[i].x;
        p0.y=lPoint[i+1].y-lPoint[i].y;
        p0.z=lPoint[i+1].z-lPoint[i].z;
        Vector p3;
        p3.x=lPoint[i].x-lPoint[i-1].x;
        p3.y=lPoint[i].y-lPoint[i-1].y;
        p3.z=lPoint[i].z-lPoint[i-1].z;
        p0=normalize(p0);
        p3=normalize(p3);
        p1.push_back(p0);
        Transform T2=Rotation(p0,p3);
        p0=T2(vecteur[i-1].at(1));
        p0=normalize(p0);
        p1.push_back(p0);
        vecteur.push_back(p1);
    }
}
void Tp::IncreaseLod() {
	vector<Point> NewPoints;

	// keep the first point
	NewPoints.push_back(lPoint[0]);
	for(unsigned int i=0;i<(lPoint.size()-1);++i) {
	
		// get 2 original points
		const Point& p0 = lPoint[i];
		const Point& p1 = lPoint[i+1];
		Point Q;
		Point R;

		// average the 2 original points to create 2 new points. For each
		// CV, another 2 verts are created.
		Q.x = 0.75f*p0.x + 0.25f*p1.x;
		Q.y = 0.75f*p0.y + 0.25f*p1.y;
		Q.z = 0.75f*p0.z + 0.25f*p1.z;

		R.x = 0.25f*p0.x + 0.75f*p1.x;
		R.y = 0.25f*p0.y + 0.75f*p1.y;
		R.z = 0.25f*p0.z + 0.75f*p1.z;

		NewPoints.push_back(Q);
		NewPoints.push_back(R);
	}
	// keep the last point
	NewPoints.push_back(lPoint[lPoint.size()-1]);

	// update the points array
	lPoint = NewPoints;
    value++;
}

void Tp::DecreaseLod() {

	// make sure we dont loose any points!!
    if(value>0)
    {
        if (lPoint.size()<=4) 
            return;

        std::vector<Point> NewPoints;

        // keep the first point
        NewPoints.push_back(lPoint[0]);

        // step over every 2 points
        for(unsigned int i=1;i<(lPoint.size()-1);i+=2) {

            // get last point found in reduced array
            const Point& pLast = NewPoints[NewPoints.size()-1];

            // get 2 original point
            const Point& p0 = lPoint[i];
            Point Q;

            // calculate the original point
            Q.x = p0.x - 0.75f*pLast.x;
            Q.y = p0.y - 0.75f*pLast.y;
            Q.z = p0.z - 0.75f*pLast.z;

            Q.x *= 4.0f;
            Q.y *= 4.0f;
            Q.z *= 4.0f;

            // add to new curve
            NewPoints.push_back(Q);
        }

        // copy over points
        lPoint = NewPoints;
        value--;
    }
}



int main()
{
    Tp app(1000,1000);
    /*app.lPoint.push_back(Point(0,0,0));
    app.lPoint.push_back(Point(0,1,0));
    app.lPoint.push_back(Point(-1,0,0));
    app.lPoint.push_back(Point(0,0,0));
    app.lPoint.push_back(Point(-1,0,0));
    Point p3(0,-1,0);
    app.lPoint.push_back(p3);
    app.lPoint.push_back(Point(0,0,0));
    app.lPoint.push_back(Point(0,-1,0));
    Point p(1,0,0);
    app.lPoint.push_back(p);
    app.lPoint.push_back(Point(0,0,0));
    app.lPoint.push_back(Point(1,0,0));
    Point p1(0,1,0);
    app.lPoint.push_back(p1);
    Point p2(0,0,0);
    app.lPoint.push_back(p2);*/
    app.lPoint.push_back(Point(-1,-1,1));
    app.lPoint.push_back(Point(-1,-1,-1));
    app.lPoint.push_back(Point(1,-1,-1));
    app.lPoint.push_back(Point(1,1,-1));
    app.lPoint.push_back(Point(-1,1,-1));
    app.lPoint.push_back(Point(-1,1,1));
    app.lPoint.push_back(Point(1,1,1));
    app.lPoint.push_back(Point(1,-1,1));
    app.lPoint.push_back(Point(5,-1,1));
    app.lPoint.push_back(Point(5,1,1));
    app.lPoint.push_back(Point(5,1,-1));
    app.lPoint.push_back(Point(5,-1,-1));
    app.lPoint.push_back(Point(3,-1,-1));
    app.lPoint.push_back(Point(3,1,-1));
    app.lPoint.push_back(Point(3,1,1));
    app.lPoint.push_back(Point(3,5,1));
    app.lPoint.push_back(Point(3,5,-1));
    app.lPoint.push_back(Point(5,5,-1));
    app.lPoint.push_back(Point(5,5,1));
    app.lPoint.push_back(Point(5,3,1));
    app.lPoint.push_back(Point(5,3,-1));
    app.lPoint.push_back(Point(3,3,-1));
    app.lPoint.push_back(Point(3,3,-5));
    app.lPoint.push_back(Point(5,3,-5));
    app.lPoint.push_back(Point(5,5,-5));
    app.lPoint.push_back(Point(3,5,-5));
    app.lPoint.push_back(Point(3,5,-3));
    app.lPoint.push_back(Point(5,5,-3));
    app.lPoint.push_back(Point(5,1,-3));
    app.lPoint.push_back(Point(5,1,-5));
    app.lPoint.push_back(Point(3,1,-5));
    app.lPoint.push_back(Point(3,-1,-5));
    app.lPoint.push_back(Point(5,-1,-5));
    app.lPoint.push_back(Point(5,-1,-3));
    app.lPoint.push_back(Point(3,-1,-3));
    app.lPoint.push_back(Point(5,-1,-3));
    app.lPoint.push_back(Point(3,-1,-3));
    app.lPoint.push_back(Point(3,1,-3));
    app.lPoint.push_back(Point(1,1,-3));
    app.lPoint.push_back(Point(1,1,-5));
    app.lPoint.push_back(Point(-1,1,-5));
    app.lPoint.push_back(Point(-1,-1,-5));
    app.lPoint.push_back(Point(1,-1,-5));
    app.lPoint.push_back(Point(1,-1,-3));
    app.lPoint.push_back(Point(-1,-1,-3));
    app.lPoint.push_back(Point(-1,3,-3));
    app.lPoint.push_back(Point(1,3,-3));
    app.lPoint.push_back(Point(1,5,-3));
    app.lPoint.push_back(Point(1,5,-5));
    app.lPoint.push_back(Point(1,3,-5));
    app.lPoint.push_back(Point(-1,3,-5));
    app.lPoint.push_back(Point(-1,5,-5));
    app.lPoint.push_back(Point(-1,5,-1));
    app.lPoint.push_back(Point(-1,3,-1));
    app.lPoint.push_back(Point(-1,3,1));
    app.lPoint.push_back(Point(1,3,1));
    app.lPoint.push_back(Point(1,3,-1));
    app.lPoint.push_back(Point(1,5,-1));
    app.lPoint.push_back(Point(1,5,1));
    app.lPoint.push_back(Point(-1,5,1));
    app.run();
    return 0;
}