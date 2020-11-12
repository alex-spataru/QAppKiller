/*
 * Copyright (c) 2020 Alex Spataru <alex_spataru@outlook.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef APP_SUSPENDER_H
#define APP_SUSPENDER_H

#include <QTimer>
#include <QObject>
#include <QNetworkAccessManager>

class AppSuspender : public QObject
{
   Q_OBJECT

   Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
   Q_PROPERTY(bool suspended READ suspended NOTIFY suspendedChanged)
   Q_PROPERTY(QString client READ client WRITE setClient NOTIFY clientChanged)
   Q_PROPERTY(bool downloadingAppCast READ downloadingAppCast NOTIFY downloadingAppCastChanged)

signals:
   void urlChanged();
   void clientChanged();
   void suspendedChanged();
   void downloadingAppCastChanged();

public:
   AppSuspender(QObject *parent = Q_NULLPTR);

   QString url() const;
   QString client() const;
   bool suspended() const;
   bool downloadingAppCast() const;

public slots:
   void setUrl(const QString &url);
   void setClient(const QString &id);

private slots:
   void downloadAppCast();
   void onReply(QNetworkReply *reply);

private:
   QString m_url;
   QString m_client;
   bool m_suspended;
   bool m_downloadingAppCast;
   QNetworkAccessManager m_networkAccessManager;
};

#endif
