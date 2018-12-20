#ifndef MESH_H
#define	MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <Shader.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

//Przechowujemy ka¿dy z wymaganych wektorów w strukturze o nazwie Vertex, której mo¿emy u¿yæ 
//do zaindeksowania ka¿dego z atrybutów wierzcho³ków. 
struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	GLfloat deltaTime;
	/*  Mesh Data  */
	vector<float> vert;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	unsigned int VAO;
	/*  Functions  */
	// constructor
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		// now that we have all the required data, set the vertex buffers and its attribute pointers.
		setupMesh();
	}

	Mesh()
	{
		//setupMesh2();
	}


	// render the mesh
	void Draw(Shader* shader, glm::mat4 *model, bool &isFromFile, bool &rotating,bool &isBlue)
	{
		// bind appropriate textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		shader->use();
		shader->setMat4("model", *model);
		
		if (!isFromFile) {
			shader->setBool("isBlue", isBlue);
		}
	
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // aktywuj odpowiedni¹ jednostkê teksturowania przed powi¹zaniem
            // pobierz numer tekstury (N dla diffuse_textureN)
			string number;
			string name = textures[i].type;
			
			if (name == "texture_diffuse") {
				number = std::to_string(diffuseNr);
				diffuseNr++;
			}
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to stream
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to stream
			else if (name == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to stream
			//cout << name << number<< endl;
													 // now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
			// and finally bind the texture
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		
		// narysuj siatkê
		glBindVertexArray(VAO);
		if (indices.size() != 0)
		{
			if (isFromFile) {
				glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
				glActiveTexture(GL_TEXTURE0);
			}
			else
				glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawArrays(GL_POINTS, 0, vert.size());
		}
		glBindVertexArray(0);

		// always good practice to set everything back to defaults once configured.
		
	}

	void generateTorus(int num_segments, int num_rings, float thickness, float R) {
		vector<unsigned int> indexBuffer;
		vector<float> vertBuffer;
		int num_vertices = (num_rings + 1) * (num_segments + 1);

		const float pi = 3.1415926535f;
		const float r1 = R;
		const float r2 = thickness;
		for (int i = 0, index = 0; i <= num_rings; ++i) {
			for (int j = 0; j <= num_segments; ++j, ++index) {
				// Compute texture coordinates (surface parameters)
				// Note that u is added to j. This creates a spiral pattern, which
				// allows us to draw the entire torus in one triangle strip without
				// using degenerate triangles.
				// (Yet we still need degenerate triangles to avoid texture
				// wrap-around)
				float u = float(i) / num_rings;
				float v = (float(j) + u) / num_segments;

				// Compute angles
				float u_angle = u * 2 * pi;
				float v_angle = v * 2 * pi;

				// Position
				float x = cos(u_angle) * (r1 + cos(v_angle) * r2);
				float y = sin(u_angle) * (r1 + cos(v_angle) * r2);
				float z = sin(v_angle) * r2;
				vertBuffer.push_back(x);
				vertBuffer.push_back(y);
				vertBuffer.push_back(z);
			}
		}

		// Compute torus indices
		for (int i = 0, index = 0; i <= num_vertices; ++i) {
			indexBuffer.push_back(int(i % num_vertices));
			indexBuffer.push_back(int((i + num_segments + 1) % num_vertices));
		}
		//cout << vert.size() << endl;
		vert = vertBuffer;
		indices = indexBuffer;
		setupMesh2();
	}











































































	void generateSphere (int max,double r)
	
	{
		vector<unsigned int> indexBuffer;
		vector<float> vertBuffer;
		vector<float> vertBuffer2;
		int ai, fj, maxai, maxfj;
	
		double x, y, z, x1, y1, z1,  fi, alf, dfi, dalf;

		
		

		fi = 0.0;

		alf = 0.0;

		maxai = max; maxfj = max;
	
		dalf = 3.14f / float(maxai);

		dfi = 3.14f / float(maxfj);
		int num_vertices=0;
		for (fj = 0; fj <= maxfj; fj++)
		{
			for (ai = 0; ai <= maxai + 1; ai++)

			{
				x = r * cos(fi)*cos(alf);

				y = r * sin(fi);

				z = r * cos(fi)*sin(alf);

				vertBuffer.push_back(x);
				vertBuffer.push_back(y);
				vertBuffer.push_back(z);
				num_vertices += 1;

				x1 = r * cos(fi + dfi)*cos(alf);

				y1 = r * sin(fi + dfi);

				z1 = r * cos(fi + dfi)*sin(alf);
				vertBuffer2.push_back(x);
				vertBuffer2.push_back(y);
				vertBuffer2.push_back(z);
				num_vertices += 1;
				alf = float(ai)*dalf;

			}
			alf = 0.0;

			fi = dfi * float(fj);
		}
		for (int i = 0, index = 0; i <= num_vertices; ++i) {
			indexBuffer.push_back(int(i % num_vertices));
			indexBuffer.push_back(int((i+ maxai+1) % num_vertices));
		}

		for (int i = 0; i < vertBuffer2.size(); i++) {
			vertBuffer.push_back(vertBuffer2[i]);
		}

		vert = vertBuffer;
		indices = indexBuffer;
		setupMesh2();
	}















private:
	/*  Render data  */
	unsigned int VBO, EBO;

	/*  Functions    */
	// initializes all the buffer objects/arrays
	void setupMesh()
	{
		// create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		
		/*przyk³ad : Vertex vertex;
		vertex.Position = glm::vec3(0.2f, 0.4f, 0.6f);
		vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
		vertex.TexCoords = glm::vec2(1.0f, 0.0f);*/
		// = [0.2f, 0.4f, 0.6f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f];
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}

	void setupMesh2()
	{
		// create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(float), &vert[0], GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		//cout << "Liczba bajtów w buforze " << vert.size() * sizeof(float) << endl;
		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//cout << "Stride: " << 3 * sizeof(float) << endl;

		glBindVertexArray(0);
	}
};
#endif