#pragma once

#include "Common.h"
#include "GameWindow.h"

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.   
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class CSphere;
class COpenAssetImportMesh;
class CAudio;
class CTetrahedron;
class CUrchin;
class CHeightMapTerrain;
class CCatmullRom;
class CPlayer; 
class CPickup;
class CFrameBufferObject;

class Game {
private:
	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Update();
	void Render();
	void RenderScene(int pass);

	// Pointers to game objects.  They will get allocated in Game::Initialise()
	CSkybox *m_pSkybox;
	CCamera *m_pCamera;
	vector <CShaderProgram *> *m_pShaderPrograms;
	CFreeTypeFont *m_pFtFont;
	COpenAssetImportMesh *m_pBarrelMesh;
	COpenAssetImportMesh *m_pHorseMesh;
	COpenAssetImportMesh* m_pTreeMesh;
	COpenAssetImportMesh* m_pOakMesh; 
	COpenAssetImportMesh* m_pPavilionMesh;
	COpenAssetImportMesh* m_pSaturnRingMesh;
	COpenAssetImportMesh* m_pCowMesh;
	COpenAssetImportMesh* m_pBikeMesh; 
	COpenAssetImportMesh* m_pBirdMesh; 
	CSphere *m_pSphere;
	CTetrahedron* m_pTetrahedron;
	CUrchin* m_pUrchin;
	CHeightMapTerrain* m_pHeightmapTerrain;
	CHighResolutionTimer *m_pHighResolutionTimer;
	CAudio *m_pAudio;
	CCatmullRom* m_pCatmullRom; 
	CPlayer* m_pPlayer; 
	CPickup* m_pPickup;
	CFrameBufferObject* m_pFBO;
	CPlane* m_pTV;

	// Some other member variables
	double m_dt;
	float m_t;
	int m_framesPerSecond;
	bool m_appActive;
	int m_levels;
	float m_currentDistance; 
	float m_cameraSpeed; 
	float m_score;
	bool m_lightswitch;
	bool m_TVActive;
	float m_close;
	float m_lightup;

public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();

private:
	static const int FPS = 60;
	void DisplayFrameRate();
	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;
};


