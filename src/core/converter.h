#pragma once

#include <QString>
#include <QStringList>
#include <QDir>
#include "data.h"

class Converter
{
public:
    enum Source {
        Tonberry,
        Remaster
    };

    Converter();
    bool convertDir(const QDir &sourceDir, Source sourceType,
                    const QDir &targetDir);
    static QStringList fileList(const QDir &sourceDir);
private:
    QPair<const Texture *, QRegularExpressionMatch> matchTexture(
        const QString &fileName, Source sourceType);
    static const QString &texturePattern(const Texture &texture,
                                         Source sourceType);
    static QString ffnxPath(const Texture &texture,
                            const QRegularExpressionMatch &match);
    static void copyTexture(const Texture &texture,
                            const QRegularExpressionMatch &match,
                            const QDir &sourceDir, const QDir &targetDir);
    static void packVramTextures(
        const QMultiMap<QString, QPair<const Texture *, QRegularExpressionMatch> > &texturesToPack,
        const QDir &sourceDir, const QDir &targetDir);
    static QImage packVramTexture(
        const Texture &texture, const QList<QRegularExpressionMatch> &matches,
        const QDir &sourceDir);
};
