#ifndef EDITORPAGE_H
#define EDITORPAGE_H

#include <QObject>
#include <MSheet>
#include <MRichTextEdit>
#include <MBasicSheetHeader>

class EditorPage : public MSheet
{
    Q_OBJECT
public:
    explicit EditorPage(QGraphicsItem *parent = 0);

    void loadFile(const QString& filePath, int currentRow);
    void setFocusOnEditor();

signals:
    void reloadModel(int oldRow);
    
public slots:

private slots:
    void processDialogRejected();
    void processDialogAccepted();
    void enableSaveButton();

private:
    QString getNewFilename();
    void writeToFile();

    QAction *saveAction;
    MRichTextEdit *editor;
    // Current filename
    QString fileName;
    int currentRow;
    
};

#endif // EDITORPAGE_H
