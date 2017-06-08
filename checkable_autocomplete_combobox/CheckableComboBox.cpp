#include <QLineEdit>
#include <QTimer>

#include "CheckableComboBox.h"

void CheckableComboBox::SetFlags(CheckableComboBox::Flags flags)
{
  mFlags = flags;
}

CheckableComboBox::Flags CheckableComboBox::GetFlags() const
{
  return mFlags;
}

QList<QStandardItem *> CheckableComboBox::GetItems(Qt::CheckState state) const
{
  QList<QStandardItem *> items;

  for (int i=0; i<mModel->rowCount(); ++i) {
    auto item = mModel->item(i);
    if (item->data(Qt::CheckStateRole).toInt() & state)
      items << item;
  }

  return items;
}

int CheckableComboBox::GetItemCount(Qt::CheckState state) const
{
  int count = 0;
  for (int i=0; i<mModel->rowCount(); ++i) {
    auto item = mModel->item(i);
    if (item->data(Qt::CheckStateRole).toInt() & state)
      ++count;
  }

  return count;
}

int CheckableComboBox::ItemCount() const
{
  return mModel->rowCount();
}

void CheckableComboBox::AddItem(const QIcon& icon, const QString& text
                                , const QVariant& data, Qt::CheckState state, const QVariant &data2)
{
  int row = mModel->rowCount();
  QStandardItem* item = new QStandardItem();
  item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  item->setIcon(icon);
  item->setText(text);
  item->setData(data, Qt::UserRole + 1);
  item->setData(data2, Qt::UserRole + 2);
  item->setData(state, Qt::CheckStateRole);
  mModel->setItem(row, 0, item);
}

void CheckableComboBox::AddItem(const QString &text)
{
  AddItem(QIcon(), text, 0, Qt::Unchecked);
}

void CheckableComboBox::SetItemState(const QString &text, Qt::CheckState state)
{
  for (int i=0; i<mModel->rowCount(); ++i) {
    if (mModel->item(i)->text() == text) {
      auto item = mModel->item(i);
      item->setCheckState(state);
      break;
    }
  }
}

void CheckableComboBox::SetItemState(const QVariant &data, Qt::CheckState state)
{
  for (int i=0; i<mModel->rowCount(); ++i) {
    if (mModel->item(i)->data(Qt::UserRole + 1) == data) {
      auto item = mModel->item(i);
      item->setCheckState(state);
      break;
    }
  }
}

void CheckableComboBox::SetItemsState(Qt::CheckState state)
{
  for (int i=0; i<mModel->rowCount(); ++i)
    mModel->item(i)->setCheckState(state);
}

void CheckableComboBox::UpdateDisplayText()
{
  mDisplayText.clear();

  for (int i = 0; i < mModel->rowCount(); ++i) {
    if (mModel->item(i, 0)->checkState() == Qt::Checked) {
      mDisplayText = mDisplayText + mModel->item(i, 0)->text() + "; ";
    }
  }
  mDisplayText.chop(1); // remove last space

  lineEdit()->setToolTip(mDisplayText);

  QTimer::singleShot(0, this, SLOT(ShowDisplayText()));
}

void CheckableComboBox::ShowDisplayText()
{
  lineEdit()->setText(mDisplayText);
}


CheckableComboBox::CheckableComboBox(QWidget *parent)
  : QComboBox(parent)
  , mFlags(NoFlags)
  , mModel(new QStandardItemModel(this))
{
  setEditable(true);
  setModel(mModel);

  ShowDisplayText();

  connect(mModel, SIGNAL(itemChanged(QStandardItem*))
          ,SLOT(UpdateDisplayText()));
  connect(mModel, SIGNAL(itemChanged(QStandardItem*))
          ,SIGNAL(SelectionChanged()));
}

CheckableComboBox::~CheckableComboBox()
{
}
