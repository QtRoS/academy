#ifndef SCOPEIMAGECACHE_H
#define SCOPEIMAGECACHE_H

#include <QQueue>
#include <QCryptographicHash>
#include <QFile>
#include <QHash>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QLoggingCategory>
#include <QMutex>
#include <QMutexLocker>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <core/net/http/request.h>
#include <core/net/uri.h>

#include <string>
#include <thread>
#include <atomic>

#include <stdio.h>
#include <curl/curl.h>

using namespace std;

Q_DECLARE_LOGGING_CATEGORY(ImgCache)

struct QueueItem
{
public:
    QueueItem(const QString& u, const QString& p)
    {
        url = u;
        localPath = p;
    }

    QString url;
    QString localPath;
};

class ScopeImageCache
{
public:
    ScopeImageCache();
    ~ScopeImageCache();

    string getByPreview(const string& preview, bool downloadOnMiss = true);

private:
    void threadProc();
    QMutex m_lock;
    std::atomic<bool> m_isTerminated;
    CURL* m_curl;
    void downloadFile(const string& strurl , const string& fname);

private:
    QString cacheLocation() const;

    QHash<QString, QString> m_hash; // <PreviewUrl, MD5>
    QQueue<QueueItem> m_queue;
    thread m_thread;
};



#endif // SCOPEIMAGECACHE_H
