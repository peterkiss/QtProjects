#ifndef XLAYER_H
#define XLAYER_H

#include <QString>

class XGraphicsScene;
class XLayer
{
public:    
    XLayer(XGraphicsScene* scene, QString othersname = "" );
    bool isActive(){return state;}
    void setActive(bool newState){ state = newState;}
    QString getName(){ return layerName;}
private:
    XGraphicsScene* containingScene;
    QString layerName;
    bool state;
};

#endif // XLAYER_H
