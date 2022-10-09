#include <bgfx/bgfx.h> //include bgfx

#include <bgfx/platform.h>

#include <bx/math.h>

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#define WIDTH 1600
#define HEIGHT 800

#include <iostream>

bgfx::ShaderHandle loadShader(const char* FILENAME)
{
	const char* shaderPath = "Runtime/";

	//switch (bgfx::getRendererType()) {
	//case bgfx::RendererType::Noop:
	//case bgfx::RendererType::Direct3D9:  shaderPath = "shaders/dx9/";   break;
	//case bgfx::RendererType::Direct3D11:
	//case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
	//case bgfx::RendererType::Gnm:        shaderPath = "shaders/pssl/";  break;
	//case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
	//case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
	//case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
	//case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;
	//}

	size_t shaderLen = strlen(shaderPath);
	size_t fileLen = strlen(FILENAME);
	char* filePath = (char*)malloc(shaderLen + fileLen + 1);
	memset(filePath, 0, shaderLen + fileLen + 1);
	memcpy(filePath, shaderPath, shaderLen);
	memcpy(&filePath[shaderLen], FILENAME, fileLen);

	FILE* file = fopen(filePath, "rb");
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
	fread(mem->data, 1, fileSize, file);
	mem->data[mem->size - 1] = '\0';
	fclose(file);

	return bgfx::createShader(mem);
}

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

	//bgfx::frame();

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

	float pos[9] = { 0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f};

	uint16_t indices[3] = { 0, 1, 2 };

	//------vertex layout------
	bgfx::VertexLayout myDecl;
	myDecl.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.end();
	//------vertex layout------

	bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(bgfx::makeRef(pos, sizeof(pos)), myDecl);
	bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(bgfx::makeRef(indices, sizeof(indices)));

	bgfx::ShaderHandle vsh = loadShader("vs_triangle.bin");
	bgfx::ShaderHandle fsh = loadShader("fs_triangle.bin");
	bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);

	while (!glfwWindowShouldClose(window) && !glfwWindowShouldClose(window2))
	{	
		//glfwMakeContextCurrent(window);
		bgfx::setViewFrameBuffer(0, BGFX_INVALID_HANDLE);

		bgfx::setVertexBuffer(0, vbh);
		bgfx::setIndexBuffer(ibh);

		bgfx::submit(0, program);//submit to view 0

		//render here
		bgfx::touch(0);

		//float view[16];
		//bx::mtxLookAt(view, eye, at);
		//float proj[16];
		//bx::mtxProj(proj, 60.0f, float(WIDTH) / float(HEIGHT), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
		//bgfx::setViewTransform(0, view, proj);		

		//glClear(GL_COLOR_BUFFER_BIT);

		bgfx::setViewFrameBuffer(1, handle);//link view1 and framebuffer

		bgfx::setVertexBuffer(0, vbh);
		bgfx::setIndexBuffer(ibh);

		bgfx::submit(1, program);//submit to view 0

		bgfx::touch(1);//submit to view1

		bgfx::frame();

		//swap front and back buffers
		//glfwSwapBuffers(window);

		//glfwSwapBuffers(window2);

		//poll for and process event
		glfwPollEvents();
		
	}

	bgfx::destroy(handle);
	//shut down bgfx
	bgfx::shutdown();

	glfwTerminate();

	return 0;
}