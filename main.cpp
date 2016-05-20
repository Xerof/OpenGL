#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

/*static const char* vertex_shader =
"#version 130\n"
"in vec3 vp;"
"void main() {"
"    gl_Position = vec4(vp, 1.0);"
"}";

static const char* fragment_shader =
"#version 130\n"
"out vec4 frag_colour;"
"void main() {"
"    frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
"}";
*/
static std::string readShader (const char *file_name)
{
    std::string contenido;

   if(file_name == NULL)
       std::cout<<"File name is NULL";

    std::ifstream file(file_name, std::ios::in);
    std::string line;

    if(!file.is_open())
    {
        std::cout<<"Can't open the file" << file_name << std::endl;
        return "";
    }

    std::string linea;
    while(!file.eof())
    {
        std::getline(file, line);
        contenido.append(line + "\n");
    }

    file.close();
    return contenido;
}
static bool isShaderCompiled(GLint shader)
{
    GLint Compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &Compiled);
    if(Compiled == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        if (length > 0)
        {
            GLint infoLength;
            char* infoBuf = (char*) malloc(sizeof(char) * length);
            glGetShaderInfoLog(shader, length, &infoLength, infoBuf);
            std::cout << infoBuf << std::endl;
            free(infoBuf);
        }
        return false;
    }

    return true;

}
static GLuint loadShader(GLenum type, const char *vertexPath, const char *fragmentPath)
{
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    std::string vertex_shader = readShader(vertexPath);
    const char *vertexSrc = vertex_shader.c_str();

    glShaderSource (vs, 1, &vertexSrc, NULL);
    glCompileShader(vs);

    if(!isShaderCompiled(vs))
    {
        std::cout<<"Can't compile " << vertex_shader << std::endl;
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragment_shader = readShader(fragmentPath);
    const char *fragmentSrc = fragment_shader.c_str();
    glShaderSource(fs, 1, &fragmentSrc, NULL);
    glCompileShader(fs);

    if(!isShaderCompiled(vs))
    {
        std::cout<<"Can't compile " << fragment_shader << std::endl;
    }

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);

    return shader_programme;
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    //glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
     std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
         std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
             std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

        float points[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
    };

float points2[] = {
        -0.45f,  0.45f,
        0.45f,  0.45f,
        0.45f, -0.45f,
        -0.45f, -0.45f,
    };

    GLuint vbo = 0;
    glGenBuffers (1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points2) , points2, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    //glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLuint program = loadShader(GL_GEOMETRY_SHADER, "linesShader.txt", "fragment_shader.txt");
    glLinkProgram (program);

    glEnableVertexAttribArray(0);
    GLint possAttrib = glGetAttribLocation(program, "pos");
    glVertexAttribPointer (possAttrib, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /*float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);*/
        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, 4);
        /*
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();*/
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
