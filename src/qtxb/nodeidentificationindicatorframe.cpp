#include "nodeidentificationindicatorframe.h"

NodeIdentificationIndicatorFrame::NodeIdentificationIndicatorFrame(QObject *parent) :
    DigiMeshFrame(parent)
{
    setFrameType(DigiMeshFrame::NodeIdentificationIndicatorFrame);
}
void NodeIdentificationIndicatorFrame::readPacket(QByteArray rx){
    Q_UNUSED(rx)
}
