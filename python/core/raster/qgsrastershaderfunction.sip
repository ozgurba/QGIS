class QgsRasterShaderFunction
{
%TypeHeaderCode
#include <qgsrastershaderfunction.h>
#include <qgscolorrampshader.h>
%End

%ConvertToSubClassCode
  if (dynamic_cast<QgsColorRampShader*>(sipCpp) != NULL)
    sipType = sipType_QgsColorRampShader;
  else
    sipType = 0;
%End


  public:
    QgsRasterShaderFunction( double minimumValue = 0.0, double maximumValue = 255.0 );
    virtual ~QgsRasterShaderFunction();

    /** \brief Set the maximum value */
    virtual void setMaximumValue( double );

    /** \brief Return the minimum value */
    virtual void setMinimumValue( double );

    /** \brief generates and new RGBA value based on one input value */
    virtual bool shade( double, int* /Out/, int* /Out/, int* /Out/, int* /Out/ );

    /** \brief generates and new RGBA value based on original RGBA value */
    virtual bool shade( double, double, double, double, int* /Out/, int* /Out/, int* /Out/, int* /Out/ );

    double minimumMaximumRange() const;

    double minimumValue() const;
    double maximumValue() const;

    virtual void legendSymbologyItems( QList< QPair< QString, QColor > >& symbolItems ) const;
};
