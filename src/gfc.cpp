#include "gfc.h"
#include <QSettings>
#include <QMessageBox>

GFC::GFC(QWidget* parent, Qt::WindowFlags flags){
  setupUi(this);
  QSettings settings("abc", "GFC");
  if (settings.contains("content_dir_")) {
    content_dir_ = settings.value("content_dir_").toString();
  }
  else {
    content_dir_ = QString("hehe"); //add button
  }
  watcher_ = new QFileSystemWatcher(this);
  //watcher_->addPath(content_dir_);
  watcher_->addPath(content_dir_ + "/.index");
  connect(watcher_, SIGNAL(directoryChanged(const QString&)), this, SLOT(onRootContentDirChanged(const QString&)));
  connect(watcher_, SIGNAL(fileChanged(const QString&)), this, SLOT(onIndexFileChanged(const QString&)));
  //restoreGeometry(settings.value("myWidget/geometry").toByteArray());
  //restoreState(settings.value("myWidget/windowState").toByteArray());
}

void GFC::SaveState() const {
  QSettings settings("abc", "GFC");
  settings.setValue("ContentDir", content_dir_);
  //settings.setValue("geometry", saveGeometry());
  //settings.setValue("windowState", saveState());
}

GFC::~GFC() {

}


void GFC::onRootContentDirChanged(const QString& path) {
  QMessageBox badIndex;
  QString messagebox_text = "dir changed";
  badIndex.setText(tr(messagebox_text.toStdString().c_str()));
  badIndex.exec();
}

void GFC::onIndexFileChanged(const QString& path) {
  QMessageBox badIndex;
  QString messagebox_text = "index changed";
  badIndex.setText(tr(messagebox_text.toStdString().c_str()));
  badIndex.exec();
}