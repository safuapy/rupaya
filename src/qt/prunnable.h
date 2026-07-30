// Copyright (c) 2019 The PIVX Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef RUPX_QT_PRUNNABLE_H
#define RUPX_QT_PRUNNABLE_H

#include <QString>

class Runnable {
public:
    virtual void run(int type) = 0;
    virtual void onError(QString error, int type) = 0;
};

#endif // RUPX_QT_PRUNNABLE_H
