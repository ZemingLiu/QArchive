#ifndef QARCHIVE_IO_READER_PRIVATE_HPP_INCLUDED
#define QARCHIVE_IO_READER_PRIVATE_HPP_INCLUDED
#include <QIODevice>
#include <QObject>
#include <QtGlobal>

namespace QArchive {
class IOReaderPrivate {
 public:
  IOReaderPrivate() = default;
  ~IOReaderPrivate();

  Q_DISABLE_COPY(IOReaderPrivate)

  void setBlockSize(int);
  void setIODevice(QIODevice*);

#if defined(__GNUC__) || defined(__clang__)
  [[gnu::warn_unused_result]]
#endif
  bool isOpen() const;
#if defined(__GNUC__) || defined(__clang__)
  [[gnu::warn_unused_result]]
#endif
  bool isReadable() const;
#if defined(__GNUC__) || defined(__clang__)
  [[gnu::warn_unused_result]]
#endif
  bool isSequential() const;

  qint64 read(char*);
  qint64 seek(qint64, /*whence=*/int);
  qint64 skip(qint64);

 private:
  int n_BlockSize = 10204; /* Default. */
  QIODevice* m_IODevice = nullptr;
};
}  // namespace QArchive

#endif  // QARCHIVE_IO_READER_PRIVATE_HPP_INCLUDED
