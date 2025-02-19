#ifndef GFC_H_INCLUDED
#define GFC_H_INCLUDED

#include <QMainWindow>
#include <QFile>
#include <QFileSystemWatcher>
#include "ui_gfc.h"


class GFC : public QMainWindow, public Ui::GFC {
  Q_OBJECT
  Q_DISABLE_COPY(GFC)

public:
  explicit GFC(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
  ~GFC();

  void SaveState() const;

private slots:
  void onRootContentDirChanged(const QString& path);
  void onIndexFileChanged(const QString& path);

private:
  QString content_dir_;
  QFileSystemWatcher* watcher_;

};

#endif