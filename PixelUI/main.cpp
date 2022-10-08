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
	//glfwWindowHint(GLFW_DECORATED, 0);//border less 
	//create window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "hello bgfx!", NULL, NULL);

	GLFWwindow* window2 = glfwCreateWindow(WIDTH, HEIGHT, "hello bgfx2!", NULL, NULL);

	if (!window)
	{
		std::cout << "create glfw window error!" << std::endl;
		glfwTerminate();
		return -1;
	}

	if (!window2)
	{
		std::cout << "create glfw window error!" << std::endl;
		glfwTerminate();
		return -1;
	}

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

	//bgfxInit.platformData.nwh = glfwGetWin32Window(window2);
	//bgfx::init(bgfxInit);
	//------init bgfx------

	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
	bgfx::setViewRect(0, 0, 0, WIDTH, HEIGHT);

	bgfx::frame();

	//glfwMakeContextCurrent(window2);
	//create a frame buffer from hwnd
	
	//bgfx::frame();
	//bgfx::setViewFrameBuffer(0, BGFX_INVALID_HANDLE);
	//bgfx::setViewFrameBuffer(1, handle);//link framebuffer and view1
	//bgfx::setViewClear(1, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
	//bgfx::setViewRect(1, 0, 0, WIDTH, HEIGHT);
	bgfx::FrameBufferHandle handle = bgfx::createFrameBuffer(glfwGetWin32Window(window2), WIDTH, HEIGHT);

	bgfx::setViewClear(1, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xcc3030ff, 1.0f, 0);
	bgfx::setViewRect(1, 0, 0, WIDTH, HEIGHT);

	while (!glfwWindowShouldClose(window) && !glfwWindowShouldClose(window2))
	{	
		//glfwMakeContextCurrent(window);
		bgfx::setViewFrameBuffer(0, BGFX_INVALID_HANDLE);
		//render here
		bgfx::touch(0);
		//glClear(GL_COLOR_BUFFER_BIT);

		bgfx::setViewFrameBuffer(1, handle);//link view1 and framebuffer

		bgfx::touch(1);//submit to view1

		bgfx::frame();

		//swap front and back buffers
		//glfwSwapBuffers(window);

		//glfwSwapBuffers(window2);

		//poll for and process event
		glfwPollEvents();
		
	}

	//shut down bgfx
	bgfx::shutdown();

	glfwTerminate();

	return 0;
}