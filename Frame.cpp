#include <QString>
#include "Frame.h"

Frame::Frame(int n,QString ta, QString ra) {

    num = n;
    TA = ta;
    RA = ra;
    SSID = "NULL";
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
