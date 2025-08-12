#ifndef CENTERED_DELEGATE_H
#define CENTERED_DELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QStyleOptionViewItem>

class CenteredDelegate1 : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CenteredDelegate1(QObject *parent = nullptr)
        : QStyledItemDelegate(parent)
    {}

    void initStyleOption(QStyleOptionViewItem *option,
                         const QModelIndex &index) const override
    {
        QStyledItemDelegate::initStyleOption(option, index);
        option->displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
    }
};

#endif // CENTERED_DELEGATE_H

