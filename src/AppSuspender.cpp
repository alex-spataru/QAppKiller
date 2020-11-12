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

#include "AppSuspender.h"

#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QGuiApplication>

/**
 * Initializes the class & connects SIGNALS/SLOTS
 */
AppSuspender::AppSuspender(QObject *parent)
   : QObject(parent)
{
   m_suspended = false;
   m_downloadingAppCast = false;

   connect(&m_networkAccessManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(onReply(QNetworkReply *)));
}

/**
 * Returns the app cast URL
 */
QString AppSuspender::url() const
{
   return m_url;
}

/**
 * Returns the client ID of the application
 */
QString AppSuspender::client() const
{
   return m_client;
}

/**
 * Returns @c true if we should suspend application operations
 */
bool AppSuspender::suspended() const
{
   return m_suspended;
}

/**
 * Returns @c true if we are currently downloading the app cast file
 */
bool AppSuspender::downloadingAppCast() const
{
   return m_downloadingAppCast;
}

/**
 * Changes the app cast URL & initializes download in 1000 milliseconds
 */
void AppSuspender::setUrl(const QString &url)
{
   if (m_url != url)
   {
      m_url = url;
      emit urlChanged();

      QTimer::singleShot(1000, this, SLOT(downloadAppCast()));
   }
}

/**
 * Changes the client ID of the application
 */
void AppSuspender::setClient(const QString &id)
{
   if (m_client != id)
   {
      m_client = id;
      emit clientChanged();
   }
}

/**
 * Begins the download process of the app cast file
 */
void AppSuspender::downloadAppCast()
{
   if (!url().isEmpty())
   {
      auto userAgent = QString("%1/%2 (Qt; QAppSuspender)").arg(qApp->applicationName(), qApp->applicationVersion());
      QNetworkRequest request(url());
      request.setRawHeader("User-Agent", userAgent.toUtf8());
      m_networkAccessManager.get(request);

      m_downloadingAppCast = true;
      emit downloadingAppCastChanged();
   }
}

/**
 * Reads the app cast file & updates internal variables
 */
void AppSuspender::onReply(QNetworkReply *reply)
{
   // Check pointer
   if (!reply)
      return;

   // Check if we need to redirect
   QUrl redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
   if (!redirect.isEmpty())
   {
      setUrl(redirect.toString());
      downloadAppCast();
      return;
   }

   // There was a network error
   if (reply->error() != QNetworkReply::NoError)
   {
      m_suspended = false;
      m_downloadingAppCast = false;

      emit suspendedChanged();
      emit downloadingAppCastChanged();

      return;
   }

   // Try to create JSON document from network reply
   QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

   // JSON document is invalid
   if (document.isNull() || document.isEmpty())
   {
      m_suspended = false;
      m_downloadingAppCast = false;

      emit suspendedChanged();
      emit downloadingAppCastChanged();

      return;
   }

   // Get suspender enabled & clients array
   auto json = document.object();
   auto enabled = json.value("enabled").toBool();
   auto clients = json.value("clients").toArray();

   // Suspender not active
   if (!enabled)
   {
      m_suspended = false;
      m_downloadingAppCast = false;
      emit suspendedChanged();
      emit downloadingAppCastChanged();
      return;
   }

   // Check if clients contains current client
   for (int i = 0; i < clients.count(); ++i)
   {
      auto clientObj = clients.at(i).toObject();
      auto clientId = clientObj.value("client").toString();
      auto suspended = clientObj.value("suspended").toBool();

      if (clientId == client())
      {
         m_suspended = suspended;
         m_downloadingAppCast = false;
         emit suspendedChanged();
         emit downloadingAppCastChanged();
         return;
      }
   }

   // We should not reach this if JSON document is valid
   m_suspended = false;
   m_downloadingAppCast = false;
   emit suspendedChanged();
   emit downloadingAppCastChanged();
}
