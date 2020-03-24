#pragma once
#include "Common.h"
#include "vertexBufferObject.h"
#include "vertexBufferObjectIndexed.h"
#include "Texture.h"


class CCatmullRom
{
public:
	CCatmullRom();
	~CCatmullRom();

	void CreateCentreline();
	void RenderCentreline();

	void CreateOffsetCurves();
	void RenderOffsetCurves();

	void CreateTrack(string directory, string front);
	void RenderTrack();

	int CurrentLap(float d); // Return the currvent lap (starting from 0) based on distance along the control curve.

	bool Sample(float d, glm::vec3 &p, glm::vec3 &up); // Return a point on the centreline based on a certain distance along the control curve.

	void ComputeTrackPoints();
	vector<glm::vec3> GetTrackPoints() const { return m_trackPoints; };
	vector<glm::vec3> GetOffsetPoints() const { return m_offsetPoints; };
	vector<glm::vec3> GetCentrelinePoints() const { return m_centrelinePoints; };
	vector<glm::vec3> GetUpPoints() const { return m_centrelineUpVectors; };

private:

	void SetControlPoints();
	void ComputeLengthsAlongControlPoints();
	void UniformlySampleControlPoints(int numSamples);
	glm::vec3 Interpolate(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, float t);

	vector<float> m_distances;

	CVertexBufferObject vbo;
	CVertexBufferObjectIndexed vboi; 
	CTexture m_texture;
	string m_directory;
	string m_filename;

	GLuint m_vaoCentreline;
	GLuint m_vaoOffsetCurve;
	GLuint m_vaoTrack;
	const GLuint SAMPLING = 400;
	const float PATHWIDTH = 20.0f; 

	vector<glm::vec3> m_controlPoints;		// Control points, which are interpolated to produce the centreline points
	vector<glm::vec3> m_controlUpVectors;	// Control upvectors, which are interpolated to produce the centreline upvectors
	vector<glm::vec3> m_centrelinePoints;	// Centreline points
	vector<glm::vec3> m_centrelineUpVectors;// Centreline upvectors
	vector<glm::vec3> m_leftOffsetPoints;	// Left offset curve points
	vector<glm::vec3> m_rightOffsetPoints;	// Right offset curve points
	vector<glm::vec3> m_trackPoints;		// Compute available positions on track
	vector<glm::vec3> m_offsetPoints;		// Computer strafe points for positions on track

	unsigned int m_vertexCount;				// Number of vertices in the track VBO
	unsigned int m_numTriangles;
};
