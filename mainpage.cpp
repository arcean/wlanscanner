#include <MLinearLayoutPolicy>
#include <QGraphicsLinearLayout>
#include <MButton>
#include <MLayout>
#include <MToolBar>
#include <MAction>
#include <MTextEdit>
#include <MSceneWindow>
#include <QDebug>
#include <MApplication>
#include <MOnDisplayChangeEvent>
#include <MApplicationWindow>
#include <MTheme>
#include <QGraphicsGridLayout>
#include <MGridLayoutPolicy>
#include <MImageWidget>
#include <MPannableViewport>
#include <MSeparator>
#include <QGraphicsItem>
#include <MMessageBox>
#include <math.h>
#include <MAbstractCellCreator>
#include <MList>
#include <MBasicListItem>
#include <QFile>
#include <MSceneManager>

#include "mainpage.h"
#include "viewHeaderWithIcon.h"
#include "mlistitemcreator.h"
#include "aboutdialog.h"
#include "networkpage.h"

MainPage::MainPage(QGraphicsItem *parent)
    : MApplicationPage(parent)
{

}

MainPage::~MainPage()

{
    delete model;
    delete objectMenu;
}

void MainPage::createContent()
{
    MTheme *theme = MTheme::instance();
    theme->loadCSS("/opt/wlanscanner/style/wlanscanner.css");
    theme->addPixmapDirectory("/opt/wlanscanner/data/");
    applicationWindow()->setStyleName("CommonApplicationWindow");
    setStyleName("CommonApplicationPage");
    MLayout *layout = new MLayout(this);

    /////////////////////////////////////////////////// HEADER VIEW

    /* Fix: lock page content */
    setPannable(false);
    MPannableViewport *viewport = this->pannableViewport();
    viewport->setAutoRange(false);
    viewport->setRange(QRectF(0,0,0,0));

    ViewHeaderWithIcon *header = new ViewHeaderWithIcon;
    header->setTitle("!!_wlanscanner_");

    MLinearLayoutPolicy *portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    layout->setPortraitPolicy(portraitPolicy);
    portraitPolicy->setContentsMargins(0, 0, 0, 0);
    portraitPolicy->setSpacing(0);
    portraitPolicy->setNotifyWidgetsOfLayoutPositionEnabled(true);
    // Create main app viewport
    MPannableViewport *viewportWidget = new MPannableViewport;
    // Create layout for the main app viewport
    MLayout *viewportLayout = new MLayout();
    // Create widget for integrating the layout with the viewport
    QGraphicsWidget *form = new QGraphicsWidget();
    form->setLayout(viewportLayout);
    viewportWidget->setWidget(form);
    // Create layout policy for the main app viewport
    viewportLayoutPolicy = new MLinearLayoutPolicy(viewportLayout, Qt::Vertical);
    viewportLayoutPolicy->setObjectName("ListViewport");
    // Add header to the layout
    portraitPolicy->addItem(header);
    // Add viewportWidget to layout (main layout)
    portraitPolicy->addItem(viewportWidget);

    /////////////////////////////////////////////////// CONTENT

    list = new MList(this);
    list->setObjectName("ListStyle");
    MListItemCreator *cellCreator = new MListItemCreator;
    list->setCellCreator(cellCreator);
    model = new ListModel();
    list->setItemModel(model);

    noResultLabel = new MLabel("No networks found");
    noResultLabel->setStyleName("CommonEmptyStateTitleInverted");
    noResultLabel->setWordWrap(true);
    noResultLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    isNoResultLabelVisible = true;

    viewportLayoutPolicy->addItem(noResultLabel);

    /////////////////////////////////////////////////// ACTIONS
    MAction *dummyAction = new MAction("", "", this);
    dummyAction->setLocation(MAction::ToolBarLocation);
    this->addAction(dummyAction);

    MAction *addNewNote = new MAction("icon-m-toolbar-addAA", "Add", this);
    addNewNote->setLocation(MAction::ToolBarLocation);
    this->addAction(addNewNote);

    MAction *aboutDialog = new MAction("icon-m-toolbar-tag", "Add", this);
    aboutDialog->setLocation(MAction::ToolBarLocation);
    this->addAction(aboutDialog);

    /////////////////////////////////////////////////// OBJECT MENU
    objectMenu = new MObjectMenu(0);

    MAction *removeNote = new MAction("Remove", this);
    removeNote->setLocation(MAction::ObjectMenuLocation);
    objectMenu->addAction(removeNote);

    /////////////////////////////////////////////////// SIGNALS
    connect(aboutDialog, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    connect(header, SIGNAL(buttonClicked()), this, SLOT(scan()));
    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(showNetworkPage(QModelIndex)));

    /////////////////////////////////////////////////// OTHER
    // Create info banner.
    infoBanner = new MBanner();

    // Create DbusHandler
    dbusHandler = new DbusHandler();
    connect(dbusHandler, SIGNAL(scanComplete(QList<ScanResult>)), model, SLOT(reload(QList<ScanResult>)));
    connect(dbusHandler, SIGNAL(scanComplete(QList<ScanResult>)), this, SLOT(scanComplete(QList<ScanResult>)));

    // Create scanTimer
    scanTimer = new QTimer();
    scanTimer->setInterval(5000);
    connect(scanTimer, SIGNAL(timeout()), this, SLOT(scan()));

    //! Start scanning
    dbusHandler->Scan();
}

void MainPage::showNetworkPage(const QModelIndex& index)
{
    int number = index.row();

    if (number < 0 || number > networks.count()) {
        qDebug() << "showNetworkPage fails with index" << number;
        return;
    }

    ScanResult network = networks.at(number);

    NetworkPage *networkPage = new NetworkPage(&network);
    networkPage->appear(MSceneWindow::DestroyWhenDone);
}

void MainPage::showAboutDialog()
{
    AboutDialog *aboutDialog = new AboutDialog();
    aboutDialog->appear(MSceneWindow::DestroyWhenDone);
}

void MainPage::scan()
{
    qDebug() << "Timeout: scanning...";
    dbusHandler->Scan();
}

void MainPage::scanComplete(const QList<ScanResult> &networks)
{
    // Update networks list
    this->networks = networks;
   // if (model)
    //    delete model;

    // Show and hide noResultLabel and list
    if (networks.count() == 0 && !isNoResultLabelVisible) {
        qDebug() << "CATCH 1";
        viewportLayoutPolicy->removeItem(list);
        viewportLayoutPolicy->addItem(noResultLabel);
        isNoResultLabelVisible = true;

       /* if(list->model() != NULL)
            delete model;*/
        qDebug() << "CATCH 1 after";
    }
    else if (networks.count() > 0 && isNoResultLabelVisible) {
        qDebug() << "CATCH 2";
        viewportLayoutPolicy->removeItem(noResultLabel);
        viewportLayoutPolicy->addItem(list);
        isNoResultLabelVisible = false;

      //  model = new ListModel(networks);
       /* list->setItemModel(model);
        qDebug() << "CATCH 2 after";*/
    }

    // Remove all rows
    list->itemModel()->removeRows(0, list->itemModel()->rowCount());
    // Insert new rows
    list->itemModel()->insertRows(0, networks.count(), QModelIndex());

    // Create new model for the list
   /* if (networks.count() > 0) {
        if(list->model() != NULL)
            delete model;
        model = new ListModel(networks);
        list->setItemModel(model);
    }*/

    // Start scanTimer
    scanTimer->start();
}

void MainPage::reloadModel(int oldRow)
{
    if (oldRow == -1)
        list->itemModel()->insertRows(0, 1, QModelIndex());
    else {
        // Firstly, remove current row
        list->itemModel()->removeRows(oldRow, 1, QModelIndex());
        // Then insert a new one at 0
        list->itemModel()->insertRows(0, 1, QModelIndex());
    }

}

void MainPage::showObjectMenu(const QModelIndex &index)
{
    // Show objectMenu
    sceneManager()->appearSceneWindow(objectMenu);
    longTappedIndex = index;
}

void MainPage::throwMessage(const QString &text)
{
    infoBanner->setStyleName(MBannerType::InformationBanner);
    infoBanner->setTitle(text);
    infoBanner->appear(scene(), MSceneWindow::KeepWhenDone);
}
