#ifndef TEXTURE_H
#define TEXTURE_H

#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_3_3_Core>

class TextureCreator : protected QOpenGLFunctions_3_3_Core{
public:
    TextureCreator();
    ~TextureCreator();

    void create2DTexture(const QString& imgPath, GLuint& texture);    
    void createCubeTexture(const QVector<QString> faces, GLuint &texture);
};

#endif // TEXTURE_H
