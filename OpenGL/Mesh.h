#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "stb_image.h"

/*unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}*/

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec2 TexCoords;
    glm::vec3 Normal;
};

class VBO {
public:
    GLuint ID;
    VBO(std::vector<Vertex>& v) {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(Vertex), &v[0], GL_STATIC_DRAW);
    }
    VBO(std::vector<GLfloat>& v) {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(GLfloat), &v[0], GL_STATIC_DRAW);
    }

    void Bind() { glBindBuffer(GL_ARRAY_BUFFER, ID); }
    void UnBind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
    void Delete() { glDeleteBuffers(1, &ID); }
};

class VAO {
public:
    GLuint ID;
    VAO() { glGenVertexArrays(1, &ID); }

    void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComp, GLenum type, GLsizeiptr stride, void* offset) {
        VBO.Bind();
        glVertexAttribPointer(layout, numComp, type, GL_FALSE, stride, offset);
        glEnableVertexAttribArray(layout);
        VBO.UnBind();
    }

    void Bind() { glBindVertexArray(ID); }
    void UnBind() { glBindVertexArray(0); }
    void Delete() { glDeleteVertexArrays(1, &ID); }
};

class EBO {
public:
    GLuint ID;
    EBO(std::vector<GLuint>& indices) {
        glGenBuffers(1, &ID);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    }

    void Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }
    void UnBind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
    void Delete() { glDeleteBuffers(1, &ID); }
};

class Texture {
public:
    GLuint ID;
	GLenum type;
	Texture(const char* image, GLenum texType, GLenum format, GLenum pixelType) {
        type = texType;
        glGenTextures(1, &ID);
        Bind();
        glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int width, height, nrChannels;
        unsigned char *data = stbi_load("./Resources/Stone.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(type, 0, GL_RGB, width, height, 0, format, pixelType, data);
            glGenerateMipmap(type);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
        UnBind();
    }

	void Bind() { glBindTexture(type, ID); }
	void UnBind() { glBindTexture(type, 0); }
	void Delete() { glDeleteTextures(1, &ID); }
};

#endif