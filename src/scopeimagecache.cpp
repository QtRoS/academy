#include <QStandardPaths>
#include <QtGui/QImage>
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

using namespace std;

Q_LOGGING_CATEGORY(ImgCache, "ScopeImageCache")

ScopeImageCache::ScopeImageCache(const string &cacheDir):
    m_thread(&ScopeImageCache::threadProc, this)
{
    m_curl = curl_easy_init();
    m_cacheDir = QString::fromStdString(cacheDir);
}

ScopeImageCache::~ScopeImageCache()
{
    m_isTerminated = true;
    m_mutex.lock();
    m_ready.notify_one();
    m_mutex.unlock();
    m_thread.join();
    curl_easy_cleanup(m_curl);
}

string ScopeImageCache::getCached(const string &previewUrl, bool downloadOnMiss)
{
    lock_guard<mutex> lock(m_mutex);

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
        m_ready.notify_one();
    }

    return string("");
}

QString ScopeImageCache::cacheLocation() const
{
    static QString cacheLocation;

    if (cacheLocation.isEmpty())
    {
        cacheLocation = m_cacheDir + QStringLiteral("/Previews");

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
        unique_lock<mutex> lock(m_mutex);

        if (!m_queue.size())
        {
            qCDebug(ImgCache) << "Background thread wait...";
            m_ready.wait(lock, [this] { return m_queue.size();} );
        }

        if (m_isTerminated)
        {
            qCDebug(ImgCache) << "Background thread finished";
            return;
        }

        qCDebug(ImgCache) << "Background thread queue length:" << m_queue.size();
        QueueItem item = m_queue.dequeue();
        lock.unlock();
        downloadFile(item.url.toStdString(), item.localPath.toStdString());
        downscaleImage(item.localPath);
    }
}

void ScopeImageCache::downloadFile(const string &strurl, const string &fname) const
{
    if (m_curl)
    {
        FILE* fp = fopen(fname.c_str(), "wb");
        curl_easy_setopt(m_curl, CURLOPT_URL, strurl.c_str());
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, fwrite);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, fp);
        /*CURLcode res = */ curl_easy_perform(m_curl);
        fclose(fp);
    }
}

void ScopeImageCache::downscaleImage(const QString &fname) const
{
    QString fileName = fname; // QString::fromStdString(fname);
    QImage source(fileName);
    QImage scaled = source.width() > 500 ? source.scaledToWidth(500) : source; // TODO
    scaled.save(fileName, "PNG");
}
