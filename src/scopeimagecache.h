#ifndef SCOPEIMAGECACHE_H
#define SCOPEIMAGECACHE_H

#include <QQueue>
#include <QCryptographicHash>
#include <QFile>
#include <QHash>
#include <QFileInfo>
#include <QDir>
#include <QLoggingCategory>

#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

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

    string getCached(const string& preview, bool downloadOnMiss = true);

private:
    void threadProc();
    std::mutex m_mutex;
    std::thread m_thread;
    std::atomic<bool> m_isTerminated;
    std::condition_variable m_ready;

private:
    CURL* m_curl;
    void downloadFile(const string& strurl , const string& fname) const;

private:
    QString cacheLocation() const;
    QHash<QString, QString> m_hash; // <PreviewUrl, MD5>
    QQueue<QueueItem> m_queue;
};



#endif // SCOPEIMAGECACHE_H
