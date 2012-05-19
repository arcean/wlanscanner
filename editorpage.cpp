#include <MLayout>
#include <MPannableViewport>
#include <QGraphicsLinearLayout>
#include <MLinearLayoutPolicy>
#include <MLabel>
#include <MRichTextEdit>
#include <MAction>
#include <MBasicSheetHeader>
#include <QDateTime>
#include <QDir>
#include <QDebug>

#include "editorpage.h"
#include "viewHeader.h"

EditorPage::EditorPage(QGraphicsItem *parent)
{
    /* Sheet header */
    MBasicSheetHeader *sheetHeader = new MBasicSheetHeader;

    sheetHeader->setPositiveAction(new QAction("Save", sheetHeader));
    sheetHeader->setNegativeAction(new QAction("Cancel", sheetHeader));

    setHeaderWidget(sheetHeader);

    MWidget *centralWidget = new MWidget;
    MLayout *layout = new MLayout(centralWidget);

    MLinearLayoutPolicy* landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    layout->setPolicy(landscapePolicy);

    this->setCentralWidget(centralWidget);
    /////////////////////////////////////////////////// ACTIONS

    /////////////////////////////////////////////////// CONTENT
    editor = new MRichTextEdit(MTextEditModel::MultiLine);

    landscapePolicy->addItem(editor);
    /////////////////////////////////////////////////// SIGNALS
    connect(sheetHeader->negativeAction(), SIGNAL(triggered()), SLOT(processDialogRejected()));
    connect(sheetHeader->positiveAction(), SIGNAL(triggered()), SLOT(processDialogAccepted()));

    /////////////////////////////////////////////////// OTHER
}

void EditorPage::processDialogRejected()
{
    this->close();
}

void EditorPage::processDialogAccepted()
{
    qDebug() << "FN:" << getNewFilename();
    if (editor->text().length() > 0)
        writeToFile();

    this->close();
}

void EditorPage::loadFile(const QString& filePath, int currentRow)
{
    QFile file (filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[E]: could not open the file" << filePath;
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();

    while (!in.atEnd()) {
        line += in.readLine();
       // line += "\n";
    }

    editor->setHtml(line);
    this->fileName = filePath;
    this->currentRow = currentRow;
}

void EditorPage::writeToFile()
{
    QString name;

    if (fileName.length() <= 0)
        name = "/home/user/MyDocs/texteditor/" + getNewFilename();
    else
        name = fileName;

    QFile file(name);
    file.remove();

    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << editor->toHtml();
    file.close();

    if (fileName.length() <= 0)
        emit reloadModel(-1);
    else
        emit reloadModel(currentRow);
}

QString EditorPage::getNewFilename()
{
    QDir appDir("/home/user/MyDocs/texteditor/");
    bool ready = false;
    int counter = 0;
    QDateTime time = QDateTime::currentDateTime();
    QString baseName = time.toString("dd_mm_yy");

    if(!appDir.exists()) {
        appDir.mkdir("/home/user/MyDocs/texteditor/");
    }

    QDir::setCurrent("/home/user/MyDocs/texteditor/");
    QFile file;

    while (!ready) {
        if(counter > 0)
            file.setFileName(baseName + "(" + QString::number(counter) +").html");
        else
            file.setFileName(baseName + ".html");

        if(!file.exists()) {
            return file.fileName();
        }

        counter++;

        if(counter > 500000)
            ready = true;
    }

    qDebug() << "New filename" << file.fileName();
    return file.fileName();
}
