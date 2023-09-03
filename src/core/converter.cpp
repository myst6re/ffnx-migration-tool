#include "converter.h"
#include <QRegularExpression>
#include <QImage>
#include <QPainter>

Converter::Converter()
{
    
}

bool Converter::convertDir(const QDir &sourceDir, Source sourceType,
                           const QDir &targetDir)
{
    QMultiMap<QString, QPair<const Texture *, QRegularExpressionMatch> > texturesToPack;
    qDebug() << "Converter::convertDir" << sourceDir << sourceType << targetDir;
    QStringList textureFiles = fileList(sourceDir);
    for (const QString &path: textureFiles) {
        QPair<const Texture *, QRegularExpressionMatch> match = matchTexture(path, sourceType);
        
        if (match.first == nullptr) {
            qWarning() << "Converter::convertDir" << path << "no match";

            continue; // FIXME: warning
        }
        
        QString id = match.second.captured("id"),
            vramPage = match.second.captured("vramPage");
        
        if (vramPage.isEmpty()) {
            copyTexture(*match.first, match.second, sourceDir, targetDir);
        } else {
            texturesToPack.insert(QString("%1-%2").arg(uintptr_t(match.first)).arg(id), match);
        }
    }
    
    packVramTextures(texturesToPack, sourceDir, targetDir);
    
    return true;
}

QPair<const Texture *, QRegularExpressionMatch> Converter::matchTexture(
    const QString &fileName, Source sourceType)
{
    for (const Texture &texture: qAsConst(Data::textures)) {
        const QString &pattern = texturePattern(texture, sourceType);
        QRegularExpression regExp(pattern + "\\.(?<ext>.*)$");
        
        QRegularExpressionMatch match = regExp.match(fileName);
        if (match.hasMatch()) {
            qDebug() << match.capturedTexts();
            
            return qMakePair(&texture, match);
        }
    }
    
    return qMakePair(nullptr, QRegularExpressionMatch());
}

QStringList Converter::fileList(const QDir &sourceDir)
{
    qDebug() << "Converter::fileList" << sourceDir;
    QStringList filters = {
        "*.png", "*.dds"
    };
    QFileInfoList list = sourceDir.entryInfoList(
        filters,
        QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    QStringList ret;

    for (const QFileInfo &fileInfo: list) {
        qDebug() << fileInfo;
        if (fileInfo.isDir()) {
            ret.append(fileList(QDir(fileInfo.filePath())));
        } else {
            ret.append(sourceDir.filePath(fileInfo.fileName()));
        }
    }

    return ret;
}

const QString &Converter::texturePattern(const Texture &texture,
                                         Source sourceType)
{
    switch (sourceType) {
    case Tonberry:
        return texture.tonberryPattern;
    case Remaster:
        return texture.remasterPattern;
    }
    return texture.ffnxPattern;
}

QString Converter::ffnxPath(const Texture &texture,
                            const QRegularExpressionMatch &match)
{
    QString targetFileName = texture.ffnxPattern;

    return targetFileName
        .replace("<id>", match.captured("id"))
        .append('.').append(match.captured("ext"));
}

void Converter::copyTexture(const Texture &texture,
                            const QRegularExpressionMatch &match,
                            const QDir &sourceDir, const QDir &targetDir)
{
    QString modFileName = sourceDir.absoluteFilePath(match.captured()),
        ffnxFileName = targetDir.absoluteFilePath(ffnxPath(texture, match));
    targetDir.mkpath(QFileInfo(ffnxFileName).dir().path());
    qDebug() << "cp" << modFileName << "to" << ffnxFileName;
    QFile::copy(modFileName, ffnxFileName);
}

void Converter::packVramTextures(
    const QMultiMap<QString, QPair<const Texture *, QRegularExpressionMatch> > &texturesToPack,
    const QDir &sourceDir, const QDir &targetDir)
{
    for (const QString &key: texturesToPack.uniqueKeys()) {
        qDebug() << "Converter::packVramTextures" << "key" << key;
        QList<QPair<const Texture *, QRegularExpressionMatch> > vramPages = texturesToPack.values(key);
        QList<QRegularExpressionMatch> matches;
        for (const QPair<const Texture *, QRegularExpressionMatch> &vramPage: qAsConst(vramPages)) {
            matches.append(vramPage.second);
        }
        const Texture *texture = vramPages.first().first;
        QString ffnxFileName = targetDir.absoluteFilePath(ffnxPath(*texture, matches.first()));
        
        qDebug() << "save packed texture to" << ffnxFileName;
        QImage tex = packVramTexture(*texture, matches, sourceDir);
        targetDir.mkpath(QFileInfo(ffnxFileName).dir().path());
        tex.save(ffnxFileName);
    }
}

QImage Converter::packVramTexture(const Texture &texture,
                                  const QList<QRegularExpressionMatch> &matches,
                                  const QDir &sourceDir)
{
    QImage ret;
    QPainter *p = nullptr;

    for (const QRegularExpressionMatch &match: qAsConst(matches)) {
        int vramPage = match.captured("vramPage").toInt();
        QImage img(sourceDir.absoluteFilePath(match.captured()));
        int ratio = img.height() / 256;

        if (p == nullptr) {
            ret = QImage(texture.w * ratio, texture.h * ratio, QImage::Format_ARGB32_Premultiplied);
            ret.fill(qRgba(0, 0, 0, 0));
            p = new QPainter(&ret);
        }
        qDebug() << "ratio" << ratio << "vramId" << vramPage << "x" << (vramPage % 16) << ((vramPage % 16) * 128 - texture.x) << "y" << (vramPage / 16) << ((vramPage / 16) * 256 - texture.y);
        
        p->drawImage(((vramPage % 16) * 128 - texture.x) * ratio, ((vramPage / 16) * 256 - texture.y) * ratio, img);
    }

    if (p != nullptr) {
        p->end();
        delete p;
    }

    return ret;
}
