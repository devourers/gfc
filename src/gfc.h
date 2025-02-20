#ifndef GFC_H_INCLUDED
#define GFC_H_INCLUDED

#include <QMainWindow>
#include <QFile>
#include <QString>
#include <QStringListModel>
#include <QFileInfo>
#include <QStandardItemModel>
#include <QFileSystemWatcher>
#include "ui_gfc.h"

#define FILE_NAME ".index.txt"


class DialogueIndex : public QObject {
  Q_OBJECT
    Q_DISABLE_COPY(DialogueIndex)

public:
  explicit DialogueIndex(QObject* parent = 0);
  ~DialogueIndex();
};

class CheckIndex : public QObject {
  Q_OBJECT
    Q_DISABLE_COPY(CheckIndex)

public:
  explicit CheckIndex(QObject* parent = 0);
  ~CheckIndex();
};

class LoreIndex : public QObject {
  Q_OBJECT
    Q_DISABLE_COPY(LoreIndex)

public:
  explicit LoreIndex(QObject* parent = 0);
  ~LoreIndex();
};

class LevelIndex : public QObject {

  Q_OBJECT
  Q_DISABLE_COPY(LevelIndex)

public:
    explicit LevelIndex(QObject* parent = 0);
    ~LevelIndex();

signals:
  void levelAdded(const QString& level_name);

private slots:


public:
  void SetRootDir(const QString& dir);
  void AddLevel(const QString& level_name);
  const QStringList& GetLevels();

private:
  QString root_dir_;
  QStringList levels_;
};

class GFC : public QMainWindow, public Ui::GFC {
  Q_OBJECT
  Q_DISABLE_COPY(GFC)

public:
  explicit GFC(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
  ~GFC();

  void SaveState() const;

private:
  void BuildIndex();
  void BindWatcher();

private slots:
  void onRootContentDirChanged(const QString& path);
  void onIndexFileChanged(const QString& path);
  void onLevelAdded(const QString& level_name);

  //overrides
  void on_LevelListView_currentChanged(QModelIndex current, QModelIndex previous);

private:
  //main
  LevelIndex* level_index_;

  QStringListModel* level_list_view_model_;
  QStandardItemModel* level_details_view_model_;

  QString content_dir_;
  QFileSystemWatcher* watcher_;
  QFile index_file;
  QString index_content;
  std::string debug;
};

#endif