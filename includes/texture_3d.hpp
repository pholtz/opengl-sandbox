#ifndef TEXTURE_3D_H
#define TEXTURE_3D_H


class Texture3D
{
public:
    GLuint ID;
    GLuint Width, Height;
    Texture3D();
    void Generate(GLuint width, GLuint height, unsigned char* data);
    void Bind() const;
};

#endif