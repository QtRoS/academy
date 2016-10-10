#include <QStandardPaths>
#include "scopeimagecache.h"

#include <chrono>
#include <thread>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <core/net/http/request.h>
#include <core/net/uri.h>

namespace http = core::net::http;
namespace net = core::net;

Q_LOGGING_CATEGORY(ImgCache, "ScopeImageCache")

ScopeImageCache::ScopeImageCache():
    m_thread(&ScopeImageCache::threadProc, this),
    m_isTerminated(false)
{
    m_curl = curl_easy_init();
}

ScopeImageCache::~ScopeImageCache()
{
    m_isTerminated = true;
    // TODO
    m_thread.join();
    curl_easy_cleanup(m_curl);
}

string ScopeImageCache::getByPreview(const string &previewUrl,  bool downloadOnMiss)
{
    QMutexLocker lock(&m_lock);

    // --------------- First (and fastest) way - hash ---------------- //
    QString preview = QString::fromStdString(previewUrl);

    if (preview.isEmpty())
        return string("");

    bool contains = m_hash.contains(preview);
    if (contains)
    {
        QString ret = m_hash[preview];
        if (!ret.isEmpty())
            return (QStringLiteral("file:/") + cacheLocation() + QDir::separator() + ret).toStdString();
    }

    // --------------- Second way - file                    ---------------- //

    QString md5 = QCryptographicHash::hash(preview.toLatin1(), QCryptographicHash::Md5).toHex();
    QString previewPath = cacheLocation() + QDir::separator() + md5;

    QFileInfo fi(previewPath);
    if (fi.exists() && fi.size() > 0 )
    {
        m_hash[preview] = md5;
        return (QStringLiteral("file:/") + previewPath).toStdString();
    }

    // --------------- Last way - network                   ---------------- //

    if (!contains && downloadOnMiss)
    {
        m_hash[preview] = QStringLiteral("");
        m_queue.enqueue(QueueItem(preview, previewPath));
    }

    return string("");
}

QString ScopeImageCache::cacheLocation() const
{
    static QString cacheLocation;

    if (cacheLocation.isEmpty())
    {
        cacheLocation = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QStringLiteral("/Previews");

        if (!QDir(cacheLocation).exists() && !QDir().mkdir(cacheLocation))
            qCCritical(ImgCache) << "Can't create directory for previews:" << cacheLocation;
        else qCDebug(ImgCache) << "Cache location:" << cacheLocation;
    }

    return cacheLocation;
}

void ScopeImageCache::threadProc()
{
    qCDebug(ImgCache) << "Background thread started...";
    while(true)
    {
        if (m_isTerminated)
            return;

        m_lock.lock();
        if (!m_queue.size())
        {
            m_lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        qCDebug(ImgCache) << "Queue length:" << m_queue.size() << &m_queue;
        QueueItem item = m_queue.dequeue();
        m_lock.unlock();
        downloadFile(item.url.toStdString(), item.localPath.toStdString());
    }
}

void ScopeImageCache::downloadFile(const string &strurl, const string &fname)
{
    if (m_curl)
    {
        FILE* fp = fopen(fname.c_str(), "wb");
        curl_easy_setopt(m_curl, CURLOPT_URL, strurl.c_str());
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, fwrite);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, fp);
        /*CURLcode res = */ curl_easy_perform(m_curl);
        curl_easy_cleanup(m_curl);
        fclose(fp);
    }
}
