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
        QString getDATA() const;
        int getsize() const;
        QString getMore_Fragments() const;
        QString getSeqnum() const;
        int getFragnum() const;
        void setSSID(QString);
        void setDATA(QString);
        void setsize(int);
        void setMore_Fragments(QString);
        void setSeqnum(QString);
        void setFragnum(int);

    private:
        int num;
        QString TA;
        QString RA;
        QString SSID;
        QString DATA;
        int size;
        QString More_Fragments;
        QString Seqnum;
        int Fragnum;

};

#endif // FRAME_H
