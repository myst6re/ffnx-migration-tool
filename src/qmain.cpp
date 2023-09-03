#include "qmain.h"
#include "core/converter.h"

#include <QDebug>

QMain::QMain()
{
    
}

void QMain::runConvert(int sourceType, const QUrl &source, const QUrl &target)
{
    if (source.isEmpty() || target.isEmpty()) {
        return;
    }

    qDebug() << "runConvert" << sourceType << source.toLocalFile() << target.toLocalFile();
    (new Converter())->convertDir(QDir(source.toLocalFile()), Converter::Source(sourceType), QDir(target.toLocalFile()));
}
