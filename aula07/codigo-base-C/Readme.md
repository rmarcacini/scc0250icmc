
# Para Windows
* Instalar o ambiente Mingw [http://www.mingw.org/]
* Fazer download do GLFW e copiar os arquivos nos diretórios (bin, include, lib, etc) nos respectivos diretórios do Mingw [https://www.glfw.org/download.html]
* Fazer download do GLEW e copiar os arquivos nos diretórios (bin, include, lib, etc) nos respectivos diretórios do Mingw [http://glew.sourceforge.net/]
* Adicionar o diretório bin do Mingw no PATH do seu sistema operacional. Desta forma, o comando "gcc" estará disponível a partir de um prompt de comando.

* Para compilar: ``gcc aula07_ex01.c -lglfw3dll -lglew32 -lopengl32''

* Observação 1: um ambiente pré-peparado está disponível em http://websensors.net.br/projects/scc0250icmc/mingw64-opengl.zip
* Observação 2: se no momento de execução do programa for solicitado dll do GLFW e GLEW, copie eles do Mingw para o mesmo diretório do seu executável.

# Para linux (Debian e Ubuntu)
* Instalar os pacotes libglfw3-dev, mesa-common-dev, libglew-dev.
* Instalar o ambiente GCC (pacote build-essential)
* Para compilar: `` gcc aula07_ex01.c -lglfw -lGL -lGLEW -lm''
