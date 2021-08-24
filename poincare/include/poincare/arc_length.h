#ifndef POINCARE_ARC_LENGTH_H
#define POINCARE_ARC_LENGTH_H

#include <poincare/parametered_expression.h>
#include <poincare/symbol.h>

namespace Poincare {

class ArcLengthNode final : public ParameteredExpressionNode {
public:

  // TreeNode
  size_t size() const override { return sizeof(ArcLengthNode); }
  int numberOfChildren() const override;
#if POINCARE_TREE_LOG
  void logNodeName(std::ostream & stream) const override {
    stream << "Arc Length";
  }
#endif

  // Properties
  Type type() const override { return Type::ArcLength; }
  int polynomialDegree(Context * context, const char * symbolName) const override;

private:
  // Layout
  Layout createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;
  int serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;

  // Simplification
  Expression shallowReduce(ReductionContext reductionContext) override;
  LayoutShape leftLayoutShape() const override { return LayoutShape::MoreLetters; };
  LayoutShape rightLayoutShape() const override { return LayoutShape::BoundaryPunctuation; }

  // Evaluation
  Evaluation<float> approximate(SinglePrecision p, ApproximationContext approximationContext) const override { return templatedApproximate<float>(approximationContext); }
  Evaluation<double> approximate(DoublePrecision p, ApproximationContext approximationContext) const override { return templatedApproximate<double>(approximationContext); }
  template<typename T>
  Evaluation<T> templatedApproximate(ApproximationContext approximationContext) const {
    return Complex<T>::Undefined();
  }
};

class ArcLength final : public ParameteredExpression {
public:
  ArcLength(const ArcLengthNode * n) : ParameteredExpression(n) {}
  static ArcLength Builder(Expression child0, Symbol child1, Expression child2, Expression child3) { return TreeHandle::FixedArityBuilder<ArcLength, ArcLengthNode>({child0, child1, child2, child3}); }
  static Expression UntypedBuilder(Expression children);

  static constexpr Expression::FunctionHelper s_functionHelper = Expression::FunctionHelper("arclen", 4, &UntypedBuilder);

  // Expression
  Expression shallowReduce(ExpressionNode::ReductionContext context);
};

}

#endif
