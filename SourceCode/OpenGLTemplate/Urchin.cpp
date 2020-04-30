#include "Common.h"

#define _USE_MATH_DEFINES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#include "Urchin.h"
#include <math.h>

CUrchin::CUrchin()
{}

CUrchin::~CUrchin()
{}

void CUrchin::Create(string a_sDirectory, string a_sFilename, float spikes)
{
	// check if filename passed in -- if so, load texture

	m_texture.Load(a_sDirectory + a_sFilename);

	m_directory = a_sDirectory;
	m_filename = a_sFilename;

	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	m_vbo.Create();
	m_vbo.Bind();

	// Compute positions
	std::vector<glm::vec3> positions;
	positions.push_back(glm::vec3(0.f, 1.f, 0.f));
	for (int i = 0; i < 5; ++i)
	{
		float theta = (i / 5.0f) * 2 * (float)M_PI;
		glm::vec3 ring_one(cos(theta), .5f, sin(theta));
		glm::vec3 ring_two(cos(theta + (float)M_PI / 5) * 1.5f * spikes, 1.5f * spikes, sin(theta + (float)M_PI / 5) * 1.5f * spikes);
		positions.push_back(ring_one);
		positions.push_back(ring_two);
	}

	for (int j = 0; j < 5; ++j)
	{
		float theta = (j / 5.0f) * 2 * (float)M_PI;
		glm::vec3 ring_three(cos(theta) * 2.f * spikes, -0.5f * spikes, sin(theta) * 2.f * spikes);
		glm::vec3 ring_four(cos(theta + (float)M_PI / 5) * 2.f * spikes, 0.4f * spikes, sin(theta + (float)M_PI / 5) * 2.f * spikes);
		positions.push_back(ring_three);
		positions.push_back(ring_four);
	}
	for (int k = 0; k < 5; ++k)
	{
		float theta = (k / 5.0f) * 2 * (float)M_PI;
		glm::vec3 ring_five(cos(theta) * 1.5f * spikes, -1.5f * spikes, sin(theta) * 1.5f * spikes);
		glm::vec3 ring_six(cos(theta + (float)M_PI / 5), -0.4f, sin(theta + (float)M_PI / 5));
		positions.push_back(ring_five);
		positions.push_back(ring_six);
	}
	positions.push_back(glm::vec3(0.f, -1.f, 0.f));

	// Compute texture coordinates 
	std::vector<glm::vec2> texcoord;
	texcoord.push_back(glm::vec2(0.f, 0.f));
	texcoord.push_back(glm::vec2(1.0f, 0.f));
	texcoord.push_back(glm::vec2(0.5, 1.f));

	// Compute normals
	std::vector<glm::vec3> normals;
	normals.push_back(glm::cross(positions.at(2) - positions.at(1), positions.at(2) - positions.at(0)));
	//face 2
	normals.push_back(glm::cross(positions.at(2) - positions.at(3), positions.at(2) - positions.at(1)));
	//face 3
	normals.push_back(glm::cross(positions.at(2) - positions.at(0), positions.at(2) - positions.at(3)));
	//face 4
	normals.push_back(glm::cross(positions.at(4) - positions.at(3), positions.at(4) - positions.at(0)));
	//face 5
	normals.push_back(glm::cross(positions.at(4) - positions.at(5), positions.at(4) - positions.at(3)));
	//face 6
	normals.push_back(glm::cross(positions.at(4) - positions.at(0), positions.at(4) - positions.at(5)));
	//face 7
	normals.push_back(glm::cross(positions.at(6) - positions.at(5), positions.at(6) - positions.at(0)));
	//face 8
	normals.push_back(glm::cross(positions.at(6) - positions.at(7), positions.at(6) - positions.at(5)));
	//face 9
	normals.push_back(glm::cross(positions.at(6) - positions.at(0), positions.at(6) - positions.at(7)));
	//face 10
	normals.push_back(glm::cross(positions.at(8) - positions.at(7), positions.at(8) - positions.at(0)));
	//face 11
	normals.push_back(glm::cross(positions.at(8) - positions.at(9), positions.at(8) - positions.at(7)));
	//face 12
	normals.push_back(glm::cross(positions.at(8) - positions.at(0), positions.at(8) - positions.at(9)));
	//face 13
	normals.push_back(glm::cross(positions.at(10) - positions.at(9), positions.at(10) - positions.at(0)));
	//face 14
	normals.push_back(glm::cross(positions.at(10) - positions.at(1), positions.at(10) - positions.at(9)));
	//face 15
	normals.push_back(glm::cross(positions.at(10) - positions.at(0), positions.at(10) - positions.at(1)));
	//face 16
	normals.push_back(glm::cross(positions.at(12) - positions.at(22), positions.at(12) - positions.at(1)));
	//face 17
	normals.push_back(glm::cross(positions.at(3) - positions.at(12), positions.at(3) - positions.at(1)));
	//face 18
	normals.push_back(glm::cross(positions.at(22) - positions.at(12), positions.at(22) - positions.at(3)));
	//face 19
	normals.push_back(glm::cross(positions.at(13) - positions.at(22), positions.at(13) - positions.at(3)));
	//face 20
	normals.push_back(glm::cross(positions.at(24) - positions.at(22), positions.at(24) - positions.at(13)));
	//face 21
	normals.push_back(glm::cross(positions.at(3) - positions.at(24), positions.at(3) - positions.at(13)));
	//face 22
	normals.push_back(glm::cross(positions.at(14) - positions.at(24), positions.at(14) - positions.at(3)));
	//face 23
	normals.push_back(glm::cross(positions.at(5) - positions.at(14), positions.at(5) - positions.at(3)));
	//face 24
	normals.push_back(glm::cross(positions.at(24) - positions.at(14), positions.at(24) - positions.at(5)));
	//face 25
	normals.push_back(glm::cross(positions.at(15) - positions.at(24), positions.at(15) - positions.at(5)));
	//face 26
	normals.push_back(glm::cross(positions.at(26) - positions.at(24), positions.at(26) - positions.at(15)));
	//face 27
	normals.push_back(glm::cross(positions.at(5) - positions.at(26), positions.at(5) - positions.at(15)));
	//face 28
	normals.push_back(glm::cross(positions.at(16) - positions.at(26), positions.at(16) - positions.at(5)));
	//face 29
	normals.push_back(glm::cross(positions.at(7) - positions.at(16), positions.at(7) - positions.at(5)));
	//face 30
	normals.push_back(glm::cross(positions.at(26) - positions.at(16), positions.at(26) - positions.at(7)));
	//face 31
	normals.push_back(glm::cross(positions.at(17) - positions.at(26), positions.at(17) - positions.at(7)));
	//face 32
	normals.push_back(glm::cross(positions.at(28) - positions.at(26), positions.at(28) - positions.at(17)));
	//face 33
	normals.push_back(glm::cross(positions.at(7) - positions.at(28), positions.at(7) - positions.at(17)));
	//face 34
	normals.push_back(glm::cross(positions.at(18) - positions.at(28), positions.at(18) - positions.at(7)));
	//face 35
	normals.push_back(glm::cross(positions.at(9) - positions.at(18), positions.at(9) - positions.at(7)));
	//face 36
	normals.push_back(glm::cross(positions.at(28) - positions.at(18), positions.at(28) - positions.at(9)));
	//face 37
	normals.push_back(glm::cross(positions.at(19) - positions.at(28), positions.at(19) - positions.at(9)));
	//face 38
	normals.push_back(glm::cross(positions.at(30) - positions.at(28), positions.at(30) - positions.at(19)));
	//face 39
	normals.push_back(glm::cross(positions.at(9) - positions.at(30), positions.at(9) - positions.at(19)));
	//face 40
	normals.push_back(glm::cross(positions.at(20) - positions.at(30), positions.at(20) - positions.at(9)));
	//face 41
	normals.push_back(glm::cross(positions.at(1) - positions.at(20), positions.at(1) - positions.at(9)));
	//face 42
	normals.push_back(glm::cross(positions.at(30) - positions.at(20), positions.at(30) - positions.at(1)));
	//face 43
	normals.push_back(glm::cross(positions.at(11) - positions.at(30), positions.at(11) - positions.at(1)));
	//face 44
	normals.push_back(glm::cross(positions.at(22) - positions.at(30), positions.at(22) - positions.at(11)));
	//face 45
	normals.push_back(glm::cross(positions.at(1) - positions.at(22), positions.at(1) - positions.at(11)));
	//face 46
	normals.push_back(glm::cross(positions.at(21) - positions.at(31), positions.at(21) - positions.at(30)));
	//face 47
	normals.push_back(glm::cross(positions.at(21) - positions.at(30), positions.at(21) - positions.at(22)));
	//face 48
	normals.push_back(glm::cross(positions.at(21) - positions.at(22), positions.at(21) - positions.at(31)));
	//face 49
	normals.push_back(glm::cross(positions.at(23) - positions.at(31), positions.at(23) - positions.at(22)));
	//face 50
	normals.push_back(glm::cross(positions.at(23) - positions.at(22), positions.at(23) - positions.at(24)));
	//face 51
	normals.push_back(glm::cross(positions.at(23) - positions.at(24), positions.at(23) - positions.at(31)));
	//face 52
	normals.push_back(glm::cross(positions.at(25) - positions.at(31), positions.at(25) - positions.at(24)));
	//face 53
	normals.push_back(glm::cross(positions.at(25) - positions.at(24), positions.at(25) - positions.at(26)));
	//face 54
	normals.push_back(glm::cross(positions.at(25) - positions.at(26), positions.at(25) - positions.at(31)));
	//face 55
	normals.push_back(glm::cross(positions.at(27) - positions.at(31), positions.at(27) - positions.at(26)));
	//face 56
	normals.push_back(glm::cross(positions.at(27) - positions.at(26), positions.at(27) - positions.at(28)));
	//face 57
	normals.push_back(glm::cross(positions.at(27) - positions.at(28), positions.at(27) - positions.at(31)));
	//face 58
	normals.push_back(glm::cross(positions.at(29) - positions.at(31), positions.at(29) - positions.at(28)));
	//face 59
	normals.push_back(glm::cross(positions.at(29) - positions.at(28), positions.at(29) - positions.at(30)));
	//face 60
	normals.push_back(glm::cross(positions.at(29) - positions.at(30), positions.at(29) - positions.at(31)));

	// Compute vertices for VBO
	CVertex
		v0, v1, v2, v3, v4, v5, v6, v7, v8, v9,
		v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,
		v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,
		v30, v31, v32, v33, v34, v35, v36, v37, v38, v39,
		v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,
		v50, v51, v52, v53, v54, v55, v56, v57, v58, v59,
		v60, v61, v62, v63, v64, v65, v66, v67, v68, v69,
		v70, v71, v72, v73, v74, v75, v76, v77, v78, v79,
		v80, v81, v82, v83, v84, v85, v86, v87, v88, v89,
		v90, v91, v92, v93, v94, v95, v96, v97, v98, v99,
		v100, v101, v102, v103, v104, v105, v106, v107, v108, v109,
		v110, v111, v112, v113, v114, v115, v116, v117, v118, v119,
		v120, v121, v122, v123, v124, v125, v126, v127, v128, v129,
		v130, v131, v132, v133, v134, v135, v136, v137, v138, v139,
		v140, v141, v142, v143, v144, v145, v146, v147, v148, v149,
		v150, v151, v152, v153, v154, v155, v156, v157, v158, v159,
		v160, v161, v162, v163, v164, v165, v166, v167, v168, v169,
		v170, v171, v172, v173, v174, v175, v176, v177, v178, v179;

	v0.pos = positions.at(2);		v0.tex = texcoord.at(0);		v0.nor = normals.at(0);	//face 1
	v1.pos = positions.at(1);		v1.tex = texcoord.at(1);		v1.nor = normals.at(0);
	v2.pos = positions.at(0);		v2.tex = texcoord.at(2);		v2.nor = normals.at(0);
	v3.pos = positions.at(2);		v3.tex = texcoord.at(0);		v3.nor = normals.at(1); //face 2
	v4.pos = positions.at(3);		v4.tex = texcoord.at(1);		v4.nor = normals.at(1);
	v5.pos = positions.at(1);		v5.tex = texcoord.at(2);		v5.nor = normals.at(1);
	v6.pos = positions.at(2);		v6.tex = texcoord.at(0);		v6.nor = normals.at(2); //face 3
	v7.pos = positions.at(0);		v7.tex = texcoord.at(1);		v7.nor = normals.at(2);
	v8.pos = positions.at(3);		v8.tex = texcoord.at(2);		v8.nor = normals.at(2);
	v9.pos = positions.at(4);		v9.tex = texcoord.at(0);		v9.nor = normals.at(3); //face 4
	v10.pos = positions.at(3);		v10.tex = texcoord.at(1);		v10.nor = normals.at(3);
	v11.pos = positions.at(0);		v11.tex = texcoord.at(2);		v11.nor = normals.at(3);
	v12.pos = positions.at(4);		v12.tex = texcoord.at(0);		v12.nor = normals.at(4); //face 5
	v13.pos = positions.at(5);		v13.tex = texcoord.at(1);		v13.nor = normals.at(4);
	v14.pos = positions.at(3);		v14.tex = texcoord.at(2);		v14.nor = normals.at(4);
	v15.pos = positions.at(4);		v15.tex = texcoord.at(0);		v15.nor = normals.at(5); //face 6
	v16.pos = positions.at(0);		v16.tex = texcoord.at(1);		v16.nor = normals.at(5);
	v17.pos = positions.at(5);		v17.tex = texcoord.at(2);		v17.nor = normals.at(5);
	v18.pos = positions.at(6);		v18.tex = texcoord.at(0);		v18.nor = normals.at(6); //face 7
	v19.pos = positions.at(5);		v19.tex = texcoord.at(1);		v19.nor = normals.at(6);
	v20.pos = positions.at(0);		v20.tex = texcoord.at(2);		v20.nor = normals.at(6);
	v21.pos = positions.at(6);		v21.tex = texcoord.at(0);		v21.nor = normals.at(7); //face 8
	v22.pos = positions.at(7);		v22.tex = texcoord.at(1);		v22.nor = normals.at(7);
	v23.pos = positions.at(5);		v23.tex = texcoord.at(2);		v23.nor = normals.at(7);
	v24.pos = positions.at(6);		v24.tex = texcoord.at(0);		v24.nor = normals.at(8); //face 9
	v25.pos = positions.at(0);		v25.tex = texcoord.at(1);		v25.nor = normals.at(8);
	v26.pos = positions.at(7);		v26.tex = texcoord.at(2);		v26.nor = normals.at(8);
	v27.pos = positions.at(8);		v27.tex = texcoord.at(0);		v27.nor = normals.at(9); //face 10
	v28.pos = positions.at(7);		v28.tex = texcoord.at(1);		v28.nor = normals.at(9);
	v29.pos = positions.at(0);		v29.tex = texcoord.at(2);		v29.nor = normals.at(9);
	v30.pos = positions.at(8);		v30.tex = texcoord.at(0);		v30.nor = normals.at(10); //face 11
	v31.pos = positions.at(9);		v31.tex = texcoord.at(1);		v31.nor = normals.at(10);
	v32.pos = positions.at(7);		v32.tex = texcoord.at(2);		v32.nor = normals.at(10);
	v33.pos = positions.at(8);		v33.tex = texcoord.at(0);		v33.nor = normals.at(11); //face 12
	v34.pos = positions.at(0);		v34.tex = texcoord.at(1);		v34.nor = normals.at(11);
	v35.pos = positions.at(9);		v35.tex = texcoord.at(2);		v35.nor = normals.at(11);
	v36.pos = positions.at(10);		v36.tex = texcoord.at(0);		v36.nor = normals.at(12); //face 13
	v37.pos = positions.at(9);		v37.tex = texcoord.at(1);		v37.nor = normals.at(12);
	v38.pos = positions.at(0);		v38.tex = texcoord.at(2);		v38.nor = normals.at(12);
	v39.pos = positions.at(10);		v39.tex = texcoord.at(0);		v39.nor = normals.at(13); //face 14
	v40.pos = positions.at(1);		v40.tex = texcoord.at(1);		v40.nor = normals.at(13);
	v41.pos = positions.at(9);		v41.tex = texcoord.at(2);		v41.nor = normals.at(13);
	v42.pos = positions.at(10);		v42.tex = texcoord.at(0);		v42.nor = normals.at(14); //face 15
	v43.pos = positions.at(0);		v43.tex = texcoord.at(1);		v43.nor = normals.at(14);
	v44.pos = positions.at(1);		v44.tex = texcoord.at(2);		v44.nor = normals.at(14);
	v45.pos = positions.at(12);		v45.tex = texcoord.at(0);		v45.nor = normals.at(15); //face 16
	v46.pos = positions.at(22);		v46.tex = texcoord.at(1);		v46.nor = normals.at(15);
	v47.pos = positions.at(1);		v47.tex = texcoord.at(2);		v47.nor = normals.at(15);
	v48.pos = positions.at(3);		v48.tex = texcoord.at(0);		v48.nor = normals.at(16); //face 17
	v49.pos = positions.at(12);		v49.tex = texcoord.at(1);		v49.nor = normals.at(16);
	v50.pos = positions.at(1);		v50.tex = texcoord.at(2);		v50.nor = normals.at(16);
	v51.pos = positions.at(22);		v51.tex = texcoord.at(0);		v51.nor = normals.at(17); //face 18
	v52.pos = positions.at(12);		v52.tex = texcoord.at(1);		v52.nor = normals.at(17);
	v53.pos = positions.at(3);		v53.tex = texcoord.at(2);		v53.nor = normals.at(17);
	v54.pos = positions.at(13);		v54.tex = texcoord.at(0);		v54.nor = normals.at(18); //face 19
	v55.pos = positions.at(22);		v55.tex = texcoord.at(1);		v55.nor = normals.at(18);
	v56.pos = positions.at(3);		v56.tex = texcoord.at(2);		v56.nor = normals.at(18);
	v57.pos = positions.at(24);		v57.tex = texcoord.at(0);		v57.nor = normals.at(19); //face 20
	v58.pos = positions.at(22);		v58.tex = texcoord.at(1);		v58.nor = normals.at(19);
	v59.pos = positions.at(13);		v59.tex = texcoord.at(2);		v59.nor = normals.at(19);
	v60.pos = positions.at(3);		v60.tex = texcoord.at(0);		v60.nor = normals.at(20); //face 21
	v61.pos = positions.at(24);		v61.tex = texcoord.at(1);		v61.nor = normals.at(20);
	v62.pos = positions.at(13);		v62.tex = texcoord.at(2);		v62.nor = normals.at(20);
	v63.pos = positions.at(14);		v63.tex = texcoord.at(0);		v63.nor = normals.at(21); //face 22
	v64.pos = positions.at(24);		v64.tex = texcoord.at(1);		v64.nor = normals.at(21);
	v65.pos = positions.at(3);		v65.tex = texcoord.at(2);		v65.nor = normals.at(21);
	v66.pos = positions.at(5);		v66.tex = texcoord.at(0);		v66.nor = normals.at(22); //face 23
	v67.pos = positions.at(14);		v67.tex = texcoord.at(1);		v67.nor = normals.at(22);
	v68.pos = positions.at(3);		v68.tex = texcoord.at(2);		v68.nor = normals.at(22);
	v69.pos = positions.at(24);		v69.tex = texcoord.at(0);		v69.nor = normals.at(23); //face 24
	v70.pos = positions.at(14);		v70.tex = texcoord.at(1);		v70.nor = normals.at(23);
	v71.pos = positions.at(5);		v71.tex = texcoord.at(2);		v71.nor = normals.at(23);
	v72.pos = positions.at(15);		v72.tex = texcoord.at(0);		v72.nor = normals.at(24); //face 25
	v73.pos = positions.at(24);		v73.tex = texcoord.at(1);		v73.nor = normals.at(24);
	v74.pos = positions.at(5);		v74.tex = texcoord.at(2);		v74.nor = normals.at(24);
	v75.pos = positions.at(26);		v75.tex = texcoord.at(0);		v75.nor = normals.at(25); //face 26
	v76.pos = positions.at(24);		v76.tex = texcoord.at(1);		v76.nor = normals.at(25);
	v77.pos = positions.at(15);		v77.tex = texcoord.at(2);		v77.nor = normals.at(25);
	v78.pos = positions.at(5);		v78.tex = texcoord.at(0);		v78.nor = normals.at(26); //face 27
	v79.pos = positions.at(26);		v79.tex = texcoord.at(1);		v79.nor = normals.at(26);
	v80.pos = positions.at(15);		v80.tex = texcoord.at(2);		v80.nor = normals.at(26);
	v81.pos = positions.at(16);		v81.tex = texcoord.at(0);		v81.nor = normals.at(27); //face 28
	v82.pos = positions.at(26);		v82.tex = texcoord.at(1);		v82.nor = normals.at(27);
	v83.pos = positions.at(5);		v83.tex = texcoord.at(2);		v83.nor = normals.at(27);
	v84.pos = positions.at(7);		v84.tex = texcoord.at(0);		v84.nor = normals.at(28); //face 29
	v85.pos = positions.at(16);		v85.tex = texcoord.at(1);		v85.nor = normals.at(28);
	v86.pos = positions.at(5);		v86.tex = texcoord.at(2);		v86.nor = normals.at(28);
	v87.pos = positions.at(26);		v87.tex = texcoord.at(0);		v87.nor = normals.at(29); //face 30
	v88.pos = positions.at(16);		v88.tex = texcoord.at(1);		v88.nor = normals.at(29);
	v89.pos = positions.at(7);		v89.tex = texcoord.at(2);		v89.nor = normals.at(29);
	v90.pos = positions.at(17);		v90.tex = texcoord.at(0);		v90.nor = normals.at(30); //face 31
	v91.pos = positions.at(26);		v91.tex = texcoord.at(1);		v91.nor = normals.at(30);
	v92.pos = positions.at(7);		v92.tex = texcoord.at(2);		v92.nor = normals.at(30);
	v93.pos = positions.at(28);		v93.tex = texcoord.at(0);		v93.nor = normals.at(31); //face 32
	v94.pos = positions.at(26);		v94.tex = texcoord.at(1);		v94.nor = normals.at(31);
	v95.pos = positions.at(17);		v95.tex = texcoord.at(2);		v95.nor = normals.at(31);
	v96.pos = positions.at(7);		v96.tex = texcoord.at(0);		v96.nor = normals.at(32); //face 33
	v97.pos = positions.at(28);		v97.tex = texcoord.at(1);		v97.nor = normals.at(32);
	v98.pos = positions.at(17);		v98.tex = texcoord.at(2);		v98.nor = normals.at(32);
	v99.pos = positions.at(18);		v99.tex = texcoord.at(0);		v99.nor = normals.at(33); //face 34
	v100.pos = positions.at(28);	v100.tex = texcoord.at(1);		v100.nor = normals.at(33);
	v101.pos = positions.at(7);		v101.tex = texcoord.at(2);		v101.nor = normals.at(33);
	v102.pos = positions.at(9);		v102.tex = texcoord.at(0);		v102.nor = normals.at(34); //face 35
	v103.pos = positions.at(18);	v103.tex = texcoord.at(1);		v103.nor = normals.at(34);
	v104.pos = positions.at(7);		v104.tex = texcoord.at(2);		v104.nor = normals.at(34);
	v105.pos = positions.at(28);	v105.tex = texcoord.at(0);		v105.nor = normals.at(35); //face 36
	v106.pos = positions.at(18);	v106.tex = texcoord.at(1);		v106.nor = normals.at(35);
	v107.pos = positions.at(9);		v107.tex = texcoord.at(2);		v107.nor = normals.at(35);
	v108.pos = positions.at(19);	v108.tex = texcoord.at(0);		v108.nor = normals.at(36); //face 37
	v109.pos = positions.at(28);	v109.tex = texcoord.at(1);		v109.nor = normals.at(36);
	v110.pos = positions.at(9);		v110.tex = texcoord.at(2);		v110.nor = normals.at(36);
	v111.pos = positions.at(30);	v111.tex = texcoord.at(0);		v111.nor = normals.at(37); //face 38
	v112.pos = positions.at(28);	v112.tex = texcoord.at(1);		v112.nor = normals.at(37);
	v113.pos = positions.at(19);	v113.tex = texcoord.at(2);		v113.nor = normals.at(37);
	v114.pos = positions.at(9);		v114.tex = texcoord.at(0);		v114.nor = normals.at(38); //face 39
	v115.pos = positions.at(30);	v115.tex = texcoord.at(1);		v115.nor = normals.at(38);
	v116.pos = positions.at(19);	v116.tex = texcoord.at(2);		v116.nor = normals.at(38);
	v117.pos = positions.at(20);	v117.tex = texcoord.at(0);		v117.nor = normals.at(39); //face 40
	v118.pos = positions.at(30);	v118.tex = texcoord.at(1);		v118.nor = normals.at(39);
	v119.pos = positions.at(9);		v119.tex = texcoord.at(2);		v119.nor = normals.at(39);
	v120.pos = positions.at(1);		v120.tex = texcoord.at(0);		v120.nor = normals.at(40); //face 41
	v121.pos = positions.at(20);	v121.tex = texcoord.at(1);		v121.nor = normals.at(40);
	v122.pos = positions.at(9);		v122.tex = texcoord.at(2);		v122.nor = normals.at(40);
	v123.pos = positions.at(30);	v123.tex = texcoord.at(0);		v123.nor = normals.at(41); //face 42
	v124.pos = positions.at(20);	v124.tex = texcoord.at(1);		v124.nor = normals.at(41);
	v125.pos = positions.at(1);		v125.tex = texcoord.at(2);		v125.nor = normals.at(41);
	v126.pos = positions.at(11);	v126.tex = texcoord.at(0);		v126.nor = normals.at(42); //face 43
	v127.pos = positions.at(30);	v127.tex = texcoord.at(1);		v127.nor = normals.at(42);
	v128.pos = positions.at(1);		v128.tex = texcoord.at(2);		v128.nor = normals.at(42);
	v129.pos = positions.at(22);	v129.tex = texcoord.at(0);		v129.nor = normals.at(43); //face 44
	v130.pos = positions.at(30);	v130.tex = texcoord.at(1);		v130.nor = normals.at(43);
	v131.pos = positions.at(11);	v131.tex = texcoord.at(2);		v131.nor = normals.at(43);
	v132.pos = positions.at(1);		v132.tex = texcoord.at(0);		v132.nor = normals.at(44); //face 45
	v133.pos = positions.at(22);	v133.tex = texcoord.at(1);		v133.nor = normals.at(44);
	v134.pos = positions.at(11);	v134.tex = texcoord.at(2);		v134.nor = normals.at(44);
	v135.pos = positions.at(21);	v135.tex = texcoord.at(0);		v135.nor = normals.at(45); //face 46
	v136.pos = positions.at(31);	v136.tex = texcoord.at(1);		v136.nor = normals.at(45);
	v137.pos = positions.at(30);	v137.tex = texcoord.at(2);		v137.nor = normals.at(45);
	v138.pos = positions.at(21);	v138.tex = texcoord.at(0);		v138.nor = normals.at(46); //face 47
	v139.pos = positions.at(30);	v139.tex = texcoord.at(1);		v139.nor = normals.at(46);
	v140.pos = positions.at(22);	v140.tex = texcoord.at(2);		v140.nor = normals.at(46);
	v141.pos = positions.at(21);	v141.tex = texcoord.at(0);		v141.nor = normals.at(47); //face 48
	v142.pos = positions.at(22);	v142.tex = texcoord.at(1);		v142.nor = normals.at(47);
	v143.pos = positions.at(31);	v143.tex = texcoord.at(2);		v143.nor = normals.at(47);
	v144.pos = positions.at(23);	v144.tex = texcoord.at(0);		v144.nor = normals.at(48); //face 49
	v145.pos = positions.at(31);	v145.tex = texcoord.at(1);		v145.nor = normals.at(48);
	v146.pos = positions.at(22);	v146.tex = texcoord.at(2);		v146.nor = normals.at(48);
	v147.pos = positions.at(23);	v147.tex = texcoord.at(0);		v147.nor = normals.at(49); //face 50
	v148.pos = positions.at(22);	v148.tex = texcoord.at(1);		v148.nor = normals.at(49);
	v149.pos = positions.at(24);	v149.tex = texcoord.at(2);		v149.nor = normals.at(49);
	v150.pos = positions.at(23);	v150.tex = texcoord.at(0);		v150.nor = normals.at(50); //face 51
	v151.pos = positions.at(24);	v151.tex = texcoord.at(1);		v151.nor = normals.at(50);
	v152.pos = positions.at(31);	v152.tex = texcoord.at(2);		v152.nor = normals.at(50);
	v153.pos = positions.at(25);	v153.tex = texcoord.at(0);		v153.nor = normals.at(51); //face 52
	v154.pos = positions.at(31);	v154.tex = texcoord.at(1);		v154.nor = normals.at(51);
	v155.pos = positions.at(24);	v155.tex = texcoord.at(2);		v155.nor = normals.at(51);
	v156.pos = positions.at(25);	v156.tex = texcoord.at(0);		v156.nor = normals.at(52); //face 53
	v157.pos = positions.at(24);	v157.tex = texcoord.at(1);		v157.nor = normals.at(52);
	v158.pos = positions.at(26);	v158.tex = texcoord.at(2);		v158.nor = normals.at(52);
	v159.pos = positions.at(25);	v159.tex = texcoord.at(0);		v159.nor = normals.at(53); //face 54
	v160.pos = positions.at(26);	v160.tex = texcoord.at(1);		v160.nor = normals.at(53);
	v161.pos = positions.at(31);	v161.tex = texcoord.at(2);		v161.nor = normals.at(53);
	v162.pos = positions.at(27);	v162.tex = texcoord.at(0);		v162.nor = normals.at(54); //face 55
	v163.pos = positions.at(31);	v163.tex = texcoord.at(1);		v163.nor = normals.at(54);
	v164.pos = positions.at(26);	v164.tex = texcoord.at(2);		v164.nor = normals.at(54);
	v165.pos = positions.at(27);	v165.tex = texcoord.at(0);		v165.nor = normals.at(55); //face 56
	v166.pos = positions.at(26);	v166.tex = texcoord.at(1);		v166.nor = normals.at(55);
	v167.pos = positions.at(28);	v167.tex = texcoord.at(2);		v167.nor = normals.at(55);
	v168.pos = positions.at(27);	v168.tex = texcoord.at(0);		v168.nor = normals.at(56); //face 57
	v169.pos = positions.at(28);	v169.tex = texcoord.at(1);		v169.nor = normals.at(56);
	v170.pos = positions.at(31);	v170.tex = texcoord.at(2);		v170.nor = normals.at(56);
	v171.pos = positions.at(29);	v171.tex = texcoord.at(0);		v171.nor = normals.at(57); //face 58
	v172.pos = positions.at(31);	v172.tex = texcoord.at(1);		v172.nor = normals.at(57);
	v173.pos = positions.at(28);	v173.tex = texcoord.at(2);		v173.nor = normals.at(57);
	v174.pos = positions.at(29);	v174.tex = texcoord.at(0);		v174.nor = normals.at(58); //face 59
	v175.pos = positions.at(28);	v175.tex = texcoord.at(1);		v175.nor = normals.at(58);
	v176.pos = positions.at(30);	v176.tex = texcoord.at(2);		v176.nor = normals.at(58);
	v177.pos = positions.at(29);	v177.tex = texcoord.at(0);		v177.nor = normals.at(59); //face 60
	v178.pos = positions.at(30);	v178.tex = texcoord.at(1);		v178.nor = normals.at(59);
	v179.pos = positions.at(31);	v179.tex = texcoord.at(2);		v179.nor = normals.at(59);

	//Add Vertex Data to GPU
	UINT sizeof_vertex = sizeof(glm::vec3) * 2 + sizeof(glm::vec2);
	m_vbo.AddVertexData(&v0, sizeof_vertex);
	m_vbo.AddVertexData(&v1, sizeof_vertex);
	m_vbo.AddVertexData(&v2, sizeof_vertex);
	m_vbo.AddVertexData(&v3, sizeof_vertex);
	m_vbo.AddVertexData(&v4, sizeof_vertex);
	m_vbo.AddVertexData(&v5, sizeof_vertex);
	m_vbo.AddVertexData(&v6, sizeof_vertex);
	m_vbo.AddVertexData(&v7, sizeof_vertex);
	m_vbo.AddVertexData(&v8, sizeof_vertex);
	m_vbo.AddVertexData(&v9, sizeof_vertex);
	m_vbo.AddVertexData(&v10, sizeof_vertex);
	m_vbo.AddVertexData(&v11, sizeof_vertex);
	m_vbo.AddVertexData(&v12, sizeof_vertex);
	m_vbo.AddVertexData(&v13, sizeof_vertex);
	m_vbo.AddVertexData(&v14, sizeof_vertex);
	m_vbo.AddVertexData(&v15, sizeof_vertex);
	m_vbo.AddVertexData(&v16, sizeof_vertex);
	m_vbo.AddVertexData(&v17, sizeof_vertex);
	m_vbo.AddVertexData(&v18, sizeof_vertex);
	m_vbo.AddVertexData(&v19, sizeof_vertex);
	m_vbo.AddVertexData(&v20, sizeof_vertex);
	m_vbo.AddVertexData(&v21, sizeof_vertex);
	m_vbo.AddVertexData(&v22, sizeof_vertex);
	m_vbo.AddVertexData(&v23, sizeof_vertex);
	m_vbo.AddVertexData(&v24, sizeof_vertex);
	m_vbo.AddVertexData(&v25, sizeof_vertex);
	m_vbo.AddVertexData(&v26, sizeof_vertex);
	m_vbo.AddVertexData(&v27, sizeof_vertex);
	m_vbo.AddVertexData(&v28, sizeof_vertex);
	m_vbo.AddVertexData(&v29, sizeof_vertex);
	m_vbo.AddVertexData(&v30, sizeof_vertex);
	m_vbo.AddVertexData(&v31, sizeof_vertex);
	m_vbo.AddVertexData(&v32, sizeof_vertex);
	m_vbo.AddVertexData(&v33, sizeof_vertex);
	m_vbo.AddVertexData(&v34, sizeof_vertex);
	m_vbo.AddVertexData(&v35, sizeof_vertex);
	m_vbo.AddVertexData(&v36, sizeof_vertex);
	m_vbo.AddVertexData(&v37, sizeof_vertex);
	m_vbo.AddVertexData(&v38, sizeof_vertex);
	m_vbo.AddVertexData(&v39, sizeof_vertex);
	m_vbo.AddVertexData(&v40, sizeof_vertex);
	m_vbo.AddVertexData(&v41, sizeof_vertex);
	m_vbo.AddVertexData(&v42, sizeof_vertex);
	m_vbo.AddVertexData(&v43, sizeof_vertex);
	m_vbo.AddVertexData(&v44, sizeof_vertex);
	m_vbo.AddVertexData(&v45, sizeof_vertex);
	m_vbo.AddVertexData(&v46, sizeof_vertex);
	m_vbo.AddVertexData(&v47, sizeof_vertex);
	m_vbo.AddVertexData(&v48, sizeof_vertex);
	m_vbo.AddVertexData(&v49, sizeof_vertex);
	m_vbo.AddVertexData(&v50, sizeof_vertex);
	m_vbo.AddVertexData(&v51, sizeof_vertex);
	m_vbo.AddVertexData(&v52, sizeof_vertex);
	m_vbo.AddVertexData(&v53, sizeof_vertex);
	m_vbo.AddVertexData(&v54, sizeof_vertex);
	m_vbo.AddVertexData(&v55, sizeof_vertex);
	m_vbo.AddVertexData(&v56, sizeof_vertex);
	m_vbo.AddVertexData(&v57, sizeof_vertex);
	m_vbo.AddVertexData(&v58, sizeof_vertex);
	m_vbo.AddVertexData(&v59, sizeof_vertex);
	m_vbo.AddVertexData(&v60, sizeof_vertex);
	m_vbo.AddVertexData(&v61, sizeof_vertex);
	m_vbo.AddVertexData(&v62, sizeof_vertex);
	m_vbo.AddVertexData(&v63, sizeof_vertex);
	m_vbo.AddVertexData(&v64, sizeof_vertex);
	m_vbo.AddVertexData(&v65, sizeof_vertex);
	m_vbo.AddVertexData(&v66, sizeof_vertex);
	m_vbo.AddVertexData(&v67, sizeof_vertex);
	m_vbo.AddVertexData(&v68, sizeof_vertex);
	m_vbo.AddVertexData(&v69, sizeof_vertex);
	m_vbo.AddVertexData(&v70, sizeof_vertex);
	m_vbo.AddVertexData(&v71, sizeof_vertex);
	m_vbo.AddVertexData(&v72, sizeof_vertex);
	m_vbo.AddVertexData(&v73, sizeof_vertex);
	m_vbo.AddVertexData(&v74, sizeof_vertex);
	m_vbo.AddVertexData(&v75, sizeof_vertex);
	m_vbo.AddVertexData(&v76, sizeof_vertex);
	m_vbo.AddVertexData(&v77, sizeof_vertex);
	m_vbo.AddVertexData(&v78, sizeof_vertex);
	m_vbo.AddVertexData(&v79, sizeof_vertex);
	m_vbo.AddVertexData(&v80, sizeof_vertex);
	m_vbo.AddVertexData(&v81, sizeof_vertex);
	m_vbo.AddVertexData(&v82, sizeof_vertex);
	m_vbo.AddVertexData(&v83, sizeof_vertex);
	m_vbo.AddVertexData(&v84, sizeof_vertex);
	m_vbo.AddVertexData(&v85, sizeof_vertex);
	m_vbo.AddVertexData(&v86, sizeof_vertex);
	m_vbo.AddVertexData(&v87, sizeof_vertex);
	m_vbo.AddVertexData(&v88, sizeof_vertex);
	m_vbo.AddVertexData(&v89, sizeof_vertex);
	m_vbo.AddVertexData(&v90, sizeof_vertex);
	m_vbo.AddVertexData(&v91, sizeof_vertex);
	m_vbo.AddVertexData(&v92, sizeof_vertex);
	m_vbo.AddVertexData(&v93, sizeof_vertex);
	m_vbo.AddVertexData(&v94, sizeof_vertex);
	m_vbo.AddVertexData(&v95, sizeof_vertex);
	m_vbo.AddVertexData(&v96, sizeof_vertex);
	m_vbo.AddVertexData(&v97, sizeof_vertex);
	m_vbo.AddVertexData(&v98, sizeof_vertex);
	m_vbo.AddVertexData(&v99, sizeof_vertex);
	m_vbo.AddVertexData(&v100, sizeof_vertex);
	m_vbo.AddVertexData(&v101, sizeof_vertex);
	m_vbo.AddVertexData(&v102, sizeof_vertex);
	m_vbo.AddVertexData(&v103, sizeof_vertex);
	m_vbo.AddVertexData(&v104, sizeof_vertex);
	m_vbo.AddVertexData(&v105, sizeof_vertex);
	m_vbo.AddVertexData(&v106, sizeof_vertex);
	m_vbo.AddVertexData(&v107, sizeof_vertex);
	m_vbo.AddVertexData(&v108, sizeof_vertex);
	m_vbo.AddVertexData(&v109, sizeof_vertex);
	m_vbo.AddVertexData(&v110, sizeof_vertex);
	m_vbo.AddVertexData(&v111, sizeof_vertex);
	m_vbo.AddVertexData(&v112, sizeof_vertex);
	m_vbo.AddVertexData(&v113, sizeof_vertex);
	m_vbo.AddVertexData(&v114, sizeof_vertex);
	m_vbo.AddVertexData(&v115, sizeof_vertex);
	m_vbo.AddVertexData(&v116, sizeof_vertex);
	m_vbo.AddVertexData(&v117, sizeof_vertex);
	m_vbo.AddVertexData(&v118, sizeof_vertex);
	m_vbo.AddVertexData(&v119, sizeof_vertex);
	m_vbo.AddVertexData(&v120, sizeof_vertex);
	m_vbo.AddVertexData(&v121, sizeof_vertex);
	m_vbo.AddVertexData(&v122, sizeof_vertex);
	m_vbo.AddVertexData(&v123, sizeof_vertex);
	m_vbo.AddVertexData(&v124, sizeof_vertex);
	m_vbo.AddVertexData(&v125, sizeof_vertex);
	m_vbo.AddVertexData(&v126, sizeof_vertex);
	m_vbo.AddVertexData(&v127, sizeof_vertex);
	m_vbo.AddVertexData(&v128, sizeof_vertex);
	m_vbo.AddVertexData(&v129, sizeof_vertex);
	m_vbo.AddVertexData(&v130, sizeof_vertex);
	m_vbo.AddVertexData(&v131, sizeof_vertex);
	m_vbo.AddVertexData(&v132, sizeof_vertex);
	m_vbo.AddVertexData(&v133, sizeof_vertex);
	m_vbo.AddVertexData(&v134, sizeof_vertex);
	m_vbo.AddVertexData(&v135, sizeof_vertex);
	m_vbo.AddVertexData(&v136, sizeof_vertex);
	m_vbo.AddVertexData(&v137, sizeof_vertex);
	m_vbo.AddVertexData(&v138, sizeof_vertex);
	m_vbo.AddVertexData(&v139, sizeof_vertex);
	m_vbo.AddVertexData(&v140, sizeof_vertex);
	m_vbo.AddVertexData(&v141, sizeof_vertex);
	m_vbo.AddVertexData(&v142, sizeof_vertex);
	m_vbo.AddVertexData(&v143, sizeof_vertex);
	m_vbo.AddVertexData(&v144, sizeof_vertex);
	m_vbo.AddVertexData(&v145, sizeof_vertex);
	m_vbo.AddVertexData(&v146, sizeof_vertex);
	m_vbo.AddVertexData(&v147, sizeof_vertex);
	m_vbo.AddVertexData(&v148, sizeof_vertex);
	m_vbo.AddVertexData(&v149, sizeof_vertex);
	m_vbo.AddVertexData(&v150, sizeof_vertex);
	m_vbo.AddVertexData(&v151, sizeof_vertex);
	m_vbo.AddVertexData(&v152, sizeof_vertex);
	m_vbo.AddVertexData(&v153, sizeof_vertex);
	m_vbo.AddVertexData(&v154, sizeof_vertex);
	m_vbo.AddVertexData(&v155, sizeof_vertex);
	m_vbo.AddVertexData(&v156, sizeof_vertex);
	m_vbo.AddVertexData(&v157, sizeof_vertex);
	m_vbo.AddVertexData(&v158, sizeof_vertex);
	m_vbo.AddVertexData(&v159, sizeof_vertex);
	m_vbo.AddVertexData(&v160, sizeof_vertex);
	m_vbo.AddVertexData(&v161, sizeof_vertex);
	m_vbo.AddVertexData(&v162, sizeof_vertex);
	m_vbo.AddVertexData(&v163, sizeof_vertex);
	m_vbo.AddVertexData(&v164, sizeof_vertex);
	m_vbo.AddVertexData(&v165, sizeof_vertex);
	m_vbo.AddVertexData(&v166, sizeof_vertex);
	m_vbo.AddVertexData(&v167, sizeof_vertex);
	m_vbo.AddVertexData(&v168, sizeof_vertex);
	m_vbo.AddVertexData(&v169, sizeof_vertex);
	m_vbo.AddVertexData(&v170, sizeof_vertex);
	m_vbo.AddVertexData(&v171, sizeof_vertex);
	m_vbo.AddVertexData(&v172, sizeof_vertex);
	m_vbo.AddVertexData(&v173, sizeof_vertex);
	m_vbo.AddVertexData(&v174, sizeof_vertex);
	m_vbo.AddVertexData(&v175, sizeof_vertex);
	m_vbo.AddVertexData(&v176, sizeof_vertex);
	m_vbo.AddVertexData(&v177, sizeof_vertex);
	m_vbo.AddVertexData(&v178, sizeof_vertex);
	m_vbo.AddVertexData(&v179, sizeof_vertex);

	std::vector<GLuint> indices;
	for (GLuint i = 0; i < 180; ++i) {
		indices.push_back(i);
	}

	for (GLuint i = 0; i < indices.size(); ++i)
	{
		m_vbo.AddIndexData(&indices[i], sizeof(unsigned int));
	}
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);

	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

}
void CUrchin::Render() {
	glBindVertexArray(m_vao);
	m_texture.Bind();
	glDrawElements(GL_TRIANGLES, 180, GL_UNSIGNED_INT, 0);
}
void CUrchin::Release() {
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}