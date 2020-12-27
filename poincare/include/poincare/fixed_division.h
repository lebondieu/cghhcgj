#ifndef POINCARE_FIXED_DIVISION_H
#define POINCARE_FIXED_DIVISION_H

#include <poincare/expression.h>
#include <poincare/approximation_helper.h>

namespace Poincare
{

  //class FixedDivision;

  class FixedDivisionNode final : public ExpressionNode
  {
  public:
    // TreeNode
    size_t size() const override
    {
      return sizeof(FixedDivisionNode);
    }
    int numberOfChildren() const override { return 2; }
#if POINCARE_TREE_LOG
    void logNodeName(std::ostream &stream) const override
    {
      stream << "FixedDivision";
    }
#endif

    // Properties
    Type type() const override { return Type::FixedDivision; }

    // Approximation
    virtual Evaluation<float> approximate(SinglePrecision p, Context *context, Preferences::ComplexFormat complexFormat, Preferences::AngleUnit angleUnit) const override
    {
      return templatedApproximate<float>(context, complexFormat, angleUnit);
    }
    virtual Evaluation<double> approximate(DoublePrecision p, Context *context, Preferences::ComplexFormat complexFormat, Preferences::AngleUnit angleUnit) const override
    {
      return templatedApproximate<double>(context, complexFormat, angleUnit);
    }

    // Layout
    Layout createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;
    int serialize(char *buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;

    // Simplification
    Expression shallowReduce(ReductionContext reductionContext) override;

  private:
    // Approximation
    template <typename T>
    Complex<T> templatedApproximate(Context *context, Preferences::ComplexFormat complexFormat, Preferences::AngleUnit angleUnit) const;
    LayoutShape leftLayoutShape() const override { return LayoutShape::MoreLetters; };
  };

  class FixedDivision final : public Expression
  {
    friend class FixedDivisionNode;

  public:
    FixedDivision(const FixedDivisionNode *n) : Expression(n) {}
    static FixedDivision Builder(Expression numerator, Expression denominator) { return TreeHandle::FixedArityBuilder<FixedDivision, FixedDivisionNode>({numerator, denominator}); }

    Expression shallowReduce(ExpressionNode::ReductionContext reductionContext);
  };

} // namespace Poincare

#endif
