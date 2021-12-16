#ifndef FRAME_H
#define FRAME_H

#include <QString>
class Frame {
    public:
        Frame(int,QString, QString);
        int getNum() const;
        QString getTA() const;
        QString getRA() const;

    private:
        int num;
        QString TA;
        QString RA;
};

#endif // FRAME_H
