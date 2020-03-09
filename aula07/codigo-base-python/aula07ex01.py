#!/usr/bin/env python
# coding: utf-8

# # Aua07.Ex01 - Praticando transformação geométrica 2D

# ### Primeiro, vamos importar as bibliotecas necessárias.
# Verifique no código anterior um script para instalar as dependências necessárias (OpenGL e GLFW) antes de prosseguir.

# In[49]:


import glfw
from OpenGL.GL import *
import OpenGL.GL.shaders
import numpy as np
import math


# ### Inicializando janela

# In[50]:


glfw.init()
glfw.window_hint(glfw.VISIBLE, glfw.FALSE);
window = glfw.create_window(800, 800, "Transformação Geométrica", None, None)
glfw.make_context_current(window)


# ### GLSL (OpenGL Shading Language)
# 
# Aqui veremos nosso primeiro código GLSL.
# 
# É uma linguagem de shading de alto nível baseada na linguagem de programação C.
# 
# Nós estamos escrevendo código GLSL como se "strings" de uma variável (mas podemos ler de arquivos texto). Esse código, depois, terá que ser compilado e linkado ao nosso programa. 
# 
# Iremos aprender GLSL conforme a necessidade do curso. Usarmos uma versão do GLSL mais antiga, compatível com muitos dispositivos.

# ### GLSL para Vertex Shader
# 
# No Pipeline programável, podemos interagir com Vertex Shaders.
# 
# No código abaixo, estamos fazendo o seguinte:
# 
# * Definindo uma variável chamada position do tipo vec2.
# * Definindo uma variável chamada mat_transformation do tipo mat4 (matriz 4x4).
# * Usamos vec2, pois nosso programa (na CPU) irá enviar apenas duas coordenadas para plotar um ponto. Podemos mandar três coordenadas (vec3) e até mesmo quatro coordenadas (vec4).
# * void main() é o ponto de entrada do nosso programa (função principal)
# * gl_Position é uma variável especial do GLSL. Variáveis que começam com 'gl_' são desse tipo. Nesse caso, determina a posição de um vértice. Observe que todo vértice tem 4 coordenadas, por isso nós combinamos nossa variável vec2 com uma variável vec4. Além disso, nós modificamos nosso vetor com base em uma matriz de transformação, conforme estudado na Aula05.

# In[51]:


vertex_code = """
        attribute vec2 position;
        uniform mat4 mat;
        void main(){
            gl_Position = mat * vec4(position,0.0,1.0);
        }
        """


# ### GLSL para Fragment Shader
# 
# No Pipeline programável, podemos interagir com Fragment Shaders.
# 
# No código abaixo, estamos fazendo o seguinte:
# 
# * void main() é o ponto de entrada do nosso programa (função principal)
# * gl_FragColor é uma variável especial do GLSL. Variáveis que começam com 'gl_' são desse tipo. Nesse caso, determina a cor de um fragmento. Nesse caso é um ponto, mas poderia ser outro objeto (ponto, linha, triangulos, etc).

# ### Possibilitando modificar a cor.
# 
# Nos exemplos anteriores, a variável gl_FragColor estava definida de forma fixa (com cor R=0, G=0, B=0).
# 
# Agora, nós vamos criar uma variável do tipo "uniform", de quatro posições (vec4), para receber o dado de cor do nosso programa rodando em CPU.

# In[52]:


fragment_code = """
        void main(){
            gl_FragColor = vec4(1.0,0.0,0.0,1.0);
        }
        """


# ### Requisitando slot para a GPU para nossos programas Vertex e Fragment Shaders

# In[53]:


# Request a program and shader slots from GPU
program  = glCreateProgram()
vertex   = glCreateShader(GL_VERTEX_SHADER)
fragment = glCreateShader(GL_FRAGMENT_SHADER)


# ### Associando nosso código-fonte aos slots solicitados

# In[54]:


# Set shaders source
glShaderSource(vertex, vertex_code)
glShaderSource(fragment, fragment_code)


# ### Compilando o Vertex Shader
# 
# Se há algum erro em nosso programa Vertex Shader, nosso app para por aqui.

# In[55]:


# Compile shaders
glCompileShader(vertex)
if not glGetShaderiv(vertex, GL_COMPILE_STATUS):
    error = glGetShaderInfoLog(vertex).decode()
    print(error)
    raise RuntimeError("Erro de compilacao do Vertex Shader")


# ### Compilando o Fragment Shader
# 
# Se há algum erro em nosso programa Fragment Shader, nosso app para por aqui.

# In[56]:


glCompileShader(fragment)
if not glGetShaderiv(fragment, GL_COMPILE_STATUS):
    error = glGetShaderInfoLog(fragment).decode()
    print(error)
    raise RuntimeError("Erro de compilacao do Fragment Shader")


# ### Associando os programas compilado ao programa principal

# In[57]:


# Attach shader objects to the program
glAttachShader(program, vertex)
glAttachShader(program, fragment)


# ### Linkagem do programa

# In[58]:


# Build program
glLinkProgram(program)
if not glGetProgramiv(program, GL_LINK_STATUS):
    print(glGetProgramInfoLog(program))
    raise RuntimeError('Linking error')
    
# Make program the default program
glUseProgram(program)


# ### Preparando dados para enviar a GPU
# 
# Nesse momento, nós compilamos nossos Vertex e Program Shaders para que a GPU possa processá-los.
# 
# Por outro lado, as informações de vértices geralmente estão na CPU e devem ser transmitidas para a GPU.
# 

# In[59]:


# preparando espaço para 3 vértices usando 2 coordenadas (x,y)
vertices = np.zeros(3, [("position", np.float32, 2)])


# In[60]:


# preenchendo as coordenadas de cada vértice
vertices['position'] = [
                            ( 0.00, +0.05), 
                            (-0.05, -0.05), 
                            (+0.05, -0.05)
    
                        ]


# ### Para enviar nossos dados da CPU para a GPU, precisamos requisitar um slot.

# In[61]:


# Request a buffer slot from GPU
buffer = glGenBuffers(1)
# Make this buffer the default one
glBindBuffer(GL_ARRAY_BUFFER, buffer)


# ### Abaixo, nós enviamos todo o conteúdo da variável vertices.
# 
# Veja os parâmetros da função glBufferData [https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml]

# In[62]:


# Upload data
glBufferData(GL_ARRAY_BUFFER, vertices.nbytes, vertices, GL_DYNAMIC_DRAW)
glBindBuffer(GL_ARRAY_BUFFER, buffer)


# ### Associando variáveis do programa GLSL (Vertex Shaders) com nossos dados
# 
# Primeiro, definimos o byte inicial e o offset dos dados.

# In[63]:


# Bind the position attribute
# --------------------------------------
stride = vertices.strides[0]
offset = ctypes.c_void_p(0)


# Em seguida, soliciamos à GPU a localização da variável "position" (que guarda coordenadas dos nossos vértices). Nós definimos essa variável no Vertex Shader.

# In[64]:


loc = glGetAttribLocation(program, "position")
glEnableVertexAttribArray(loc)


# A partir da localização anterior, nós indicamos à GPU onde está o conteúdo (via posições stride/offset) para a variável position (aqui identificada na posição loc).
# 
# Outros parâmetros:
# 
# * Definimos que possui duas coordenadas
# * Que cada coordenada é do tipo float (GL_FLOAT)
# * Que não se deve normalizar a coordenada (False)
# 
# Mais detalhes: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml

# In[65]:


glVertexAttribPointer(loc, 2, GL_FLOAT, False, stride, offset)


# ### Capturando eventos de teclado e modificando variáveis para a matriz de transformação

# In[66]:


# translacao
x_inc = 0.0
y_inc = 0.0
r_inc = 0.0


def key_event(window,key,scancode,action,mods):
    global x_inc, y_inc, r_inc
    
    if key == 263: x_inc -= 0.0001
    if key == 262: x_inc += 0.0001

    if key == 265: y_inc += 0.0001
    if key == 264: y_inc -= 0.0001
        
    if key == 65: r_inc += 0.1
    if key == 83: r_inc -= 0.1
        
    #print(key)
    
glfw.set_key_callback(window,key_event)


# ### Nesse momento, nós exibimos a janela!
# 

# In[67]:


glfw.show_window(window)


# ### Loop principal da janela.
# Enquanto a janela não for fechada, esse laço será executado. É neste espaço que trabalhamos com algumas interações com a OpenGL.
# 
# A novidade agora é a função glDrawArrays()
# 
# Tal função recebe o tipo de primitiva (GL_TRIANGLES), o índice inicial do array de vértices (vamos mostrar todos os três vértices, por isso começamos com 0) e a quantidade de vértices ( len(vertices) ).

# In[68]:


t_x = 0.0
t_y = 0.0
angulo = 0.0


def multiplica_matriz(a,b):
    m_a = a.reshape(4,4)
    m_b = b.reshape(4,4)
    m_c = np.dot(m_a,m_b)
    c = m_c.reshape(1,16)
    return c


while not glfw.window_should_close(window):

    t_x += x_inc
    t_y += y_inc
    angulo += r_inc
    
    c = math.cos( math.radians(angulo) )
    s = math.sin( math.radians(angulo) )
    
    glfw.poll_events() 

    
    glClear(GL_COLOR_BUFFER_BIT) 
    glClearColor(1.0, 1.0, 1.0, 1.0)
    
    
    #Draw Triangle
    
    mat_rotation = np.array([  c  , -s , 0.0, 0.0, 
                               s  , c  , 0.0, 0.0, 
                               0.0, 0.0, 1.0, 0.0, 
                               0.0, 0.0, 0.0, 1.0], np.float32)
    
    mat_translation = np.array([  1.0, 0.0, 0.0, t_x, 
                                  0.0, 1.0, 0.0, t_y, 
                                  0.0, 0.0, 1.0, 0.0, 
                                  0.0, 0.0, 0.0, 1.0], np.float32)
    


    mat_transform = multiplica_matriz(mat_translation,mat_rotation)
    

    
    loc = glGetUniformLocation(program, "mat")
    glUniformMatrix4fv(loc, 1, GL_TRUE, mat_transform)
    
    glDrawArrays(GL_TRIANGLES, 0, len(vertices))

    glfw.swap_buffers(window)

glfw.terminate()

