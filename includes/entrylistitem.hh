#ifndef ENTRYLISTITEM_HH
#define ENTRYLISTITEM_HH

#include <QObject>
#include <QWidget>
#include <QListWidgetItem>

class EntryListItem : public QWidget, public QListWidgetItem
{
    Q_OBJECT

public:
    explicit    EntryListItem();

    void        applyBorderRadius();
};

#endif // ENTRYLISTITEM_HH
