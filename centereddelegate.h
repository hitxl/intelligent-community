#ifndef CENTEREDDELEGATE_H
#define CENTEREDDELEGATE_H
#include <QStyledItemDelegate>
#include <Qt>

class CenteredDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:

    explicit CenteredDelegate(QObject *parent = nullptr)
        : QStyledItemDelegate(parent)
    {}
    void initStyleOption(QStyleOptionViewItem *option,
                         const QModelIndex &index) const override
    {

        QStyledItemDelegate::initStyleOption(option, index);

        option->displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
    }
};

#endif // CENTEREDDELEGATE_H
