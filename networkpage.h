#ifndef NETWORKPAGE_H
#define NETWORKPAGE_H

#include <MApplicationPage>
#include <MLinearLayoutPolicy>

#include "scanresult.h"

class NetworkPage : public MApplicationPage {
    Q_OBJECT
public:
    NetworkPage(ScanResult *network, QGraphicsItem *parent = 0);
    ~NetworkPage();

protected:
    virtual void createContent();

private:
    MLinearLayoutPolicy *viewportLayoutPolicy;

    ScanResult *network;

};

#endif // NETWORKPAGE_H
