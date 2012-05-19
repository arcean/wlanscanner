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
#include "cell.h"
#include "viewHeader.h"
#include "editorpage.h"
#include "mlistitemcreator.h"

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
    theme->loadCSS("/opt/texteditor/style/texteditor.css");
    applicationWindow()->setStyleName("CommonApplicationWindow");
    setStyleName("CommonApplicationPage");
    MLayout *layout = new MLayout(this);

    /////////////////////////////////////////////////// HEADER VIEW

    /* Fix: lock page content */
    setPannable(false);
    MPannableViewport *viewport = this->pannableViewport();
    viewport->setAutoRange(false);
    viewport->setRange(QRectF(0,0,0,0));

    ViewHeader *header = new ViewHeader;
    header->setTitle("__AM__TXT_HTML_EDITOR__");

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
    MLinearLayoutPolicy *viewportLayoutPolicy = new MLinearLayoutPolicy(viewportLayout, Qt::Vertical);
    // Add header to the layout
    portraitPolicy->addItem(header);
    // Add viewportWidget to layout (main layout)
    portraitPolicy->addItem(viewportWidget);

    /////////////////////////////////////////////////// CONTENT

    list = new MList(this);
    MListItemCreator *cellCreator = new MListItemCreator;
    list->setCellCreator(cellCreator);
    model = new FileModel();
    list->setItemModel(model);

    viewportLayoutPolicy->addItem(list);

    /////////////////////////////////////////////////// ACTIONS
    // Toolbar
    MAction *addNewNote = new MAction("icon-s-common-add", "Add", this);
    addNewNote->setLocation(MAction::ToolBarLocation);
    this->addAction(addNewNote);

    /////////////////////////////////////////////////// OBJECT MENU
    objectMenu = new MObjectMenu(0);

    MAction *removeNote = new MAction("Remove", this);
    removeNote->setLocation(MAction::ObjectMenuLocation);
    objectMenu->addAction(removeNote);

    /////////////////////////////////////////////////// SIGNALS
    connect(addNewNote, SIGNAL(triggered()), this, SLOT(showNewEditor()));
    connect(list, SIGNAL(itemLongTapped(QModelIndex)), this, SLOT(showObjectMenu(QModelIndex)));
    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(showEditor(QModelIndex)));
    connect(removeNote, SIGNAL(triggered()), this, SLOT(removeNoteSlot()));

    /////////////////////////////////////////////////// OTHER
    // Create info banner.
    infoBanner = new MBanner();
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

void MainPage::removeNoteSlot()
{
    qDebug() << "B0";
    if(longTappedIndex.isValid()) {
        qDebug() << "Row about to be removed: " << longTappedIndex.row();
        QString filePath = model->getFilePath(longTappedIndex.row());
        QFile file(filePath);
        file.remove();

        //list->itemModel()->removeRow(longTappedIndex.row(), longTappedIndex.parent());
        qDebug() << "B1";
        list->itemModel()->removeRows(longTappedIndex.row(), 1, longTappedIndex.parent());
        qDebug() << "B2";
        longTappedIndex = QModelIndex();
        qDebug() << "B3";
    }
    qDebug() << "B4";
}

void MainPage::showEditor(const QModelIndex& index)
{
    QString filePath = model->getFilePath(index.row());
    qDebug() << "filePath" << filePath;

    EditorPage *editor = new EditorPage();
    editor->loadFile(filePath, index.row());

    connect(editor, SIGNAL(reloadModel(int)), this, SLOT(reloadModel(int)));
    editor->appear(MSceneWindow::DestroyWhenDismissed);
}

void MainPage::showNewEditor()
{
    EditorPage *editor = new EditorPage();
    connect(editor, SIGNAL(reloadModel(int)), this, SLOT(reloadModel(int)));
    editor->appear(MSceneWindow::DestroyWhenDismissed);
}

void MainPage::showQueryDialog()
{
    MMessageBox *messageBox = new MMessageBox("", M::YesButton|M::NoButton);

    messageBox->setTitle(qtTrId("Are you sure?"));

    messageBox->setText(qtTrId("Are you sure that you want to <b>uninstall</b> system modification? Your device will be <b><font color=\"#6604BD\">rebooted</font></b>."));
    QPixmap icon("/opt/videozoom/data/dialog-question.png");
    messageBox->setIconPixmap(icon);

    messageBox->appear(MSceneWindow::DestroyWhenDone);
}

void MainPage::showAboutDialog()
{
    MDialog *dialog = new MDialog("", M::CloseButton);

    const QChar CopyrightSymbol(0x00a9);

    QString text(qtTrId("Video zoom modification for MeeGo 1.2 Harmattan.\n"));
    QString copyright_string(QString(CopyrightSymbol) + QString::fromUtf8(" 2012 Tomasz Pieniążek"));

    MLabel *textSystemModal= new MLabel(text + copyright_string);
    textSystemModal->setStyleName("CommonBodyTextInverted");
    textSystemModal->setAlignment(Qt::AlignCenter);
    textSystemModal->setWordWrap(true);
    textSystemModal->setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    MLabel *title= new MLabel(qtTrId("Video zoom 1.0"));
    title->setStyleName("title_label");
    title->setAlignment(Qt::AlignCenter);
    title->setWordWrap(true);
    title->setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    QGraphicsLinearLayout *layoutDesc = new QGraphicsLinearLayout(Qt::Horizontal);
    layoutDesc->addStretch();
    layoutDesc->addItem(textSystemModal);
    layoutDesc->addStretch();

    QGraphicsLinearLayout *layoutTitle = new QGraphicsLinearLayout(Qt::Horizontal);
    layoutTitle->addStretch();
    layoutTitle->addItem(title);
    layoutTitle->addStretch();

    MImageWidget *image = new MImageWidget(new QPixmap("/usr/share/icons/hicolor/80x80/apps/videozoom.png"));

    QGraphicsLinearLayout *layout1 = new QGraphicsLinearLayout(Qt::Vertical);
    layout1->addItem(image);
    layout1->addItem(layoutTitle);
    layout1->addItem(layoutDesc);
    layout1->addStretch();

    dialog->centralWidget()->setLayout(layout1);
    dialog->setObjectName("about_dialog");

    dialog->appear(MSceneWindow::DestroyWhenDone);
}

void MainPage::throwMessage(const QString &text)
{
    infoBanner->setStyleName(MBannerType::InformationBanner);
    infoBanner->setTitle(text);
    infoBanner->appear(scene(), MSceneWindow::KeepWhenDone);
}
