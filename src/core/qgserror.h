/***************************************************************************
                         qgserror.h  -  Error container
                             -------------------
    begin                : October 2012
    copyright            : (C) 2012 Radim Blazek
    email                : radim dot blazek at gmail dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSERROR_H
#define QGSERROR_H

#include <QString>
#include <QList>

#include "qgis_core.h"

// Macro to create Error message including info about where it was created.
#define QGS_ERROR_MESSAGE(message, tag) QgsErrorMessage(QString(message),QString(tag), QString(__FILE__), QString(__FUNCTION__), __LINE__)

/** \ingroup core
 * QgsErrorMessage represents single error message.
*/
class CORE_EXPORT QgsErrorMessage
{
  public:
    //! Format
    enum Format
    {
      Text, // Plain text
      Html
    };

    QgsErrorMessage()
      : mLine( 0 )
      , mFormat( Text )
    {}

    /** Constructor.
     *  @param message error message string
     *  @param tag error label, for example GDAL, GDAL Provider, Raster layer
     *  @param file the file where error was created
     *  @param function the function where error was created
     *  @param line the line where error was created
     */
    QgsErrorMessage( const QString &message, const QString &tag = QString::null, const QString &file = QString::null, const QString &function = QString::null, int line = 0 );

    QString message() const { return mMessage; }
    QString tag() const { return mTag; }
    QString file() const { return mFile; }
    QString function() const { return mFunction; }
    int line() const { return mLine; }

  private:
    //! Error messages
    QString mMessage;

    //! Short description
    QString mTag;

    //! Detailed debug info
    QString mFile;
    QString mFunction;
    int mLine;

    //! Message format
    Format mFormat;
};

/** \ingroup core
 * QgsError is container for error messages (report). It may contain chain
 * (sort of traceback) of error messages (e.g. GDAL - provider - layer).
 * Higher level messages are appended at the end.
*/
class CORE_EXPORT QgsError
{
  public:

    QgsError() {}

    /** Constructor with single message.
     *  @param message error message
     *  @param tag short description, e.g. GDAL, Provider, Layer
     */
    QgsError( const QString &message, const QString &tag );

    /** Append new error message.
     *  @param message error message string
     *  @param tag error label, for example GDAL, GDAL Provider, Raster layer
     */
    void append( const QString &message, const QString &tag );

    /** Append new error message.
     *  @param message error message
     */
    void append( const QgsErrorMessage &message );

    /** Test if any error is set.
     *  @return true if contains error
     */
    bool isEmpty() const { return mMessageList.isEmpty(); }

    /** Full error messages description
     *  @param format output format
     *  @return error report
     */
    QString message( QgsErrorMessage::Format format = QgsErrorMessage::Html ) const;

    /** Short error description, usually the first error in chain, the real error.
     *  @return error description
     */
    QString summary() const;

    //! Clear error messages
    void clear() { mMessageList.clear(); }

  private:
    //! List of messages
    QList<QgsErrorMessage> mMessageList;
};

#endif
