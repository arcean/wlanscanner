#ifndef EDITORPAGE_H
#define EDITORPAGE_H

#include <QObject>
#include <MSheet>
#include <MRichTextEdit>

class EditorPage : public MSheet
{
    Q_OBJECT
public:
    explicit EditorPage(QGraphicsItem *parent = 0);

    void loadFile(const QString& filePath, int currentRow);

signals:
    void reloadModel(int oldRow);
    
public slots:

private slots:
    void processDialogRejected();
    void processDialogAccepted();

private:
    QString getNewFilename();
    void writeToFile();

    MRichTextEdit *editor;
    // Current filename
    QString fileName;
    int currentRow;
    
};

#endif // EDITORPAGE_H
