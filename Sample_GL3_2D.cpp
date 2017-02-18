#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <random>
#include <bits/stdc++.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

typedef pair<int,int>   II;
typedef vector< II >      VII;
typedef vector<int>     VI;
typedef vector< VI > 	VVI;
typedef long long int 	LL;

#define PB push_back
#define MP make_pair
#define F first
#define S second
#define SZ(a) (int)(a.size())
#define ALL(a) a.begin(),a.end()
#define SET(a,b) memset(a,b,sizeof(a))

#define din(n) scanf("%d",&n)
#define dout(n) printf("%d\n",n)
#define llin(n) scanf("%lld",&n)
#define llout(n) printf("%lld\n",n)
#define strin(n) scanf(" %s",n)
#define strout(n) printf("%s\n",n)
#define fast_io ios_base::sync_with_stdio(false);cin.tie(NULL)

#define TRACE

#ifdef TRACE
#define trace(...) __f(#__VA_ARGS__, __VA_ARGS__)
template <typename Arg1>
void __f(const char* name, Arg1&& arg1){
	cerr << name << " : " << arg1 << std::endl;
}
template <typename Arg1, typename... Args>
void __f(const char* names, Arg1&& arg1, Args&&... args){
	const char* comma = strchr(names + 1, ',');cerr.write(names, comma - names) << " : " << arg1<<" | ";__f(comma+1, args...);
}
#else
#define trace(...)
#endif

int score = 0 ,c_destroyed = 0,d_destroyed = 0,c_col = 0, d_col = 0;

struct VAO {
	GLuint VertexArrayID;
	GLuint VertexBuffer;
	GLuint ColorBuffer;

	GLenum PrimitiveMode;
	GLenum FillMode;
	int NumVertices;
	int xpos,ypos;
	double x1,x2,y1,y2;
	double angle;
	int color;
};
typedef struct VAO VAO;

typedef struct Rays
{
	GLuint VertexArrayID;
	GLuint VertexBuffer;
	GLuint ColorBuffer;

	GLenum PrimitiveMode;
	GLenum FillMode;
	int NumVertices;
	int x,y;
	double angle;	
	double length;
}rays;
struct GLMatrices {
	glm::mat4 projection;
	glm::mat4 model;
	glm::mat4 view;
	GLuint MatrixID;
} Matrices;

typedef struct objects {
	GLfloat vertex[18];
	GLfloat color[18];
} OBJECTS;


GLuint programID;

/* Function to load Shaders - Use it as it is */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

	// Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void quit(GLFWwindow *window)
{
	glfwDestroyWindow(window);
	glfwTerminate();
}
void finalQuit()
{
	printf("#############################\n#############################\n");
	printf("\nSCORE: %d\n",score );
	printf("CORRECT BLOCKS COLLECTED: %d\n",c_col );
	printf("WRONG BLOCKS COLLECTED: %d\n",d_col );
	printf("CORRECT BLOCKS DESTROYED: %d\n", c_destroyed);
	printf("DANGEROUS BLOCKS DESTROYED: %d\n\n",d_destroyed );
	printf("#############################\n#############################\n");
	exit(1);
}

/* Generate VAO, VBOs and return VAO handle */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data, GLenum fill_mode=GL_FILL)
{
	struct VAO* vao = new struct VAO;
	vao->PrimitiveMode = primitive_mode;
	vao->NumVertices = numVertices;
	vao->FillMode = fill_mode;

	// Create Vertex Array Object
	// Should be done after CreateWindow and before any other GL calls
	glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
	glGenBuffers (1, &(vao->VertexBuffer)); // VBO - vertices
	glGenBuffers (1, &(vao->ColorBuffer));  // VBO - colors

	glBindVertexArray (vao->VertexArrayID); // Bind the VAO 
	glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); // Bind the VBO vertices 
	glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
	glVertexAttribPointer(
			0,                  // attribute 0. Vertices
			3,                  // size (x,y,z)
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

	glBindBuffer (GL_ARRAY_BUFFER, vao->ColorBuffer); // Bind the VBO colors 
	glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), color_buffer_data, GL_STATIC_DRAW);  // Copy the vertex colors
	glVertexAttribPointer(
			1,                  // attribute 1. Color
			3,                  // size (r,g,b)
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

	return vao;
}

/* Generate VAO, VBOs and return VAO handle - Common Color for all vertices */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode=GL_FILL)
{
	GLfloat* color_buffer_data = new GLfloat [3*numVertices];
	for (int i=0; i<numVertices; i++) {
		color_buffer_data [3*i] = red;
		color_buffer_data [3*i + 1] = green;
		color_buffer_data [3*i + 2] = blue;
	}

	return create3DObject(primitive_mode, numVertices, vertex_buffer_data, color_buffer_data, fill_mode);
}

/* Render the VBOs handled by VAO */
void draw3DObject (struct VAO* vao)
{
	// Change the Fill Mode for this object
	glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);

	// Bind the VAO to use
	glBindVertexArray (vao->VertexArrayID);

	// Enable Vertex Attribute 0 - 3d Vertices
	glEnableVertexAttribArray(0);
	// Bind the VBO to use
	glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

	// Enable Vertex Attribute 1 - Color
	glEnableVertexAttribArray(1);
	// Bind the VBO to use
	glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);

	// Draw the geometry !
	glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
}

/**************************
 * Customizable functions *
 **************************/

float bucket_pos1 = 0;
float bucket_pos2 = 0;
float rectangle_rot_dir = 1;
bool triangle_rot_status = true;
bool rectangle_rot_status = true;
int press_a = 0;
int press_d =0,press_s=0,press_w=0,press_f=0,press_left=0,press_right=0,press_down=0,press_up=0,press_space = 0;
int ctrl = 0;
int alt =0;
int laser_pos = 0;
int laser_rotation = 0;
int mouse_press = 0;
int prevMouse_x = 0;
int prevMouse_y = 0;
float angle = 0;
int mouse_left_click = 0;
VAO *batteryBox, *batteryNib, *cell[4];
int box_speed = 1;
double maxCoord = 400,xCap = 0,yCap = 0;
int selected = 0;

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */
void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Function is called first on GLFW_PRESS.


	if (action == GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_A:
			press_a = 0;
			break;
			case GLFW_KEY_D:
			press_d = 0;
			break;
			case GLFW_KEY_RIGHT_CONTROL:
			ctrl = 0;
			break;
			case GLFW_KEY_LEFT_CONTROL:
			ctrl = 0;
			break;
			case GLFW_KEY_RIGHT_ALT:
			alt = 0;
			break;
			case GLFW_KEY_LEFT_ALT:
			alt = 0;
			break;	
			case GLFW_KEY_W:
			press_w = 0;
			break;
			case GLFW_KEY_S:
			press_s = 0;
			break;
			case GLFW_KEY_F:
			press_f=0;
			break;
			case GLFW_KEY_LEFT:
			press_left = 0;
			break;
			case GLFW_KEY_RIGHT:
			press_right = 0;
			break;
			case GLFW_KEY_UP:
			press_up = 0;
			break;
			case GLFW_KEY_DOWN:
			press_down = 0;
			break;
			case GLFW_KEY_SPACE:
			press_space = 0;
			break;
			default:
			break;
		}
	}
	else if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_ESCAPE:
			finalQuit();
			break;
			case GLFW_KEY_A:
			press_a = 1;
			break;
			case GLFW_KEY_D:
			press_d = 1;
			break;
			case GLFW_KEY_RIGHT_CONTROL:
			ctrl = 1;
			break;
			case GLFW_KEY_LEFT_CONTROL:
			ctrl = 1;
			break;
			case GLFW_KEY_RIGHT_ALT:
			alt = 1;
			break;
			case GLFW_KEY_LEFT_ALT:
			alt = 1;
			break;
			case GLFW_KEY_W:
			press_w = 1;
			break;
			case GLFW_KEY_S:
			press_s = 1;
			break;
			case GLFW_KEY_F:
			press_f=1;
			break;
			case GLFW_KEY_LEFT:
			press_left = 1;
			break;
			case GLFW_KEY_RIGHT:
			press_right = 1;
			break;
			case GLFW_KEY_UP:
			press_up = 1;
			break;
			case GLFW_KEY_DOWN:
			press_down = 1;
			break;
			case GLFW_KEY_M:
			box_speed++;
			break;
			case GLFW_KEY_N:
			box_speed--;
			if(box_speed<1)
				box_speed = 1;
			break;
			case GLFW_KEY_SPACE:
			press_space = 1;
			break;

			default:
			break;
		}
	}
}

/* Executed for character input (like in text boxes) */
void keyboardChar (GLFWwindow* window, unsigned int key)
{
	switch (key) {
		case 'Q':
		case 'q':
		finalQuit();
		break;
		default:
		break;
	}
}

/* Executed when a mouse button is pressed/released */
double mouse_x,mouse_y;
void mouseButton (GLFWwindow* window, int button, int action, int mods)
{
	switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
		if (action == GLFW_RELEASE)
		{
			mouse_press = 0;
		}
		else if (action == GLFW_PRESS) {
			mouse_press = 1;
			glfwGetCursorPos(window,&mouse_x,&mouse_y);
			double x= -400 + mouse_x;
			double y = 400 - mouse_y;

			if(x <-170+bucket_pos1 && x>-330+bucket_pos1 && y<-350 )
			{
				selected = 1;
			}

			else if(x<330+bucket_pos2 && x>170+bucket_pos2 && y<-350)
			{
				selected = 2;
			}
			else if(x<-350 && y<laser_pos+10 && y>laser_pos-10)
			{
				selected = 0;

			}
		}
		break;
		case GLFW_MOUSE_BUTTON_RIGHT:
		if(action == GLFW_RELEASE)
			mouse_left_click = 0;
		else if(action == GLFW_PRESS)
			mouse_left_click = 1;
		default:
		break;
	}
}


/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
double max(double a,double b)
{
	if(a>b)
		return a;
	return b;
}
double min(double a, double b)
{
	if(a<b)
		return a;
	return b;
}

void reshapeWindow (GLFWwindow* window, int width, int height)
{
	int fbwidth=width, fbheight=height;
	/* With Retina display on Mac OS X, GLFW's FramebufferSize
	   is different from WindowSize */
	glfwGetFramebufferSize(window, &fbwidth, &fbheight);

	GLfloat fov = 90.0f;

	// sets the viewport of openGL renderer
	glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);

	// set the projection matrix as perspective
	/* glMatrixMode (GL_PROJECTION);
	   glLoadIdentity ();
	   gluPerspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1, 500.0); */
	// Store the projection matrix in a variable for future use
	// Perspective projection for 3D views
	// Matrices.projection = glm::perspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 500.0f);

	// Ortho projection for 2D views
	Matrices.projection = glm::ortho(-1*maxCoord+xCap, maxCoord+xCap, -maxCoord+yCap, maxCoord+yCap, (double)0.1, (double)500.0);

}

VAO *triangle, *bucket1, *bucket2, *laser1, *laser2;
vector<VAO*> to_spawn,falling;
vector<VAO*> mirrors;
vector<int> xcoor;
vector<int> ycoor;
vector<VAO*> FinalBeam,lastBeam;
double lazerCap = 1.5;

// Creates the bucket objects used in this sample code
void createCells()
{
	GLfloat vertex_buffer_data1[]={
		-390.0,390.0,1,
		-380.0,390.0,1,
		-380,365.0,1,

		-380,365,1,
		-390,365,1,
		-390,390,1
	};

	GLfloat color_buffer_data1[]={
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0
	};

	cell[0]=create3DObject(GL_TRIANGLES, 6, vertex_buffer_data1, color_buffer_data1, GL_FILL);

	GLfloat vertex_buffer_data2[]={
		-380,390,1,
		-370,390,1,
		-370,365,1,

		-370,365,1,
		-380,365,1,
		-380,390,1
	};

	GLfloat color_buffer_data2[]={
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0
	};

	cell[1]=create3DObject(GL_TRIANGLES, 6, vertex_buffer_data2, color_buffer_data2, GL_FILL);


	GLfloat vertex_buffer_data3[]={
		-370,390,1,
		-360,390,1,
		-360,365,1,

		-360,365,1,
		-370,365,1,
		-370,390,1
	};

	GLfloat color_buffer_data3[]={
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0
	};

	cell[2]=create3DObject(GL_TRIANGLES, 6, vertex_buffer_data3, color_buffer_data3, GL_FILL);

	GLfloat vertex_buffer_data4[]={
		-360,390,1,
		-350,390,1,
		-350,365,1,
		-350,365,1,
		-360,365,1,
		-360,390,1
	};

	GLfloat color_buffer_data4[]={
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0,
		11.0/255.0, 79.0/255.0, 26.0/255.0
	};

	cell[3]=create3DObject(GL_TRIANGLES, 6, vertex_buffer_data4, color_buffer_data4, GL_FILL);
}

void createBattery()
{
	GLfloat vertex_buffer_data[]={
		-395, 395, 0,
		-345, 395, 0,
		-345, 360, 0,
		-345, 360, 0,
		-395, 360, 0,
		-395, 395, 0
	};

	GLfloat color_buffer_data[]={
		0, 0 ,0,
		0, 0, 0,
		0, 0, 0,
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
	};

	batteryBox=create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_LINE);

	GLfloat vertex_buffer_data1[]={
		-345, 370, 0,
		-345, 380, 0,
		-340, 380, 0,
		-340, 380, 0,
		-340, 370, 0,
		-345, 370, 0
	};

	GLfloat color_buffer_data1[]={
		0,0,0,
		0,0,0,
		0,0,0,
		0,0,0,
		0,0,0,
		0,0,0
	};
	batteryNib=create3DObject(GL_TRIANGLES, 6, vertex_buffer_data1, color_buffer_data1, GL_FILL);

}
void newDabba(int i )
{
	// GL3 accepts only Triangles. Quads are not supported
	VAO *temp;
	static const GLfloat vertex_buffer_data [] = {
		-425,425,0, // vertex 1
		-400,425,0, // vertex 2
		-425, 400,0, // vertex 3

		-400, 400,0, // vertex 3
		-425, 400,0, // vertex 4
		-400, 425,0  // vertex 1
	};

	static const GLfloat color_buffer_data1 [] = {
		0,1,0, // color 1
		0,1,0, // color 2
		0,1,0, // color 3

		0,1,0, // color 3
		0,1,0, // color 4
		0,1,0  // color 1
	};

	static const GLfloat color_buffer_data2 [] = {
		1,0,0, // color 3
		1,0,0, // color 2
		1,0,0, // color 1

		1,0,0, // color 3
		1,0,0, // color 4
		1,0,0  // color 1
	};

	static const GLfloat color_buffer_data3 [] = {
		0,0,0, // color 1
		0,0,0, // color 2
		0,0,0, // color 3

		0,0,0, // color 3
		0,0,0, // color 4
		0,0,0  // color 1
	};
	srand(glfwGetTime()*i);
	double r = (rand()%3);

	// create3DObject creates and returns a handle to a VAO that can be used later
	if(r == 0)
		temp = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data1, GL_FILL);
	else if(r==1)
		temp = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data2, GL_FILL);
	else 
		temp = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data3, GL_FILL);
	temp->color = r;
	srand(glfwGetTime()*i);
	r = (rand()%800);
	if(r<100)
		r+=100;
	temp->xpos = r;
	temp->ypos = 0;
	to_spawn.push_back(temp);

}
void createBuckets ()
{
	// GL3 accepts only Triangles. Quads are not supported
	static const GLfloat vertex_buffer_data [] = {
		-300,-400,0, // vertex 1
		-200,-400,0, // vertex 2
		-330, -350,0, // vertex 3

		-330, -350,0, // vertex 3
		-170, -350,0, // vertex 4
		-200,-400,0  // vertex 1
	};

	static const GLfloat color_buffer_data [] = {
		0,1,0, // color 1
		0,1,0, // color 2
		0,1,0, // color 3

		0,1,0, // color 3
		0,1,0, // color 4
		0,1,0  // color 1
	};

	// create3DObject creates and returns a handle to a VAO that can be used later
	bucket1 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);

	// create bucket2
	static const GLfloat vertex_buffer_data1 [] = {
		300,-400,0, // vertex 1
		200,-400,0, // vertex 2
		330, -350,0, // vertex 3

		330, -350,0, // vertex 3
		170, -350,0, // vertex 4
		200,-400,0  // vertex 1
	};

	static const GLfloat color_buffer_data1 []= {
		1,0,0, // color 1
		1,0,0, // color 2
		1,0,0, // color 3

		1,0,0, // color 3
		1,0,0, // color 4
		1,0,0  // color 1
	};

	// create3DObject creates and returns a handle to a VAO that can be used later
	bucket2 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data1, color_buffer_data1, GL_FILL);


	static const GLfloat vertex_buffer_data2 [] = {
		-400,20,0, // vertex 1
		-360,20,0, // vertex 2
		-400,-20,0, // vertex 3

		-400, -20,0, // vertex 3
		-360, -20,0, // vertex 4
		-360,20,0,  // vertex 1

		-360,20,0,
		-360,-20,0,
		-330,-10,0,

		-360,20,0,
		-330,10,0,
		-330,-10,0

	};

	static const GLfloat color_buffer_data2 [] = {
		0,1,0, // color 1
		0,1,0, // color 2
		0,1,0, // color 3

		0,1,0, // color 3
		0,1,0, // color 4
		0,1,0,  // color 1

		0,1,0, // color 3
		0,1,0, // color 4
		0,1,0,  // color 1

		0,1,0, // color 3
		0,1,0, // color 4
		0,1,0  // color 1

	};

	// create3DObject creates and returns a handle to a VAO that can be used later
	laser1 = create3DObject(GL_TRIANGLES, 12, vertex_buffer_data2, color_buffer_data2, GL_FILL);

	static const GLfloat vertex_buffer_data3 [] = {
		-300,-400,0, // vertex 1
		-200,-400,0, // vertex 2
		-330, -300,0, // vertex 3

		-330, -300,0, // vertex 3
		-170, -300,0, // vertex 4
		-200,-400,0  // vertex 1
	};

	static const GLfloat color_buffer_data3 [] = {
		0,1,0, // color 1
		0,1,0, // color 2
		0,1,0, // color 3

		0,1,0, // color 3
		0,1,0, // color 4
		0,1,0  // color 1
	};


}
void createLaser(double x1,double y1,double x2,double y2)
{

	const GLfloat vertex_buffer_data [] = {
		x1,y1,0,
		x2,y2,0

	};

	const GLfloat color_buffer_data [] = {
		0,1,0, // color 1
		0,1,0 // color 2
	};

	// create3DObject creates and returns a handle to a VAO that can be used later
	VAO * beam = create3DObject(GL_LINES, 2, vertex_buffer_data, color_buffer_data, GL_FILL);
	beam->x1 = x1;
	beam->y1 = y1;
	beam->x2 = x2;
	beam->y2 = y2;
	double first = y2-y1;
	double second = x2-x1;
	double angle1 = atan(first/second)*180/M_PI;
	beam->angle = angle1;
	FinalBeam.push_back(beam);

}
void CreateMirror (double x1, double y1,double x2,double y2)
{
	const GLfloat vertex_buffer_data [] = {
		x1, y1, 0, 
		x2, y2, 0 

	};

	const GLfloat color_buffer_data [] = {
		0, 1, 0, 
		0, 1, 0 



	};
	VAO * temp1 = create3DObject(GL_LINES, 2, vertex_buffer_data, color_buffer_data, GL_FILL);
	temp1->x1 = x1;
	temp1->y1 = y1;
	temp1->x2 = x2;
	temp1->y2 = y2;
	double first = y2-y1;
	double second = x2-x1;
	double angle1 = atan(first/second)*180/M_PI;
	temp1->angle = angle1;
	if(mirrors.size()<2)
		mirrors.push_back(temp1);
}

/* Render the scene with openGL */
/* Edit this function according to your assignment */
float camera_rotation_angle = 90;
float rectangle_rotation = 0;
float triangle_rotation = 0;

int cmp(const void *a, const void *b)
{
	double one,two;
	double x1 = ((VAO *) a)->x1;
	if((((VAO *) a))->x1 < (((VAO *) a))->x2)
		one = (((VAO *) a))->x1;
	else 
		one = (((VAO *) a))->x2;
	if((((VAO *) b))->x1 < (((VAO *) b))->x2)
		two = (((VAO *) b))->x1;
	else 
		two = (((VAO *) b))->x1;
	return two - one;

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if(yoffset==-1)
	{
		maxCoord+=5;
		maxCoord = min(400,maxCoord);

	}
	else if(yoffset==1)
	{
		maxCoord-=5;
		maxCoord = max(100,maxCoord);
	}

}
void draw ()
{
	// clear the color and depth in the frame buffer
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use the loaded shader program
	// Don't change unless you know what you are doing
	glUseProgram (programID);

	// Eye - Location of camera. Don't change unless you are sure!!
	glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
	// Target - Where is the camera looking at.  Don't change unless you are sure!!
	glm::vec3 target (0, 0, 0);
	// Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
	glm::vec3 up (0, 1, 0);

	// Compute Camera matrix (view)
	// Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
	//  Don't change unless you are sure!!
	Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane

	// Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
	//  Don't change unless you are sure!!
	glm::mat4 VP = Matrices.projection * Matrices.view;

	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	//  Don't change unless you are sure!!
	glm::mat4 MVP;	// MVP = Projection * View * Model

	// Load identity to model matrix
	Matrices.model = glm::mat4(1.0f);

	/* Render your scene *//**************************************************************************************************/
	xCap = max(min(xCap,400- maxCoord),maxCoord - 400);
	Matrices.model = glm::mat4(1.0f);
	MVP = VP * Matrices.model;
	draw3DObject(batteryBox);
	Matrices.model = glm::mat4(1.0f);
	MVP = VP * Matrices.model;
	draw3DObject(batteryNib);
	if(lazerCap>=2.0)
		draw3DObject(cell[3]);
	if(lazerCap>=1.5)
		draw3DObject(cell[2]);
	if(lazerCap>=1.0)
		draw3DObject(cell[1]);
	if(lazerCap>0.0)
		draw3DObject(cell[0]);

	Matrices.model = glm::mat4(1.0f);
	glm::mat4 translateLaser = glm::translate (glm::vec3(0-laser_pos * sin(angle*M_PI/180.0), laser_pos*cos(angle*M_PI/180.0)  , 0));        // glTranslatef
	glm::mat4 translateLaser_origin = glm::translate (glm::vec3(400, -1*laser_pos, 0));        // glTranslatef  
	glm::mat4 translateLaser_back = glm::translate (glm::vec3(-400, laser_pos, 0));        // glTranslatef    	
	glm::mat4 rotateLaser = glm::rotate((float)(angle*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
	Matrices.model *= (translateLaser * translateLaser_back* rotateLaser* translateLaser_origin);
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(laser1);

	// translate bucket1
	Matrices.model = glm::mat4(1.0f);
	glm::mat4 translateBucket1 = glm::translate (glm::vec3(bucket_pos1, 0, 0));        // glTranslatef
	Matrices.model *= (translateBucket1 );
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(bucket1);


	//translate bucket2
	Matrices.model = glm::mat4(1.0f);
	glm::mat4 translateBucket2 = glm::translate (glm::vec3(bucket_pos2, 0, 0));        // glTranslatef
	Matrices.model *= (translateBucket2 );
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(bucket2);

	//translate boxes 
	for(int i =0;i<falling.size();i++)
	{
		Matrices.model = glm::mat4(1.0f);
		// translate bucket1
		glm::mat4 translateDabba = glm::translate (glm::vec3(falling[i]->xpos, -1*falling[i]->ypos, 0));        // glTranslatef
		Matrices.model *= (translateDabba );
		MVP = VP * Matrices.model;
		glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// draw3DObject draws the VAO given to it using current MVP matrix
		draw3DObject(falling[i]);
	}
	for(int i=0;i<mirrors.size();i++)
	{
		Matrices.model = glm::mat4(1.0f);
		// translate bucket1
		glm::mat4 translateMirror = glm::translate (glm::vec3(0, 0, 0));        // glTranslatef
		Matrices.model *= (translateMirror );
		MVP = VP * Matrices.model;
		glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
		// draw3DObject draws the VAO given to it using current MVP matrix
		draw3DObject(mirrors[i]);

	}


	for (int i = 0; i < falling.size(); ++i)
	{
		double y2 = 400 - falling[i]->ypos;
		double x2 = -400 + falling[i]->xpos;


		if(y2 <-400)
		{

			falling.erase(falling.begin()+i);

		}
		else if(x2 <-170+bucket_pos1 && x2>-330+bucket_pos1 && y2<-350 && falling[i]->color == 0)
		{
			score+=10;
			falling.erase(falling.begin()+i);
			c_col++;
		}
		else if(x2 <-170+bucket_pos1 && x2>-330+bucket_pos1 && y2<-350 && falling[i]->color == 1)
		{
			score-=10;
			falling.erase(falling.begin()+i);
			d_col++;
		}
		else if(x2 <-170+bucket_pos1 && x2>-330+bucket_pos1 && y2<-350 && falling[i]->color == 2)
		{
			falling.erase(falling.begin()+i);
			finalQuit();
			// exit(1);
		}
		else if(x2<330+bucket_pos2 && x2>170+bucket_pos2 && y2<-350 && falling[i]->color == 1)
		{
			score+=10;
			falling.erase(falling.begin()+i);
			c_col++;
		}			
		else if(x2<330+bucket_pos2 && x2>170+bucket_pos2 && y2<-350 && falling[i]->color == 0)
		{
			score-=10;
			falling.erase(falling.begin()+i);
			d_col++;
		}			
		else if(x2<330+bucket_pos2 && x2>170+bucket_pos2 && y2<-350 && falling[i]->color == 2)
		{

			falling.erase(falling.begin()+i);
			finalQuit();
			exit(1);
		}			




	}
	//translate laser if mouse clicked
	if(press_space && lazerCap !=0)
	{

		double x1,x2,y1,y2,result1,result2;
		FinalBeam.clear();
		double rotate_x = 1000*cos(angle*M_PI/180) - 400;
		double rotate_y = 1000*sin(angle*M_PI/180) +laser_pos;
		createLaser(-400,laser_pos,rotate_x,rotate_y);
		for(int i=0 ; i<FinalBeam.size() ;i++)
		{
			for (int j = mirrors.size()-1; j >=0; j--)
			{
				x1=mirrors[j]->x1;
				x2=mirrors[j]->x2;
				y1=mirrors[j]->y1;
				y2=mirrors[j]->y2;
				double x11 = FinalBeam[i]->x1;
				double y11 = FinalBeam[i]->y1;
				double slope_mirror = tan((mirrors[j]->angle)*M_PI/180);
				double slope_laser = tan((FinalBeam[i]->angle)*M_PI/180);
				result1 = y1 - y11 - slope_laser*x1 + slope_laser*x11;
				result2 = y2 - y11 - slope_laser*x2 + slope_laser*x11;
				if(result1*result2 < 0)
				{
					double m1 = slope_laser,m2 = slope_mirror;
					double c1 = -1*(x11*m1) + y11;
					double c2 = mirrors[j]->y1 - (m2 * mirrors[j]->x1);
					double f1 = c1-c2;
					double f2 = m2-m1;
					double int_x = f1/f2;
					double int_y = (m1 * int_x) + c1;
					double rangle = -1*FinalBeam[i]->angle + 2*mirrors[j]->angle;
					rotate_x = 1000*cos(rangle*M_PI/180)+int_x;
					rotate_y = 1000* sin(rangle*M_PI/180)+int_y;
					result1 = FinalBeam[i]->y1 - mirrors[j]->y1 - slope_mirror * FinalBeam[i]->x1 + slope_mirror * mirrors[j]->x1;
					result2 = rotate_y - mirrors[j]->y1 - slope_mirror * rotate_x + slope_mirror * mirrors[j]->x1;
					if(result1*result2 < 0)
					{
						rangle += 180;
						rotate_x = 1000*cos(rangle*M_PI/180)+int_x;
						rotate_y = 1000* sin(rangle*M_PI/180)+int_y;						
					}
					if(fabs(int_x -FinalBeam[i]->x1) > 0.00001 )
					{
						double sx1 = FinalBeam[i]->x1;
						double sy1 = FinalBeam[i]->y1;
						FinalBeam.erase(FinalBeam.begin()+i);
						createLaser(sx1,sy1,int_x,int_y);
						createLaser(int_x,int_y,rotate_x,rotate_y);
						break;
					}
				}	
			}

		}
		lastBeam.clear();

		for (int i = 0; i < FinalBeam.size(); ++i)
		{
			lastBeam.push_back(FinalBeam[i]);


		}
		for(int i=0;i<lastBeam.size();i++)
		{
			Matrices.model = glm::mat4(1.0f);
			MVP = VP * Matrices.model;
			glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
			draw3DObject(lastBeam[i]);

		}
		for(int i=0 ; i<falling.size();i++)
		{
			x1 = -425 + falling[i]->xpos;
			y1 = 425 - falling[i]->ypos;
			x2 = -400 + falling[i]->xpos;
			y2 = 400 - falling[i]->ypos;
			double x3 = -400 + falling[i]->xpos;
			double y3 = 425 - falling[i]->ypos;
			double x4 = -400 +falling[i]->xpos;
			double y4 = 400 - falling[i]->ypos;

			for (int j = 0; j < FinalBeam.size(); ++j)
			{
				double x11 = FinalBeam[j]->x1;
				double y11 = FinalBeam[j]->y1;
				double slope_laser = tan((FinalBeam[j]->angle)*(M_PI/180));
				result1 = y1 - y11 - slope_laser*x1 + slope_laser*x11;
				result2 = y2 - y11 - slope_laser*x2 + slope_laser*x11;
				double result3 = y3 - y11 - slope_laser*x3 + slope_laser*x11;
				double result4 = y4 - y11 - slope_laser*x4 + slope_laser*x11;
				if(result1*result2 < 0  )
				{
					double compare;
					if(FinalBeam[j]->x1 > FinalBeam[j]->x2)
						compare = FinalBeam[j]->x1;
					else 
						compare =FinalBeam[j]->x2;
					if(x1<=compare)
					{
						if(falling[i]->color == 0 || falling[i]->color == 1)
						{
							score-=5;
							c_destroyed++;
						}
						else
						{
							score+=10;
							d_destroyed++;
						}
						falling.erase(falling.begin()+i);

					}
				}
				if(result4*result3 < 0  && slope_laser < 0)
				{
					double compare;
					if(FinalBeam[j]->x1 > FinalBeam[j]->x2)
						compare = FinalBeam[j]->x1;
					else 
						compare =FinalBeam[j]->x2;
					if(x1<=compare)
					{
						if(falling[i]->color == 0 || falling[i]->color == 1)
						{
							score-=5;
							c_destroyed++;
						}
						else
						{
							score+=10;
							d_destroyed++;
						}
						falling.erase(falling.begin()+i);


					}
				}

			}

		}

	}


}

/* Initialise glfw window, I/O callbacks and the renderer to use */
/* Nothing to Edit here */
// void check_limits()

GLFWwindow* initGLFW (int width, int height)
{
	GLFWwindow* window; // window desciptor/handle

	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		//        exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Sample OpenGL 3.3 Application", NULL, NULL);

	if (!window) {
		glfwTerminate();
		//        exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSwapInterval( 1 );

	/* --- register callbacks with GLFW --- */

	/* Register function to handle window resizes */
	/* With Retina display on Mac OS X GLFW's FramebufferSize
	   is different from WindowSize */
	glfwSetFramebufferSizeCallback(window, reshapeWindow);
	glfwSetWindowSizeCallback(window, reshapeWindow);

	/* Register function to handle window close */
	glfwSetWindowCloseCallback(window, quit);

	/* Register function to handle keyboard input */
	glfwSetKeyCallback(window, keyboard);      // general keyboard input
	glfwSetCharCallback(window, keyboardChar);  // simpler specific character handling

	/* Register function to handle mouse click */
	glfwSetMouseButtonCallback(window, mouseButton);  // mouse button clicks
	glfwSetScrollCallback(window, scroll_callback);

	return window;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL (GLFWwindow* window, int width, int height)
{
	/* Objects should be created before any other gl function and shaders */
	// Create the models
	// createTriangle (); // Generate the VAO, VBOs, vertices data & copy into the array buffer
	createBuckets ();
	createBattery();
	createCells();

	// CreateMirror(20,20,40,-20);
	// CreateMirror(200,200,-200,-200);
	// Create and compile our GLSL program from the shaders
	programID = LoadShaders( "Sample_GL.vert", "Sample_GL.frag" );
	// Get a handle for our "MVP" uniform
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


	reshapeWindow (window, width, height);

	// Background color of the scene
	glClearColor (0.3f, 0.3f, 0.3f, 0.0f); // R, G, B, A
	glClearDepth (1.0f);

	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

	cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
	cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
	cout << "VERSION: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}
double coord[4];
int main (int argc, char** argv)
{
	int width = 800;
	int height = 800;

	GLFWwindow* window = initGLFW(width, height);

	initGL (window, width, height);

	double last_update_time = glfwGetTime(), current_time;
	/* Draw in loop */
	srand(time(NULL));
	for(int i=0;i<4;i++)
	{
		double rnd = (rand()%400);
		coord[i] = rnd;
	}
	CreateMirror(coord[0],coord[1],coord[2],coord[3]);
	srand(time(NULL));

	for(int i=0;i<4;i++)
	{
		double rnd = (rand()%400);
		coord[i] = rnd;
	}
	CreateMirror(coord[1],-1*coord[0],coord[3],-1*coord[2]);

	createLaser(-400,-2.5,1000,0);
	sort(mirrors.begin(), mirrors.end(), cmp);

	while (!glfwWindowShouldClose(window)) {
		reshapeWindow(window,width,height);
		for(int i=0; i<to_spawn.size();i++)
		{
			falling.push_back(to_spawn[i]);
			to_spawn.erase(to_spawn.begin()+i);
		}
		for(int i=0;i<falling.size();i++)
		{
			falling[i]->ypos+=box_speed;
		}
		if(press_right && bucket_pos1 <570 && ctrl)
			bucket_pos1+= 8;

		else if(press_left && bucket_pos1 > -70 && ctrl){
			bucket_pos1-= 8;
		}
		if(press_right && bucket_pos2 <70 && alt)
			bucket_pos2+= 8;

		else if(press_left && bucket_pos2 > -570 && alt){
			bucket_pos2-= 8;
		}
		if(press_a && laser_pos < 400)
			laser_pos+=5;
		else if(press_d && laser_pos > -400)
			laser_pos-=5;
		if(mouse_press )
		{
			glfwGetCursorPos(window,&mouse_x,&mouse_y);
			if(selected ==0 )
			{
				if(prevMouse_y != mouse_y || prevMouse_x != mouse_x)
				{
					if(prevMouse_x != mouse_x)
						prevMouse_x = mouse_x;
					if(prevMouse_y != mouse_y)
						prevMouse_y = mouse_y;
					if(mouse_x==0)
						angle=90;
					else{
						double y_pos = (800.0-(laser_pos+400)) ;
						angle=atan((-mouse_y+y_pos)/(mouse_x)); 
						angle*=180/M_PI;
					}
				}
			}
			else if(selected==1 )
			{
				if(prevMouse_y != mouse_y || prevMouse_x != mouse_x)
				{
					if(prevMouse_x != mouse_x)
						prevMouse_x = mouse_x;
					if(prevMouse_y != mouse_y)
						prevMouse_y = mouse_y;
					mouse_x =mouse_x-400;
					bucket_pos1 = mouse_x+250;
					if(bucket_pos1 < -70)
						bucket_pos1 = -70;
					if(bucket_pos1 > 570)
						bucket_pos1 = 570;
				}

			}
			else if(selected==2)
			{
				if(prevMouse_y != mouse_y || prevMouse_x != mouse_x)
				{
					if(prevMouse_x != mouse_x)
						prevMouse_x = mouse_x;
					if(prevMouse_y != mouse_y)
						prevMouse_y = mouse_y;
					mouse_x =mouse_x - 400;
					bucket_pos2 = mouse_x-250;
					if(bucket_pos1 > 70)
						bucket_pos1 = 70;
					if(bucket_pos1 < -570)
						bucket_pos1 = -570;
				}

			}

		}
		if(press_s)
		{
			angle+=3;
			if(angle>90)
				angle=89;
		}
		if(press_f)
		{
			angle-=3;
			if(angle<-90)
				angle = -89;
		}
		if(press_right && !ctrl && !alt)
		{
			xCap+=5;

		}
		if(press_left && !ctrl && !alt)
		{
			xCap-=5;
		}
		if(press_up && !ctrl && !alt)
		{
			maxCoord-=5;
			maxCoord = max(100,maxCoord);

		}
		if(press_down && !ctrl && !alt)
		{
			maxCoord+=5;
			maxCoord = min(400,maxCoord);

		}
		draw();

		glfwSwapBuffers(window);

		glfwPollEvents();

		// Control based on time (Time based transformation like 5 degrees rotation every 0.5s)
		current_time = glfwGetTime(); // Time in seconds
		if(press_space && (current_time-last_update_time)>=0.1)
		{
			lazerCap-=0.1;
			if(lazerCap<0)
				lazerCap=0;
		}
		else if(press_space==0 && (current_time-last_update_time)>=0.1)
		{
			lazerCap+=0.1;
			if(lazerCap>2)
				lazerCap=2;
		}

		if ((current_time - last_update_time) >= 1) { // atleast 0.5s elapsed since last frame
			// do something every 0.5 seconds ..
			last_update_time = current_time;
			if(falling.size()<20)
			{
				std::random_device rd;
				std::mt19937 mt(rd());
				std::uniform_real_distribution<double> dist(1.0, 10.0);
				int a = (dist(mt)*100) ;
				newDabba(a);
			}
		}

	}

	glfwTerminate();
}
