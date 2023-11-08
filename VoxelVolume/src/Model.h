#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include "glm/glm.hpp"

class Model {
private:
	// Store the VAO and other related information in your ModelObject class, assuming you have the following member variables:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	GLsizei m_indexCount;
	std::vector<glm::vec3> m_vertexArray;
	std::vector<glm::vec3> m_normalArray;
	std::vector<glm::vec2> m_texCoordArray;
	bool initialized;

public:
	unsigned int numIndices;


	Model();
	Model(const std::string& filepath);


	~Model();
	bool hasModel();
	void loadModel(const std::string& filepath);
	void createFlatGroundModelSimple(float width, float depth);
	//Model(float width, float depth, int numRows, int numCols);
	//Model(float width, float depth, float height, TextureData* mapTexture);

	

	void render();

	void Bind() const;
	void Unbind() const;
	//void createFlatGround(float width, float depth);
	//void createFlatGround(float width, float depth, int numRows, int numCols);
	//void createHeightmap(float width, float depth, float height, TextureData* mapTexture);
	std::vector<glm::vec3> getVertexArray() { return m_vertexArray; }
	std::vector<glm::vec3> getNormalArray() { return m_normalArray; }
	std::vector<glm::vec2> getTexCoordArray() { return m_texCoordArray; }
private: 
	void loadThroughTiny(const std::string& filepath);
	void createFlatGround(float width, float depth);
};