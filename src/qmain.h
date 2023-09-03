#pragma once

#include <QObject>
#include <QUrl>

class QMain : public QObject {
    Q_OBJECT
public:
    QMain();
public slots:
    void runConvert(int sourceType, const QUrl &source, const QUrl &target);
};
