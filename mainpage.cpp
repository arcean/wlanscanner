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
#include "viewHeader.h"
#include "editorpage.h"
#include "mlistitemcreator.h"
#include "aboutdialog.h"

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
    viewportLayoutPolicy->setObjectName("ListViewport");
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
    MAction *dummyAction = new MAction("", "", this);
    dummyAction->setLocation(MAction::ToolBarLocation);
    this->addAction(dummyAction);

    MAction *addNewNote = new MAction("icon-m-toolbar-add", "Add", this);
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
    connect(addNewNote, SIGNAL(triggered()), this, SLOT(showNewEditor()));
    connect(list, SIGNAL(itemLongTapped(QModelIndex)), this, SLOT(showObjectMenu(QModelIndex)));
    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(showEditor(QModelIndex)));
    connect(removeNote, SIGNAL(triggered()), this, SLOT(removeNoteSlot()));
    connect(aboutDialog, SIGNAL(triggered()), this, SLOT(showAboutDialog()));

    /////////////////////////////////////////////////// OTHER
    // Create info banner.
    infoBanner = new MBanner();
}

void MainPage::showAboutDialog()
{
    AboutDialog *aboutDialog = new AboutDialog();
    aboutDialog->appear(MSceneWindow::DestroyWhenDone);
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

void MainPage::throwMessage(const QString &text)
{
    infoBanner->setStyleName(MBannerType::InformationBanner);
    infoBanner->setTitle(text);
    infoBanner->appear(scene(), MSceneWindow::KeepWhenDone);
}
