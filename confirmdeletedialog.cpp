#include <MButton>

#include "confirmdeletedialog.h"

ConfirmDeleteDialog::ConfirmDeleteDialog()
{
    MButtonModel *buttonModel = button(M::OkButton);

    if (buttonModel)
        removeButton(buttonModel);

    this->addButton(M::YesButton);
    this->addButton(M::NoButton);


    setTitle(qtTrId("Are you sure?"));
    setText(qtTrId("Are you sure that you want to delete this note?"));

    QPixmap icon("/opt/texteditor/data/dialog-question.png");
    setIconPixmap(icon);
}
