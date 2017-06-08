#pragma once

#include <QVariant>
#include <QString>
#include <QStringList>
#include <QIcon>
#include <QComboBox>
#include <QStandardItemModel>
#include <QStandardItem>


class CheckableComboBox : public QComboBox
{
  Q_OBJECT

public:
  enum Flags {
    NoFlags,
    ShowTooltip,
  };

private:
  Flags mFlags;
  QStandardItemModel* mModel;
  QString mDisplayText;

public:
  void SetFlags(Flags flags);
  Flags GetFlags() const;
  QList<QStandardItem*> GetItems(Qt::CheckState state) const;
  int GetItemCount(Qt::CheckState state) const;
  int ItemCount() const;
  void AddItem(const QIcon &icon, const QString &text
               , const QVariant &data, Qt::CheckState state = Qt::Unchecked
               , const QVariant& data2 = QVariant());
  void AddItem(const QString &text);
  void SetItemState(const QString& text, Qt::CheckState state);
  void SetItemState(const QVariant& data, Qt::CheckState state);
  void SetItemsState(Qt::CheckState state);

public slots:
  void UpdateDisplayText();
  void ShowDisplayText();

signals:
  void SelectionChanged();


public:
  explicit CheckableComboBox(QWidget* parent = nullptr);
  virtual ~CheckableComboBox();
};
