/***************************************************************************
    testqgsdualview.cpp
     --------------------------------------
    Date                 : 14.2.2013
    Copyright            : (C) 2013 Matthias Kuhn
    Email                : matthias at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "qgstest.h"

#include <editorwidgets/core/qgseditorwidgetregistry.h>
#include <attributetable/qgsattributetableview.h>
#include <attributetable/qgsdualview.h>
#include "qgsattributeform.h"
#include <qgsapplication.h>
#include "qgsfeatureiterator.h"
#include <qgsvectorlayer.h>
#include "qgsvectordataprovider.h"
#include <qgsmapcanvas.h>
#include <qgsfeature.h>

#include "qgstest.h"

class TestQgsDualView : public QObject
{
    Q_OBJECT
  public:
    TestQgsDualView()
      : mCanvas( 0 )
      , mPointsLayer( 0 )
      , mDualView( 0 )
    {}

  private slots:
    void initTestCase(); // will be called before the first testfunction is executed.
    void cleanupTestCase(); // will be called after the last testfunction was executed.
    void init(); // will be called before each testfunction is executed.
    void cleanup(); // will be called after every testfunction.

    void testColumnCount();

    void testColumnHeaders();

    void testData();

    void testSelectAll();

    void testSort();

    void testAttributeFormSharedValueScanning();
    void testNoGeom();

  private:
    QgsMapCanvas *mCanvas = nullptr;
    QgsVectorLayer *mPointsLayer = nullptr;
    QString mTestDataDir;
    QgsDualView *mDualView = nullptr;
};

void TestQgsDualView::initTestCase()
{
  QgsApplication::init();
  QgsApplication::initQgis();
  QgsApplication::showSettings();

  QgsEditorWidgetRegistry::initEditors();

  // Setup a map canvas with a vector layer loaded...
  QString myDataDir( TEST_DATA_DIR ); //defined in CmakeLists.txt
  mTestDataDir = myDataDir + '/';

  //
  // load a vector layer
  //
  QString myPointsFileName = mTestDataDir + "points.shp";
  QFileInfo myPointFileInfo( myPointsFileName );
  mPointsLayer = new QgsVectorLayer( myPointFileInfo.filePath(),
                                     myPointFileInfo.completeBaseName(), QStringLiteral( "ogr" ) );

  mCanvas = new QgsMapCanvas();
}

void TestQgsDualView::cleanupTestCase()
{
  delete mPointsLayer;
  delete mCanvas;
  QgsApplication::exitQgis();
}

void TestQgsDualView::init()
{
  mDualView = new QgsDualView();
  mDualView->init( mPointsLayer, mCanvas );
}

void TestQgsDualView::cleanup()
{
  delete mDualView;
}

void TestQgsDualView::testColumnCount()
{
  QCOMPARE( mDualView->tableView()->model()->columnCount(), mPointsLayer->fields().count() );
}

void TestQgsDualView::testColumnHeaders()
{
  for ( int i = 0; i < mPointsLayer->fields().count(); ++i )
  {
    QgsField fld = mPointsLayer->fields().at( i );
    QCOMPARE( mDualView->tableView()->model()->headerData( i, Qt::Horizontal ).toString(), fld.name() );
  }
}

void TestQgsDualView::testData()
{
  QgsFeature feature;
  mPointsLayer->getFeatures( QgsFeatureRequest().setFilterFid( 0 ) ).nextFeature( feature );

  for ( int i = 0; i < mPointsLayer->fields().count(); ++i )
  {
    QgsField fld = mPointsLayer->fields().at( i );

    QModelIndex index = mDualView->tableView()->model()->index( 0, i );
    QCOMPARE( mDualView->tableView()->model()->data( index ).toString(), fld.displayString( feature.attribute( i ) ) );
  }
}

void TestQgsDualView::testSelectAll()
{
  mDualView->setFilterMode( QgsAttributeTableFilterModel::ShowVisible );
  // Only show parts of the canvas, so only one selected feature is visible
  mCanvas->setExtent( QgsRectangle( -139, 23, -100, 48 ) );
  mDualView->mTableView->selectAll();
  QCOMPARE( mPointsLayer->selectedFeatureCount(), 10 );

  mPointsLayer->selectByIds( QgsFeatureIds() );
  mCanvas->setExtent( QgsRectangle( -110, 40, -100, 48 ) );
  mDualView->mTableView->selectAll();
  QCOMPARE( mPointsLayer->selectedFeatureCount(), 1 );
}

void TestQgsDualView::testSort()
{
  mDualView->setSortExpression( QStringLiteral( "Class" ) );

  QStringList classes;
  classes << QStringLiteral( "B52" )
          << QStringLiteral( "B52" )
          << QStringLiteral( "B52" )
          << QStringLiteral( "B52" )
          << QStringLiteral( "Biplane" )
          << QStringLiteral( "Biplane" )
          << QStringLiteral( "Biplane" )
          << QStringLiteral( "Biplane" )
          << QStringLiteral( "Biplane" )
          << QStringLiteral( "Jet" )
          << QStringLiteral( "Jet" )
          << QStringLiteral( "Jet" )
          << QStringLiteral( "Jet" )
          << QStringLiteral( "Jet" )
          << QStringLiteral( "Jet" )
          << QStringLiteral( "Jet" )
          << QStringLiteral( "Jet" );

  for ( int i = 0; i < classes.length(); ++i )
  {
    QModelIndex index = mDualView->tableView()->model()->index( i, 0 );
    QCOMPARE( mDualView->tableView()->model()->data( index ).toString(), classes.at( i ) );
  }

  QStringList headings;
  headings << QStringLiteral( "0" )
           <<  QStringLiteral( "0" )
           <<  QStringLiteral( "12" )
           <<  QStringLiteral( "34" )
           <<  QStringLiteral( "80" )
           <<  QStringLiteral( "85" )
           <<  QStringLiteral( "90" )
           <<  QStringLiteral( "90" )
           <<  QStringLiteral( "95" )
           <<  QStringLiteral( "100" )
           <<  QStringLiteral( "140" )
           <<  QStringLiteral( "160" )
           <<  QStringLiteral( "180" )
           <<  QStringLiteral( "240" )
           <<  QStringLiteral( "270" )
           <<  QStringLiteral( "300" )
           <<  QStringLiteral( "340" );

  mDualView->setSortExpression( QStringLiteral( "Heading" ) );

  for ( int i = 0; i < headings.length(); ++i )
  {
    QModelIndex index = mDualView->tableView()->model()->index( i, 1 );
    QCOMPARE( mDualView->tableView()->model()->data( index ).toString(), headings.at( i ) );
  }
}

void TestQgsDualView::testAttributeFormSharedValueScanning()
{
  // test QgsAttributeForm::scanForEqualAttributes

  QSet< int > mixedValueFields;
  QHash< int, QVariant > fieldSharedValues;

  // make a temporary layer to check through
  QgsVectorLayer *layer = new QgsVectorLayer( QStringLiteral( "Point?field=col1:integer&field=col2:integer&field=col3:integer&field=col4:integer" ), QStringLiteral( "test" ), QStringLiteral( "memory" ) );
  QVERIFY( layer->isValid() );
  QgsFeature f1( layer->dataProvider()->fields(), 1 );
  f1.setAttribute( QStringLiteral( "col1" ), 1 );
  f1.setAttribute( QStringLiteral( "col2" ), 1 );
  f1.setAttribute( QStringLiteral( "col3" ), 3 );
  f1.setAttribute( QStringLiteral( "col4" ), 1 );
  QgsFeature f2( layer->dataProvider()->fields(), 2 );
  f2.setAttribute( QStringLiteral( "col1" ), 1 );
  f2.setAttribute( QStringLiteral( "col2" ), 2 );
  f2.setAttribute( QStringLiteral( "col3" ), 3 );
  f2.setAttribute( QStringLiteral( "col4" ), 2 );
  QgsFeature f3( layer->dataProvider()->fields(), 3 );
  f3.setAttribute( QStringLiteral( "col1" ), 1 );
  f3.setAttribute( QStringLiteral( "col2" ), 2 );
  f3.setAttribute( QStringLiteral( "col3" ), 3 );
  f3.setAttribute( QStringLiteral( "col4" ), 2 );
  QgsFeature f4( layer->dataProvider()->fields(), 4 );
  f4.setAttribute( QStringLiteral( "col1" ), 1 );
  f4.setAttribute( QStringLiteral( "col2" ), 1 );
  f4.setAttribute( QStringLiteral( "col3" ), 3 );
  f4.setAttribute( QStringLiteral( "col4" ), 2 );
  layer->dataProvider()->addFeatures( QgsFeatureList() << f1 << f2 << f3 << f4 );

  QgsAttributeForm form( layer );

  QgsFeatureIterator it = layer->getFeatures();

  form.scanForEqualAttributes( it, mixedValueFields, fieldSharedValues );

  QCOMPARE( mixedValueFields, QSet< int >() << 1 << 3 );
  QCOMPARE( fieldSharedValues.value( 0 ).toInt(), 1 );
  QCOMPARE( fieldSharedValues.value( 2 ).toInt(), 3 );

  // add another feature so all attributes are different
  QgsFeature f5( layer->dataProvider()->fields(), 5 );
  f5.setAttribute( QStringLiteral( "col1" ), 11 );
  f5.setAttribute( QStringLiteral( "col2" ), 11 );
  f5.setAttribute( QStringLiteral( "col3" ), 13 );
  f5.setAttribute( QStringLiteral( "col4" ), 12 );
  layer->dataProvider()->addFeatures( QgsFeatureList() << f5 );

  it = layer->getFeatures();

  form.scanForEqualAttributes( it, mixedValueFields, fieldSharedValues );
  QCOMPARE( mixedValueFields, QSet< int >() << 0 << 1 << 2 << 3 );
  QVERIFY( fieldSharedValues.isEmpty() );

  // single feature, all attributes should be shared
  it = layer->getFeatures( QgsFeatureRequest().setFilterFid( 4 ) );
  form.scanForEqualAttributes( it, mixedValueFields, fieldSharedValues );
  QCOMPARE( fieldSharedValues.value( 0 ).toInt(), 1 );
  QCOMPARE( fieldSharedValues.value( 1 ).toInt(), 1 );
  QCOMPARE( fieldSharedValues.value( 2 ).toInt(), 3 );
  QCOMPARE( fieldSharedValues.value( 3 ).toInt(), 2 );
  QVERIFY( mixedValueFields.isEmpty() );
}

void TestQgsDualView::testNoGeom()
{
  //test that both the master model and cache for the dual view either both request geom or both don't request geom
  std::unique_ptr< QgsDualView > dv( new QgsDualView() );

  // request with geometry
  QgsFeatureRequest req;
  dv->init( mPointsLayer, mCanvas, req );
  // check that both master model AND cache are using geometry
  QgsAttributeTableModel *model = dv->masterModel();
  QVERIFY( model->layerCache()->cacheGeometry() );
  QVERIFY( !( model->request().flags() & QgsFeatureRequest::NoGeometry ) );

  // request with NO geometry, but using filter rect (which should override and request geom)
  req = QgsFeatureRequest().setFilterRect( QgsRectangle( 1, 2, 3, 4 ) );
  dv.reset( new QgsDualView() );
  dv->init( mPointsLayer, mCanvas, req );
  model = dv->masterModel();
  QVERIFY( model->layerCache()->cacheGeometry() );
  QVERIFY( !( model->request().flags() & QgsFeatureRequest::NoGeometry ) );

  // request with NO geometry
  req = QgsFeatureRequest().setFlags( QgsFeatureRequest::NoGeometry );
  dv.reset( new QgsDualView() );
  dv->init( mPointsLayer, mCanvas, req );
  model = dv->masterModel();
  QVERIFY( !model->layerCache()->cacheGeometry() );
  QVERIFY( ( model->request().flags() & QgsFeatureRequest::NoGeometry ) );
}

QGSTEST_MAIN( TestQgsDualView )
#include "testqgsdualview.moc"
