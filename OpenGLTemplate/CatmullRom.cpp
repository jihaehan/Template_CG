#include "CatmullRom.h"
#define _USE_MATH_DEFINES
#include <math.h>



CCatmullRom::CCatmullRom()
{
	m_vertexCount = 0;
}

CCatmullRom::~CCatmullRom()
{}

// Perform Catmull Rom spline interpolation between four points, interpolating the space between p1 and p2
glm::vec3 CCatmullRom::Interpolate(glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, float t)
{
	float t2 = t * t;
	float t3 = t2 * t;

	glm::vec3 a = p1;
	glm::vec3 b = 0.5f * (-p0 + p2);
	glm::vec3 c = 0.5f * (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3);
	glm::vec3 d = 0.5f * (-p0 + 3.0f * p1 - 3.0f * p2 + p3);

	return a + b * t + c * t2 + d * t3;

}


void CCatmullRom::SetControlPoints()
{
	// Set control points (m_controlPoints) here, or load from disk
	m_controlPoints.push_back(glm::vec3(0, 5, 0));				//1
	m_controlPoints.push_back(glm::vec3(0, 5, -100));			//2
	m_controlPoints.push_back(glm::vec3(-17, 67, -170));			//3
	m_controlPoints.push_back(glm::vec3(-84, 117, -100));		//4
	m_controlPoints.push_back(glm::vec3(-150, 140, 57));			//5
	m_controlPoints.push_back(glm::vec3(-266, 117, 100));		//6
	m_controlPoints.push_back(glm::vec3(-400, 100, 00));			//6
	m_controlPoints.push_back(glm::vec3(-300, 100, -140));		//8
	m_controlPoints.push_back(glm::vec3(-217, 67, -42));			//9
	m_controlPoints.push_back(glm::vec3(-100, 5, 20));			//10

	// Optionally, set upvectors (m_controlUpVectors, one for each control point as well)
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));	//1
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));	//2
	m_controlUpVectors.push_back(glm::vec3(0, 0, 1));	//3
	m_controlUpVectors.push_back(glm::vec3(0, -1, 0));	//4
	m_controlUpVectors.push_back(glm::vec3(0, -1, 0));	//5
	m_controlUpVectors.push_back(glm::vec3(0, 0, -1));	//6
	m_controlUpVectors.push_back(glm::vec3(1, 0, 0));	//7
	m_controlUpVectors.push_back(glm::vec3(0, 0, 1));	//8
	m_controlUpVectors.push_back(glm::vec3(-1, 0, 0));	//9
	m_controlUpVectors.push_back(glm::vec3(0, 0, -1));	//10
}


// Determine lengths along the control points, which is the set of control points forming the closed curve
void CCatmullRom::ComputeLengthsAlongControlPoints()
{
	int M = (int)m_controlPoints.size();

	float fAccumulatedLength = 0.0f;
	m_distances.push_back(fAccumulatedLength);
	for (int i = 1; i < M; i++) {
		fAccumulatedLength += glm::distance(m_controlPoints[i - 1], m_controlPoints[i]);
		m_distances.push_back(fAccumulatedLength);
	}

	// Get the distance from the last point to the first
	fAccumulatedLength += glm::distance(m_controlPoints[M - 1], m_controlPoints[0]);
	m_distances.push_back(fAccumulatedLength);
}


// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
bool CCatmullRom::Sample(float d, glm::vec3& p, glm::vec3& up)
{
	if (d < 0)
		return false;

	int M = (int)m_controlPoints.size();
	if (M == 0)
		return false;


	float fTotalLength = m_distances[m_distances.size() - 1];

	// The the current length along the control polygon; handle the case where we've looped around the track
	float fLength = d - (int)(d / fTotalLength) * fTotalLength;

	// Find the current segment
	int j = -1;
	for (int i = 0; i < (int)m_distances.size(); i++) {
		if (fLength >= m_distances[i] && fLength < m_distances[i + 1]) {
			j = i; // found it!
			break;
		}
	}

	if (j == -1)
		return false;

	// Interpolate on current segment -- get t
	float fSegmentLength = m_distances[j + 1] - m_distances[j];
	float t = (fLength - m_distances[j]) / fSegmentLength;

	// Get the indices of the four points along the control polygon for the current segment
	int iPrev = ((j - 1) + M) % M;
	int iCur = j;
	int iNext = (j + 1) % M;
	int iNextNext = (j + 2) % M;

	// Interpolate to get the point (and upvector)
	p = Interpolate(m_controlPoints[iPrev], m_controlPoints[iCur], m_controlPoints[iNext], m_controlPoints[iNextNext], t);
	if (m_controlUpVectors.size() == m_controlPoints.size())
		up = glm::normalize(Interpolate(m_controlUpVectors[iPrev], m_controlUpVectors[iCur], m_controlUpVectors[iNext], m_controlUpVectors[iNextNext], t));


	return true;
}



// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CCatmullRom::UniformlySampleControlPoints(int numSamples)
{
	glm::vec3 p, up;

	// Compute the lengths of each segment along the control polygon, and the total length
	ComputeLengthsAlongControlPoints();
	float fTotalLength = m_distances[m_distances.size() - 1];

	// The spacing will be based on the control polygon
	float fSpacing = fTotalLength / numSamples;

	// Call PointAt to sample the spline, to generate the points
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);
	}

	// Repeat once more for truly equidistant points
	m_controlPoints = m_centrelinePoints;
	m_controlUpVectors = m_centrelineUpVectors;
	m_centrelinePoints.clear();
	m_centrelineUpVectors.clear();
	m_distances.clear();
	ComputeLengthsAlongControlPoints();
	fTotalLength = m_distances[m_distances.size() - 1];
	fSpacing = fTotalLength / numSamples;
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);
	}
}



void CCatmullRom::CreateCentreline()
{
	// Call Set Control Points
	SetControlPoints(); 

	// Call UniformlySampleControlPoints with the number of samples required
	UniformlySampleControlPoints(SAMPLING);

	// Create a VAO called m_vaoCentreline and a VBO to get the points onto the graphics card
	glGenVertexArrays(1, &m_vaoCentreline);
	glBindVertexArray(m_vaoCentreline);

	vbo.Create();
	vbo.Bind();
	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < m_centrelinePoints.size(); ++i) {
		vbo.AddData(&m_centrelinePoints[i], sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
	}
	vbo.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,	(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}


void CCatmullRom::CreateOffsetCurves()
{ 
	// Compute the offset curves, one left, and one right.  Store the points in m_leftOffsetPoints and m_rightOffsetPoints respectively
	unsigned int lastpt = m_centrelinePoints.size() - 1;
	for (int i = 0; i < lastpt; ++i) {
		int j = i + 1; 

		glm::vec3 T = glm::normalize(m_centrelinePoints[j] - m_centrelinePoints[i]);
		glm::vec3 N = glm::normalize(glm::cross(T, m_controlUpVectors[i]));
		glm::vec3 B = glm::normalize(glm::cross(N, T));

		glm::vec3 l = m_centrelinePoints[i] - (PATHWIDTH / 2) * N;
		glm::vec3 r = m_centrelinePoints[i] + (PATHWIDTH / 2) * N; 
		m_leftOffsetPoints.push_back(l);
		m_rightOffsetPoints.push_back(r);
	} 
	glm::vec3 T = glm::normalize(m_centrelinePoints[0] - m_centrelinePoints[lastpt]);
	glm::vec3 N = glm::normalize(glm::cross(T, m_controlUpVectors[lastpt]));
	glm::vec3 B = glm::normalize(glm::cross(N, T));

	glm::vec3 l = m_centrelinePoints[lastpt] - (PATHWIDTH / 2) * N;
	glm::vec3 r = m_centrelinePoints[lastpt] + (PATHWIDTH / 2) * N;
	m_leftOffsetPoints.push_back(l);
	m_rightOffsetPoints.push_back(r);

	// Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
	// Note it is possible to only use one VAO / VBO with all the points instead.
	glGenVertexArrays(1, &m_vaoOffsetCurve);
	glBindVertexArray(m_vaoOffsetCurve);

	vbo.Create();
	vbo.Bind();
	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < m_leftOffsetPoints.size(); ++i) {
		vbo.AddData(&m_leftOffsetPoints[i], sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
	}
	vbo.AddData(&m_leftOffsetPoints[0], sizeof(glm::vec3));
	vbo.AddData(&texCoord, sizeof(glm::vec2));
	vbo.AddData(&normal, sizeof(glm::vec3));
	for (int i = 0; i < m_rightOffsetPoints.size(); ++i) {
		vbo.AddData(&m_rightOffsetPoints[i], sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
	}
	vbo.AddData(&m_rightOffsetPoints[0], sizeof(glm::vec3));
	vbo.AddData(&texCoord, sizeof(glm::vec2));
	vbo.AddData(&normal, sizeof(glm::vec3));

	vbo.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

}


void CCatmullRom::CreateTrack(string directory, string front)
{
	
	// Load texture
	m_texture.Load(directory + front);

	m_directory = directory;
	m_filename = front;

	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card
	glGenVertexArrays(1, &m_vaoTrack);
	glBindVertexArray(m_vaoTrack);

	vboi.Create();
	vboi.Bind();
	
	//Compute vertex data to store in vbo
	for (int count = 0; count < m_leftOffsetPoints.size(); ++count) {
		
		glm::vec2 tl = glm::vec2(0, count % 2);
		glm::vec2 tr = glm::vec2(1, count % 2);

		vboi.AddVertexData(&m_leftOffsetPoints[count], sizeof(glm::vec3));
		vboi.AddVertexData(&tl, sizeof(glm::vec2));
		vboi.AddVertexData(&m_centrelineUpVectors[count], sizeof(glm::vec3));
		m_vertexCount++;

		vboi.AddVertexData(&m_rightOffsetPoints[count], sizeof(glm::vec3));
		vboi.AddVertexData(&tr, sizeof(glm::vec2));
		vboi.AddVertexData(&m_centrelineUpVectors[count], sizeof(glm::vec3));
		m_vertexCount++;
	}
	vboi.AddVertexData(&m_leftOffsetPoints[0], sizeof(glm::vec3));
	vboi.AddVertexData(&glm::vec2(0, m_leftOffsetPoints.size()%2), sizeof(glm::vec2));
	vboi.AddVertexData(&m_centrelineUpVectors[0], sizeof(glm::vec3));
	m_vertexCount++;
	vboi.AddVertexData(&m_rightOffsetPoints[0], sizeof(glm::vec3));
	vboi.AddVertexData(&glm::vec2(1, m_leftOffsetPoints.size() % 2), sizeof(glm::vec2));
	vboi.AddVertexData(&m_centrelineUpVectors[0], sizeof(glm::vec3));
	m_vertexCount++;

	//Compute index data to store in vbo
	for (int vertex = 0; vertex < (m_vertexCount -3); ++vertex) {
		unsigned int index0 = vertex;
		unsigned int index1 = vertex + 1;
		unsigned int index2 = vertex + 2;
		unsigned int index3 = vertex + 3;

		vboi.AddIndexData(&index0, sizeof(unsigned int));
		vboi.AddIndexData(&index1, sizeof(unsigned int));
		vboi.AddIndexData(&index2, sizeof(unsigned int));
		m_numTriangles++;

		vboi.AddIndexData(&index2, sizeof(unsigned int));
		vboi.AddIndexData(&index1, sizeof(unsigned int));
		vboi.AddIndexData(&index3, sizeof(unsigned int));
		m_numTriangles++;
	}
	vboi.UploadDataToGPU(GL_STATIC_DRAW);

	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture positions
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}


void CCatmullRom::RenderCentreline()
{
	// Bind the VAO m_vaoCentreline and render it
	glLineWidth(5.0f); 	
	glPointSize(10.f);
	glBindVertexArray(m_vaoCentreline);
	glDrawArrays(GL_LINE_STRIP, 0, SAMPLING);
	glDrawArrays(GL_POINTS, 0, SAMPLING);
}

void CCatmullRom::RenderOffsetCurves()
{ 
	// Bind the VAO m_vaoLeftOffsetCurve and render it && Bind the VAO m_vaoRightOffsetCurve and render it
	glLineWidth(5.0f);	glPointSize(10.f);
	glBindVertexArray(m_vaoOffsetCurve);
	glDrawArrays(GL_LINE_STRIP, 0, m_leftOffsetPoints.size() + 1);
	glDrawArrays(GL_LINE_STRIP, m_leftOffsetPoints.size() + 1, m_rightOffsetPoints.size()+1);
	glDrawArrays(GL_POINTS, 0, m_leftOffsetPoints.size()*2);
}


void CCatmullRom::RenderTrack()
{
	// temp visuals
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Bind the VAO m_vaoTrack and render it
	glBindVertexArray(m_vaoTrack);
	m_texture.Bind();
	glDrawElements(GL_TRIANGLES, m_numTriangles * 3, GL_UNSIGNED_INT, 0);

	// restores other visuals
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

int CCatmullRom::CurrentLap(float d)
{

	return (int)(d / m_distances.back());

}