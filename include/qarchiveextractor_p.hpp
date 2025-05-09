#ifndef QARCHIVE_EXTRACTOR_PRIVATE_HPP_INCLUDED
#define QARCHIVE_EXTRACTOR_PRIVATE_HPP_INCLUDED
#include <QBuffer>
#include <QDir>
#include <QEventLoop>
#include <QIODevice>
#include <QJsonObject>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <QVector>

#include <memory>

#include "qarchivememoryextractoroutput.hpp"
#include "qarchiveutils_p.hpp"

namespace QArchive {

class ArchiveFilter;

class MutableMemoryFile {
 public:
  void setFileInformation(QJsonObject);
  void setBuffer(QBuffer*);

#if defined(__GNUC__) || defined(__clang__)
  [[gnu::warn_unused_result]]
#endif
  QJsonObject getFileInformation() const;
#if defined(__GNUC__) || defined(__clang__)
  [[gnu::warn_unused_result]]
#endif
  std::shared_ptr<QBuffer> getBuffer() const;

 private:
  QJsonObject m_FileInformation;
  std::shared_ptr<QBuffer> m_Buffer;
};

class ExtractorPrivate : public QObject {
  Q_OBJECT
 public:
  Q_DISABLE_COPY(ExtractorPrivate)
  explicit ExtractorPrivate(bool memoryMode = false);
  ~ExtractorPrivate() override;
 public Q_SLOTS:
  void setArchive(QIODevice*);
  void setArchive(const QString&);
  void setBlockSize(int);
  void setCalculateProgress(bool);
  void setOutputDirectory(const QString&);
  void setPassword(const QString&);
  void addFilter(const QString&);
  void addFilter(const QStringList&);
  void addIncludePattern(const QString&);
  void addIncludePattern(const QStringList&);
  void addExcludePattern(const QString&);
  void addExcludePattern(const QStringList&);
  void setBasePath(const QString&);
  void setRawMode(bool);
  void setRawOutputFilename(const QString&);
  void clear();

  void getInfo();

  void start();
  void cancel();
  void pause();
  void resume();

 private Q_SLOTS:
  short openArchive();
  short checkOutputDirectory();
  short getTotalEntriesCount();
  short processArchiveInformation();
  short writeData(struct archive_entry*);
  short extract();
  void toggleArchiveFormat(struct archive*);

 Q_SIGNALS:
  void started();
  void canceled();
  void paused();
  void resumed();
  void diskFinished();
  void memoryFinished(MemoryExtractorOutput*);
  void error(short);
  void info(QJsonObject);
  void progress(QString, int, int, qint64, qint64);
  void getInfoRequirePassword(int);
  void extractionRequirePassword(int);

 private:
  bool b_MemoryMode = false;
  bool b_RawMode = false;
  bool b_ProcessingArchive = false;
  bool b_StartRequested = false;

  bool b_PauseRequested = false;
  bool b_CancelRequested = false;
  bool b_Paused = false;
  bool b_Started = false;
  bool b_Finished = false;
  bool b_NoProgress = true;
  bool b_ArchiveOpened = false;
  bool b_QIODeviceOwned = false;  // If set, free m_Archive on clear()
  int n_PasswordTriedCountGetInfo = 0;
  int n_PasswordTriedCountExtract = 0;
  int n_ProcessedEntries = 0;
  int n_TotalEntries = -1;
  int n_BlockSize = 10240;
  int n_Flags = 0;
  qint64 n_BytesProcessed = 0;
  qint64 n_BytesTotal = 0;

  QString m_OutputDirectory;
  QString m_Password;
  QString m_ArchivePath;
  QString m_RawOutputFilename;
  QIODevice* m_Archive = nullptr;
  archive_entry* m_CurrentArchiveEntry = nullptr;
  MutableMemoryFile m_CurrentMemoryFile;
  QSharedPointer<struct archive> m_ArchiveRead;
  QSharedPointer<struct archive> m_ArchiveWrite;
  QStringList m_ExtractFilters;
  QJsonObject m_Info;
  std::unique_ptr<QVector<MemoryFile>> m_ExtractedFiles;
  std::unique_ptr<ArchiveFilter> m_archiveFilter;
  bool b_hasBasePath = false;
  QDir m_basePath;
};
}  // namespace QArchive
#endif  // QARCHIVE_EXTRACTOR_PRIVATE_HPP_INCLUDED
