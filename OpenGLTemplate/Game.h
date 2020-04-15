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
class CBomb;
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
	CSkybox* m_pNightbox;
	CCamera *m_pCamera;
	vector <CShaderProgram *> *m_pShaderPrograms;
	CFreeTypeFont *m_pFtFont;
	COpenAssetImportMesh* m_pOakMesh; 
	COpenAssetImportMesh* m_pPavilionMesh;
	COpenAssetImportMesh* m_pSaturnRingMesh;
	COpenAssetImportMesh* m_pBikeMesh; 
	COpenAssetImportMesh* m_pBirdMesh;
	CSphere *m_pSphere;
	CTetrahedron* m_pTetrahedron;
	CTetrahedron* m_pTrackWall;
	CUrchin* m_pUrchin;
	CHeightMapTerrain* m_pHeightmapTerrain;
	CHighResolutionTimer *m_pHighResolutionTimer;
	CAudio *m_pAudio;
	CCatmullRom* m_pCatmullRom; 
	CPlayer* m_pPlayer; 
	vector <CPickup*> *m_pPickups;
	vector <CBomb*>* m_pBombs;
	CFrameBufferObject* m_pFBO;
	CPlane* m_pTV;
	CPlane* m_pHeart;
	CPlane* m_pIntro;
	CPlane* m_pDeath;
	CPlane* m_pFilter;

	// Some other member variables
	double m_dt;
	float m_t;
	int m_framesPerSecond;
	bool m_appActive;
	int m_levels;
	float m_currentDistance; 
	float m_cameraSpeed; 
	int m_score;
	int m_health;
	bool m_lightswitch;
	bool m_TVActive;
	float m_close;
	float m_trap;
	float m_lightup;
	int m_pickup_num;
	int m_bomb_num;
	int m_lives;
	int m_cameraControl;
	bool m_freeview;
	bool m_start;
	int m_timerStart;
	int m_switchColour;
	int width;  //screen width
	int height; //screen height

	//store quaternion vector values at initiaization bc 
	//performance intensive in runtime
	vector<glm::vec3> wall_lefttrack;
	vector<glm::vec3> wall_righttrack; 
	vector<glm::mat4> wall_lefttrackrot; 
	vector<glm::mat4> wall_righttrackrot; 
	vector<glm::vec3> pavilion_track;
	vector<glm::mat4> pavilion_trackrot;
	vector<glm::vec3> ring_track;
	vector<glm::mat4> ring_trackrot;

public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();

private:
	static const int FPS = 60;
	void DisplayHUD(int pass);
	void GameLoop();
	void GameStart();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;

	void CameraControl(glm::vec3& pos, glm::vec3 &player, glm::vec3& viewpt, glm::vec3& strafe, glm::vec3& up, glm::vec3& upnext);
	void ColourControl(CShaderProgram* shaderProgram);
	glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);
	glm::quat LookAt(glm::vec3 direction, glm::vec3 desiredUp);
};


