#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

const char* vertex_shader =
"#version 430 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 normal;\n" //normal vector thats perpendicular to our triangles on our cube. 
//
"out vec3 Normal;\n"
"out vec3 FragPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"gl_Position = projection * view * model * vec4(position, 1);\n"
"FragPos = vec3(model * vec4(position, 1.0f));\n"
"Normal = mat3(transpose(inverse(model))) * normal;\n"
"}\n";
const char* fragment_shader =
"#version 430 core\n"
"out vec4 color;\n"
"vec3 objectColor = vec3(1.0, 0.3, 0.1);\n"
"float ambient_constant = 0.1;\n"
"vec3 lightColor = vec3(1.0, 0.3, 0.1);\n"
"vec3 ambient = ambient_constant * lightColor;\n"
"vec3 result = ambient * objectColor;\n"
"void main()\n"
"{\n" 
  "color = vec4(result, 1.0);\n"
"}\n";
const char* lighting_shader =
"#version 430 core\n"
"layout (location = 0) in vec3 position;\n"
"uniform mat4 model2;\n"
"uniform mat4 view2;\n"
"uniform mat4 projection2;\n"
"void main()\n"
"{\n"
"gl_Position = projection2 * view2 * model2 * vec4(position, 1.0);\n"
"}\n";

const char* fragment_lighting_shader =
"#version 430 core\n"
"out vec4 color;\n"
"in vec3 FragPos;\n"
"in vec3 Normal;\n"
"uniform vec3 objectColor;\n"
"uniform vec3 lightColor;\n"
"void main()\n"
"{\n"
"color = vec4(1.0);\n"
"}\n";

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//origin where camera is

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

//facing negative because thats where our box is
//where our camera is will face in look at matrix


glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//which way is our camera up


void framebuffer_size_callback(GLFWwindow* pointer_to_window, int width, int height);
void processInput(GLFWwindow* pointer_to_window);
glm::vec3 lightPos(-1.2, 1.0f, 2.0f);
int main() {

	glfwInit();
	GLFWwindow* pointer_to_window = glfwCreateWindow(800, 600, "E", NULL, NULL);
	//rather than just copy and pasting a the function we just pass it to a pointer
	if (pointer_to_window == NULL)
	{
		cout << "terminate";
	}

	glfwMakeContextCurrent(pointer_to_window);
	glewInit();
	glfwSetFramebufferSizeCallback(pointer_to_window, framebuffer_size_callback);
	int vertex = glCreateShader(GL_VERTEX_SHADER);
	int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	const char* pointer_to_strings[1];
	pointer_to_strings[0] = vertex_shader;
	glShaderSource(vertex, 1, pointer_to_strings, 0);
	pointer_to_strings[0] = fragment_shader;
	glShaderSource(fragment, 1, pointer_to_strings, 0);
	glCompileShader(vertex);
	glCompileShader(fragment);
	int program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);


	int light_vertex = glCreateShader(GL_VERTEX_SHADER);
	int light_fragment = glCreateShader(GL_FRAGMENT_SHADER);
	const char* pointer_to_lighting_strings[1];
	pointer_to_lighting_strings[0] = lighting_shader;
	glShaderSource(light_vertex, 1, pointer_to_lighting_strings, 0);
	pointer_to_lighting_strings[0] = fragment_lighting_shader;
	glShaderSource(light_fragment, 1, pointer_to_lighting_strings, 0);
	glCompileShader(light_vertex);
	glCompileShader(light_fragment);
	int light_program = glCreateProgram();
	glAttachShader(light_program, light_vertex);
	glAttachShader(light_program, light_fragment);
	glLinkProgram(light_program);
	glEnable(GL_DEPTH_TEST);
	float vertices[]{
		  -0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(pointer_to_window))
	{

		//Add ambient light

		// input

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




		// create transformations
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first


		//THIS IS THE MATRIX THAT DEFINES THE CAMERA 
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		//FUNCTION FIRST TAKES IN
		//CAMERAPOS FOR ORIGIN
		//WE NEED THE PLACE WHERE THE CAMERA WILL LOOK SO WE NEED CAMERA FRONT
		// UP DIRECTION WIll NEVER CHANGE
		//OUR CAMERA POSITION WILL CHANGE THOUGH BASED ON CAMERA FRONT
		//OR OUR OWN KEYBOARD

		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);


		glUseProgram(program);
		unsigned int modelLoc = glGetUniformLocation(program, "model");
		unsigned int viewLoc = glGetUniformLocation(program, "view");
		unsigned int projectionLoc = glGetUniformLocation(program, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(cubeVAO);


		glUseProgram(light_program);
		glm::mat4 view2 = glm::mat4(1.0f);
		glm::mat4 model2 = glm::mat4(1.0f);
		glm::mat4 projection2 = glm::mat4(1.0f);
		view2 = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		model2 = glm::rotate(model2, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		projection2 = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);

		glm::vec3 light_color = glm::vec3(1.0f, 0.5f, 0.31f);
		glm::vec3 object_color = glm::vec3(1.0f, 1.0f, 1.0f);
		model2 = glm::translate(model2, lightPos);
		model2 = glm::scale(model2, glm::vec3(0.2f));
		unsigned int lightColLoc = glGetUniformLocation(light_program, "lightColor");
		unsigned int ObjectColLoc = glGetUniformLocation(light_program, "objectColor");
		glUniformMatrix4fv(lightColLoc, 1, GL_FALSE, glm::value_ptr(light_color));
		glUniformMatrix4fv(ObjectColLoc, 1, GL_FALSE, glm::value_ptr(object_color));

		unsigned int modelLoc2 = glGetUniformLocation(light_program, "model2");
		unsigned int viewLoc2 = glGetUniformLocation(light_program, "view2");
		unsigned int projectionLoc2 = glGetUniformLocation(light_program, "projection2");
		glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
		glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view2));
		glUniformMatrix4fv(projectionLoc2, 1, GL_FALSE, glm::value_ptr(projection2));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(lightCubeVAO);

		glfwPollEvents();
		glfwSwapBuffers(pointer_to_window);
		processInput(pointer_to_window);

	};
	glfwTerminate();
	return 0;

}
void processInput(GLFWwindow* pointer_to_window)
{
	//closes window
	if (glfwGetKey(pointer_to_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(pointer_to_window, true);

	float cameraSpeed = 0.003; //THIS IS A NICE SPEED

	if (glfwGetKey(pointer_to_window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos = cameraPos + cameraSpeed * cameraFront;
	//Camera Position Plus DIRECTION CAMERA FACES, * CAMERA SPEED
	// will equal the new camera POS vector

	//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	//this new vec3 = original vec3 + (speed times, the direction vector which faces negative)
	//0.0f               0.0f
	//0.0f  + (0.003 *   0.0f )
	//3.0f              -1.0f 
	if (glfwGetKey(pointer_to_window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos = cameraPos - cameraSpeed * cameraFront;
	//0.0f            0.0f
	//0.0f - (0.003 * 0.0f )
	//0.0f           -1.0f 


	if (glfwGetKey(pointer_to_window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos = cameraPos - glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	//so the cross product of cameraFront and camera up * camera speed
	//a x b
	//   0.0f     0.0      1
	//   0.0f  *  1.0   =  0 the world moves from, or the box   * 0.03 = x

	// - 1.0f     0.0      0


	//0.0f                       
	//0.0f   -      x        =     new position that world moves to 
	//3 f                        
	if (glfwGetKey(pointer_to_window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos = cameraPos + glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;



	//so the cross product of cameraFront and camera up * camera speed
	//a x b
	//   0.0f     0.0      1
	//   0.0f  *  1.0   =  0 the world moves from, or the box   * 0.03 = x

	// - 1.0f     0.0      0


	//0.0f                       
	//0.0f   +      x        =     new position that world moves to 
	//3 f         
	//this might actually just change all the vectors not just camera position as i am looking at my graph
}
void framebuffer_size_callback(GLFWwindow* pointer_to_window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
