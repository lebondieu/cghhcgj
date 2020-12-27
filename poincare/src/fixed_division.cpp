#include <poincare/fixed_division.h>
#include <poincare/fraction_layout.h>
#include <poincare/layout_helper.h>
#include <poincare/rational.h>
#include <poincare/power.h>
#include <poincare/serialization_helper.h>
#include <poincare/undefined.h>
#include <cmath>
#include <assert.h>

namespace Poincare
{

  Layout FixedDivisionNode::createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const
  {
    const ExpressionNode *numerator = childAtIndex(0);
    const ExpressionNode *denominator = childAtIndex(1);
    return FractionLayout::Builder(numerator->createLayout(floatDisplayMode, numberOfSignificantDigits), denominator->createLayout(floatDisplayMode, numberOfSignificantDigits));
    //return LayoutHelper::Infix(FixedDivision(this), floatDisplayMode, numberOfSignificantDigits, "/");
  }

  int FixedDivisionNode::serialize(char *buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const
  {
    return SerializationHelper::Infix(this, buffer, bufferSize, floatDisplayMode, numberOfSignificantDigits, "/");
  }

  Expression FixedDivisionNode::shallowReduce(ReductionContext reductionContext)
  {
    return FixedDivision(this).shallowReduce(reductionContext);
  }

  template <typename T>
  Complex<T> FixedDivisionNode::templatedApproximate(Context *context, Preferences::ComplexFormat complexFormat, Preferences::AngleUnit angleUnit) const
  {
    return Complex<T>::Builder(FixedDivision(this).approximateToScalar<T>(context, complexFormat, angleUnit));
  }

  Expression FixedDivision::shallowReduce(ExpressionNode::ReductionContext reductionContext)
  {

    /*{
      Expression e = Expression::defaultShallowReduce();
      e = e.defaultHandleUnitsInChildren();
      if (e.isUndefined())
      {
        return e;
      }
    }*/
    if (childAtIndex(1).deepIsMatrix(reductionContext.context()))
    {
      return replaceWithUndefinedInPlace();
    }
    if (childAtIndex(0).type() == ExpressionNode::Type::Matrix)
    {
      return mapOnMatrixFirstChild(reductionContext);
    }
    /* We reduce only round(Rational, Rational). We do not reduce
   * round(Float, Float) which is equivalent to what is done in approximate. */
    if (childAtIndex(0).type() == ExpressionNode::Type::Rational && childAtIndex(1).type() == ExpressionNode::Type::Rational)
    {

      Rational r1 = childAtIndex(0).convert<Rational>();
      Rational r2 = childAtIndex(1).convert<Rational>();
      if (!r2.isInteger())
      {
        return replaceWithUndefinedInPlace();
      }
      //TODO: multiply up if there are fractional bits
      IntegerDivision d = Integer::Division(r1.signedIntegerNumerator(), r2.signedIntegerNumerator());
      Integer rounding = d.quotient;
      Rational result = Rational::toFixedPoint(Rational::Builder(rounding));
      if (result.numeratorOrDenominatorIsInfinity())
      {
        return *this;
      }
      replaceWithInPlace(result);
      return std::move(result);
    }
  }
} // namespace Poincare
