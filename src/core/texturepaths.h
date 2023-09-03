#pragma once

#include <QStringList>

class TexturePaths
{
public:
    TexturePaths(const QStringList &paths);
    TexturePaths fromDirName(const QString &dirName);
private:
    QStringList _paths;
};
