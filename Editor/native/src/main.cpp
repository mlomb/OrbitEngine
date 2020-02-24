
#include <iostream>
#include <random>

#include "windows.h"
#include <WinUser.h>

#include <OE/Application/Window.hpp>
#include <OE/Application/Looper.hpp>

using namespace OrbitEngine;
using namespace OrbitEngine::Application;
using namespace OrbitEngine::Misc;

HANDLE hPipe;
int image_width = 2550;
int image_height = 1296;
int image_size = 3 * image_width * image_height;
unsigned char* image = NULL;

using namespace std;

random_device rd;
mt19937 engine(rd());
uniform_int_distribution<> dist(0, 255);

static int g_seed = 24235;
inline int fastrand() {
	g_seed = (214013 * g_seed + 2531011);
	return (g_seed >> 16) & 0x7FFF;
}

class Service : public Loopeable {
public:
	void render() {
		if (!image) {
			image = (unsigned char*)malloc(image_size);
			
			for (int x = 0; x < image_width; x++) {
				for (int y = 0; y < image_height; y++) {
					int p = (y * image_width + x) * 3;
					image[p + 0] = 255;
					image[p + 1] = 0;
					image[p + 2] = 0;
				}
			}
		}

		/*
		for (int x = 0; x < image_width; x++) {
			for (int y = 0; y < image_height; y++) {
				int p = (y * image_width + x) * 3;
				image[p + 0] = rand() % 255;
				image[p + 1] = rand() % 255;
				image[p + 2] = rand() % 255;
			}
		}
		*/

		for (int i = 0; i < image_size; i += 3) {
			image[i] = image[i+1] = image[i+2] = fastrand() % 255;
		}

		BOOL bResult;
		DWORD cbBytes;
		bResult = WriteFile(
			hPipe,
			image,
			image_size,
			&cbBytes,
			NULL);
	}
};

#define BUFFER_SIZE 1024 * 1024 * 20

int main() {
	hPipe = CreateNamedPipe(
		"\\\\.\\pipe\\OrbitEngineEditorService",             // pipe name 
		PIPE_ACCESS_DUPLEX,       // read/write access 
		PIPE_TYPE_MESSAGE |       // message type pipe 
		PIPE_READMODE_MESSAGE   // message-read mode 
		/* | PIPE_WAIT*/,                // blocking mode 
		PIPE_UNLIMITED_INSTANCES, // max. instances  
		BUFFER_SIZE,              // output buffer size 
		BUFFER_SIZE,              // input buffer size 
		NMPWAIT_USE_DEFAULT_WAIT, // client time-out 
		NULL);                    // default security attribute 

	if (INVALID_HANDLE_VALUE == hPipe)
	{
		printf("\nError occurred while "
			"creating the pipe: %d", GetLastError());
		fflush(stdout);
		return 1;  //Error
	}

	//Wait for the client to connect
	BOOL bClientConnected = ConnectNamedPipe(hPipe, NULL);

	if (FALSE == bClientConnected)
	{
		printf("\nError occurred while connecting"
			" to the client: %d", GetLastError());
		fflush(stdout);
		CloseHandle(hPipe);
		return 1;  //Error
	}

	Service* service = new Service();

	Window* w = new Window();
	Context* context = new Context(RenderAPI::OPENGL, w);
	Looper* looper = new Looper(context, service);
	looper->loop();

	CloseHandle(hPipe);

    return 0;
}
