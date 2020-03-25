import org.lwjgl.*;
import org.lwjgl.glfw.*;
import org.lwjgl.opengl.*;
import java.nio.*;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL45.*;
import static org.lwjgl.system.MemoryUtil.*;

public class Main {


    float angulo = 0.0f;
    float angulo_inc = 0.1f;
    float c = 1.0f;
    float s = 0.0f;

    public void run() {
        // inicicializando o sistema de janelas
        glfwInit();

        // deixando a janela invisivel, por enquanto
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);


        // criando uma janela
        long window = glfwCreateWindow(800, 800, "Minha Janela", NULL, NULL);


        // tornando a janela como principal 
        glfwMakeContextCurrent(window);
        GL.createCapabilities(); // isso eh especifico para Java!


        // GLSL para Vertex Shader
        String vertex_code =
        "attribute vec3 position;\n" +
        "uniform mat4 mat_transformation;\n" +
        "void main()\n" +
        "{\n" +
        "    gl_Position = mat_transformation * vec4(position, 1.0);\n" +
        "}\n";

        // GLSL para Fragment Shader
        String fragment_code =
        "uniform vec4 color;\n" +
        "void main()\n" +
        "{\n" +
        "    gl_FragColor = color;\n" +
        "}\n";

        // Requisitando slot para a GPU para nossos programas Vertex e Fragment Shaders
        int program = glCreateProgram();
        int vertex = glCreateShader(GL_VERTEX_SHADER);
        int fragment = glCreateShader(GL_FRAGMENT_SHADER);

        // Associando nosso código-fonte GLSL aos slots solicitados
        glShaderSource(vertex, vertex_code);
        glShaderSource(fragment, fragment_code);

        // Compilando o Vertex Shader e verificando erros
        glCompileShader(vertex);

        int[] isCompiled = new int[1];
        glGetShaderiv(vertex, GL_COMPILE_STATUS, isCompiled);
        if(isCompiled[0] == GL_FALSE){
            
            System.out.println("Erro de compilacao no Vertex Shader.\n");
            System.out.println(glGetShaderInfoLog(vertex));

        }



        // Compilando o Fragment Shader e verificando erros
        glCompileShader(fragment);

        isCompiled = new int[1];
        glGetShaderiv(fragment, GL_COMPILE_STATUS, isCompiled);
        if(isCompiled[0] == GL_FALSE){
            
            System.out.println("Erro de compilacao no Fragment Shader.\n");
            System.out.println(glGetShaderInfoLog(fragment));

        }

        // Associando os programas compilado ao programa principal
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);

        // Linkagem do programa e definindo como default
        glLinkProgram(program);
        glUseProgram(program);


        // Preparando dados para enviar a GPU        
        FloatBuffer vertices = BufferUtils.createFloatBuffer(72);
        vertices.put(new float[] {
            // Face 1 do Cubo
            -0.2f, -0.2f, +0.2f,
            +0.2f, -0.2f, +0.2f,
            -0.2f, +0.2f, +0.2f,
            +0.2f, +0.2f, +0.2f,

            // Face 2 do Cubo
            +0.2f, -0.2f, +0.2f,
            +0.2f, -0.2f, -0.2f,         
            +0.2f, +0.2f, +0.2f,
            +0.2f, +0.2f, -0.2f,

            // Face 3 do Cubo
            +0.2f, -0.2f, -0.2f,
            -0.2f, -0.2f, -0.2f,            
            +0.2f, +0.2f, -0.2f,
            -0.2f, +0.2f, -0.2f,

            // Face 4 do Cubo
            -0.2f, -0.2f, -0.2f,
            -0.2f, -0.2f, +0.2f,         
            -0.2f, +0.2f, -0.2f,
            -0.2f, +0.2f, +0.2f,

            // Face 5 do Cubo
            -0.2f, -0.2f, -0.2f,
            +0.2f, -0.2f, -0.2f,         
            -0.2f, -0.2f, +0.2f,
            +0.2f, -0.2f, +0.2f,

            // Face 6 do Cubo
            -0.2f, +0.2f, +0.2f,
            +0.2f, +0.2f, +0.2f,           
            -0.2f, +0.2f, -0.2f,
            +0.2f, +0.2f, -0.2f
        }).flip();


        int buffer = glGenBuffers();
        glBindBuffer(GL_ARRAY_BUFFER, buffer);


        // Abaixo, nós enviamos todo o conteúdo da variável vertices.
        glBufferData(GL_ARRAY_BUFFER, vertices, GL_DYNAMIC_DRAW);


        // Associando variáveis do programa GLSL (Vertex Shaders) com nossos dados
        int loc = glGetAttribLocation(program, "position");
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 3, GL_FLOAT, false, 0, 0); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml

        // Associando variávels do programa GLSL (Fragment Shader) para definir cores
        int loc_color = glGetUniformLocation(program, "color");
        
        
     



        // Exibindo nossa janela
        glfwShowWindow(window);
        glEnable(GL_DEPTH_TEST);// ### importante para 3D

        while (!glfwWindowShouldClose(window))
        {



            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


   
            angulo += angulo_inc;

            // definindo a matriz de translacao (na realidade eh um vetor, mas o OpenGl processa como matriz 4x4)


            // definindo a matriz de rotacao (na realidade eh um vetor, mas o OpenGl processa como matriz 4x4)
            c = (float)Math.cos(Math.toRadians(angulo));
            s = (float)Math.sin(Math.toRadians(angulo));

            FloatBuffer mat_rotation_z = BufferUtils.createFloatBuffer(16);
            mat_rotation_z.put(new float[] {
                c   , -s  , 0.0f, 0.0f,
                s   ,  c  , 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            }).flip();

            
            FloatBuffer mat_rotation_x = BufferUtils.createFloatBuffer(16);
            mat_rotation_x.put(new float[] {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f,  c  ,  -s , 0.0f,
                0.0f,  s  ,   c , 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            }).flip();
            
            
            FloatBuffer mat_rotation_y = BufferUtils.createFloatBuffer(16);
            mat_rotation_y.put(new float[] {
                c   , 0.0f,   s , 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                -s  , 0.0f,   c , 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            }).flip();
            
            FloatBuffer mat_transformation = multiplica(mat_rotation_z,mat_rotation_x);
            mat_transformation = multiplica(mat_rotation_y,mat_transformation);


            // enviando a matriz de transformacao para a GPU
            loc = glGetUniformLocation(program, "mat_transformation");
            glUniformMatrix4fv(loc, true, mat_transformation);

            glUniform4f(loc_color, 1.0f, 0.0f, 0.0f, 1.0f);// ### vermelho
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glUniform4f(loc_color, 0.0f, 0.0f, 1.0f, 1.0f);// ### azul
            glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

            glUniform4f(loc_color, 0.0f, 1.0f, 0.0f, 1.0f);// ### verde
            glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

            glUniform4f(loc_color, 1.0f, 1.0f, 0.0f, 1.0f);// ### amarela
            glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);


            glUniform4f(loc_color, 0.5f, 0.5f, 0.5f, 1.0f);// ### cinza
            glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);

            glUniform4f(loc_color, 0.5f, 0.0f, 0.0f, 1.0f);// ### marrom
            glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

            glfwSwapBuffers(window);

        }

        glfwDestroyWindow(window);

        glfwTerminate();
    }


    
    /** multiplica duas matrizes 4x4 (obs as matrizes na realidade sao vetores de 16 elementos ) */
    private FloatBuffer multiplica(FloatBuffer m1, FloatBuffer m2){

        // recuperando os valores das matrizes (em um vetor de 16 elementos)
        float[] v1 = new float[m1.limit()];
        m1.get(v1);

        float[] v2 = new float[m2.limit()];
        m2.get(v2);

        // gerando as matrizes para multiplicacao
        float[][] m_a = new float[4][4];
        float[][] m_b = new float[4][4];
        float[][] m_c = new float[4][4]; // m_c = m_a * m_b

        int n = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m_a[i][j] = v1[n];
                m_b[i][j] = v2[n];
                n += 1;
            }
        }

        // multiplicando...
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m_c[i][j] = 0.0f;
                for (int k = 0; k < 4; k++) {
                    m_c[i][j] += m_a[i][k] * m_b[k][j];
                }
            }
        }

        // voltando a resposta para o formato do OpenGL
        n = 0;
        float[] m_resultado = new float[16];
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m_resultado[n] = m_c[i][j];
                System.out.print(m_c[i][j] + " \t");
                n += 1;
            }
            System.out.println("");
        }
        System.out.println("-----------------------------------\n");

        FloatBuffer mat_transformation = BufferUtils.createFloatBuffer(16);
        mat_transformation.put(m_resultado).flip();

        return mat_transformation;
    }
    
    public static void main(String[] args) {
        new Main().run();
    }

}