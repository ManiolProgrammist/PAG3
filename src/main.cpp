#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <../shader_s.h>
#include <../camera.h>
#include <../model.h>
#include <../GraphNode.h>
#include <iostream>
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif
#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool showGrid = false;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int NumerSegmentow = 30;
int LiczbaPierscieni = 30;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 4.3 + GLSL 430
	const char* glsl_version = "#version 430";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif


	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//GLFW_CURSOR_NORMAL   GLFW_CURSOR_DISABLED

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/*IMGUI_CHECKVERSION();*/
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();



	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader ObjShader("../../res/shaders/1.model_loading.vs", "../../res/shaders/1.model_loading.fs");
	Shader GenerateObjShader("../../res/shaders/ForObject.vs", "../../res/shaders/ForObject.fs");
	






	
	// load models
	// -----------
	Model *ourModel1=new Model(("res/models/nanosuit/nanosuit.obj"));
	Model *plant1=new Model(("res/models/pot/eb_house_plant_02.obj"));
	Model *plant2 = new Model(("res/models/Death_Star/Death_Star.obj"));


	Mesh *thorusMesh = new Mesh();
	thorusMesh->generateTorus(50, 45, 4, 10);
	Model *thorusModel = new Model(thorusMesh, false);


	thorusModel->SetShader(&GenerateObjShader);
	ourModel1->SetShader(&ObjShader);
	plant1->SetShader(&ObjShader);
	plant2->SetShader(&ObjShader);



	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	GraphNode *World = new GraphNode(false);

	GraphNode *Node1 = new GraphNode(false, ourModel1);
	//Node1->AddOrbit(10, &GenerateObjShader, true);
	GraphNode *handler1 = new GraphNode(true);
	GraphNode *handler2 = new GraphNode(true);

	GraphNode *Node2 = new GraphNode(true, thorusModel);

	GraphNode *Node3 = new GraphNode(false, plant1);
	GraphNode *handler3 = new GraphNode(true);

	GraphNode *Node4 = new GraphNode(true, plant2);

	//GraphNode *handler4 = new GraphNode(true);
	
	
	



	glm::mat4 *TransformNode1 = new glm::mat4(1);
	*TransformNode1 = glm::translate(*(TransformNode1), glm::vec3(0.0f, -2.75f, 0.0f)); // translate it down so it's at the center of the scene
	*TransformNode1 = glm::scale(*TransformNode1, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down

	glm::mat4 *TransformNode2 = new glm::mat4(1);
	*TransformNode2 = glm::translate(*(TransformNode2), glm::vec3(10.0f, 9.0f, 0.0f));
	*TransformNode2 = glm::scale(*TransformNode2, glm::vec3(0.3f, 0.3f, 0.3f));	


	glm::mat4 *TransformNode3 = new glm::mat4(1);
	*TransformNode3 = glm::translate(*(TransformNode3), glm::vec3(20.0f, 8.0f, 0.0f)); // translate it down so it's at the center of the scene
	*TransformNode3 = glm::scale(*TransformNode3, glm::vec3(0.1f, 0.1f, 0.1f));	



	glm::mat4 *TransformNode4 = new glm::mat4(1);
	*TransformNode4 = glm::translate(*(TransformNode4), glm::vec3(20.0f, -2.0f, 0.0f)); // translate it down so it's at the center of the scene
	*TransformNode4 = glm::scale(*TransformNode4, glm::vec3(2.0f, 2.0f, 2.0f));	



	//World->SetTransform(new glm::mat4(1));
	Node1->SetTransform(TransformNode1);
	Node1->AddOrbit(10, &GenerateObjShader, true, 0.1f, 9);
	Node1->AddOrbit(20, &GenerateObjShader, true, 0.1f, 9);
	Node2->SetTransform(TransformNode2);
	Node3->SetTransform(TransformNode3);
	Node2->AddOrbit(20, &GenerateObjShader, false, 0.5f, 0);
	Node4->SetTransform(TransformNode4);




	handler1->AddChild(Node2);
	handler2->AddChild(Node3);
	handler3->AddChild(Node4);

	Node2->AddChild(handler3);
	Node1->AddChild(handler2);
	Node1->AddChild(handler1);
	
	World->AddChild(Node1);

	





	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{

		// per-frame time logic
	// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			ImGui::SetWindowPos(ImVec2(60, 60));
			ImGui::SetWindowSize(ImVec2(250, 100));
			ImGui::Begin("Settings");
			ImGui::Text("liczba Segmentow");
			ImGui::SliderInt("seg", &NumerSegmentow, 7, 60);
			ImGui::Text("Liczba Pierscieni");
			ImGui::SliderInt("pi", &LiczbaPierscieni, 6, 60);
				if (ImGui::Button("Generuj torus"))
				{
					thorusMesh->generateTorus(NumerSegmentow, LiczbaPierscieni, 4, 10);
				}
			if (ImGui::Checkbox("Show grid", &showGrid)) {

				if (showGrid) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				else {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
			}
			ImGui::End();
		}

		// per-frame time logic


		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		//ObjShader.use();
	
		/*ObjShader.use();
		GenerateObjShader.use();*/
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
	/*	ObjShader.setMat4("projection", projection);
		ObjShader.setMat4("view", view);*/
		GenerateObjShader.use();
		GenerateObjShader.setMat4("projection", projection);
		GenerateObjShader.setMat4("view", view);

		ObjShader.use();
		ObjShader.setMat4("projection", projection);
		ObjShader.setMat4("view", view);


		Node2->Rotate(3.2f, glm::vec3(0, 1, 0));
		
		//Node3->Rotate(2.1f, glm::vec3(0, 1, 0));
		Node4->Rotate(2.3f, glm::vec3(0, 1, 0));


		handler1->Rotate(1.3f, glm::vec3(0, -1, 0));
		handler2->Rotate(2.5f, glm::vec3(0, -1, 0));
		handler3->Rotate(5.0f, glm::vec3(0, -1, 0));
		World->Update(0);
		World->Draw();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)) {

		if (firstMouse)
		{
			lastX = xpos;
				lastY = ypos;
				firstMouse = false;
		}

		float xoffset = xpos - lastX;
			float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

			lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}