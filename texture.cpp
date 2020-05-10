#include "texture.h"
#include <QDebug>
#include <QImage>

TextureCreator::TextureCreator(){
    this->initializeOpenGLFunctions();   //这一步做了什么东西，还不能少了！
}
TextureCreator::~TextureCreator(){}

void TextureCreator::create2DTexture(const QString &imgPath, GLuint &texture)
{
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    QImage img1=QImage(imgPath).convertToFormat(QImage::Format_RGBA8888);
    if(!img1.isNull()){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img1.width(), img1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img1.bits());
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        qDebug()<<"Load image failed:"<<endl;
    }

    //set texture parameter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void TextureCreator::TextureCreator::createCubeTexture(const QVector<QString> faces, GLuint &texture)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    for(int i=0; i<6; i++){
        QImage img1=QImage(faces[i]).convertToFormat(QImage::Format_RGBA8888);
        if(!img1.isNull()){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, img1.width(), img1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img1.bits());
        }else{
            qDebug()<<"Load image failed:"<<endl;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
