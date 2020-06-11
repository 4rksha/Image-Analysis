#include "shader.h"

#include "draw.h"

#include "program.h"


void shader::init()
{
    program= 0;
    program_filename= Filename("data/projet/shader/shader7.glsl");
    reloade_program();
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    glClearDepth(1.f);
    
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glDisable(GL_CULL_FACE);
    
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
}

void shader::reloade_program( )
{
    if(program == 0)
        program= read_program(program_filename);
    else
        reload_program(program, program_filename);
    
    // recupere les erreurs, si necessaire
    program_area= program_format_errors(program, program_log);
    
    if(program_log.size() > 0)
        printf("[boom]\n%s\n", program_log.c_str());
    
    program_failed= (program_log.size() > 0);
}

void shader::quit()
{
    release_program(program);
}

void shader::edraw(Mesh &mesh,Transform T,Orbiter & camera,GLuint & texture,Point luxPosition,Point Direction)
{
    // recupere les transformations
    
    Transform model= T;
    Transform view= camera.view();
    Transform projection= camera.projection(window_width(), window_height(), 45);
    Transform viewport= Viewport(window_width(), window_height());
    
    Transform mvp= projection * view * model;
    Transform mvpInv= Inverse(mvp);
    Transform mv= model * view;
    
    GLuint vao= mesh.create_buffers(mesh.has_texcoord(), mesh.has_normal(), mesh.has_color());
        
    // configuration minimale du pipeline
    glBindVertexArray(vao);
    glUseProgram(program);

    // affecte une valeur aux uniforms
    // transformations standards
    program_uniform(program, "modelMatrix", model);
    program_uniform(program, "viewInvMatrix", view.inverse());

    
    program_uniform(program, "mvpMatrix", mvp);
    
    program_uniform(program, "source", luxPosition);
    program_uniform(program,"direction",Point(0,0,0)-Direction);

    char uniform[1024];
    sprintf(uniform, "diffuse_color");
    program_use_texture(program, uniform, 1, texture);
    
    
    // textures
    
    // go
    glDrawArrays(GL_TRIANGLES, 0,mesh.vertex_count());
    //draw(mesh, T ,camera);
}