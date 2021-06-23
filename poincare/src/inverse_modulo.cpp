#include <poincare/inverse_modulo.h>
#include <poincare/arithmetic.h>
#include <poincare/layout_helper.h>
#include <poincare/serialization_helper.h>

namespace Poincare {

constexpr Expression::FunctionHelper InverseModulo::s_functionHelper;

int InverseModuloNode::numberOfChildren() const { return InverseModulo::s_functionHelper.numberOfChildren(); }

Layout InverseModuloNode::createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return LayoutHelper::Prefix(InverseModulo(this), floatDisplayMode, numberOfSignificantDigits, InverseModulo::s_functionHelper.name());
}

int InverseModuloNode::serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return SerializationHelper::Prefix(this, buffer, bufferSize, floatDisplayMode, numberOfSignificantDigits, InverseModulo::s_functionHelper.name());
}

Expression InverseModuloNode::shallowReduce(ReductionContext reductionContext) {
  return InverseModulo(this).shallowReduce(reductionContext.context());
}


template<typename T>
Evaluation<T> InverseModuloNode::templatedApproximate(ApproximationContext approximationContext) const {
  return Arithmetic::IMOD<T>(*this, approximationContext);
}


Expression InverseModulo::shallowReduce(Context * context) {
  {
    Expression e = Expression::defaultShallowReduce();
    e = e.defaultHandleUnitsInChildren();
    if (e.isUndefined()) {
      return e;
    }
  }

  // Compute IMOD
  Expression result = Arithmetic::IMOD(*this);

  replaceWithInPlace(result);
  return result;
}

template Evaluation<float> InverseModuloNode::templatedApproximate<float>(ApproximationContext approximationContext) const;
template Evaluation<double> InverseModuloNode::templatedApproximate<double>(ApproximationContext approximationContext) const;

}
