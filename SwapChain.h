#pragma once
#include <d3d11.h>

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

	bool init();
	bool release();
	
	void present();

private:

};

