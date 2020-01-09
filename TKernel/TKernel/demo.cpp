#include "demo.h"
#include "resource.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
	return TKernelDemo::Execute(hInstance);
}