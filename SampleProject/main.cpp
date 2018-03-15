
/*
* Ros D3D 0.5b by n7

How to compile:
- compile with visual studio community 2017 (..\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.exe)
- select Release x86
- click: project -> properties -> configuration properties -> general -> character set -> change to "not set"
- compile with CTRL + Shift + B

Optional: remove dependecy on vs runtime:
- click: project -> properties -> configuration properties -> C/C++ -> code generation -> runtime library: Multi-threaded (/MT)

-Default values
speed hack on = 90 off= 60 float



*/

#include "stdafx.h"
#include "D:\\d3d9\\main.h" //less important stuff & helper funcs here


//zoom 0x1716BCC
//speed 0x1763A30


//offsets
#define OFFSET_WALL 0x17586A8

const unsigned long ultra_zoom = (DWORD)GetModuleHandle(NULL) + 0x1716BCC;
const unsigned long move_speed = (DWORD)GetModuleHandle(NULL) + 0x1763A30;
const unsigned long grass1 = (DWORD)GetModuleHandle(NULL) + 0xC37F77;
const unsigned long grass2 = (DWORD)GetModuleHandle(NULL) + 0xC37F7B;

//triggers
bool isZoom = false;
bool isSpeed = false;
bool isGrass = false;
bool isChams = false;

typedef HRESULT(APIENTRY *SetStreamSource_t)(IDirect3DDevice9*, UINT, IDirect3DVertexBuffer9*, UINT, UINT);
HRESULT APIENTRY SetStreamSource_hook(IDirect3DDevice9*, UINT, IDirect3DVertexBuffer9*, UINT, UINT);
SetStreamSource_t SetStreamSource_orig = 0;

typedef HRESULT(APIENTRY *SetVertexShaderConstantF_t)(IDirect3DDevice9*, UINT, const float*, UINT);
HRESULT APIENTRY SetVertexShaderConstantF_hook(IDirect3DDevice9*, UINT, const float*, UINT);
SetVertexShaderConstantF_t SetVertexShaderConstantF_orig = 0;

typedef HRESULT(APIENTRY *DrawIndexedPrimitive_t)(IDirect3DDevice9*, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
HRESULT APIENTRY DrawIndexedPrimitive_hook(IDirect3DDevice9*, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
DrawIndexedPrimitive_t DrawIndexedPrimitive_orig = 0;

typedef HRESULT(APIENTRY* EndScene_t) (IDirect3DDevice9*);
HRESULT APIENTRY EndScene_hook(IDirect3DDevice9*);
EndScene_t EndScene_orig = 0;

typedef HRESULT(APIENTRY *Reset_t)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
HRESULT APIENTRY Reset_hook(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
Reset_t Reset_orig = 0;

//==========================================================================================================================

HRESULT APIENTRY SetStreamSource_hook(LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT sStride)
{
	try {
		if (StreamNumber == 0)
			Stride = sStride;

		return SetStreamSource_orig(pDevice, StreamNumber, pStreamData, OffsetInBytes, sStride);
	}
	catch (exception e) {

	}

}

//=====================================================================================================================

HRESULT APIENTRY SetVertexShaderConstantF_hook(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, const float *pConstantData, UINT Vector4fCount)
{
	try {

		if (pConstantData != NULL)
		{
			mStartregister = StartRegister;
			mVectorCount = Vector4fCount;
		}

		return SetVertexShaderConstantF_orig(pDevice, StartRegister, pConstantData, Vector4fCount);
	}
	catch (exception e) {

	}

}

//==========================================================================================================================

HRESULT APIENTRY DrawIndexedPrimitive_hook(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	try {
		DWORD MODULEROS;
		MODULEROS = (DWORD)GetModuleHandleA("ros.exe");
		DWORD InGameAdrress = (MODULEROS + 0x1E23F30);
		DWORD* Verify = (DWORD*)(InGameAdrress);

		if (*Verify == 1) {

			if (InitOnce)
			{
				InitOnce = false;
				GenerateTexture(pDevice, &Red, D3DCOLOR_ARGB(255, 255, 0, 0));
				GenerateTexture(pDevice, &Green, D3DCOLOR_RGBA(0, 255, 0, 255));
				GenerateTexture(pDevice, &Blue, D3DCOLOR_ARGB(255, 0, 0, 255));
				GenerateTexture(pDevice, &Yellow, D3DCOLOR_ARGB(255, 255, 255, 0));
				GenerateTexture(pDevice, &Orange, D3DCOLOR_ARGB(255, 255, 165, 0));
				GenerateTexture(pDevice, &Purple, D3DCOLOR_ARGB(255, 102, 0, 153));
			}

			if (isChams) {
				if ((Stride == 36 && mStartregister == 20 || Stride == 36 && mStartregister == 21 || Stride == 36 && mStartregister == 8 && mVectorCount == 1) || //models
					(Stride == 48 || Stride == 36 && mStartregister == 17))//weapons
				{
					//red for not seen player
					pDevice->SetTexture(0, Red);
					pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

					float bias = 1000.0f;
					float bias_float = static_cast<float>(-bias);
					bias_float /= 10000.0f;
					pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&bias_float);
					DrawIndexedPrimitive_orig(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
					pDevice->SetRenderState(D3DRS_DEPTHBIAS, 0);

					//green for seen player
					pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
					pDevice->SetTexture(0, Green);
				}
			}
			if (isZoom)
			{
				*(PFLOAT)(ultra_zoom) = 0.00545329228;
			}
			if (!isZoom)
			{
				*(PFLOAT)(ultra_zoom) = 0.0174532924;
			}
			if (isSpeed)
			{
				*(PFLOAT)(move_speed) = 90;
			}
			if (!isSpeed)
			{
				*(PFLOAT)(move_speed) = 60;
			}
			if (isGrass)
			{
				*(PFLOAT)(grass1) = -223293.5938;
				*(PFLOAT)(grass2) = 152108848;
			}
			if (!isGrass)
			{
				*(PFLOAT)(grass1) = -3.407189846;
				*(PFLOAT)(grass2) = 2320.996826;
			}
			//worldtoscreen weapons in hand
			if (Stride == 48) //weapons in hand
			{
				AddWeapons(pDevice);
			}


			//logger, hold down P key until a texture disappears
			if (GetAsyncKeyState('[') & 1) //-
			{
				countnum--;
			}
			if (GetAsyncKeyState(']') & 1) //+
			{
				countnum++;
			}
			if (countnum == mStartregister)
			{
				return D3D_OK; //delete texture
			}

			return DrawIndexedPrimitive_orig(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		}
	}
	catch (exception e) {

	}

}

//==========================================================================================================================

HRESULT APIENTRY EndScene_hook(IDirect3DDevice9* pDevice)
{
	if (EndScene_orig != NULL) {

	}
	try {
		//get viewport
		pDevice->GetViewport(&Viewport);
		ScreenCX = (float)Viewport.Width / 2.0f;
		ScreenCY = (float)Viewport.Height / 2.0f;


		if (GetAsyncKeyState(VK_UP))
		{
			isGrass = !isGrass;
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			isSpeed = !isSpeed;
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			isZoom = !isZoom;
		}
		if (GetAsyncKeyState(VK_INSERT))
		{
			!isZoom;
			isSpeed = !isSpeed;
			isChams = !isChams;
		}

		WeaponEspInfo.clear();

		return EndScene_orig(pDevice);
	}
	catch (exception e) {

	}

	//create font
	/*if (Font == NULL)
		D3DXCreateFont(pDevice, 14, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &Font);
*/
//do esp
//removing the ESP
//if (WeaponEspInfo.size() != NULL)
//{
//	for (unsigned int i = 0; i < WeaponEspInfo.size(); i++)
//	{
//		//box esp
//		if (WeaponEspInfo[i].pOutX > 1 && WeaponEspInfo[i].pOutY > 1)
//			DrawBox(pDevice, (int)WeaponEspInfo[i].pOutX, (int)WeaponEspInfo[i].pOutY, 6.0f, 12.0f, D3DCOLOR_ARGB(255, 255, 0, 0));

//		//line esp
//		if (WeaponEspInfo[i].pOutX > 1 && WeaponEspInfo[i].pOutY > 1)
//			DrawLine(pDevice, (int)WeaponEspInfo[i].pOutX, (int)WeaponEspInfo[i].pOutY, ScreenCX, ScreenCY * 1.0f, 20, D3DCOLOR_ARGB(255, 255, 255, 255));//0.1up, 1.0middle, 2.0down

//																																						  //distance esp
//		if (WeaponEspInfo[i].pOutX > 1 && WeaponEspInfo[i].pOutY > 1)
//			DrawString(Font, (int)WeaponEspInfo[i].pOutX, (int)WeaponEspInfo[i].pOutY, D3DCOLOR_ARGB(255, 255, 255, 255), (PCHAR)"%.f", (float)WeaponEspInfo[i].distance / 10.0f);

//		//text esp
//		if (WeaponEspInfo[i].pOutX > 1 && WeaponEspInfo[i].pOutY > 1)
//			DrawString(Font, (int)WeaponEspInfo[i].pOutX, (int)WeaponEspInfo[i].pOutY, D3DCOLOR_ARGB(255, 255, 255, 255), "o");
//	}
//}


/*
//draw logger
if (Font)
{
char szString[255];
sprintf_s(szString, "countnum = %d", countnum);
DrawString(Font, 217, 100, D3DCOLOR_ARGB(255, 0, 0, 0), (char*)&szString[0]);
DrawString(Font, 220, 100, D3DCOLOR_ARGB(255, 255, 255, 255), (char*)&szString[0]);
DrawString(Font, 220, 110, D3DCOLOR_ARGB(255, 255, 255, 255), "hold P to +");
DrawString(Font, 220, 120, D3DCOLOR_ARGB(255, 255, 255, 255), "hold O to -");
}
if (GetAsyncKeyState(0x70))
{
Mem.WriteMemory<float>(Mem.BaseAddress + 0x17586A8, -0.89999998f);
}*/

//setting menus
/*DrawString(Font, 100, 10, D3DCOLOR_ARGB(255, 255, 0, 0), "==================================");
DrawString(Font, 100, 20, D3DCOLOR_ARGB(255, 255, 0, 0), "< INTERNAL HACK >");
DrawString(Font, 100, 30, D3DCOLOR_ARGB(255, 255, 0, 0), "==================================");
DrawString(Font, 100, 50, D3DCOLOR_ARGB(255, 255, 0, 0), "Arrow UP - Green Chams");
DrawString(Font, 100, 60, D3DCOLOR_ARGB(255, 255, 0, 0), "Arrow Down - Red Chams");
DrawString(Font, 100, 70, D3DCOLOR_ARGB(255, 255, 0, 0), "Arrow Left - Yellow Chams");
DrawString(Font, 100, 80, D3DCOLOR_ARGB(255, 255, 0, 0), "Arrow Right - Purple Chams");*/

}
template <class cData>
cData Read(DWORD dwAddress)
{
	try {
		cData cRead;
		ReadProcessMemory(hProcess, (LPVOID)dwAddress, &cRead, sizeof(cData), NULL);
		return cRead;
	}
	catch (exception e) {

	}

}


template <class cData>
cData Read(DWORD dwAddress, char *Offset, BOOL Type)
{
	try {
		int iSize = iSizeOfArray(Offset) - 1;
		dwAddress = Read<DWORD>(dwAddress);


		for (int i = 0; i < iSize; i++) {
			dwAddress = Read<DWORD>(dwAddress + Offset[i]);
		}

		if (!Type) {
			return dwAddress + Offset[iSize];
		}
		else {
			return Read<cData>(dwAddress + Offset[iSize]);
		}
	}
	catch (exception e) {

	}

}


template <class cData>

void Read(DWORD dwAddress, cData Value)
{
	try {

		ReadProcessMemory(hProcess, (LPVOID)dwAddress, &Value, sizeof(cData), NULL);
	}
	catch (exception e) {

	}
}


template <class cData>
void Read(DWORD dwAddress, char *Offset, cData Value)
{
	try {

		Read<cData>(Read<cData>(dwAddress, Offset, false), Value);
	}
	catch (exception e) {

	}
}
//==========================================================================================================================

HRESULT APIENTRY Reset_hook(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	try {
		if (Font) {
			Font->OnLostDevice();
		}

		HRESULT ResetReturn = Reset_orig(pDevice, pPresentationParameters);

		if (SUCCEEDED(ResetReturn))
		{
			if (Font) {
				Font->OnResetDevice();
			}
		}

		return ResetReturn;
	}
	catch (exception e) {

	}
}

//==========================================================================================================================

DWORD WINAPI RosD3D(__in LPVOID lpParameter)
{
	try {
		HMODULE dDll = NULL;
		while (!dDll)
		{
			dDll = GetModuleHandleA("d3d9.dll");
			Sleep(100);
		}
		CloseHandle(dDll);

		IDirect3D9* d3d = NULL;
		IDirect3DDevice9* d3ddev = NULL;

		HWND tmpWnd = CreateWindowA("BUTTON", "RosD3D", WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, NULL, NULL, Hand, NULL);
		if (tmpWnd == NULL)
		{
			//Log("[DirectX] Failed to create temp window");
			return 0;
		}

		d3d = Direct3DCreate9(D3D_SDK_VERSION);
		if (d3d == NULL)
		{
			DestroyWindow(tmpWnd);
			//Log("[DirectX] Failed to create temp Direct3D interface");
			return 0;
		}

		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = tmpWnd;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

		HRESULT result = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, tmpWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
		if (result != D3D_OK)
		{
			d3d->Release();
			DestroyWindow(tmpWnd);
			//Log("[DirectX] Failed to create temp Direct3D device");
			return 0;
		}

		// We have the device, so walk the vtable to get the address of all the dx functions in d3d9.dll
#if defined _M_X64
		DWORD64* dVtable = (DWORD64*)d3ddev;
		dVtable = (DWORD64*)dVtable[0];
#elif defined _M_IX86
		DWORD* dVtable = (DWORD*)d3ddev;
		dVtable = (DWORD*)dVtable[0]; // == *d3ddev
#endif
									  //Log("[DirectX] dVtable: %x", dVtable);

									  //for(int i = 0; i < 95; i++)
									  //{
									  //Log("[DirectX] vtable[%i]: %x, pointer at %x", i, dVtable[i], &dVtable[i]);
									  //}

									  // Detour functions x86 & x64
		if (MH_Initialize() != MH_OK) { return 1; }
		if (MH_CreateHook((DWORD_PTR*)dVtable[42], &EndScene_hook, reinterpret_cast<void**>(&EndScene_orig)) != MH_OK) { return 1; }
		if (MH_EnableHook((DWORD_PTR*)dVtable[42]) != MH_OK) { return 1; }
		if (MH_CreateHook((DWORD_PTR*)dVtable[94], &SetVertexShaderConstantF_hook, reinterpret_cast<void**>(&SetVertexShaderConstantF_orig)) != MH_OK) { return 1; }
		if (MH_EnableHook((DWORD_PTR*)dVtable[94]) != MH_OK) { return 1; }
		if (MH_CreateHook((DWORD_PTR*)dVtable[100], &SetStreamSource_hook, reinterpret_cast<void**>(&SetStreamSource_orig)) != MH_OK) { return 1; }
		if (MH_EnableHook((DWORD_PTR*)dVtable[100]) != MH_OK) { return 1; }
		if (MH_CreateHook((DWORD_PTR*)dVtable[82], &DrawIndexedPrimitive_hook, reinterpret_cast<void**>(&DrawIndexedPrimitive_orig)) != MH_OK) { return 1; }
		if (MH_EnableHook((DWORD_PTR*)dVtable[82]) != MH_OK) { return 1; }
		if (MH_CreateHook((DWORD_PTR*)dVtable[16], &Reset_hook, reinterpret_cast<void**>(&Reset_orig)) != MH_OK) { return 1; }
		if (MH_EnableHook((DWORD_PTR*)dVtable[16]) != MH_OK) { return 1; }

		//Log("[Detours] Detours attached\n");

		d3ddev->Release();
		d3d->Release();
		DestroyWindow(tmpWnd);

		return 1;
	}
	catch (exception) {

	}

}

//==========================================================================================================================

BOOL WINAPI DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	try {
		switch (fdwReason)
		{
		case DLL_PROCESS_ATTACH: // A process is loading the DLL.
			Hand = hinstDLL;
			DisableThreadLibraryCalls(hinstDLL); // disable unwanted thread notifications to reduce overhead
			CreateThread(0, 0, RosD3D, 0, 0, 0); //init our hooks
			break;

		case DLL_PROCESS_DETACH: // A process unloads the DLL.
			break;
		}
		return TRUE;
	}
	catch (exception e) {

	}
}