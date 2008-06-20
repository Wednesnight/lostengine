#include <SDL/SDL.h>
#include <iostream>
#include "Application.h"
#include <exception>

using namespace std;

#if defined WIN32
#include <windows.h>
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
#else
int main(int argc, char *argv[])
#endif
{
	try
	{
		Application app;		
		app.run();
	}
	catch(exception& e)
	{
		cout << "caught error: " << typeid(e).name() << " : " << e.what() << endl;
		return 1;
	}
	
	return 0;
}
