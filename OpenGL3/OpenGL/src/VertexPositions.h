#pragma once
#include <GL/glew.h>
#include <vector>

#include "Shader.h"

using namespace std;

class VertexPositions
{
public:
	unsigned int VAO;
	Shader mShader;
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> colors;
	std::vector<unsigned int> indices;


	glm::mat4 *Model , *View, *Projection;
	glm::vec3  *lightcolor, *lightposition, *viewposition;

	bool isLighting=false;


	unsigned int bufferObjects;
	unsigned int bufferNormals;
	unsigned int bufferColors;
	unsigned int bufferIndices;

	VertexPositions(const std::string& filepath, glm::mat4 *_Model, glm::mat4 *_View, glm::mat4 *_Projection, glm::vec3* _lightcolor=NULL, glm::vec3* _lightposition = NULL, glm::vec3* _viewposition = NULL)
	{
		Model = _Model;
		View = _View;
		Projection = _Projection;
		lightcolor = _lightcolor;
		lightposition = _lightposition;
		viewposition = _viewposition;
		mShader.InitShader(filepath);

		if (_lightcolor != NULL)
		{
			isLighting = true;
		}
	}

	void AddCube(float x, float y)
	{
		float s=0.2f;
		x = x * s * 2;
		y = y * s * 2;
		AddElements({
			-s + x, -s + y, -s,
			s + x, -s + y, -s,
			s + x,  s + y, -s,
			-s + x, s + y, -s,
		}, {}, new glm::vec3(0.0f, 0.0f, -1.0f));

		AddElements({
			-s + x, -s + y,  s,
			s + x, -s + y,  s,
			s + x,  s + y,  s,
			-s + x, s + y,  s,
		}, {}, new glm::vec3(0.0f, 0.0f, 1.0f));


		AddElements({
			-s + x,  s + y,  s,
			-s + x,  s + y, -s,
			-s + x, -s + y, -s,
			-s + x, -s + y,  s,
		}, {}, new glm::vec3(-1.0f, 0.0f, 0.0f));

		AddElements({
			s + x,  s + y,  s,
			s + x,  s + y, -s,
			s + x, -s + y, -s,
			s + x, -s + y,  s,
		}, {}, new glm::vec3(1.0f, 0.0f, 0.0f));

		AddElements({
			-s + x, -s + y, -s,
			s + x, -s + y, -s,
			s + x, -s + y,  s,
			-s + x, -s + y, s,
		}, {}, new glm::vec3(0.0f, -1.0f, 0.0f));

		AddElements({
			-s + x, s + y, -s,
			s + x,  s + y, -s,
			s + x,  s + y,  s,
			-s + x, s + y, s,
		}, {}, new glm::vec3(0.0f, 1.0f, 0.0f));

	}

	void AddElements(std::vector<float> vertex, std::vector<float> _colors = {}, glm::vec3 *_normals = NULL)
	{

		if (_colors.empty())
		{
			//multicolor by default
			_colors = {
				1.0f,0.0f,0.0f,
				0.0f,1.0f,0.0f,
				0.0f,0.0f,1.0f,
				1.0f,0.0f,0.0f,
			};
		}

		for (int i = 0; i < _colors.size(); i++)
		{
			colors.push_back(_colors[i]);
		}

		int vertexSize = positions.size() / 3; //el 3 por el numero de componentes x,y,z por vertice
		indices.push_back(vertexSize + 0);
		indices.push_back(vertexSize + 1);
		indices.push_back(vertexSize + 2);
		indices.push_back(vertexSize + 2);
		indices.push_back(vertexSize + 3);
		indices.push_back(vertexSize + 0);

		for (int i = 0; i<vertex.size(); i++)
		{
			positions.push_back(vertex[i]);
			if (_normals != NULL)
			{
				if (i % 3 == 0)
				{
					normals.push_back(_normals->x);
					normals.push_back(_normals->y);
					normals.push_back(_normals->z);
				}
			}
			
		}

	}
	

	void ConstructBuffers()
	{
		/*Attach to the VAO*/
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);


		//positions
		glGenBuffers(1, &bufferObjects);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObjects);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

		//colors
		glGenBuffers(1, &bufferColors);
		glBindBuffer(GL_ARRAY_BUFFER, bufferColors);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);


		//normals
		if (!normals.empty())
		{
			glGenBuffers(1, &bufferNormals);
			glBindBuffer(GL_ARRAY_BUFFER, bufferNormals);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		}
		


		//indices
		glGenBuffers(1, &bufferIndices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	}
	void draw()
	{
		
		mShader.UseShader();

		mShader.setMat4("Model",*Model);
		mShader.setMat4("View", *View);
		mShader.setMat4("Projection", *Projection);

		if (isLighting)
		{
			mShader.setVec3("lightcolor", *lightcolor);
			mShader.setVec3("lightposition", *lightposition);
			mShader.setVec3("viewposition", *viewposition);
		}

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		glUseProgram(0);
	}
};