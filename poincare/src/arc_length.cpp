#include <poincare/arc_length.h>
#include <poincare/addition.h>
#include <poincare/constant.h>
#include <poincare/derivative.h>
#include <poincare/serialization_helper.h>
#include <poincare/layout_helper.h>
#include <poincare/expression.h>
#include "parsing/token.h"
#include <poincare/integer.h>
#include <poincare/integral.h>
#include <poincare/expression.h>
#include <poincare/rational.h>
#include <poincare/matrix.h>
#include <poincare/power.h>
#include <poincare/square_root.h>
#include <utility>

namespace Poincare {

constexpr Expression::FunctionHelper ArcLength::s_functionHelper;

int ArcLengthNode::numberOfChildren() const { return ArcLength::s_functionHelper.numberOfChildren(); }

int ArcLengthNode::polynomialDegree(Context * context, const char * symbolName) const {
  if (childAtIndex(0)->polynomialDegree(context, symbolName) == 0
      && childAtIndex(1)->polynomialDegree(context, symbolName) == 0
      && childAtIndex(2)->polynomialDegree(context, symbolName) == 0
      && childAtIndex(3)->polynomialDegree(context, symbolName) == 0)
  {
    // If no child depends on the symbol, the polynomial degree is 0.
    return 0;
  }
  return ExpressionNode::polynomialDegree(context, symbolName);
}

Layout ArcLengthNode::createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return LayoutHelper::Prefix(ArcLength(this), floatDisplayMode, numberOfSignificantDigits, ArcLength::s_functionHelper.name());
}

int ArcLengthNode::serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return SerializationHelper::Prefix(this, buffer, bufferSize, floatDisplayMode, numberOfSignificantDigits, ArcLength::s_functionHelper.name());
}

Expression ArcLengthNode::shallowReduce(ExpressionNode::ReductionContext reductionContext) {
  return ArcLength(this).shallowReduce(reductionContext);
}

Expression ArcLength::UntypedBuilder(Expression children) {
  assert(children.type() == ExpressionNode::Type::Matrix);
  if (children.childAtIndex(1).type() != ExpressionNode::Type::Symbol) {
    // Second parameter must be a Symbol.
    return Expression();
  }

  if (children.childAtIndex(0).type() == ExpressionNode::Type::Matrix &&
      children.childAtIndex(0).convert<Matrix>().vectorType() == Array::VectorType::None) {
    // Parametric curve must be in vector form.
    return Expression();
  }

  return Builder(children.childAtIndex(0), children.childAtIndex(1).convert<Symbol>(), children.childAtIndex(2), children.childAtIndex(3));
}

Expression ArcLength::shallowReduce(ExpressionNode::ReductionContext reductionContext) {
  Expression function = childAtIndex(0);
  Expression symbol_exp = childAtIndex(1);
  Expression a = childAtIndex(2);
  Expression b = childAtIndex(3);

  Matrix matrix = Matrix::Builder();
  if (function.type() != ExpressionNode::Type::Matrix) {
    // Single variable function converted to parametric curve [x, f(x)]
    matrix.addChildAtIndexInPlace(symbol_exp.clone(), 0, 0);
    matrix.addChildAtIndexInPlace(function, 1, 0);
    matrix.setDimensions(2, 1);
  } else {
    matrix = function.convert<Matrix>();
  }

  /* Length of a parametric curve = integral_a^b ds
   * ds^2 = (dx/dt)^2 + (dy/dt)^2 + ... */
  Addition length_element = Addition::Builder();
  for (int i = 0; i < matrix.numberOfChildren(); i++) {
    Expression derivative = Derivative::Builder(matrix.childAtIndex(i), symbol_exp.convert<Symbol>(), symbol_exp.clone()).shallowReduce(reductionContext);
    Expression square = Power::Builder(derivative, Rational::Builder(2));
    length_element.addChildAtIndexInPlace(square, length_element.numberOfChildren(), length_element.numberOfChildren());
  }

  Integral arc_len = Integral::Builder(SquareRoot::Builder(length_element), symbol_exp.clone().convert<Symbol>(), a, b);
  replaceWithInPlace(arc_len);
  return arc_len.deepReduce(reductionContext);
}

}
