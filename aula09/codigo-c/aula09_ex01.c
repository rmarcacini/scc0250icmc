/* para linux, instalar os pacotes libglfw3-dev mesa-common-dev libglew-dev */
/* para compilar no linux: gcc aula09_ex01.c -lglfw -lGL -lGLEW -lm */

/* para windows, instalar bibliotecas compiladas do glfw3 e glew no ambiente mingw */
/* para compilar no windows: gcc aula09_ex01.c -lglfw3dll -lglew32 -lopengl32 */


#include <GL/glew.h>  

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> /* verifique no seu SO onde fica o glfw3.h */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

 

// variaveis globais
typedef struct{
    float x, y, z;
} coordenadas;


float angulo = 0.0f;
float angulo_inc = 0.5f;

float c = 1.0f; // armazenar cosseno (da matriz de rotacao)
float s = 0.0f; // armazenar seno  (da matriz de rotcao)
 

 


// funcao para multiplicar duas "matrizes 4x4" (na realidade sao vetores de 16 elementos)
void multiplica(float *m1, float *m2, float *m_resultado){

    // OpenGL lida recebe vetores de 16 elementos e interpreta como matrizes 4x4.
    // Nessa funcao, transformamos as matrizes de volta para float[4][4] para facilitar a multiplicacao

    float m_a[4][4];
    float m_b[4][4];
    float m_c[4][4]; // m_c = m_a * m_b

    int n = 0;
    for(int i=0; i < 4; i++){
        for(int j=0; j < 4; j++){
            m_a[i][j] = m1[n];
            m_b[i][j] = m2[n];
            n += 1;
        }
    }


    for(int i=0;i<4;i++){    
        for(int j=0;j<4;j++){
            m_c[i][j]=0.0;
            for(int k=0; k < 4; k++){
                m_c[i][j] += m_a[i][k]*m_b[k][j];    
            }
        }
    }

    // voltando a resposta para o formato do OpenGL
    n = 0;
    for(int i=0; i < 4; i++){
        for(int j=0; j < 4; j++){
            m_resultado[n] = m_c[i][j];
            printf("%f \t",m_c[i][j]);
            n += 1;
        }
        printf("\n");
    }
    printf("-----------------------------------\n");


}
 
int main(void){
 
    // inicicializando o sistema de\ janelas
    glfwInit();

    // deixando a janela invisivel, por enquanto
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

 
    // criando uma janela
    GLFWwindow* window = glfwCreateWindow(800, 800, "Cubo", NULL, NULL);

    
    // tornando a janela como principal 
    glfwMakeContextCurrent(window);

    // inicializando Glew (para lidar com funcoes OpenGL)
    GLint GlewInitResult = glewInit();
    printf("GlewStatus: %s", glewGetErrorString(GlewInitResult));


    // GLSL para Vertex Shader
    char* vertex_code =
    "attribute vec3 position;\n"
    "uniform mat4 mat_transformation;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = mat_transformation * vec4(position, 1.0);\n"
    "}\n";

    // GLSL para Fragment Shader
    char* fragment_code =
    "uniform vec4 color;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = color;\n"
    "}\n";

    // Requisitando slot para a GPU para nossos programas Vertex e Fragment Shaders
    GLuint program = glCreateProgram();
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

    // Associando nosso código-fonte GLSL aos slots solicitados
    glShaderSource(vertex, 1, &vertex_code, NULL);
    glShaderSource(fragment, 1, &fragment_code, NULL);

    // Compilando o Vertex Shader e verificando erros
    glCompileShader(vertex);

    GLint isCompiled = 0;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE){

        //descobrindo o tamanho do log de erro
        int infoLength = 512;
        glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &infoLength);

        //recuperando o log de erro e imprimindo na tela
        char info[infoLength];
        glGetShaderInfoLog(vertex, infoLength, NULL, info);

        printf("Erro de compilacao no Vertex Shader.\n");
        printf("--> %s\n",&info);

    }

    

    // Compilando o Fragment Shader e verificando erros
    glCompileShader(fragment);

    isCompiled = 0;
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE){

        //descobrindo o tamanho do log de erro
        int infoLength = 512;
        glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &infoLength);

        //recuperando o log de erro e imprimindo na tela
        char info[infoLength];
        glGetShaderInfoLog(fragment, infoLength, NULL, info);

        printf("Erro de compilacao no Fragment Shader.\n");
        printf("--> %s\n",&info);

    }

    // Associando os programas compilado ao programa principal
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    // Linkagem do programa e definindo como default
    glLinkProgram(program);
    glUseProgram(program);
 

    // Preparando dados para enviar a GPU
    coordenadas vertices[24] = { // criando tres vertices e preenchendo
        // Face 1 do Cubo
        {-0.2f, -0.2f, +0.2f},
        {+0.2f, -0.2f, +0.2f},
        {-0.2f, +0.2f, +0.2f},
        {+0.2f, +0.2f, +0.2f},

        // Face 2 do Cubo
        {+0.2f, -0.2f, +0.2f},
        {+0.2f, -0.2f, -0.2f},         
        {+0.2f, +0.2f, +0.2f},
        {+0.2f, +0.2f, -0.2f},
        
        // Face 3 do Cubo
        {+0.2f, -0.2f, -0.2f},
        {-0.2f, -0.2f, -0.2f},            
        {+0.2f, +0.2f, -0.2f},
        {-0.2f, +0.2f, -0.2f},

        // Face 4 do Cubo
        {-0.2f, -0.2f, -0.2f},
        {-0.2f, -0.2f, +0.2f},         
        {-0.2f, +0.2f, -0.2f},
        {-0.2f, +0.2f, +0.2f},

        // Face 5 do Cubo
        {-0.2f, -0.2f, -0.2f},
        {+0.2f, -0.2f, -0.2f},         
        {-0.2f, -0.2f, +0.2f},
        {+0.2f, -0.2f, +0.2f},
        
        // Face 6 do Cubo
        {-0.2f, +0.2f, +0.2f},
        {+0.2f, +0.2f, +0.2f},           
        {-0.2f, +0.2f, -0.2f},
        {+0.2f, +0.2f, -0.2f}
    };




    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);


    // Abaixo, nós enviamos todo o conteúdo da variável vertices.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // Associando variáveis do programa GLSL (Vertex Shaders) com nossos dados
    GLint loc = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
 

    // Associando variávels do programa GLSL (Fragment Shader) para definir cores
    GLint loc_color = glGetUniformLocation(program, "color");


    // Exibindo nossa janela
    glfwShowWindow(window);


    glEnable(GL_DEPTH_TEST);// ### importante para 3D

    while (!glfwWindowShouldClose(window))
    {



        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0, 1.0, 1.0, 1.0);



        angulo += angulo_inc;



        // definindo a matriz de rotacao (na realidade eh um vetor, mas o OpenGl processa como matriz 4x4)
        c = cos( ((angulo) * M_PI / 180.0) ); // cos considerando conversao para radianos
        s = sin( ((angulo) * M_PI / 180.0) );

        float mat_rotation_z[16] = {
            c   , -s  , 0.0f, 0.0f,
            s   ,  c  , 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        float mat_rotation_x[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f,  c  ,  -s , 0.0f,
            0.0f,  s  ,   c , 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        float mat_rotation_y[16] = {
            c   , 0.0f,   s , 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -s  , 0.0f,   c , 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        float mat_transformation[16] = {
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f
        };


        multiplica(mat_rotation_z,mat_rotation_x,&mat_transformation);
        multiplica(mat_rotation_y,mat_transformation,&mat_transformation);

        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

        // enviando a matriz de transformacao para a GPU
        loc = glGetUniformLocation(program, "mat_transformation");
        glUniformMatrix4fv(loc, 1, GL_TRUE, mat_transformation);

        glUniform4f(loc_color, 1.0, 0.0, 0.0, 1.0);// ### vermelho
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        glUniform4f(loc_color, 0.0, 0.0, 1.0, 1.0);// ### azul
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        
        glUniform4f(loc_color, 0.0, 1.0, 0.0, 1.0);// ### verde
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
        
        glUniform4f(loc_color, 1.0, 1.0, 0.0, 1.0);// ### amarela
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        
        
        glUniform4f(loc_color, 0.5, 0.5, 0.5, 1.0);// ### cinza
        glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
        
        glUniform4f(loc_color, 0.5, 0.0, 0.0, 1.0);// ### marrom
        glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
 
        glfwSwapBuffers(window);
        
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
