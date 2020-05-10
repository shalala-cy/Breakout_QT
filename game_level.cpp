#include "game_level.h"
#include "game_brick.h"

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>

//texture
extern unsigned int textureSolid, textureBlock;

GameLevel::GameLevel()
{
}

GameLevel::~GameLevel()
{
}

void GameLevel::load(const char *path, float w, float h)
{
    QVector<QVector<qint32>> levelData;
    //Txt file
    QString txtPath(path);
    QFile aFile(txtPath);
    if(!aFile.exists()){
        qDebug()<<"File don't exist!"<<endl;
        return;
    }
    if(!aFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"Failed to open file!"<<endl;
        return;
    }
    //read txt file by QTextStream
    QTextStream aStream(&aFile);
    while(!aStream.atEnd()){
        QString lingString=aStream.readLine();
        QTextStream lineStream(&lingString);
        qint32 data;
        QVector<qint32> lineData;
        while(!lineStream.atEnd()){
            lineStream>>data;
            lineData.push_back(data);
        }
        levelData.push_back(lineData);
    }
    if(levelData.size()>0){
        init(levelData, w, h);
    }else{
        qDebug()<<"Failed to read level file!"<<endl;
    }
}

void GameLevel::init(QVector<QVector<qint32>> levelData, float w, float h)
{
    this->bricks.clear();
    unsigned int numOfRow, numOfCOl;
    numOfRow = levelData.size();
    numOfCOl = levelData[0].size();
    float unitWidth, unitHeight;
    unitWidth = w/numOfCOl;
    unitHeight = 0.5 * h/numOfRow;

    for(int i=0;i<numOfRow;i++)
        for(int j=0;j<numOfCOl;j++)
        {
            //used to construct brick obj
            QVector3D color;
            unsigned int* tex;
            bool solid=false;
            bool destroyed=false;

            if(levelData[i][j] > 0 ){
                if(levelData[i][j]==1){
                    color=QVector3D(1.0, 1.0, 1.0);
                    solid=true;
                    tex=&textureSolid;
                }else if(levelData[i][j]==2){
                    color=QVector3D(0.2f, 0.6f, 1.0f);
                    tex=&textureBlock;
                }else if(levelData[i][j]==3){
                    color=QVector3D(0.0f, 0.7f, 0.0f);
                    tex=&textureBlock;
                }else{
                    color=QVector3D(0.8f, 0.8f, 0.4f);
                    tex=&textureBlock;
                }
            }else{
                destroyed=true;
            }
            //push a temparuary Brick object into Qvector, that's why the Copy Constructor is necessary for Brick Class.
            this->bricks.push_back(Brick(QVector2D(unitWidth*j, unitHeight*i), QVector2D(unitWidth, unitHeight), tex, color, destroyed, solid));
        }
}
