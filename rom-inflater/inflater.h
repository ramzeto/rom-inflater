#ifndef INFLATER_H
#define INFLATER_H

#include <QThread>

#include <archive.h>
#include <archive_entry.h>

class Inflater : public QThread
{
public:
    static const int RESULT_OK;
    static const int RESULT_ERROR;
    static const int RESULT_CANCELED;

    QString directoryName;
    QString fileName;
    size_t fileSize;
    size_t fileInflatedSize;
    size_t readBytes;
    int pastProgress;
    bool canceled;


    Inflater(QObject *parent, QString directoryName, QString fileName);
    void cancel();
    bool isCanceled();

    Q_OBJECT
    void run() override;

signals:
    void onProgress(const int &progress);
    void onFinished(const int &result, const QString &message);

private:
    void calculateInflatedSize();
    int copyData(struct archive *ar, struct archive *aw);
};

#endif // INFLATER_H
