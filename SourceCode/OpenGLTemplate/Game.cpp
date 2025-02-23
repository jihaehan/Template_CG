/* 
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting, 
 different camera controls, different shaders, etc.
 
 Template version 5.0a 29/01/2017
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 

 version 6.0a 29/01/2019
 Dr Eddie Edwards (Philip.Edwards@city.ac.uk)
*/


#include "game.h"


// Setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"

// Game includes
#include "Camera.h"
#include "Skybox.h"
#include "Plane.h"
#include "Tetrahedron.h"
#include "Urchin.h"
#include "Quad.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "Sphere.h"
#include "HeightMapTerrain.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "CatmullRom.h"
#include "Player.h"
#include "Pickup.h"
#include "Bomb.h"
#include "FrameBufferObject.h"

// Constructor
Game::Game()
{
	m_pSkybox = NULL;
	m_pNightbox = NULL;
	m_pCamera = NULL;
	m_pShaderPrograms = NULL;
	m_pFtFont = NULL;
	m_pOakMesh = NULL;
	m_pPavilionMesh = NULL;
	m_pSaturnRingMesh = NULL;
	m_pBikeMesh = NULL;
	m_pBirdMesh = NULL;
	m_pSphere = NULL;
	m_pTetrahedron = NULL;
	m_pTrackWall = NULL;
	m_pUrchin = NULL;
	m_pQuad = NULL;
	m_pHeightmapTerrain = NULL;
	m_pHighResolutionTimer = NULL;
	m_pAudio = NULL;
	m_pCatmullRom = NULL;
	m_pPlayer = NULL; 
	m_pPickups = NULL;
	m_pBombs = NULL;
	m_pFBO = NULL;
	m_pTV = NULL;
	m_pIntro = NULL;
	m_pInstruct = NULL;
	m_pDeath = NULL;
	m_pHeart = NULL;
	m_pFilter = NULL;

	// Initialize member variables
	m_dt = 0.0;
	m_t = 0.0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;
	m_levels = 3;
	m_currentDistance = 0.f;
	m_cameraSpeed = 0.05f;
	m_score = 0; 
	m_health = 50;
	m_lightswitch = true;
	m_TVActive = false;
	m_pickup_num = 40;
	m_bomb_num = 20;
	m_lives = 3;
	m_cameraControl = 1;
	m_freeview = false; 
	m_start = -1;
	m_timerStart = 4;
	m_switchColour = 0;
	m_treeDist = 0;
	m_treeExplode = 0;

}

// Destructor
Game::~Game() 
{ 
	//game objects
	delete m_pCamera;
	delete m_pSkybox;
	delete m_pNightbox;
	delete m_pFtFont;
	delete m_pOakMesh;
	delete m_pPavilionMesh;
	delete m_pSaturnRingMesh;
	delete m_pBikeMesh;
	delete m_pBirdMesh; 
	delete m_pSphere;
	delete m_pTetrahedron;
	delete m_pTrackWall;
	delete m_pUrchin;
	delete m_pQuad;
	delete m_pHeightmapTerrain;
	delete m_pAudio;
	delete m_pCatmullRom; 
	delete m_pPlayer;
	delete m_pFBO;
	delete m_pTV;
	delete m_pIntro;
	delete m_pInstruct;
	delete m_pDeath;
	delete m_pHeart;
	delete m_pFilter;

	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	if (m_pPickups != NULL) {
		for (unsigned int i = 0; i < m_pPickups->size(); i++)
			delete (*m_pPickups)[i];
	}
	delete m_pPickups;

	if (m_pBombs != NULL) {
		for (unsigned int i = 0; i < m_pBombs->size(); i++)
			delete (*m_pBombs)[i];
	}
	delete m_pBombs;

	//setup objects
	delete m_pHighResolutionTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{
	// Set the clear colour and depth
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	/// Create objects
	m_pCamera = new CCamera;
	m_pSkybox = new CSkybox;
	m_pNightbox = new CSkybox;
	m_pShaderPrograms = new vector <CShaderProgram *>;
	m_pFtFont = new CFreeTypeFont;
	m_pOakMesh = new COpenAssetImportMesh;
	m_pPavilionMesh = new COpenAssetImportMesh;
	m_pSaturnRingMesh = new COpenAssetImportMesh;
	m_pBikeMesh = new COpenAssetImportMesh;
	m_pBirdMesh = new COpenAssetImportMesh;
	m_pSphere = new CSphere;
	m_pTetrahedron = new CTetrahedron;
	m_pTrackWall = new CTetrahedron;
	m_pUrchin = new CUrchin;
	m_pQuad = new CQuad;
	m_pHeightmapTerrain = new CHeightMapTerrain;
	m_pAudio = new CAudio;
	m_pCatmullRom = new CCatmullRom;
	m_pPlayer = new CPlayer; 
	m_pPickups = new vector <CPickup*>;
	m_pBombs = new vector <CBomb*>;
	m_pFBO = new CFrameBufferObject;
	m_pTV = new CPlane;
	m_pIntro = new CPlane;
	m_pInstruct = new CPlane;
	m_pDeath = new CPlane;
	m_pHeart = new CPlane;
	m_pFilter = new CPlane;

	RECT dimensions = m_gameWindow.GetDimensions();

	width = dimensions.right - dimensions.left;
	height = dimensions.bottom - dimensions.top;

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(width, height); 
	m_pCamera->SetPerspectiveProjectionMatrix(45.0f, (float) width / (float) height, 0.5f, 5000.0f);
	
	//============================ SHADERS ======================================//

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("mainShader.vert");	   //modified main shader to accomodate spotlights and etc.
	sShaderFileNames.push_back("mainShader.frag");
	sShaderFileNames.push_back("textShader.vert");	   //modified text shader to allow other 2D rendering
	sShaderFileNames.push_back("textShader.frag");
	sShaderFileNames.push_back("sphereShaderEd.vert");  //modified version of Ed's toon shader
	sShaderFileNames.push_back("sphereShaderEd.geom");
	sShaderFileNames.push_back("sphereShaderEd.frag"); 
	sShaderFileNames.push_back("treeShader.vert");		//treeShader for instanced rendering
	sShaderFileNames.push_back("treeShader.geom");
	sShaderFileNames.push_back("treeShader.frag");
	sShaderFileNames.push_back("explodeShader.vert");	//explosion shader for bombs
	sShaderFileNames.push_back("explodeShader.geom");
	sShaderFileNames.push_back("explodeShader.frag");	

	for (int i = 0; i < (int) sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int) sShaderFileNames[i].size()-4, 4);
		int iShaderType;
		if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
		else iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
		shader.LoadShader("resources\\shaders\\"+sShaderFileNames[i], iShaderType);
		shShaders.push_back(shader);
	}

	// Create the main shader program
	CShaderProgram *pMainProgram = new CShaderProgram;		
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);
	pMainProgram->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram);				//m_pShaderPrograms[0]

	// Create a shader program for fonts
	CShaderProgram *pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	m_pShaderPrograms->push_back(pFontProgram);				//m_pShaderPrograms[1]

	// Create the toon shader program
	CShaderProgram* pToonProgram = new CShaderProgram;
	pToonProgram->CreateProgram();
	pToonProgram->AddShaderToProgram(&shShaders[4]);
	pToonProgram->AddShaderToProgram(&shShaders[5]);
	pToonProgram->AddShaderToProgram(&shShaders[6]);
	pToonProgram->LinkProgram();
	m_pShaderPrograms->push_back(pToonProgram);				//m_pShaderPrograms[2]

	// Create the tree shader program
	CShaderProgram* pTreeShader = new CShaderProgram;
	pTreeShader->CreateProgram();
	pTreeShader->AddShaderToProgram(&shShaders[7]);
	pTreeShader->AddShaderToProgram(&shShaders[8]);
	pTreeShader->AddShaderToProgram(&shShaders[9]);
	pTreeShader->LinkProgram();
	m_pShaderPrograms->push_back(pTreeShader);				//m_pShaderPrograms[3]

	CShaderProgram* pExplodeShader = new CShaderProgram;
	pExplodeShader->CreateProgram();
	pExplodeShader->AddShaderToProgram(&shShaders[10]);
	pExplodeShader->AddShaderToProgram(&shShaders[11]);
	pExplodeShader->AddShaderToProgram(&shShaders[12]);
	pExplodeShader->LinkProgram();
	m_pShaderPrograms->push_back(pExplodeShader);			//m_pShaderPrograms[4]

	// You can follow this pattern to load additional shaders

	//============================ OBJECTS, CREATE ======================================//
	// Create the skybox and nightbox
	m_pSkybox->Create(2500.0f, true);		//created by Jihae Han
	m_pNightbox->Create(2500.0f, false);	//created by Jihae Han
	
	m_pFtFont->LoadSystemFont("OCRAEXT.ttf", 32);
	m_pFtFont->SetShaderProgram(pFontProgram);

	// Load some meshes in OBJ/3ds format
	m_pOakMesh->Load("resources\\models\\Tree\\quercus_A_packed_v1.obj");	//https://opengameart.org/content/oak-tree downloaded on March 19
	m_pPavilionMesh->Load("resources\\models\\Pavilion\\pavilion_textured.3ds"); //created by Jihae Han
	m_pSaturnRingMesh->Load("resources\\models\\Saturn_Ring\\saturn_ring.3ds");	//created by Jihae Han
	m_pBikeMesh->Load("resources\\models\\Warbike\\bike.obj");	//https://opengameart.org/content/harakiri-warbike-1000cc downloaded on March 21
	m_pBirdMesh->Load("resources\\models\\Bird\\CARDNL_F.3ds"); //https://www.turbosquid.com/FullPreview/Index.cfm/ID/1100939 downloaded on April 3

	// Create a sphere
	m_pSphere->Create("resources\\textures\\", "intro.png", 25, 25);  
	glEnable(GL_CULL_FACE);

	// Create a tetrahedron
	m_pTetrahedron->Create("resources\\textures\\", "green_marble.png", 5);  // Texture downloaded and modified from http://www.freepik.com on March 14 2020
	glEnable(GL_CULL_FACE);
	m_pTrackWall->Create("resources\\textures\\", "green_marble.png", 14);  //  Texture downloaded and modified from http://www.freepik.com on March 14 2020
	glEnable(GL_CULL_FACE);

	// Create an urchin
	m_pUrchin->Create("resources\\textures\\", "dirtpile02.png", 4);  // Texture modified from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
	glEnable(GL_CULL_FACE);

	// Create a quad
	m_pQuad->Create(glm::vec4(1));
		
	// Create a heightmap terrain
	m_pHeightmapTerrain->Create("resources\\textures\\heightmap_s.png", "resources\\textures\\vangogh_sower6.png", glm::vec3(0, 0, 0), 900.0f, 900.0f, 150.f); 
		//texture modified from https://www.nytimes.com/2015/06/12/arts/design/review-van-gogh-and-nature-exploring-the-outside-world-in-high-relief.html
		//and donwloaded on Feb 20

	//============================ CATMULL ====================================//
	m_pCatmullRom->CreateCentreline();
	m_pCatmullRom->CreateOffsetCurves();
	m_pCatmullRom->CreateTrack("resources\\textures\\", "dirtpile01.jpg");
	m_pCatmullRom->ComputeTrackPoints();

	//initialize offset track demarcators
	for (int i = 0; i < 100; i++) {
		int j = i * 4;
		wall_lefttrack.push_back(m_pCatmullRom->GetLeftOffsetPoints()[j]); 
		wall_lefttrackrot.push_back(glm::toMat4(Game::LookAt(m_pCatmullRom->GetUpPoints()[j], -m_pCatmullRom->GetOffsetPoints()[j])));
		wall_righttrack.push_back(m_pCatmullRom->GetRightOffsetPoints()[j]);
		wall_righttrackrot.push_back(glm::toMat4(Game::LookAt(m_pCatmullRom->GetUpPoints()[j], m_pCatmullRom->GetOffsetPoints()[j])));
	}

	for (int i = 0; i < 5; i++) {
		int j = i * 25 + 100; 
		pavilion_track.push_back(m_pCatmullRom->GetCentrelinePoints()[j] + m_pCatmullRom->GetOffsetPoints()[j] * 40.f);
		pavilion_trackrot.push_back(glm::toMat4(Game::LookAt(m_pCatmullRom->GetOffsetPoints()[j], m_pCatmullRom->GetUpPoints()[j])));
	}

	// Render the saturn ring
	for (int i = 0; i < 7; i++)
	{
		int j = i * 15; 
		ring_track.push_back(m_pCatmullRom->GetCentrelinePoints()[j]);
		ring_trackrot.push_back(glm::toMat4(Game::LookAt(m_pCatmullRom->GetUpPoints()[j + 1], normalize(m_pCatmullRom->GetCentrelinePoints()[j + 1] - m_pCatmullRom->GetCentrelinePoints()[j]))));
	}

	//==================== Player, Pickups, and Bombs =========================//

	// Initialise Player 
	m_pPlayer->Initialise(m_pBikeMesh);
	srand(time(NULL)); //initialize random value

	// Initialise Vector of Pickup
	for (int i = 0; i < m_pickup_num; i++) {
		int random = rand() % 400;
		glm::vec3 pickup_pos = m_pCatmullRom->GetTrackPoints()[random] + (m_pCatmullRom->GetOffsetPoints()[random])*(float)(rand()%20);
		m_pPickups->push_back(new CPickup(m_pSphere, pickup_pos));
	}

	// Initialise Vector of Bomb
	for (int i = 0; i < m_bomb_num; i++) {
		int random = rand() % 400;
		glm::vec3 bom_pos = m_pCatmullRom->GetTrackPoints()[random] + (m_pCatmullRom->GetOffsetPoints()[random]) * (float)(rand() % 19);
		m_pBombs->push_back(new CBomb(m_pUrchin, bom_pos));
	}

	//============================ AUDIO ======================================//
	// Initialise audio and play background music
	
	m_pAudio->Initialise();
	m_pAudio->LoadEventSound("resources\\Audio\\Boing.wav");		// Royalty free sound from freesound.org
	m_pAudio->LoadMusicStream("resources\\Audio\\DST-Canopy.mp3");	// Royalty free music from http://www.nosoapradio.us/. Downloaded on Feb 4;
	m_pAudio->PlayMusicStream();
	

	//============================ FBO AND SCREENS =============================//

	m_pTV->Create("resources\\textures\\", "grassfloor01.jpg", width/3, height/3, 1.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
	m_pIntro->Create("resources\\textures\\", "intro.png", width, height, 1.0f);	//texture made by Jihae Han, with modified assets from http://www.freepik.com on April 2 2020
	m_pInstruct->Create("resources\\textures\\", "instructions.png", width, height, 1.0f);	//texture made by Jihae Han, with modified assets from http://www.freepik.com on April 25 2020
	m_pDeath->Create("resources\\textures\\", "death.png", width, height, 1.0f);	//created by Jihae Han
	m_pFilter->Create("resources\\textures\\", "death.png", width, height, 1.0f);	
	m_pHeart->Create("resources\\textures\\", "heart.png", 25.f, 25.f, 1.f);		//created by Jihae Han
	m_pFBO->Create(width, height);
}

// Render method runs repeatedly in a loop
void Game::Render() 
{
	m_pFBO->Bind(); 
	RenderScene(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	RenderScene(1);

	// Swap buffers to show the rendered image
	SwapBuffers(m_gameWindow.Hdc());
}

void Game::RenderScene(int pass)
{
	// Clear the buffers and enable depth testing (z-buffering)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	//============================ MAIN SHADER ======================================//
	// Use the main shader program 
	CShaderProgram* pMainProgram = (*m_pShaderPrograms)[0];
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("bUseTransparency", false);
	pMainProgram->SetUniform("vTransparency", 0.f);
	pMainProgram->SetUniform("sampler0", 0);
	// Note: cubemap and non-cubemap textures should not be mixed in the same texture unit.  Setting unit 10 to be a cubemap texture.
	int cubeMapTextureUnit = 10;
	pMainProgram->SetUniform("CubeMapTex", cubeMapTextureUnit);

	// Set the projection matrix
	pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
	// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
	
	if (pass == 0 && m_TVActive == true) //create top-down view that follows player for minimap.
		modelViewMatrixStack.LookAt(m_pPlayer->GetPosition() + glm::vec3(0,1,0)*100.f,m_pPlayer->GetPosition(), m_pPlayer->GetView());
	else
 		modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());

	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);

	// Set light and materials in main shader program
	glm::vec4 lightPosition1(-150, 200, -50, 1);
	glm::vec4 spotLightPosition0(-150, 500, -50, 1);
	glm::vec4 spotLightPosition1(m_pPlayer->GetPosition() + m_pPlayer->GetUpVector() * 10.f, 1);
	glm::vec4 spotLightPosition2(m_pPlayer->GetPosition() + m_pPlayer->GetUpVector() * 21.f, 1);
	glm::vec4 spotLightPosition3(-700, 500, 350, 1);
	pMainProgram->SetUniform("light1.position", viewMatrix * lightPosition1);	// Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light1.La", glm::vec3(1.0f * m_lightswitch));		// Ambient colour of light
	pMainProgram->SetUniform("light1.Ld", glm::vec3(1.0f * m_lightswitch));		// Diffuse colour of light
	pMainProgram->SetUniform("light1.Ls", glm::vec3(1.0f * m_lightswitch));		// Specular colour of light
	pMainProgram->SetUniform("spotlight[0].position", viewMatrix * spotLightPosition0);
	pMainProgram->SetUniform("spotlight[0].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, -1, 0)));
	pMainProgram->SetUniform("spotlight[0].La", glm::vec3(.7f - .7f * m_lightswitch, 0.f, 1.0f - 1.0f * m_lightswitch));
	pMainProgram->SetUniform("spotlight[0].Ld", glm::vec3(.7f - .7f * m_lightswitch, 0.f, 1.0f - 1.0f * m_lightswitch));
	pMainProgram->SetUniform("spotlight[0].Ls", glm::vec3(.7f - .7f * m_lightswitch, 0.f, 1.0f - 1.0f * m_lightswitch));
	pMainProgram->SetUniform("spotlight[0].exponent", 20.0f);
	pMainProgram->SetUniform("spotlight[0].cutoff", 100.f);
	pMainProgram->SetUniform("spotlight[1].position", viewMatrix * spotLightPosition1);
	pMainProgram->SetUniform("spotlight[1].direction", glm::normalize(viewNormalMatrix * (m_pPlayer->GetView() - m_pPlayer->GetUpVector())));
	pMainProgram->SetUniform("spotlight[1].La", glm::vec3(0.f, 1.0f - 1.0f * m_lightswitch, 1.0f - 1.0f * m_lightswitch));
	pMainProgram->SetUniform("spotlight[1].Ld", glm::vec3(0.f, 1.0f - 1.0f * m_lightswitch, 1.0f - 1.0f * m_lightswitch));
	pMainProgram->SetUniform("spotlight[1].Ls", glm::vec3(0.f, 1.0f - 1.0f * m_lightswitch, 1.0f - 1.0f * m_lightswitch));
	pMainProgram->SetUniform("spotlight[1].exponent", 10.0f);
	pMainProgram->SetUniform("spotlight[1].cutoff", 50.f);
	pMainProgram->SetUniform("spotlight[2].position", viewMatrix * spotLightPosition2);
	pMainProgram->SetUniform("spotlight[2].direction", glm::normalize(viewNormalMatrix * (-m_pPlayer->GetUpVector())));
	pMainProgram->SetUniform("spotlight[2].La", glm::vec3(0.f, 1.0f - 1.0f * m_lightswitch, 1.0f - 1.0f * m_lightswitch));
	pMainProgram->SetUniform("spotlight[2].Ld", glm::vec3(0.f, 1.0f - 1.0f * m_lightswitch, 1.0f - 1.0f * m_lightswitch));
	pMainProgram->SetUniform("spotlight[2].Ls", glm::vec3(0.f, 1.0f - 1.0f * m_lightswitch, 1.0f - 1.0f * m_lightswitch));
	pMainProgram->SetUniform("spotlight[2].exponent", 30.0f);
	pMainProgram->SetUniform("spotlight[2].cutoff", 30.f);
	pMainProgram->SetUniform("spotlight[3].position", viewMatrix * spotLightPosition3);
	pMainProgram->SetUniform("spotlight[3].direction", glm::normalize(viewNormalMatrix * glm::vec3(1, -1, -1)));
	pMainProgram->SetUniform("spotlight[3].La", glm::vec3(0.f, 0.f, 1.0f - 1.0f * m_lightswitch));
	pMainProgram->SetUniform("spotlight[3].Ld", glm::vec3(0.f, 0.f, 1.0f - 1.0f * m_lightswitch));
	pMainProgram->SetUniform("spotlight[3].Ls", glm::vec3(0.f, 0.f, 1.0f - 1.0f * m_lightswitch));
	pMainProgram->SetUniform("spotlight[3].exponent", 20.0f);
	pMainProgram->SetUniform("spotlight[3].cutoff", 100.f);
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.9f * m_lightswitch));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.6f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance
	pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property

	// Render the skybox/nightbox and terrain with full ambient reflectance 
	modelViewMatrixStack.Push();
	pMainProgram->SetUniform("renderSkybox", true);
	// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
	glm::vec3 vEye = m_pCamera->GetPosition();
	modelViewMatrixStack.Translate(vEye);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	if (m_lightswitch) m_pSkybox->Render(cubeMapTextureUnit);
	else m_pNightbox->Render(cubeMapTextureUnit);
	pMainProgram->SetUniform("renderSkybox", false);
	modelViewMatrixStack.Pop();

	// Render the heightmap terrain
	pMainProgram->SetUniform("renderTerrain", m_lightswitch);
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(-140.f, 40.f, 0.f));
	modelViewMatrixStack.RotateRadians({ 0.f, 1.f, 0.f }, (float)M_PI);
	modelViewMatrixStack.Scale({ 1.f });
	pMainProgram->SetUniform("sampler0", 0);
	pMainProgram->SetUniform("sampler1", 1);
	pMainProgram->SetUniform("sampler2", 2);
	pMainProgram->SetUniform("sampler3", 3);
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.8f * m_lightswitch));	
	pMainProgram->SetUniform("material1.shininess", 50.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	if (m_lightswitch) m_pHeightmapTerrain->RenderDay(); else m_pHeightmapTerrain->RenderDark();
	modelViewMatrixStack.Pop();
	pMainProgram->SetUniform("renderTerrain", false);

	// Turn on diffuse + specular materials 
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f * m_lightswitch));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	
	pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property

	// Render the tetrahedron towers
	for (int i = 0; i < 20; i++) {
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(-154.0f, 10.f + i * 20.f, -323.f));
		modelViewMatrixStack.RotateRadians(glm::vec3(0.5f, 1.f, 0.5f), (float)M_PI/(3) * i);
		modelViewMatrixStack.Scale(10.f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pTetrahedron->Render();
		modelViewMatrixStack.Pop();
	}
	for (int i = 0; i < 25; i++) {
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(-445.0f, 10.f + i * 10.f, -333.f));
		modelViewMatrixStack.RotateRadians(glm::vec3(0.5f, 1.f, 0.5f), (float)M_PI / (4) * i);
		modelViewMatrixStack.Scale(6.f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pTetrahedron->Render();
		modelViewMatrixStack.Pop();
	}

	//Change lighting settins so that the objects will have some ambient light in 'dark mode'
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.2f + 0.7f * m_lightswitch));	
	pMainProgram->SetUniform("material1.Ms", glm::vec3(5.0f));	
	pMainProgram->SetUniform("material1.shininess", 50.0f);	

	// Render the pavilion 
	for (int i = 0; i < 5; i++) {
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(pavilion_track[i]);
		modelViewMatrixStack.RotateQuat(pavilion_trackrot[i]);
		modelViewMatrixStack.Scale(6.2f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pPavilionMesh->Render();
		modelViewMatrixStack.Pop();
	}

	// Render the saturn ring
	for (int i = 0; i < 7; i++)
	{
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(ring_track[i]);
		modelViewMatrixStack.RotateQuat(ring_trackrot[i]);
		modelViewMatrixStack.Scale(20.0f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSaturnRingMesh->Render();
		modelViewMatrixStack.Pop();
	}

	//============================ TRANSPARENCY ==========================================//
	pMainProgram->SetUniform("bUseTexture", 1 - m_lightswitch); 
	pMainProgram->SetUniform("bUseTransparency", m_lightswitch);

	// Render Track 
	modelViewMatrixStack.Push(); {
		pMainProgram->SetUniform("material1.shininess", 50.0f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pCatmullRom->RenderTrack();
	} modelViewMatrixStack.Pop();

	// Render the player
	pMainProgram->SetUniform("vTransparency", 0.35f);
	m_pPlayer->Render(modelViewMatrixStack, pMainProgram, m_pCamera);
	
	//============================ TREE SHADER ======================================//
	// Switch to the tree program
	// tree program has instanced rendering, so the 5 trees rendered below appear as a forest of trees
	
	CShaderProgram* pTreeProgram = (*m_pShaderPrograms)[3];
	pTreeProgram->UseProgram();
	pTreeProgram->SetUniform("sampler0", 0);
	pTreeProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	//Setup lighting
	pTreeProgram->SetUniform("light1.position", viewMatrix * lightPosition1);	
	pTreeProgram->SetUniform("light1.La", glm::vec3(1.0f * m_lightswitch));		
	pTreeProgram->SetUniform("light1.Ld", glm::vec3(1.0f * m_lightswitch));		
	pTreeProgram->SetUniform("light1.Ls", glm::vec3(1.0f * m_lightswitch));	
	pTreeProgram->SetUniform("spotlight[0].position", viewMatrix * spotLightPosition0);
	pTreeProgram->SetUniform("spotlight[0].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, -1, 0)));
	pTreeProgram->SetUniform("spotlight[0].La", glm::vec3(.7f - .7f * m_lightswitch, 0.f, 1.0f - 1.0f * m_lightswitch));
	pTreeProgram->SetUniform("spotlight[0].Ld", glm::vec3(.7f - .7f * m_lightswitch, 0.f, 1.0f - 1.0f * m_lightswitch));
	pTreeProgram->SetUniform("spotlight[0].Ls", glm::vec3(.7f - .7f * m_lightswitch, 0.f, 1.0f - 1.0f * m_lightswitch));
	pTreeProgram->SetUniform("spotlight[0].exponent", 20.0f);
	pTreeProgram->SetUniform("spotlight[0].cutoff", 100.f);
	pTreeProgram->SetUniform("spotlight[1].position", viewMatrix * spotLightPosition1);
	pTreeProgram->SetUniform("spotlight[1].direction", glm::normalize(viewNormalMatrix * (m_pPlayer->GetView() - m_pPlayer->GetUpVector())));
	pTreeProgram->SetUniform("spotlight[1].La", glm::vec3(0.f, 1.0f - 1.0f * m_lightswitch, 1.0f - 1.0f * m_lightswitch));
	pTreeProgram->SetUniform("spotlight[1].Ld", glm::vec3(0.f, 1.0f - 1.0f * m_lightswitch, 1.0f - 1.0f * m_lightswitch));
	pTreeProgram->SetUniform("spotlight[1].Ls", glm::vec3(0.f, 1.0f - 1.0f * m_lightswitch, 1.0f - 1.0f * m_lightswitch));
	pTreeProgram->SetUniform("spotlight[1].exponent", 10.0f);
	pTreeProgram->SetUniform("spotlight[1].cutoff", 50.f);
	pTreeProgram->SetUniform("spotlight[2].position", viewMatrix * spotLightPosition2);
	pTreeProgram->SetUniform("spotlight[2].direction", glm::normalize(viewNormalMatrix * (-m_pPlayer->GetUpVector())));
	pTreeProgram->SetUniform("spotlight[2].La", glm::vec3(0.f, 1.0f - 1.0f * m_lightswitch, 1.0f - 1.0f * m_lightswitch));
	pTreeProgram->SetUniform("spotlight[2].Ld", glm::vec3(0.f, 1.0f - 1.0f * m_lightswitch, 1.0f - 1.0f * m_lightswitch));
	pTreeProgram->SetUniform("spotlight[2].Ls", glm::vec3(0.f, 1.0f - 1.0f * m_lightswitch, 1.0f - 1.0f * m_lightswitch));
	pTreeProgram->SetUniform("spotlight[2].exponent", 30.0f);
	pTreeProgram->SetUniform("spotlight[2].cutoff", 30.f);
	pTreeProgram->SetUniform("spotlight[3].position", viewMatrix * spotLightPosition3);
	pTreeProgram->SetUniform("spotlight[3].direction", glm::normalize(viewNormalMatrix * glm::vec3(1, -1, -1)));
	pTreeProgram->SetUniform("spotlight[3].La", glm::vec3(0.f, 0.f, 1.0f - 1.0f * m_lightswitch));
	pTreeProgram->SetUniform("spotlight[3].Ld", glm::vec3(0.f, 0.f, 1.0f - 1.0f * m_lightswitch));
	pTreeProgram->SetUniform("spotlight[3].Ls", glm::vec3(0.f, 0.f, 1.0f - 1.0f * m_lightswitch));
	pTreeProgram->SetUniform("spotlight[3].exponent", 20.0f);
	pTreeProgram->SetUniform("spotlight[3].cutoff", 100.f);
	pTreeProgram->SetUniform("material1.Ma", glm::vec3(0.5f * m_lightswitch));	
	pTreeProgram->SetUniform("material1.Md", glm::vec3(0.5f));	
	pTreeProgram->SetUniform("material1.Ms", glm::vec3(1.0f));		
	pTreeProgram->SetUniform("material1.shininess", 15.0f);		
	pTreeProgram->SetUniform("t", m_t);		

	// Render the oak trees
	glDisable(GL_CULL_FACE);
	glm::vec3 treePosition = glm::vec3(50.0f, 0.0f, 0.0f);
	treePosition.y = m_pHeightmapTerrain->ReturnGroundHeight(treePosition);
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(treePosition);
	modelViewMatrixStack.Scale(6.f);
	pTreeProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pTreeProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pOakMesh->RenderInstances(30);
	modelViewMatrixStack.Pop();

	//this particular set of trees is set to explode upon proximity
	treePosition = glm::vec3(-300.0f, -25, -40);
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), M_PI / 24.f);
	modelViewMatrixStack.Translate(treePosition);
	modelViewMatrixStack.Scale(7.f);
	pTreeProgram->SetUniform("explodeFactor", pow(m_treeExplode, 4)); //feed information for explosion size to shader
	pTreeProgram->SetUniform("dist", m_treeDist);	//feed proximity information to shader
	pTreeProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pTreeProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pOakMesh->RenderInstances(30);
	modelViewMatrixStack.Pop();
	
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), M_PI / 24.f);
	modelViewMatrixStack.Translate(treePosition);
	modelViewMatrixStack.Scale(7.f);
	pTreeProgram->SetUniform("dist", 100.f);
	pTreeProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pTreeProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pOakMesh->RenderInstances(30);
	modelViewMatrixStack.Pop();

	treePosition = glm::vec3(-200.0f, -50, -350);
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(treePosition);
	modelViewMatrixStack.Scale(6.f);
	pTreeProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pTreeProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pOakMesh->RenderInstances(30);
	modelViewMatrixStack.Pop();

	treePosition = glm::vec3(-600, -50, -400);
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), 0.78f);
	modelViewMatrixStack.Translate(treePosition);
	modelViewMatrixStack.Scale(5.f);
	pTreeProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pTreeProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pOakMesh->RenderInstances(30);
	modelViewMatrixStack.Pop();

	glEnable(GL_CULL_FACE);

	//============================ EXPLODE SHADER ===================================//
	// It makes bombs explode upon contact
	CShaderProgram* pExplodeProgram = (*m_pShaderPrograms)[4];
	pExplodeProgram->UseProgram();
	pExplodeProgram->SetUniform("material1.Ma", glm::vec3(0.5f * m_lightswitch));	// Ambient material reflectance
	pExplodeProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pExplodeProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	
	pExplodeProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property
	pExplodeProgram->SetUniform("light1.La", glm::vec3(1.f, 1.f, 1.f));
	pExplodeProgram->SetUniform("light1.Ld", glm::vec3(1.0f, 1.0f, 1.0f));
	pExplodeProgram->SetUniform("light1.Ls", glm::vec3(1.0f, 1.0f, 1.0f));
	pExplodeProgram->SetUniform("light1.position", viewMatrix* lightPosition1);
	pExplodeProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
	pExplodeProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	pExplodeProgram->SetUniform("sampler0", 0);

	// Render the Bomb  
	for (int i = 0; i < m_bomb_num; i++) {
		(*m_pBombs)[i]->Render(modelViewMatrixStack, pExplodeProgram, m_pCamera, m_t);
	}

	//============================ TOON SHADER ======================================//
	// Switch to the toon program
	// This is a modified implementation fo the toon shader
	CShaderProgram* pToonProgram = (*m_pShaderPrograms)[2];
	pToonProgram->UseProgram();
	// Set light and materials in sphere programme
	pToonProgram->SetUniform("material1.Ma", glm::vec3(0.f, 1.0f , 1.f));
	pToonProgram->SetUniform("material1.Md", glm::vec3(0.f, 1.0f, 1.f));
	pToonProgram->SetUniform("material1.Ms", glm::vec3(0.f, 1.0f, 1.f));
	pToonProgram->SetUniform("material1.shininess", 50.0f);
	pToonProgram->SetUniform("light1.La", glm::vec3(0.15f, 0.15f, 0.15f));
	pToonProgram->SetUniform("light1.Ld", glm::vec3(1.0f, 1.0f, 1.0f));
	pToonProgram->SetUniform("light1.Ls", glm::vec3(1.0f, 1.0f, 1.0f));
	pToonProgram->SetUniform("light1.position", viewMatrix * lightPosition1);
	pToonProgram->SetUniform("t", m_t);
	pToonProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
	pToonProgram->SetUniform("levels", m_levels);
	pToonProgram->SetUniform("sampler0", 0);

	// Render the Pickup  
	for (int i = 0; i < m_pickup_num; i++) {
		(*m_pPickups)[i]->Render(modelViewMatrixStack, pToonProgram, m_pCamera);
	}

	// Render track outlines (and back to main program)
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTransparency", true);
	pMainProgram->SetUniform("vTransparency", .3f - m_lightswitch * 0.3f);
	for (int i = 0; i < 100; i++)
	{
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(wall_lefttrack[i]);
		modelViewMatrixStack.RotateQuat(wall_lefttrackrot[i]);
		modelViewMatrixStack.Scale(1.f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pTrackWall->Render();
		modelViewMatrixStack.Pop();

		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(wall_righttrack[i]);
		modelViewMatrixStack.RotateQuat(wall_righttrackrot[i]);
		modelViewMatrixStack.Scale(1.f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pTrackWall->Render();
		modelViewMatrixStack.Pop();
	}
	//============================ 2D SHADER ==========================================//
	// Draw the 2D graphics after the 3D graphics
	DisplayHUD(pass);

}


// Update method runs repeatedly with the Render method
void Game::Update() 
{
	m_pAudio->Update();	//update audio
	m_t += (float)(0.01f * m_dt); //update continuously increasing timer value

	if (m_start >= 1) GameStart(); //allows the gameplay state to begin 

}

// Update method for gameplay state after passing intro screen
void Game::GameStart()
{
	//Set Catmull Spline
	glm::vec3 p, up, pNext, pNextNext, upNext, upNextNext, playerUp, playerP;
	m_pCatmullRom->Sample(m_currentDistance, p, up);
	m_pCatmullRom->Sample(m_currentDistance + 25.0f, pNext, upNext);			//calculates T
	m_pCatmullRom->Sample(m_currentDistance + 26.0f, playerP, playerUp);		//calculates player position
	m_pCatmullRom->Sample(m_currentDistance + 50.0f, pNextNext, upNextNext);	//causes delay in camera rotation

	//Set Camera
	glm::vec3 T = glm::normalize(pNext - p);
	glm::vec3 P = p + up * 10.f;
	glm::vec3 viewpt = P + 10.0f * T;

	//Set Player
	glm::vec3 PlayerT = glm::normalize(playerP - pNext);
	glm::vec3 PlayerN = glm::normalize(cross(PlayerT, playerUp));
	if (m_start >= 1) m_pPlayer->Set(pNext, PlayerT, upNext);

	//Set Camera Options
	if (m_freeview == false) CameraControl(P, playerP, viewpt, PlayerN, playerUp, upNextNext);
	else { m_pCamera->Update(m_dt); }
	//Update game variables
	if (m_timerStart < 0) m_currentDistance += (float)(m_cameraSpeed * m_dt);

	//Update pickups
	for (int i = 0; i < m_pickup_num; i++) {
		(*m_pPickups)[i]->Update(m_dt, m_pPlayer->GetPosition(), m_score);
	}
	//Update bombs
	for (int i = 0; i < m_bomb_num; i++) {
		(*m_pBombs)[i]->Update(m_dt, m_pPlayer->GetPosition(), m_health);
	}
	//Update lives
	if (m_health <= 0) {
		m_lives -= 1;
		m_health = 30;
	}
	//Update tree explosion distance
	m_treeDist = glm::distance(m_pPlayer->GetPosition(), glm::vec3(-179.f, 32.f, -7.f));
	if (m_treeDist < 70.f) m_treeExplode += m_dt * 0.001f; else m_treeExplode = 0;

	//Number of Laps
	//m_pCatmullRom->CurrentLap(m_currentDistance); 
}


void Game::CameraControl(glm::vec3& pos, glm::vec3& player, glm::vec3& viewpt, glm::vec3& strafe, glm::vec3& up, glm::vec3& upnext)
{	
	m_pPlayer->Update(m_dt); //player keys control
	switch (m_cameraControl) {
	case 1: { //third person camera
		m_pCamera->Set(pos, viewpt, upnext);
		break; }
	case 2: { //side view camera 
		glm::vec3 camera_side_pos = pos - strafe * 50.f + up * 10.f;
		m_pCamera->Set(camera_side_pos, player, up);
		break; }
	case 3: { //top view camera
		glm::vec3 camera_top_pos = pos + up * 50.f;
		m_pCamera->Set(camera_top_pos, player, up);
		break; }
	default:
		m_pCamera->Set(pos, viewpt, upnext);
		break;
	}
}

void Game::ColourControl(CShaderProgram* fontProgram)
{
	fontProgram->SetUniform("switchColour", m_switchColour);
}

void Game::DisplayHUD(int pass)
{
	//set up matrix stack for 2D graphics
	glutil::MatrixStack screenViewMatrixStack;
	screenViewMatrixStack.SetIdentity();	

	//pass font shader and define uniform values
	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];
	fontProgram->UseProgram();
	glDisable(GL_DEPTH_TEST);
	fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
	fontProgram->SetUniform("vColour", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	fontProgram->SetUniform("t", (float)m_t/3);
	fontProgram->SetUniform("bText", true);


	//render in-game hud
	if (m_start >= 1) { 

		// render countdown before start
		if (m_elapsedTime == 0)  m_timerStart -= 1;
		if (m_timerStart >= 0) m_pFtFont->Render(width/2 - 2, height/2 - 10, 40, "%d", m_timerStart);

		// render player stats
		m_pFtFont->Render(20, height - 20, 20, "Score: %d", m_score);
		m_pFtFont->Render(width * 5 / 6, height - 20, 20, "Health: %d", m_health);

		// render number of lives (in the shape of hearts)
		fontProgram->SetUniform("bText", false);
		fontProgram->SetUniform("bRGB", true);
		for (int i = 0; i < m_lives; i++) {
			screenViewMatrixStack.Push(); {
				screenViewMatrixStack.Translate(width - 20 - 35 * i, height - 55, 0);
				screenViewMatrixStack.RotateRadians(glm::vec3(0, 1, 0), (float)M_PI);
				fontProgram->SetUniform("sampler0", 0);
				fontProgram->SetUniform("matrices.modelViewMatrix", screenViewMatrixStack.Top());
				m_pHeart->Render();
			} screenViewMatrixStack.Pop();
		}
				
	}
	//render intro screen
	else if (m_start < 0)
	{
		//render intro screen background
		glDisable(GL_CULL_FACE);
		screenViewMatrixStack.Push();
		screenViewMatrixStack.Translate(0.f, 0.f, 0.0f);
		screenViewMatrixStack.RotateRadians(glm::vec3(0.0f, 0.0f, 1.0f), (float)M_PI);
		screenViewMatrixStack.Scale(-1.0);
		fontProgram->SetUniform("bText", false);
		fontProgram->SetUniform("matrices.modelViewMatrix", screenViewMatrixStack.Top());
		fontProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(screenViewMatrixStack.Top()));
		m_pIntro->Render();
		screenViewMatrixStack.Pop();
		glEnable(GL_CULL_FACE);

		//turn on rainbow shader for title text
		fontProgram->SetUniform("bText", true);
		m_pFtFont->Render(width * 5 / 8, height / 2 - 30, 32, "Road Runner");
		m_pFtFont->Render(width * 2/3 + 18, height/2 - 70, 14, "SPACE TO PLAY");

	}
	//render instructions scene
	else 
	{
		glDisable(GL_CULL_FACE);
		screenViewMatrixStack.Push();
		screenViewMatrixStack.Translate(0.f, 0.f, 0.0f);
		screenViewMatrixStack.RotateRadians(glm::vec3(0.0f, 0.0f, 1.0f), (float)M_PI);
		screenViewMatrixStack.Scale(-1.0);
		fontProgram->SetUniform("bText", false);
		fontProgram->SetUniform("matrices.modelViewMatrix", screenViewMatrixStack.Top());
		fontProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(screenViewMatrixStack.Top()));
		m_pInstruct->Render();
		screenViewMatrixStack.Pop();
		glEnable(GL_CULL_FACE);

		fontProgram->SetUniform("bText", true);
		m_pFtFont->Render(width * 2 / 3 - 10, height / 2 - 35, 15, "SPACE TO PLAY");
		m_pFtFont->Render(width * 6 / 10 - 6, height / 2 + 40, 15, " WASD move . M minimap .");
		m_pFtFont->Render(width * 6 / 10 - 18, height / 2 + 20, 15, " F1-3 viewpt . 0-5 colours .");
		m_pFtFont->Render(width * 6 / 10 - 10, height / 2, 15, "TAB day/night . F freeview");
		m_pFtFont->Render(width * 6 / 10 - 10, height / 2 - 75 , 15, "lives      points       bombs");
	}

	//render death screen
	if (m_lives <= 0)
	{
		if (m_freeview == true) m_lives = 3;
		glDisable(GL_CULL_FACE);
		screenViewMatrixStack.Push();
		screenViewMatrixStack.Translate(0.f, 0.f, 0.0f);
		screenViewMatrixStack.RotateRadians(glm::vec3(0.0f, 0.0f, 1.0f), (float)M_PI);
		screenViewMatrixStack.Scale(-1.0);
		fontProgram->SetUniform("matrices.modelViewMatrix", screenViewMatrixStack.Top());
		fontProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(screenViewMatrixStack.Top()));
		m_pDeath->Render();
		screenViewMatrixStack.Pop();
		glEnable(GL_CULL_FACE);

		//Displays score
		fontProgram->SetUniform("bText", true);
		m_pFtFont->Render(width/2 - 40.f, height * 1/5, 22, "Score: %d", m_score);
	}

	// Increase the elapsed time and frame counter
	m_elapsedTime += m_dt;
	m_frameCount++;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
	if (m_elapsedTime > 2000 && m_start >= 1)
    {
		m_elapsedTime = 0;
		m_framesPerSecond = m_frameCount;

		// Reset the frames per second
		m_frameCount = 0;
    }

	if (m_framesPerSecond > 0) {
		// Use the font shader program and render the text
		//m_pFtFont->Render(20, height - 40, 20, "FPS: %d", m_framesPerSecond);
	}
	
	if (pass == 1 && m_TVActive == false)
	{
		//Controls type of colour filter
		ColourControl(fontProgram);
		glDisable(GL_CULL_FACE);
		screenViewMatrixStack.Push();
		screenViewMatrixStack.Translate(glm::vec3(0.f, 0.f, 0.0f));
		screenViewMatrixStack.RotateRadians(glm::vec3(0.0f, 0.0f, 1.0f), (float)M_PI);
		screenViewMatrixStack.Scale(-1.0);
		fontProgram->SetUniform("bText", false);
		fontProgram->SetUniform("bRGB", false);
		fontProgram->SetUniform("matrices.modelViewMatrix", screenViewMatrixStack.Top());
		fontProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(screenViewMatrixStack.Top()));
		m_pFBO->BindTexture(0);
		m_pFilter->Render(false);
		screenViewMatrixStack.Pop();
		glEnable(GL_CULL_FACE);
	} 
	
	if (pass == 1 && m_TVActive == true) {
		// Render the plane for the MINIMAP
		glDisable(GL_CULL_FACE);
		screenViewMatrixStack.Push();
		screenViewMatrixStack.Translate(glm::vec3(0, height*2/3, 0.0f));
		screenViewMatrixStack.RotateRadians(glm::vec3(0.0f, 0.0f, 1.0f), (float)M_PI);
		screenViewMatrixStack.Scale(-1.0);
		fontProgram->SetUniform("bText", false);
		fontProgram->SetUniform("bRGB", false);
		fontProgram->SetUniform("matrices.modelViewMatrix", screenViewMatrixStack.Top());
		fontProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(screenViewMatrixStack.Top()));
		m_pFBO->BindTexture(0);
		m_pTV->Render(false);
		screenViewMatrixStack.Pop();
		glEnable(GL_CULL_FACE);
	}	
	fontProgram->SetUniform("bText", false);
	fontProgram->SetUniform("bRGB", false);

}

// The game loop runs repeatedly until game over
void Game::GameLoop()
{
	/*
	// Fixed timer
	dDt = pHighResolutionTimer->Elapsed();
	if (dDt > 1000.0 / (double) Game::FPS) {
		pHighResolutionTimer->Start();
		Update();
		Render();
	}
	*/
	
	
	// Variable timer
	m_pHighResolutionTimer->Start();
	Update();
	Render();
	m_dt = m_pHighResolutionTimer->Elapsed();
	

}


WPARAM Game::Execute() 
{
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hInstance);

	if(!m_gameWindow.Hdc()) {
		return 1;
	}

	Initialise();

	m_pHighResolutionTimer->Start();

	
	MSG msg;

	while(1) {													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		} else if (m_appActive) {
			GameLoop();
		} 
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	m_gameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
	LRESULT result = 0;

	switch (message) {


	case WM_ACTIVATE:
	{
		switch(LOWORD(w_param))
		{
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				m_appActive = true;
				m_pHighResolutionTimer->Start();
				break;
			case WA_INACTIVE:
				m_appActive = false;
				break;
		}
		break;
		}

	case WM_SIZE:
			RECT dimensions;
			GetClientRect(window, &dimensions);
			m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

	case WM_KEYDOWN:
		switch(w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case VK_SPACE: //starts gameplay state
			m_start += 1;
			break;
		case 'P': //plays event sound
			m_pAudio->PlayEventSound();
			break;
		case VK_TAB: //turns ON/OFF 'dark' mode
			if (m_lightswitch == true)
				m_lightswitch = 0;
			else m_lightswitch = true;
			break;
		case 'M': //turns ON/OFF the TV
			if (m_TVActive == true)
				m_TVActive = 0;
			else m_TVActive = true;
			break;
		case 'F': //toggle 'free view'
			if (m_freeview == true)
				m_freeview = 0;
			else m_freeview = true;
			m_pCamera->Reset(glm::vec3(0, 15, 0), glm::vec3(0, 15, -10), glm::vec3(0,1,0));
			break; 
		case VK_F1: //switches to 3rd person camera
			m_cameraControl = 1;
			break;
		case VK_F2: //switches to side view camera
			m_cameraControl = 2;
			break;
		case VK_F3: //switches to top-down view camera
			m_cameraControl = 3;
			break;
		case '0':	//See 'Camera Control' function for more information on keys 0-5
			m_switchColour = 0;
			break;
		case '1':
			m_switchColour = 1;
			break;
		case '2':
			m_switchColour = 2;
			break;
		case '3':
			m_switchColour = 3;
			break;
		case '4':
			m_switchColour = 4;
			break;		
		case '5':
			m_switchColour = 5;
			break;
		case 189: //cases for toon shader
			if (m_levels > 1) m_levels = m_levels - 1;
			break;
		case 187:
			if (m_levels < 256) m_levels = m_levels + 1;
			break;
		}
		break;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance() 
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
	m_hInstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
	Game &game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return game.Execute();
}

// Implementation from Stan Melax's Game Programming Gems 1 article
// Helper function for quaternion rotations along the 3D spline (track)
// Adapted from: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/#how-do-i-create-a-quaternion-in-c-
glm::quat Game::RotationBetweenVectors(glm::vec3 start, glm::vec3 dest) {
	start = normalize(start);
	dest = normalize(dest);

	float cosTheta = dot(start, dest);
	glm::vec3 rotationAxis;

	rotationAxis = cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}

// Helper function for quaternion rotations along the 3D spline (track)
// Ensures that the up direction of the objects points to the desired direction 
// for rotation
glm::quat Game::LookAt(glm::vec3 direction, glm::vec3 desiredUp) {

	if (length2(direction) < 0.0001f)
		return glm::quat();

	glm::vec3 right;
	right = cross(direction, desiredUp);
	desiredUp = cross(right, direction);

	glm::quat rot1 = RotationBetweenVectors(glm::vec3(0.0f, 0.0f, 1.0f), direction);

	glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
	glm::quat rot2 = RotationBetweenVectors(newUp, desiredUp);

	return rot2 * rot1; 
}
