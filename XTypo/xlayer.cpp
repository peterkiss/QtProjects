#include "xlayer.h"
#include "xgraphicsscene.h"

XLayer::XLayer(XGraphicsScene* scene,QString othersname)
{
    state = true;
    this->containingScene = scene;

    if(!othersname.isEmpty())
        this->layerName = othersname;
    else
        this->layerName = "layer" + QString::number(containingScene->getLayers()->count());
    state = true;
    this->containingScene = scene;
}
