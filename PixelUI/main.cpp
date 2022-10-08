#include <bgfx/bgfx.h> //include bgfx

#include <bgfx/platform.h>

#include <bx/math.h>

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#define WIDTH 1600
#define HEIGHT 800

#include <iostream>

int main()
{
	//glfw init
	if (!glfwInit())
	{
		std::cout << "create glfw error!" << std::endl;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	//create window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "hello bgfx!", NULL, NULL);

	if (!window)
	{
		std::cout << "create glfw window error!" << std::endl;
		glfwTerminate();
		return -1;
	}

	//make the window context current
	glfwMakeContextCurrent(window);

	//------init bgfx------
	bgfx::PlatformData pd;
	pd.ndt = NULL;
	pd.nwh = glfwGetWin32Window(window);

	bgfx::Init bgfxInit;
	bgfxInit.type = bgfx::RendererType::Count;
	bgfxInit.resolution.width = WIDTH;
	bgfxInit.resolution.height = HEIGHT;
	bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
	bgfxInit.platformData = pd;

	if (!bgfx::init(bgfxInit))
	{
		std::cout << "create bgfx error!" << std::endl;
		return -1;
	}
	//------init bgfx------

	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
	bgfx::setViewRect(0, 0, 0, WIDTH, HEIGHT);

	while (!glfwWindowShouldClose(window))
	{
		//render here
		bgfx::touch(0);
		//glClear(GL_COLOR_BUFFER_BIT);
		bgfx::frame();

		//swap front and back buffers
		glfwSwapBuffers(window);

		//poll for and process event
		glfwPollEvents();
	}

	//shut down bgfx
	bgfx::shutdown();

	glfwTerminate();

	return 0;
}