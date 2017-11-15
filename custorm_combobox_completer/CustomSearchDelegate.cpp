#include <QDebug>

#include "CustomSearchDelegate.h"


void CustomSearchDelegate::paint(QPainter* painter,
                                 const QStyleOptionViewItem& option,
                                 const QModelIndex& index) const
{
  if (index.isValid()) {

    QFont fontNormal = option.font;
    QFontMetrics fontNormalMetrics(fontNormal);

    QFont fontBold = option.font;
    fontBold.setBold(true);
    QFontMetrics fontBoldMetrics(fontBold);

    painter->save();

//    if (option.state & QStyle::State_Selected)
//      painter->fillRect(option.rect, option.palette.highlight());

    QString txt = index.data().toString();
    QString fnd = mLineEdit->text();

    int ind = txt.indexOf(fnd);

    QString firstPart = txt.mid(0, ind);
    QString boldPart = txt.mid(ind, fnd.size());
    QString lastPart = txt.mid(ind + fnd.size());


    auto r1 = fontNormalMetrics.boundingRect(firstPart);
    QRect r11(QPoint(r1.width()/* + 1*/, option.rect.y()),
              QPoint(option.rect.width()/* - r1.width()*/, option.rect.height()));
//    QRect r11 = option.rect;
    painter->setFont(fontNormal);
    painter->drawText(r11, firstPart);


    auto r2 = fontBoldMetrics.boundingRect(boldPart);
    QRect r22(QPoint(r1.width() + 1, option.rect.y()),
              QPoint(option.rect.width()/* - r1.width()*/, option.rect.height()));
    painter->setFont(fontBold);
    painter->drawText(r22, boldPart);


    QRect r33(QPoint(r1.width() + r2.width() + 1, option.rect.y()),
              QPoint(option.rect.width() /*- r1.width()*/ /*- r2.width()*/, option.rect.height()));
    painter->setFont(fontNormal);
    painter->drawText(r33, lastPart);


    qDebug() << firstPart << boldPart << lastPart;
//    qDebug() << ">>" << txt.size() << fnd.size() << ind;
//    qDebug() << r1 << r2;
//    qDebug() << option.rect << r11 << r22 << r33;
    qDebug()
        << "option" << option.rect << "\n"
        << "r1" << r1 << "\n"
        << "r11" << r11 << "\n"
        << "r2" << r2 << "\n"
        << "r22" << r22 << "\n"
        << "r33" << r33;

    painter->restore();

  } else {
    QStyledItemDelegate::paint(painter, option, index);
  }
}

QSize CustomSearchDelegate::sizeHint(const QStyleOptionViewItem& option,
                                     const QModelIndex& index) const
{
  if (index.isValid()) {
    QFont font = option.font;
    font.setBold(true);
    QFontMetrics metrics(font);
    return metrics.boundingRect(index.data().toString()).size();
  } else {
    return QStyledItemDelegate::sizeHint(option, index);
  }
}
