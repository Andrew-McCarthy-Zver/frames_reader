#include <QString>
#include "Frame.h"

Frame::Frame(int n,QString ta, QString ra) {

    num = n;
    TA = ta;
    RA = ra;
    SSID = "NULL";
    DATA = "NULL";
    size = 0;
    More_Fragments = "NULL";
    Seqnum = "NULL";
    Fragnum = 0;

}

int Frame::getNum() const {

    return num;
}

QString Frame::getTA() const {

    return TA;
}

QString Frame::getRA() const {

    return RA;
}
QString Frame::getSSID() const {

    return SSID;
}
void Frame::setSSID(QString ssid) {
    SSID = ssid;
}
QString Frame::getDATA() const {

    return DATA;
}
void Frame::setDATA(QString data) {
    DATA = data;
}
int Frame::getsize() const {

    return size;
}
void Frame::setsize(int sze) {
    size = sze;
}
QString Frame::getMore_Fragments() const {

    return More_Fragments;
}
void Frame::setMore_Fragments(QString M_F) {
    More_Fragments = M_F;
}
QString Frame::getSeqnum() const {

    return Seqnum;
}
void Frame::setSeqnum(QString seqnum) {
    Seqnum = seqnum;
}
int Frame::getFragnum() const {

    return Fragnum;
}
void Frame::setFragnum(int fragnum) {
    Fragnum = fragnum;
}
