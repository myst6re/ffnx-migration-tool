#pragma once

#include <QString>
#include <QVector>

enum Bpp {
    Bpp0 = 0,
    Bpp1,
    Bpp2
};

struct Texture {
    QString ffnxPattern;
    QString tonberryPattern;
    QString remasterPattern;
    quint16 x, y, w, h;
    Bpp bpp;
};

class Data
{
public:
    Data();
    
    inline static const QVector<Texture> textures {
        {"battle/A0STG<id>.X-0_00", "a0/a0stg\\d+/a0stg(?<id>\\d+)_(?<vramPage>\\d+)", "battle.fs/hd_new/a0stg(?<id>\\d+)_(?<vramPage>\\d+)", 0, 256, 674, 256, Bpp::Bpp1}
    };
};
