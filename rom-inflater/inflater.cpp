#include "inflater.h"

#include <QFile>

#include <iostream>

const int Inflater::RESULT_OK = 0;
const int Inflater::RESULT_ERROR = 1;
const int Inflater::RESULT_CANCELED = 2;


Inflater::Inflater(QObject *parent, QString directoryName, QString fileName): QThread(parent)
{
    this->directoryName = directoryName;
    this->fileName = fileName;

    QFile file(fileName);
    fileSize = (size_t)file.size();

    fileInflatedSize = 0;
    readBytes = 0;
    pastProgress = 0;
    canceled = false;
}


void Inflater::run()
{
    calculateInflatedSize();

    QString message;
    int result = RESULT_OK;

    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int r;

    a = archive_read_new();
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS);
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);

    if ((r = archive_read_open_filename(a, fileName.toLocal8Bit().constData(), 10240)) != ARCHIVE_OK)
    {
        std::cerr << "Inflater::" << __FUNCTION__ << " " << archive_error_string(a) << std::endl;
        result = RESULT_ERROR;
        message = QString(archive_error_string(a));
    }

    if(r == ARCHIVE_OK)
    {
        for (;;)
        {
            r = archive_read_next_header(a, &entry);
            if (r == ARCHIVE_EOF)
            {
                break;
            }
            if (r != ARCHIVE_OK)
            {
                std::cerr << "Inflater::" << __FUNCTION__ << " " << archive_error_string(a) << std::endl;
                result = RESULT_ERROR;
                message = QString(archive_error_string(a));
                break;
            }

            QString outputFileName = directoryName + "/" + QString(archive_entry_pathname(entry));
            archive_entry_set_pathname(entry, outputFileName.toLocal8Bit().constData());
            r = archive_write_header(ext, entry);
            if (r != ARCHIVE_OK)
            {
                std::cerr << "Inflater::" << __FUNCTION__ << " " << archive_error_string(a) << std::endl;
                result = RESULT_ERROR;
                message = QString(archive_error_string(a));
                break;
            }
            else
            {
                copyData(a, ext);
                r = archive_write_finish_entry(ext);
                if (r != ARCHIVE_OK)
                {
                    std::cerr << "Inflater::" << __FUNCTION__ << " " << archive_error_string(ext) << std::endl;
                    result = RESULT_ERROR;
                    message = QString(archive_error_string(ext));
                    break;
                }
            }

            // Cancels
            if(canceled)
            {
                result = RESULT_CANCELED;
                break;
            }
            //_____
        }
    }
    archive_read_close(a);
    archive_read_free(a);

    archive_write_close(ext);
    archive_write_free(ext);

    emit onFinished(result, message);
}

void Inflater::cancel()
{
    canceled = true;
}

bool Inflater::isCanceled()
{
    return canceled;
}

void Inflater::calculateInflatedSize()
{
    int r;
    struct archive *a;
    struct archive_entry *entry;

    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);

    if ((r = archive_read_open_filename(a, fileName.toLocal8Bit().constData(), 10240)) != ARCHIVE_OK)
    {
        std::cerr << "Inflater::" << __FUNCTION__ << " " << archive_error_string(a) << std::endl;
    }

    if(r == ARCHIVE_OK)
    {
        fileInflatedSize = 0;
        for (;;)
        {
            r = archive_read_next_header(a, &entry);
            if (r == ARCHIVE_EOF)
            {
                break;
            }
            if (r != ARCHIVE_OK)
            {
                break;
            }
            fileInflatedSize += archive_entry_size(entry);
        }
    }
    archive_read_close(a);
    archive_read_free(a);


    // To prevent a posible division by 0
    if(fileInflatedSize <= 0)
    {
        fileInflatedSize = fileSize * 2;
    }
    //_______
}

int Inflater::copyData(archive* ar, archive* aw)
{
    int r;
    const void *buff;
    size_t size;
#if ARCHIVE_VERSION_NUMBER >= 3000000
    int64_t offset;
#else
    off_t offset;
#endif

    for (;;) {
            r = archive_read_data_block(ar, &buff, &size, &offset);
            if (r == ARCHIVE_EOF)
                    return (ARCHIVE_OK);
            if (r != ARCHIVE_OK)
                    return (r);
            r = archive_write_data_block(aw, buff, size, offset);
            if (r != ARCHIVE_OK) {
                std::cerr << "Inflater::" << __FUNCTION__ << " " << archive_error_string(aw) << std::endl;
                return (r);
            }

            readBytes += size;
            if(readBytes > fileInflatedSize)
            {
                readBytes = fileInflatedSize;
            }
            int progress = ((double)readBytes / (double)fileInflatedSize) * 100.0;
            //std::cerr << "Inflater::" << __FUNCTION__ << " readBytes: " << readBytes << " progress: " << progress << std::endl;

            if(progress > pastProgress)
            {
                emit onProgress(progress);
                pastProgress = progress;
            }

            // Cancels
            if(canceled)
            {
                break;
            }
            //_____
    }
}
