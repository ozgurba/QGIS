/***************************************************************************
                         qgspolygon.h
                         -------------------
    begin                : September 2014
    copyright            : (C) 2014 by Marco Hugentobler
    email                : marco at sourcepole dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSPOLYGONV2_H
#define QGSPOLYGONV2_H

#include "qgis_core.h"
#include "qgscurvepolygon.h"

/** \ingroup core
 * \class QgsPolygonV2
 * \brief Polygon geometry type.
 * \note added in QGIS 2.10
 * \note this API is not considered stable and may change for 2.12
 */
class CORE_EXPORT QgsPolygonV2: public QgsCurvePolygon
{
  public:
    QgsPolygonV2();

    bool operator==( const QgsPolygonV2 &other ) const;
    bool operator!=( const QgsPolygonV2 &other ) const;

    virtual QString geometryType() const override { return QStringLiteral( "Polygon" ); }
    virtual QgsPolygonV2 *clone() const override;
    void clear() override;

    virtual bool fromWkb( QgsConstWkbPtr &wkb ) override;

    // inherited: bool fromWkt( const QString &wkt );

    QByteArray asWkb() const override;
    // inherited: QString asWkt( int precision = 17 ) const;
    // inherited: QDomElement asGML2( QDomDocument& doc, int precision = 17, const QString& ns = "gml" ) const;
    // inherited: QDomElement asGML3( QDomDocument& doc, int precision = 17, const QString& ns = "gml" ) const;
    // inherited: QString asJSON( int precision = 17 ) const;

    QgsPolygonV2 *surfaceToPolygon() const override;

    /** Returns the geometry converted to the more generic curve type QgsCurvePolygon
     @return the converted geometry. Caller takes ownership*/
    QgsAbstractGeometry *toCurveType() const override;

    void addInteriorRing( QgsCurve *ring ) override;
    //overridden to handle LineString25D rings
    virtual void setExteriorRing( QgsCurve *ring ) override;

    virtual QgsAbstractGeometry *boundary() const override;

    /**
     * Returns the distance from a point to the boundary of the polygon (either the
     * exterior ring or any closer interior rings). The returned distance will be
     * negative if the point lies outside the polygon.
     * @note added in QGIS 3.0
     */
    double pointDistanceToBoundary( double x, double y ) const;

};
#endif // QGSPOLYGONV2_H
