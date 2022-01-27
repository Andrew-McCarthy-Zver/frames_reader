#ifndef FRAME_H
#define FRAME_H

#include <QString>
class Frame {
    public:
        Frame(int,QString, QString);
        int getNum() const;
        QString getTA() const;
        QString getRA() const;
        QString getSSID() const;
        void setSSID(QString);

    private:
        int num;
        QString TA;
        QString RA;
        QString SSID;
};

#endif // FRAME_H
