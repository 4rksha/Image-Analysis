
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
#include "libs/imgui-master/imgui.h"
#include "libs/imgui-master/examples/imgui_impl_sdl.h"
#include "libs/imgui-master/examples/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <string>

class TP : public App
{
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    TP() : App(1024, 640) {}

    // creation des objets de l'application
    int init()
    {

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplSDL2_InitForOpenGL(this->m_window, this->m_context);
        ImGui_ImplOpenGL3_Init("#version 330");

        m_objet.mesh = read_mesh("data/projet/mesh/main.obj");
        m_terrain.mesh = read_mesh("data/projet/mesh/sale2.obj");
        m_terrain2.mesh = read_mesh("data/projet/mesh/terrain2.obj");
        lit.mesh = read_mesh("data/projet/mesh/litdéformé.obj");
        cheval.mesh = read_mesh("data/projet/mesh/chevalabasculedéformé.obj");
        coussin1.mesh = read_mesh("data/projet/mesh/coussindéformé.obj");
        coussin2.mesh = read_mesh("data/projet/mesh/coussin.obj");
        canape.mesh = read_mesh("data/projet/mesh/canapédéformé.obj");
        vase.mesh = read_mesh("data/projet/mesh/vase.obj");
        vaseC.mesh = read_mesh("data/projet/mesh/vasecassé.obj");
        CouteauSang.mesh = read_mesh("data/projet/mesh/couteausang.obj");
        Shelf.mesh = read_mesh("data/projet/mesh/shelf_pleine.obj");
        Piano.mesh = read_mesh("data/projet/mesh/piano.obj");
        Cape.mesh = read_mesh("data/projet/mesh/cape.obj");
        plane = read_mesh("data/bigguy.obj");

        m_texture = read_texture(0, "data/projet/img/main.png");
        m_text_terrain = read_texture(0, "data/projet/img/sale2text.png");
        text_lit = read_texture(0, "data/projet/img/lit.png");
        m_text_terrain2 = read_texture(0, "data/projet/img/textureTerrain.png");
        textChev = read_texture(0, "data/projet/img/textCheval.png");
        textCoussin = read_texture(0, "data/projet/img/textCoussin1.png");
        tCanape = read_texture(0, "data/projet/img/textCanapé.png");
        tVase = read_texture(0, "data/projet/img/testVase1.png");
        tCouteau = read_texture(0, "data/projet/img/couteau.png");
        textFull = read_texture(0, "data/projet/img/shelf_pleine.png");
        texPiano = read_texture(0, "data/projet/img/textpiano.png");
        texCape = read_texture(0, "data/projet/img/robe.png");
        texPlane = read_texture(0, "data/projet/img/black.png");

        m_objet.setNumberCube(1);
        Point pmin, pmax;
        m_objet.mesh.bounds(pmin, pmax);
        m_objet.cube[0].set_position(pmax, pmin);
        lit.setNumberCube(1);
        lit.mesh.bounds(pmin, pmax);
        m_objet.cube[0].set_position(pmax, pmin);

        shad.init();
        aud.audio_Init();
        // etat openGL par defaut
        glClearColor(0.2f, 0.2f, 0.2f, 1.f); // couleur par defaut de la fenetre

        glClearDepth(1.f);       // profondeur par defaut
        glDepthFunc(GL_LESS);    // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST); // activer le ztest
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glEnable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        CC.getCam().lookAt((Point)CC.position(), 5);
        m_view = CC.getCam();

        return 0; // ras, pas d'erreur
    }

    // destruction des objets de l'application
    int quit()
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
        Shelf.mesh.release();
        Piano.mesh.release();
        Cape.mesh.release();
        glDeleteTextures(1, &m_texture);
        glDeleteTextures(1, &m_text_terrain2);
        glDeleteTextures(1, &m_text_terrain);
        glDeleteTextures(1, &textChev);
        glDeleteTextures(1, &textCoussin);
        glDeleteTextures(1, &tCanape);
        glDeleteTextures(1, &tVase);
        glDeleteTextures(1, &text_lit);
        glDeleteTextures(1, &tCouteau);
        glDeleteTextures(1, &textFull);
        glDeleteTextures(1, &texPiano);
        glDeleteTextures(1, &texCape);
        shad.quit();
        aud.audio_Quit();

        return 0;
    }

    void box_transform(Transform T, Objet m)
    {
        for (unsigned int i = 0; i < m.cube.size(); i++)
        {
            Cube c = m.cube[i];
            c.set_position(T(c.get_pmax()), T(c.get_pmin()));
        }
    }

    void afficheE()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(m_window);
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(480, 250));
        ImGui::SetNextWindowSize({100.0f, 30.0f});
        ImGui::Begin("Press E");
        ImGui::Text("............");
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    void afficheWin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(m_window);
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(480, 250));
        ImGui::SetNextWindowSize({100.0f, 30.0f});
        ImGui::Begin("YOU WIN");
        ImGui::Text(".............");
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    void afficheLoose()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(m_window);
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(480, 250));
        ImGui::SetNextWindowSize({100.0f, 30.0f});
        ImGui::Begin("YOU LOOSE");
        ImGui::Text("..............");
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void afficheMenu()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(m_window);
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(480, 250));
        ImGui::Begin("Menu"); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        if (ImGui::Button("Play"))
        {
            controlefin = 5;
            aud.play_audio(aud.getPluie());
        }
        if (ImGui::Button("how to play?"))
            controlefin = 4;
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void afficheInfo()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(m_window);
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(450, 250));
        ImGui::Begin("how to play?");
        ImGui::Text("Z to moving forward");
        ImGui::Text("S to back off");
        ImGui::Text("D to turn right");
        ImGui::Text("Q to turn left");
        ImGui::Text("E to interact");
        if (ImGui::Button("Back"))
            controlefin = 3;
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    void affichetext(const char *s, const char *people, int line_number = 1, bool skippable=false)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(m_window);
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(350, 100));
        ImGui::SetNextWindowSize({400.0f, 50.f + 18.f * line_number});
        ImGui::Begin(people);
        ImGui::Text("%s", s);
        if (skippable && ImGui::Button("skip")) {
            delay = 200.f;
            dialogue += 2000 - dialogue % 2000;
        }
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    // dessiner une nouvelle image
    int render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (controlefin == 0)
        {
            Point luxPosition;
            if (shad.foudreControle > 2000)
            {
                luxPosition = (Point)((Point)CC.Position+Point(0,0.2,0)-0.3*(Point)CC.direction());
            }
            else
            {
                luxPosition = Point(10, 0, 10);
            }
            Point Direction = (Point)CC.direction();
            
            shad.edraw(m_objet.mesh, CC.getCh2w()*Translation(-0.1,0.3,0)*RotationY(-90) * Scale(0.08, 0.1, 0.05), m_view, m_texture, luxPosition, Direction);
            shad.edraw(Cape.mesh, CC.getCh2w()*Translation(0,0.3,0)*RotationX(90)* Scale(0.3, 0.3, 0.2), m_view, texCape, luxPosition, Direction);
            box_transform(CC.getCh2w() * Scale(0.4, 0.4, 0.4), m_objet);
            shad.edraw(lit.mesh, Translation(0, 0, -6) * Scale(0.3, 0.3, 0.3), m_view, text_lit, luxPosition, Direction);
            box_transform(Translation(0, 0, -6) * Scale(0.3, 0.3, 0.3), lit);
            shad.edraw(m_terrain.mesh, Identity(), m_view, m_text_terrain, luxPosition, Direction);
            box_transform(Identity(), m_terrain);
            shad.edraw(m_terrain2.mesh, Translation(0, -2, 0) * Scale(20, 10, 20), m_view, m_text_terrain2, luxPosition, Direction);
            box_transform(Translation(0, -2, 0) * Scale(20, 10, 20), m_terrain2);
            shad.edraw(cheval.mesh, Translation(2, 0, 3) * Scale(0.2, 0.2, 0.2), m_view, textChev, luxPosition, Direction);
            shad.edraw(coussin1.mesh, Translation(4, 0, 2) * Scale(0.3, 0.3, 0.3), m_view, textCoussin, luxPosition, Direction);
            shad.edraw(coussin2.mesh, Translation(4, 0, 3.5) * Scale(0.3, 0.3, 0.3), m_view, textCoussin, luxPosition, Direction);
            shad.edraw(canape.mesh, Translation(5.5, 0, 3.5) * Scale(0.4, 0.4, 0.4), m_view, tCanape, luxPosition, Direction);
            shad.edraw(vase.mesh, Translation(5, 0, -6) * Scale(0.2, 0.2, 0.2), m_view, tVase, luxPosition, Direction);
            shad.edraw(vaseC.mesh, Translation(5.5, 0.1, -5) * RotationX(-75) * Scale(0.2, 0.2, 0.2), m_view, tVase, luxPosition, Direction);
            shad.edraw(CouteauSang.mesh, Translation(-1, 0.7, -7) * RotationX(-90) * Scale(0.05, 0.05, 0.05), m_view, tCouteau, luxPosition, Direction);
            shad.edraw(CouteauSang.mesh, Translation(1, 0.7, -7) * RotationX(90) * RotationY(180) * Scale(0.05, 0.05, 0.05), m_view, tCouteau, luxPosition, Direction);
            shad.edraw(Shelf.mesh, Translation(-5, 0, -1) * RotationY(90) * Scale(0.3, 0.3, 0.3), m_view, textFull, luxPosition, Direction);
            shad.edraw(Piano.mesh, Translation(0, 0, -1) * Scale(0.5, 0.5, 0.5) , m_view, texPiano, luxPosition, Direction);
            
            if (shad.foudreControle <= 2000)
            {
                shad.edraw(plane, Translation(CC.Position) * Translation(1.2*CC.direction())*Scale(0.05,0.05,0.05), m_view, texPlane, luxPosition, Direction);
            }
        }
        if (controleindice == 1 && controlefin == 0)
        {
            afficheE();
        }
        if (controlefin == 1)
        {
            afficheWin();
        }
        if (controlefin == 2)
        {
            afficheLoose();
        }
        if (controlefin == 3)
        {
            afficheMenu();
        }
        if (controlefin == 4)
        {
            afficheInfo();
        }
        if (controlefin == 5)
        {
            if (dialogue < 2000) 
                affichetext("Hum...? Where am I?", "child", 1, true);
            if (dialogue < 4000 && dialogue > 2000) 
                affichetext("What am I doing here?", "child", 1, true);
            if (dialogue < 6000 && dialogue > 4000) 
                affichetext("Hello young man.", "voice", 1, true);
            if (dialogue < 8000 && dialogue > 6000) 
                affichetext("I am delighted to welcome you.", "voice", 1, true);
            if (dialogue < 10000 && dialogue > 8000) 
                affichetext("Who are you?", "child", 1, true);
            if (dialogue < 12000 && dialogue > 10000) 
                affichetext("Lets play a game.", "voice", 1, true);
            if (dialogue < 14000 && dialogue > 12000) 
                affichetext("You have one hundred and twenty secondes to find\na way to exit.", "voice", 2, true);
            if (dialogue < 16000 && dialogue > 14000) 
                affichetext("After this time...", "voice", 1, true);
            if (dialogue < 18000 && dialogue > 16000) 
                affichetext("Good Bye.", "voice", 1, true);
            if (dialogue < 20000 && dialogue > 18000) 
                affichetext("But I'm magnanimous.", "voice", 1, true);
            if (dialogue < 22000 && dialogue > 20000) 
                affichetext("I left you a lamp and some advice to help you.", "voice", 1, true);
            if (dialogue < 24000 && dialogue > 22000) 
                affichetext("Listen carrefully.", "voice", 1, true);
            if (dialogue < 32000 && dialogue > 24000) 
                affichetext("My first is a place to sleep.\nMy second is  a game from our childhood.\nMy third is a way to grow up.\nMy fourth is.\nMy fifth is.\nMy sixth is. ", "voice", 6, true);
            if (dialogue < 34000 && dialogue > 32000) 
                affichetext("Good luck.", "voice", 1, true);
            if (dialogue > 34000) 
                controlefin = 0;
        }

        return 1;
    }

    int CalculateContact(Objet m)
    {
        int n = 0;
        for (unsigned int i = 0; i < m.cube.size(); i++)
        {
            n = m_objet.cube[0].calculate_collision(m.cube[i]);
        }
        return n;
    }

    void verifPos(Vector v, int &controlePos, int value)
    {
        if (length(CC.Position - v) < 2 && controleindice == 0 && controlePoss[value] == 0)
        {
            controleindice = 1;
            controlePos = value;
        }
    }

    int update(const float time, const float delta)
    {
        if (controlefin == 5)
        {
            dialogue += delta;
            delay -= delta;
            if (controlefin == 5 && key_state(' ') && delay <= 0)
            {
                delay = 200.f;
                dialogue += 2000 - dialogue % 2000;
            }
        }
        if (controlefin == 0)
        {
            shad.time = time*0.001;
            int n = 0;
            n = CalculateContact(m_terrain);
            n = CalculateContact(lit);

            if (n == 0 && shad.foudreControle>2000)
            {
                CC.update(delta);
            }
            m_view = CC.getCam();

            static int controlePos = -1;

            verifPos(Vector(5.5, 0, 3.5), controlePos, 0);
            verifPos(Vector(0, 0, -6), controlePos, 1);
            verifPos(Vector(-5, 0, -1), controlePos, 2);
            if (controleindice == 1 && controlePoss[controlePos] == 1)
            {
                controleindice = 0;
            }
            if (controleindice == 1 && key_state('e'))
            {
                shad.nbindice++;
                clear_key_state('e');
                controlePoss[controlePos] = 1;
            }
            if (shad.nbindice == 6)
            {
                controlefin = 1;
                aud.pause_audio(aud.getPluie());
            }
            if (countFoudre == 6)
            {
                controlefin = 2;
                aud.pause_audio(aud.getPluie());
                aud.play_audio(aud.getCri());
            }

            static int controleMusic = 0;

            if ((key_state('s') || key_state('z') || key_state('q') || key_state('d')) && controleMusic == 0)
            {
                controleMusic = 1;
                aud.play_audio(aud.getWav());
            }
            if (!(key_state('s') || key_state('z') || key_state('q') || key_state('d')) && controleMusic == 1)
            {
                controleMusic = 0;
                aud.pause_audio(aud.getWav());
            }

            static int controleVase = 0;
            if (length(CC.Position - Vector(5, 0, -6)) < 6 && controleVase == 0)
            {
                aud.play_audio(aud.getVase());
                controleVase = 1;
            }

            static int controleSaut = 0;

            if (length(CC.Position - Vector(5.5, 0, 3.5)) < 4 && controleSaut == 0)
            {
                controleSaut = 1;
                aud.play_audio(aud.getSaut());
            }
            if (!(length(CC.Position - Vector(5.5, 0, 3.5)) < 4) && controleSaut == 1)
            {
                controleSaut = 0;
                aud.pause_audio(aud.getSaut());
            }

            static int j = 20000;
            static int timed = 0;
            static int controle = 0;
            if (controle == 0)
            {
                shad.foudreControle = j;
                controle = 1;
            }

            if (timed < j)
            {
                shad.foudreControle = j - timed;
            }
            else
            {
                j = 20000;
                shad.foudreControle = j;
                timed = 0;
                countFoudre++;
            }
            timed += delta;
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
    Objet Shelf;
    Objet Piano;
    Objet Cape;
    Mesh plane;
    GLuint m_texture;
    GLuint m_text_terrain;
    GLuint m_text_terrain2;
    GLuint text_lit;
    GLuint textChev;
    GLuint textCoussin;
    GLuint tCanape;
    GLuint tVase;
    GLuint tCouteau;
    GLuint textFull;
    GLuint texPlane;
    GLuint texPiano;
    GLuint texCape;
    CharacterController CC;
    Orbiter m_view;
    shader shad;
    audio aud;
    int controleindice = 0;
    int controlefin = 3;
    int controlePoss[6] = {0};
    int dialogue = 0;
    int countFoudre = 0;
    int delay = 0;
};

int main(int argc, char **argv)
{
    // il ne reste plus qu'a creer un objet application et la lancer
    TP tp;
    tp.run();

    return 0;
}
