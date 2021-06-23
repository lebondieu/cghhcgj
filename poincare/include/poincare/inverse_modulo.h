#ifndef POINCARE_INVERSE_MODULO_H
#define POINCARE_INVERSE_MODULO_H

#include <poincare/expression.h>
#include <poincare/evaluation.h>

namespace Poincare {

class InverseModuloNode final : public ExpressionNode {
public:

  // TreeNode
  size_t size() const override { return sizeof(InverseModuloNode); }
  int numberOfChildren() const override;
#if POINCARE_TREE_LOG
  void logNodeName(std::ostream & stream) const override {
    stream << "InverseModulo";
  }
#endif

  // ExpressionNode
  Sign sign(Context * context) const override { return Sign::Positive; }
  Type type() const override { return Type::InverseModulo; }

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
  template<typename T> Evaluation<T> templatedApproximate(ApproximationContext approximationContext) const;
};

class InverseModulo final : public Expression {
public:
  InverseModulo(const InverseModuloNode * n) : Expression(n) {};
  static InverseModulo Builder(Expression child0, Expression child1) { return TreeHandle::FixedArityBuilder<InverseModulo, InverseModuloNode>({child0, child1}); }
  static constexpr Expression::FunctionHelper s_functionHelper = Expression::FunctionHelper("imod", 2, &UntypedBuilderTwoChildren<InverseModulo>);

  Expression shallowReduce(Context * context);
};

}

#endif
