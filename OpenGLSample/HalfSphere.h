#ifndef HALFSPHERE_H
#define HALFSPHERE_H
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <glad/glad.h>

class HalfSphere
{
private:
	std::vector<float> halfsphere_vertices;
	std::vector<float> halfsphere_texcoord;
	std::vector<int> halfsphere_indices;
	GLuint VBO1, VAO1, EBO1;
	float radius = 1.0f;
	int sectorCount = 36;
	int stackCount = 18;

public:

	~HalfSphere()
	{
		glDeleteVertexArrays(1, &VAO1);
		glDeleteBuffers(1, &VBO1);
		glDeleteBuffers(1, &EBO1);
	}
	HalfSphere(float r, int sectors, int stacks)
	{
		radius = r;
		sectorCount = sectors;
		stackCount = stacks;


		/* GENERATE VERTEX ARRAY */
		float x, y, z, xy;                              // vertex position
		float lengthInv = 1.0f / radius;    // vertex normal
		float s, t;                                     // vertex texCoord

		float sectorStep = (float)(2 * M_PI / sectorCount);
		float stackStep = (float)(M_PI / stackCount);
		float sectorAngle, stackAngle;

		for (int i = 0; i <= stackCount; ++i)
		{
			stackAngle = (float)(M_PI / 2 - (float)(i * stackStep) / 2);        // starting from pi/2 to -pi/2
			xy = 1.02f * radius * cosf(stackAngle);             // r * cos(u)
			z = radius * sinf(stackAngle);              // r * sin(u)

														// add (sectorCount+1) vertices per stack
														// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= sectorCount; ++j)
			{
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

														// vertex position (x, y, z)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);				// r * cos(u) * sin(v)
				halfsphere_vertices.push_back(x);
				halfsphere_vertices.push_back(y);
				halfsphere_vertices.push_back(z);


				// vertex tex coord (s, t) range between [0, 1]
				s = (float)j / sectorCount;
				t = (float)i / stackCount;
				halfsphere_vertices.push_back(s);
				halfsphere_vertices.push_back(t);

			}
		}
		/* GENERATE VERTEX ARRAY */


		/* GENERATE INDEX ARRAY */
		int k1, k2;
		for (int i = 0; i < stackCount; ++i)
		{
			k1 = i * (sectorCount + 1);     // beginning of current stack
			k2 = k1 + sectorCount + 1;      // beginning of next stack

			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					halfsphere_indices.push_back(k1);
					halfsphere_indices.push_back(k2);
					halfsphere_indices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (stackCount - 1))
				{
					halfsphere_indices.push_back(k1 + 1);
					halfsphere_indices.push_back(k2);
					halfsphere_indices.push_back(k2 + 1);
				}
			}
		}
		/* GENERATE INDEX ARRAY */


		/* GENERATE VAO-EBO */
		//GLuint VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO1);
		glGenBuffers(1, &VBO1);
		glGenBuffers(1, &EBO1);
		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(VAO1);

		glBindBuffer(GL_ARRAY_BUFFER, VBO1);
		glBufferData(GL_ARRAY_BUFFER, (unsigned int)halfsphere_vertices.size() * sizeof(float), halfsphere_vertices.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned int)halfsphere_indices.size() * sizeof(unsigned int), halfsphere_indices.data(), GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		/* GENERATE VAO-EBO */


	}
	void Draw()
	{
		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES,
			(unsigned int)halfsphere_indices.size(),
			GL_UNSIGNED_INT,
			(void*)0);
		glBindVertexArray(0);
	}
};


#endif
