#include "gfc.h"
#include <QSettings>
#include <QMessageBox>
#include <QlistView>
#include <QListWidget>

//level index

LevelIndex::LevelIndex(QObject* parent) : QObject(parent) {
  return;
}

LevelIndex::~LevelIndex(){
  return;
}

void LevelIndex::AddLevel(const QString& level_name) {
  levels_.push_back(level_name);
  
  //dialogue
  DialogueIndex* dg_idx = new DialogueIndex(this);
  dg_idx->SetLevel(level_name);
  dg_idx->BuildIndex();
  dialogue_indexes_[level_name] = dg_idx;
  QString dg_path = root_dir_ + level_name + "/Dialogue/" + FILE_NAME;
  //dialogue_indexes_files_.insert(level_name, QFile());
  //dialogue_indexes_files_[level_name].setFileName(dg_path);
  //dialogue_indexes_files_[level_name].open(QIODevice::Text | QIODevice::ReadOnly);

  watcher_->addPath(dg_path);

  InventoryIndex* inv_idx = new InventoryIndex(this);
  watcher_->addPath(root_dir_ + level_name + "/Inventory/" + FILE_NAME);

  emit levelAdded(level_name);
}

const QStringList& LevelIndex::GetLevels() {
  return levels_;
}

void LevelIndex::CreateWatcher(){
  QFileSystemWatcher* watcher = new QFileSystemWatcher(this);
  watcher_ = watcher;
  connect(watcher_, SIGNAL(fileChanged(const QString&)), this, SLOT(onIndexFileChanged(const QString&)));
}

void LevelIndex::SetRootDir(const QString& dir) {
  root_dir_ = dir;
}

void LevelIndex::onIndexFileChanged(const QString& path) {

}


//gfc
GFC::GFC(QWidget* parent, Qt::WindowFlags flags){
  setupUi(this);

  level_list_view_model_ = new QStringListModel(this);
  LevelListView->setModel(level_list_view_model_);

  QItemSelectionModel* selectionModel = LevelListView->selectionModel();

  connect(
    selectionModel,
    SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), 
    this, 
    SLOT(on_LevelListView_currentChanged(QModelIndex, QModelIndex))
  );

  level_details_view_model_ = new QStandardItemModel(this);
  level_details_view_model_->setVerticalHeaderLabels({ "Label", "Value" });
  LevelDetails->setModel(level_details_view_model_);

  QSettings settings("abc", "GFC");
  if (settings.contains("ContentDir")) {
    content_dir_ = settings.value("ContentDir").toString();
  }
  else {
    content_dir_ = QString("hehe"); //cfg file
  }
  //restoreGeometry(settings.value("myWidget/geometry").toByteArray());
  //restoreState(settings.value("myWidget/windowState").toByteArray());

  BindWatcher();

  index_file.setFileName(content_dir_ + FILE_NAME);
  bool test = index_file.open(QIODevice::Text | QIODevice::ReadOnly);

  BuildIndex();
}

void GFC::SaveState() const {
  QSettings settings("abc", "GFC");
  settings.setValue("ContentDir", content_dir_);
  //settings.setValue("geometry", saveGeometry());
  //settings.setValue("windowState", saveState());
}

void GFC::BuildIndex(){
  level_index_ = new LevelIndex();
  level_index_->CreateWatcher();

  connect(level_index_, SIGNAL(levelAdded(const QString&)), this, SLOT(onLevelAdded(const QString&)));
  level_index_->SetRootDir(content_dir_);
  while (!index_file.atEnd()) {
    level_index_->AddLevel(index_file.readLine());
  }

  level_list_view_model_->setStringList(level_index_->GetLevels());
}

void GFC::BindWatcher(){
  watcher_ = new QFileSystemWatcher(this);
  watcher_->addPath(content_dir_ + FILE_NAME);

  connect(watcher_, SIGNAL(fileChanged(const QString&)), this, SLOT(onIndexFileChanged(const QString&)));
}

GFC::~GFC() {
  SaveState();
}


void GFC::onRootContentDirChanged(const QString& path) {
  QMessageBox badIndex;
  QString messagebox_text = "dir changed";
  badIndex.setText(tr(messagebox_text.toStdString().c_str()));
  badIndex.exec();
}

void GFC::onIndexFileChanged(const QString& path) {
  if (path == content_dir_ + FILE_NAME) {
    QString level_name = index_file.readLine();
    level_index_->AddLevel(level_name);
  }
}

void GFC::onLevelAdded(const QString& level_name){
  level_list_view_model_->insertRow(level_list_view_model_->rowCount());
  QModelIndex idx = level_list_view_model_->index(level_list_view_model_->rowCount() - 1);
  level_list_view_model_->setData(idx, level_name);
}

void GFC::on_LevelListView_currentChanged(QModelIndex current, QModelIndex previous){
  level_details_view_model_->clear();
  QStandardItem* parent_item = level_details_view_model_->invisibleRootItem();

  //labels
  QStandardItem* level_name_label = new QStandardItem("Level Title");
  QStandardItem* chapter_label = new QStandardItem("Level Chapter");
  QStandardItem* num_of_actors = new QStandardItem("Total Actors");
  QStandardItem* num_of_inventory_entries = new QStandardItem("Total Inventory Entries");
  QStandardItem* num_of_dialogues = new QStandardItem("Total Dialogues");
  QStandardItem* num_of_note_entries = new QStandardItem("Total Notes");
  QStandardItem* num_of_lore_entries = new QStandardItem("Total Lore Entries"); //TODO -- lore should be it's own thing, this just signals which entry keys are granted on interaction

   //values
  QString level_name_str = level_list_view_model_->data(current).toString();
  level_name_str = level_name_str.left(level_name_str.length() - 1);
  QStandardItem* level_name = new QStandardItem(level_name_str);
  parent_item->appendColumn({ level_name_label, chapter_label, num_of_actors, num_of_inventory_entries, num_of_dialogues, num_of_note_entries, num_of_lore_entries });
  parent_item->appendColumn({ level_name });
  parent_item = level_name;
}

InventoryIndex::InventoryIndex(QObject* parent) : QObject(parent) {
  return;
}

InventoryIndex::~InventoryIndex(){
  return;
}

DialogueIndex::DialogueIndex(QObject* parent) : QObject(parent) {
  return;
}

DialogueIndex::~DialogueIndex(){
  return;
}

void DialogueIndex::SetLevel(const QString& level){
  level_ = level;
}

void DialogueIndex::BuildIndex(){
  QString idx_file = dir_ + level_ + "/" + FILE_NAME;

}
