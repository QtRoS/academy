#include <QStandardPaths>
#include "scopeimagecache.h"

Q_LOGGING_CATEGORY(ImgCache, "ScopeImageCache")

ScopeImageCache::ScopeImageCache(QObject *parent) :
    QObject(parent),
    m_isBusy(false),
    m_reply(0)
{ }

ScopeImageCache::~ScopeImageCache()
{ }

QString ScopeImageCache::getByPreview(const QString &preview,  bool downloadOnMiss)
{
    // --------------- First (and fastest) way - hash ---------------- //
    if (preview.isEmpty())
        return QString("");

    bool contains = m_hash.contains(preview);
    if (contains)
    {
        QString ret = m_hash[preview];
        if (!ret.isEmpty())
            return QStringLiteral("file:/") + cacheLocation() + QDir::separator() + ret;
    }

    // --------------- Second way - file                    ---------------- //

    QString md5 = QCryptographicHash::hash(preview.toLatin1(), QCryptographicHash::Md5).toHex();
    QString previewPath = cacheLocation() + QDir::separator() + md5;

    QFileInfo fi(previewPath);
    if (fi.exists() && fi.size() > 0 )
    {
        m_hash[preview] = md5;
        return QStringLiteral("file:/") + previewPath;
    }

    // --------------- Last way - network                   ---------------- //

    if (!contains && downloadOnMiss)
    {
        m_hash[preview] = QStringLiteral("");
        m_queue.enqueue(QueueItem(preview, previewPath));

        if (!m_isBusy)
            downloadNext();
    }

    return QString("");
}

void ScopeImageCache::slotDownloadDataAvailable()
{
    qint64 dataLen = m_reply->bytesAvailable();
    qint64 written = m_file.write(m_reply->read(dataLen));
    qCDebug(ImgCache) << "Downloading" << QFileInfo(m_file).fileName() << "available" << dataLen << "written" << written;
}

void ScopeImageCache::makeRequest(const QString &url)
{
    QUrl reqUrl(url);
    QNetworkRequest req(reqUrl);

    //req.setRawHeader("Authorization", "OAuth " + m_token.toLatin1());
    m_reply = m_manager.get(req);

    connect(m_reply, &QNetworkReply::readyRead, this, &ScopeImageCache::slotDownloadDataAvailable);
    connect(m_reply, &QNetworkReply::finished, this, &ScopeImageCache::slotFinished);
    connect(m_reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &ScopeImageCache::slotError);
}

void ScopeImageCache::downloadNext()
{
    if (m_queue.isEmpty())
    {
        setIsBusy(false);
        return;
    }

    qCDebug(ImgCache) << "Queue length:" << m_queue.length();

    setIsBusy(true);

    QueueItem item = m_queue.dequeue();

    m_file.setFileName(item.localPath);
    if (!m_file.open(QIODevice::Truncate | QIODevice::WriteOnly))
        qCWarning(ImgCache) << "File '" + item.localPath + "' can't be opened:" << m_file.errorString() << m_file.error();

    makeRequest(item.url);
}

QString ScopeImageCache::cacheLocation() const
{
    static QString cacheLocation;

    if (cacheLocation.isEmpty())
    {
        cacheLocation = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QStringLiteral("/Previews");
        //cacheLocation = QStringLiteral("/Previews");

        if (!QDir(cacheLocation).exists() && !QDir().mkdir(cacheLocation))
            qCCritical(ImgCache) << "Can't create directory for previews:" << cacheLocation;
        else qCDebug(ImgCache) << "Cache location:" << cacheLocation;
    }

    return cacheLocation;
}

void ScopeImageCache::slotError(QNetworkReply::NetworkError code)
{
    qCWarning(ImgCache) << "Download error:" << code << m_reply->errorString();
}

void ScopeImageCache::slotFinished()
{
    int status = m_reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
    QString location = m_reply->rawHeader("Location");

    m_reply->deleteLater();

    // Check if redirect.
    if (status / 100 == 3 || !location.isEmpty())
    {
        qCDebug(ImgCache) << "Redirected: " << location;
        makeRequest(location);
    }
    else
    {
        m_file.close();
        downloadNext();
    }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

QString ScopeImageCache::token() const
{
    return m_token;
}

bool ScopeImageCache::isBusy() const
{
    return m_isBusy;
}

void ScopeImageCache::setToken(const QString &t)
{
    m_token = t;
    Q_EMIT tokenChanged();
}

void ScopeImageCache::setIsBusy(bool v)
{
    m_isBusy = v;
    Q_EMIT isBusyChanged();
}
