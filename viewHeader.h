#ifndef HEADER_H
#define HEADER_H

class ViewHeader : public MWidgetController
{
public:
    ViewHeader(QGraphicsItem *parent = 0) :
        MWidgetController(parent),
        linearLayout(0),
        titleWidget(0)
    {
        setStyleName("CommonHeaderPanel");
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        linearLayout = new QGraphicsLinearLayout(Qt::Horizontal, this);
        titleWidget = new MLabel();
        titleWidget->setTextElide(true);
        titleWidget->setStyleName("CommonHeaderInverted");
        linearLayout->addItem(titleWidget);
    }

    void setTitle(const QString &title)
    {
        titleWidget->setText(title);
    }

private:
    QGraphicsLinearLayout *linearLayout;
    MLabel *titleWidget;

};

#endif // HEADER_H
