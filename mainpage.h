#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <MApplicationPage>
#include <MDialog>
#include <QColor>
#include <MAction>
#include <MWidgetAction>
#include <MLabel>
#include <MBanner>
#include <MList>
#include <MObjectMenu>
#include <QTimer>
#include <MLinearLayoutPolicy>

#include "listmodel.h"
#include "dbushandler.h"

/* To enable debug msgs: */
/* -output-level debug */

class MainPage : public MApplicationPage {
    Q_OBJECT
public:
    MainPage(QGraphicsItem *parent = 0);
    ~MainPage();

protected:
    virtual void createContent();

public slots:

private slots:
    void showAboutDialog();

    void reloadModel(int oldRow);

    void showObjectMenu(const QModelIndex &index);
    void showNetworkPage(const QModelIndex& index);

    void scan();
    void scanComplete(const QList<ScanResult> &networks);

private:
    void throwMessage(const QString &text);

    MBanner *infoBanner;

    ListModel *model;
    DbusHandler *dbusHandler;

    QTimer *scanTimer;

    MList *list;
    MLabel *noResultLabel;
    bool isNoResultLabelVisible;
    MLinearLayoutPolicy *viewportLayoutPolicy;
    MObjectMenu *objectMenu;

    QModelIndex longTappedIndex;
    QList<ScanResult> networks;

};

#endif // MAINPAGE_H
