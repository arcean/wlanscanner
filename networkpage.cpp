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

#include "networkpage.h"
#include "viewHeader.h"
#include "mlistitemcreator.h"
#include "aboutdialog.h"

NetworkPage::NetworkPage(ScanResult *network, QGraphicsItem *parent)
    : MApplicationPage(parent),
      network(network)
{

}

NetworkPage::~NetworkPage()
{

}

void NetworkPage::createContent()
{
    setStyleName("CommonApplicationPage");
    MLayout *layout = new MLayout(this);

    /////////////////////////////////////////////////// HEADER VIEW

    /* Fix: lock page content */
    setPannable(false);
    MPannableViewport *viewport = this->pannableViewport();
    viewport->setAutoRange(false);
    viewport->setRange(QRectF(0,0,0,0));

    ViewHeader *header = new ViewHeader;
    header->setTitle("!!network informations");

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
    // ESSID
    QGraphicsLinearLayout *essidContainerLayout = new QGraphicsLinearLayout(Qt::Vertical);
    MWidgetController *essidContainer = new MWidgetController();
    //essidContainer->setStyleName("CommonTextFrameInverted");
    essidContainer->setStyleName("CommonTextFrame");
    essidContainer->setContentsMargins(0, 0, 0, 0);
    essidContainer->setLayout(essidContainerLayout);

    MLabel *essidTitleLabel = new MLabel("SSID");
    //essidLabel->setStyleName("CommonTitleInverted");
    essidTitleLabel->setStyleName("CommonTitle");
    essidTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    MLabel *essidLabel = new MLabel(network->getEssid());
    //essidLabel->setStyleName("CommonSubTitleInverted");
    essidLabel->setStyleName("CommonSubTitle");
    essidLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    essidContainerLayout->addItem(essidTitleLabel);
    essidContainerLayout->addItem(essidLabel);
    viewportLayoutPolicy->addItem(essidContainer);

    // SIGNAL STRENGTH
    QGraphicsLinearLayout *signalStrengthContainerLayout = new QGraphicsLinearLayout(Qt::Vertical);
    MWidgetController *signalStrengthContainer = new MWidgetController();
    //signalStrengthContainer->setStyleName("CommonTextFrameInverted");
    signalStrengthContainer->setStyleName("CommonTextFrame");
    signalStrengthContainer->setContentsMargins(0, 0, 0, 0);
    signalStrengthContainer->setLayout(signalStrengthContainerLayout);

    MLabel *signalStrengthTitleLabel = new MLabel("Signal strength");
    //signalStrengthLabel->setStyleName("CommonTitleInverted");
    signalStrengthTitleLabel->setStyleName("CommonTitle");
    signalStrengthTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    MLabel *signalStrengthLabel = new MLabel(network->getSignalLevel());
    //signalStrengthLabel->setStyleName("CommonSubTitleInverted");
    signalStrengthLabel->setStyleName("CommonSubTitle");
    signalStrengthLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    signalStrengthContainerLayout->addItem(signalStrengthTitleLabel);
    signalStrengthContainerLayout->addItem(signalStrengthLabel);
    viewportLayoutPolicy->addItem(signalStrengthContainer);

    // ADDRESS
    QGraphicsLinearLayout *addressContainerLayout = new QGraphicsLinearLayout(Qt::Vertical);
    MWidgetController *addressContainer = new MWidgetController();
    //addressContainer->setStyleName("CommonTextFrameInverted");
    addressContainer->setStyleName("CommonTextFrame");
    addressContainer->setContentsMargins(0, 0, 0, 0);
    addressContainer->setLayout(addressContainerLayout);

    MLabel *addressTitleLabel = new MLabel("Address");
    //addressLabel->setStyleName("CommonTitleInverted");
    addressTitleLabel->setStyleName("CommonTitle");
    addressTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    MLabel *addressLabel = new MLabel(network->getAddress());
    //addressLabel->setStyleName("CommonSubTitleInverted");
    addressLabel->setStyleName("CommonSubTitle");
    addressLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    addressContainerLayout->addItem(addressTitleLabel);
    addressContainerLayout->addItem(addressLabel);
    viewportLayoutPolicy->addItem(addressContainer);

    // CHANNEL
    QGraphicsLinearLayout *channelContainerLayout = new QGraphicsLinearLayout(Qt::Vertical);
    MWidgetController *channelContainer = new MWidgetController();
    //channelContainer->setStyleName("CommonTextFrameInverted");
    channelContainer->setStyleName("CommonTextFrame");
    channelContainer->setContentsMargins(0, 0, 0, 0);
    channelContainer->setLayout(channelContainerLayout);

    MLabel *channelTitleLabel = new MLabel("Channel");
    //channelLabel->setStyleName("CommonTitleInverted");
    channelTitleLabel->setStyleName("CommonTitle");
    channelTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    MLabel *channelLabel = new MLabel(network->getChannel());
    //channelLabel->setStyleName("CommonSubTitleInverted");
    channelLabel->setStyleName("CommonSubTitle");
    channelLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    channelContainerLayout->addItem(channelTitleLabel);
    channelContainerLayout->addItem(channelLabel);
    viewportLayoutPolicy->addItem(channelContainer);

    // ENCRYPTION
    QGraphicsLinearLayout *encryptionContainerLayout = new QGraphicsLinearLayout(Qt::Vertical);
    MWidgetController *encryptionContainer = new MWidgetController();
    //encryptionContainer->setStyleName("CommonTextFrameInverted");
    encryptionContainer->setStyleName("CommonTextFrame");
    encryptionContainer->setContentsMargins(0, 0, 0, 0);
    encryptionContainer->setLayout(encryptionContainerLayout);

    MLabel *encryptionTitleLabel = new MLabel("Encryption");
    //encryptionLabel->setStyleName("CommonTitleInverted");
    encryptionTitleLabel->setStyleName("CommonTitle");
    encryptionTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    MLabel *encryptionLabel = new MLabel(network->getEncryption());
    //encryptionLabel->setStyleName("CommonSubTitleInverted");
    encryptionLabel->setStyleName("CommonSubTitle");
    encryptionLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    encryptionContainerLayout->addItem(encryptionTitleLabel);
    encryptionContainerLayout->addItem(encryptionLabel);
    viewportLayoutPolicy->addItem(encryptionContainer);

    viewportLayoutPolicy->addStretch();

    /////////////////////////////////////////////////// ACTIONS


    /////////////////////////////////////////////////// OBJECT MENU


    /////////////////////////////////////////////////// SIGNALS


    /////////////////////////////////////////////////// OTHER
}
